
////////////////////////////////////////////////////////////////////////////////
//
// Atlas based segmentation using the Expectation Maximization algorithm
//
// Designed for 3D MRI
//
// Van Leemput K, Maes F, Vandermeulen D, Suetens P. Automated model based
// tissue classification of MR images of the brain. IEEE TMI 1999; 18:897-908.
//
////////////////////////////////////////////////////////////////////////////////

// prastawa@cs.unc.edu 3/2004

#ifndef _EMSegmentationFilter_h
#define _EMSegmentationFilter_h

#include <string>

#include "itkArray.h"
#include "itkImage.h"
#include "itkObject.h"
#include "itkVector.h"

#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"

#include "vnl/algo/vnl_matrix_inverse.h"

#include "DynArray.h"

#include "PairRegistrationMethod.h"

template <class TInputImage, class TProbabilityImage>
class EMSegmentationFilter: public itk::Object
{

public:

  // Standard class typedefs
  typedef EMSegmentationFilter Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Method for creation through the object factory
  itkNewMacro(Self);

  // The dimension of the image we're working with
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  // Image types
  typedef TInputImage InputImageType;
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TInputImage::IndexType InputImageIndexType;
  typedef typename TInputImage::OffsetType InputImageOffsetType;
  typedef typename TInputImage::PixelType InputImagePixelType;
  typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TInputImage::SizeType InputImageSizeType;
  typedef typename TInputImage::SpacingType InputImageSpacingType;

  typedef itk::Image<unsigned char, itkGetStaticConstMacro(ImageDimension)> ByteImageType;
  typedef typename ByteImageType::Pointer ByteImagePointer;
  typedef typename ByteImageType::IndexType ByteImageIndexType;
  typedef typename ByteImageType::OffsetType ByteImageOffsetType;
  typedef typename ByteImageType::PixelType ByteImagePixelType;
  typedef typename ByteImageType::RegionType ByteImageRegionType;
  typedef typename ByteImageType::SizeType ByteImageSizeType;

  typedef itk::Image<short, itkGetStaticConstMacro(ImageDimension)> ShortImageType;
  typedef typename ShortImageType::Pointer ShortImagePointer;
  typedef typename ShortImageType::IndexType ShortImageIndexType;
  typedef typename ShortImageType::OffsetType ShortImageOffsetType;
  typedef typename ShortImageType::PixelType ShortImagePixelType;
  typedef typename ShortImageType::RegionType ShortImageRegionType;
  typedef typename ShortImageType::SizeType ShortImageSizeType;

  typedef TProbabilityImage ProbabilityImageType;
  typedef typename ProbabilityImageType::Pointer ProbabilityImagePointer;
  typedef typename ProbabilityImageType::IndexType ProbabilityImageIndexType;
  typedef typename ProbabilityImageType::OffsetType ProbabilityImageOffsetType;
  typedef typename ProbabilityImageType::PixelType ProbabilityImagePixelType;
  typedef typename ProbabilityImageType::RegionType ProbabilityImageRegionType;
  typedef typename ProbabilityImageType::SizeType ProbabilityImageSizeType;
  typedef typename ProbabilityImageType::SpacingType ProbabilityImageSpacingType;

  typedef itk::Vector<float, 3> VectorPixelType;
  typedef itk::Image<VectorPixelType, 3> VectorFieldType;
  typedef typename VectorFieldType::Pointer VectorFieldPointer;

  typedef vnl_vector<float> VectorType;
  typedef vnl_matrix<float> MatrixType;
  typedef vnl_matrix_inverse<float> MatrixInverseType;

  typedef typename PairRegistrationMethod<InputImagePixelType>::
    BSplineTransformType::Pointer BSplineTransformPointer;

  // Set/Get the maximum polynomial degree of the bias field estimate
  itkSetMacro(MaxBiasDegree, unsigned int);
  itkGetMacro(MaxBiasDegree, unsigned int);

  itkSetMacro(BiasLikelihoodTolerance, float);
  itkGetMacro(BiasLikelihoodTolerance, float);

  itkSetMacro(LikelihoodTolerance, float);
  itkGetMacro(LikelihoodTolerance, float);

  itkSetMacro(MaximumIterations, unsigned int);
  itkGetMacro(MaximumIterations, unsigned int);

  itkSetMacro(SampleSpacing, float);
  itkGetMacro(SampleSpacing, float);

  void SetInputImages(DynArray<InputImagePointer> data);

  void SetPriors(DynArray<ProbabilityImagePointer> probs);

  void SetPriorWeights(VectorType w);

  unsigned int* GetNumberOfGaussians() { return m_NumberOfGaussians; }
  void SetNumberOfGaussians(unsigned int* n);

