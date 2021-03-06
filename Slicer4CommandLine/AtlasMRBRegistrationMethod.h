
////////////////////////////////////////////////////////////////////////////////
//
//  Registration of a dataset to an atlas using affine transformation and
//  MI image match metric
//
//  Only for 3D!
//
//  Given a list of filenames for atlas template and probabilities along with
//  the dataset, this class generate images that are in the space of the first
//  image (all data and probability images).
//
////////////////////////////////////////////////////////////////////////////////

// prastawa@cs.unc.edu 10/2003

#ifndef _AtlasMRBRegistrationMethod_h
#define _AtlasMRBRegistrationMethod_h

#include "itkAffineTransform.h"
#include "itkArray.h"
#include "itkImage.h"
#include "itkObject.h"

#include "DynArray.h"

#include "ChainedAffineTransform3D.h"
#include "PairRegistrationMethod.h"

#include "vtkMRMLSubjectHierarchyNode.h"

#include <string>

template <class TOutputPixel, class TProbabilityPixel>
class AtlasMRBRegistrationMethod : public itk::Object
{

public:

  /** Standard class typedefs. */
  typedef AtlasMRBRegistrationMethod Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  // Image types
  typedef itk::Image<TOutputPixel, 3> OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename OutputImageType::OffsetType OutputImageOffsetType;
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename OutputImageType::SizeType OutputImageSizeType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef itk::Image<TProbabilityPixel, 3> ProbabilityImageType;
  typedef typename ProbabilityImageType::Pointer ProbabilityImagePointer;
  typedef typename ProbabilityImageType::IndexType ProbabilityImageIndexType;
  typedef typename ProbabilityImageType::OffsetType ProbabilityImageOffsetType;
  typedef typename ProbabilityImageType::PixelType ProbabilityImagePixelType;
  typedef typename ProbabilityImageType::SizeType ProbabilityImageSizeType;
  typedef typename ProbabilityImageType::RegionType ProbabilityImageRegionType;

  typedef itk::Image<float, 3> InternalImageType;
  typedef typename InternalImageType::Pointer InternalImagePointer;
  typedef typename InternalImageType::IndexType InternalImageIndexType;
  typedef typename InternalImageType::OffsetType InternalImageOffsetType;
  typedef typename InternalImageType::PixelType InternalImagePixelType;
  typedef typename InternalImageType::RegionType InternalImageRegionType;
  typedef typename InternalImageType::SizeType InternalImageSizeType;

  typedef itk::Image<unsigned char, 3> ByteImageType;
  typedef typename ByteImageType::Pointer ByteImagePointer;
  typedef typename ByteImageType::IndexType ByteImageIndexType;
  typedef typename ByteImageType::OffsetType ByteImageOffsetType;
  typedef typename ByteImageType::PixelType ByteImagePixelType;
  typedef typename ByteImageType::RegionType ByteImageRegionType;
  typedef typename ByteImageType::SizeType ByteImageSizeType;

  typedef DynArray<ProbabilityImagePointer> ProbabilityImageList;
  typedef DynArray<OutputImagePointer> OutputImageList;

  typedef ChainedAffineTransform3D AffineTransformType;
  typedef typename AffineTransformType::Pointer AffineTransformPointer;

  typedef enum{ID_TRANSFORM, RIGID_TRANSFORM, AFFINE_TRANSFORM}
    LinearTransformChoice;

  typedef DynArray<std::string> StringList;

  typedef itk::Array<unsigned char> FlagArrayType;

  void WriteParameters();
  void ReadParameters();

  void SetSuffix(std::string suffix);

  itkGetMacro(OutputDirectory, std::string);
  itkSetMacro(OutputDirectory, std::string);

  void SetAtlasHierarchy(vtkMRMLSubjectHierarchyNode* h);

  void SetImageFileNames(StringList filenames);

  void SetTemplateFileName(std::string filename);

  void SetAtlasOrientation(std::string orient);
  void SetImageOrientations(StringList orientations);

  ProbabilityImageList GetProbabilities();
  OutputImageList GetImages();
  OutputImagePointer GetAffineTemplate();

// HACK
  void SetOtherTemplateFileName(std::string filename) { m_OtherTemplateFileName = filename; }
  OutputImagePointer GetOtherAffineTemplate() { return m_OtherAffineTemplate; }

  void ReadImages();
  void RegisterImages();
  void ResampleImages();

  AffineTransformPointer GetTemplateAffineTransform()
  { return m_TemplateAffineTransform; }

  DynArray<AffineTransformPointer> GetAffineTransforms()
  { return m_AffineTransforms; }

  itkGetMacro(UseNonLinearInterpolation, bool);
  itkSetMacro(UseNonLinearInterpolation, bool);

  itkGetMacro(OutsideFOVCode, float);
  itkSetMacro(OutsideFOVCode, float);

  inline ByteImagePointer GetFOVMask() { return m_FOVMask; }

  void Update();

  void SetPrefilteringMethod(const char* m)
  { m_PrefilteringMethod = m; }
  itkGetConstMacro(PrefilteringMethod, std::string);

  itkSetMacro(PrefilteringIterations, unsigned int);
  itkGetConstMacro(PrefilteringIterations, unsigned int);

  itkSetMacro(PrefilteringTimeStep, double);
  itkGetConstMacro(PrefilteringTimeStep, double);

  void SetAtlasLinearTransformChoice(LinearTransformChoice c)
  { m_AtlasLinearTransformChoice = c; }
  void SetImageLinearTransformChoice(LinearTransformChoice c)
  { m_ImageLinearTransformChoice = c; }

  void FastRegistrationOn() { m_FastRegistration = true; }
  void FastRegistrationOff() { m_FastRegistration = false; }

protected:

  AtlasMRBRegistrationMethod();
  ~AtlasMRBRegistrationMethod();

  void VerifyInitialization();

  OutputImagePointer CopyOutputImage(InternalImagePointer img);
  ProbabilityImagePointer CopyProbabilityImage(InternalImagePointer img);

  InternalImagePointer PrefilterImage(InternalImagePointer& img);

private:

  std::string m_Suffix;

  std::string m_OutputDirectory;

  vtkMRMLSubjectHierarchyNode* m_AtlasHierarchy;

  StringList m_ImageFileNames;

  std::string m_TemplateFileName;

// HACK
  std::string m_OtherTemplateFileName;

  std::string m_AtlasOrientation;
  StringList m_ImageOrientations;

  AffineTransformPointer m_TemplateAffineTransform;
  DynArray<AffineTransformPointer> m_AffineTransforms;

  DynArray<ProbabilityImagePointer> m_Probabilities;
  DynArray<OutputImagePointer> m_Images;
  OutputImagePointer m_AffineTemplate;

  DynArray<InternalImagePointer> m_InputImages;

// HACK
  OutputImagePointer m_OtherAffineTemplate;

  bool m_UseNonLinearInterpolation;

  FlagArrayType m_AffineTransformReadFlags;

  bool m_DoneRegistration;
  bool m_DoneResample;

  float m_OutsideFOVCode;
  ByteImagePointer m_FOVMask;

  bool m_Modified;

  std::string m_PrefilteringMethod;
  unsigned int m_PrefilteringIterations;
  double m_PrefilteringTimeStep;

  LinearTransformChoice m_AtlasLinearTransformChoice;
  LinearTransformChoice m_ImageLinearTransformChoice;

  bool m_FastRegistration;

};

#ifndef MU_MANUAL_INSTANTIATION
#include "AtlasMRBRegistrationMethod.txx"
#endif

#endif