  ByteImagePointer GetOutput();

  DynArray<ByteImagePointer> GetBytePosteriors();
  DynArray<ShortImagePointer> GetShortPosteriors();
  DynArray<ProbabilityImagePointer> GetPosteriors();

  DynArray<InputImagePointer> GetCorrected();

  void Update();

  itkGetMacro(FOVMask, ByteImagePointer);
  itkSetMacro(FOVMask, ByteImagePointer);

  itkGetMacro(DoMSTClustering, bool);
  itkSetMacro(DoMSTClustering, bool);

  itkGetMacro(TemplateImage, InputImagePointer);
  itkSetMacro(TemplateImage, InputImagePointer);

  void WarpingOn() { m_DoWarp = true; }
  void WarpingOff() { m_DoWarp = false; }

  itkGetMacro(TemplateBSplineTransform, BSplineTransformPointer);

  itkGetMacro(TemplateFluidMomenta, VectorFieldPointer);
  itkGetMacro(TemplateFluidVelocity, VectorFieldPointer);

  itkGetMacro(WarpedTemplateImage, InputImagePointer);

  itkGetConstMacro(WarpFluidIterations, unsigned int);
  itkSetMacro(WarpFluidIterations, unsigned int);

  itkGetConstMacro(WarpFluidMaxStep, float);
  itkSetMacro(WarpFluidMaxStep, float);

  itkGetConstMacro(WarpFluidKernelWidth, float);
  itkSetMacro(WarpFluidKernelWidth, float);

  itkGetConstMacro(InitialDistributionEstimator, std::string);
  itkSetMacro(InitialDistributionEstimator, std::string);

protected:

  EMSegmentationFilter();
  ~EMSegmentationFilter();

  void CheckInput();

  void DownsampleInputs(float factor);
  void UpsampleOutputs();

  void ComputeMask();
  void ComputePriorLookupTable();

  // Determine the initial parameters of classes with the same prior using
  // MST clustering
  void ClusterFromPriorMST(unsigned int iprior);

  void ClusterFromPrior(unsigned int iprior);

  void ComputeDistributions();
  void ComputeDistributionsRobust(); // Same, but with robust mean

  void ComputePosteriors();

  void CorrectBias(unsigned int degree);

  void EMLoop();

  void ComputeLabels();

  void CleanUp();

  // Returns total log likelihood and normalize the posteriors
  double NormalizePosteriors();

  void ComputeAtlasWarpingFromProbabilities();
  void ComputeAtlasWarpingFromIntensities();

  InputImagePointer DownsampleImage(InputImagePointer img, float factor);
  InputImagePointer RestoreDownsampledImage(
    InputImagePointer img, float defaultValue=0.0);

private:

  DynArray<InputImagePointer> m_InputImages;
  DynArray<InputImagePointer> m_OriginalInputImages;

  DynArray<InputImagePointer> m_CorrectedImages;

  DynArray<InputImagePointer> m_LogBiasFields;

  DynArray<ProbabilityImagePointer> m_Priors;
  DynArray<ProbabilityImagePointer> m_OriginalPriors;
  DynArray<ProbabilityImagePointer> m_DownsampledOriginalPriors;

  DynArray<ProbabilityImagePointer> m_Likelihoods;
  DynArray<ProbabilityImagePointer> m_Posteriors;

  ByteImagePointer m_Mask;

  ByteImagePointer m_Labels;

  bool m_InputModified;

  float m_SampleSpacing;

  unsigned int m_MaxBiasDegree;
  float m_BiasLikelihoodTolerance;
  float m_LikelihoodTolerance;
  unsigned m_MaximumIterations;

  VectorType m_PriorWeights;

  // Number of Gaussian distributions for each associated prior
  unsigned int* m_NumberOfGaussians;

  unsigned int* m_PriorLookupTable;

  MatrixType m_Means;
  DynArray<MatrixType> m_Covariances;

  ByteImagePointer m_FOVMask;

  bool m_DoMSTClustering;

  InputImagePointer m_TemplateImage;
  InputImagePointer m_WarpedTemplateImage;

  ByteImagePointer m_OriginalMask;

  bool m_DoWarp;

  BSplineTransformPointer m_TemplateBSplineTransform;

  VectorFieldPointer m_TemplateFluidMomenta;
  VectorFieldPointer m_TemplateFluidVelocity;

  unsigned int m_WarpFluidIterations;

  float m_WarpFluidMaxStep;

  float m_WarpFluidKernelWidth;

  float m_WarpLikelihoodTolerance;

  std::string m_InitialDistributionEstimator;
};

#ifndef MU_MANUAL_INSTANTIATION
#include "EMSegmentationFilter.txx"
#endif

#endif
