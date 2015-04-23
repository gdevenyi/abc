
////////////////////////////////////////////////////////////////////////////////
//
// Bias field estimation using linear least squares polynomial fitting.
// Requires input image intensities and probability estimates for each voxel.
//
// Corrections are done either on subsampled grid or full resolution.
// This is designed to be run iteratively. Corrections should not be
// accumulated. Always use original image as the input, otherwise may get
// strange results.
//
// Van Leemput K, Maes F, Vandermeulen D, Suetens P. Automated model based
// bias field correction of MR images of the brain. IEEE TMI 1999; 18:885-896.
//
////////////////////////////////////////////////////////////////////////////////

// prastawa@cs.unc.edu 3/2004

#ifndef _LLSBiasCorrector_h
#define _LLSBiasCorrector_h

#include "itkImage.h"
#include "itkObject.h"

#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"
#include "vnl/algo/vnl_matrix_inverse.h"
#include "vnl/algo/vnl_qr.h"
#include "vnl/algo/vnl_svd.h"

#include "DynArray.h"

template <class TInputImage, class TProbabilityImage>
class LLSBiasCorrector : public itk::Object
{

public:

  /** Standard class typedefs. */
  typedef LLSBiasCorrector Self;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** The dimension of the image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  // Image types
  typedef TInputImage InputImageType;
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TInputImage::IndexType InputImageIndexType;
  typedef typename TInputImage::PixelType InputImagePixelType;
  typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TInputImage::SizeType InputImageSizeType;
  typedef typename TInputImage::SpacingType InputImageSpacingType;

  typedef itk::Image<unsigned char, 3> MaskImageType;
  typedef MaskImageType::Pointer MaskImagePointer;
  typedef MaskImageType::IndexType MaskImageIndexType;
  typedef MaskImageType::PixelType MaskImagePixelType;
  typedef MaskImageType::RegionType MaskImageRegionType;
  typedef MaskImageType::SizeType MaskImageSizeType;

  typedef TProbabilityImage ProbabilityImageType;
  typedef typename ProbabilityImageType::Pointer ProbabilityImagePointer;
  typedef typename ProbabilityImageType::IndexType ProbabilityImageIndexType;
  typedef typename ProbabilityImageType::PixelType ProbabilityImagePixelType;
  typedef typename ProbabilityImageType::RegionType ProbabilityImageRegionType;
  typedef typename ProbabilityImageType::SizeType ProbabilityImageSizeType;

  typedef itk::Image<float, 3> InternalImageType;
  typedef InternalImageType::Pointer InternalImagePointer;
  typedef InternalImageType::IndexType InternalImageIndexType;
  typedef InternalImageType::PixelType InternalImagePixelType;
  typedef InternalImageType::RegionType InternalImageRegionType;
  typedef InternalImageType::SizeType InternalImageSizeType;

  typedef float ScalarType;

  typedef vnl_matrix<ScalarType> MatrixType;
  typedef vnl_vector<ScalarType> VectorType;

  typedef vnl_matrix_inverse<ScalarType> MatrixInverseType;
  typedef vnl_qr<ScalarType> MatrixQRType;
  typedef vnl_svd<ScalarType> MatrixSVDType;

  // The maximum polynomial degree of the bias field estimate
  void SetMaxDegree(unsigned int);
  itkGetMacro(MaxDegree, unsigned int);

  // Spacing for determining voxels in LLS
  void SetSampleSpacing(float s);
  itkGetMacro(SampleSpacing, float);

  // Spacing for determining which voxels need to be updated
  // if correction is not done at full resolution
  itkSetMacro(WorkingSpacing, float);
  itkGetMacro(WorkingSpacing, float);

  itkSetMacro(ClampBias, bool);
  itkGetMacro(ClampBias, bool);

  void SetAdditive() { m_DoLog = false; }
  void SetMultiplicative() { m_DoLog = true; }

  // Bias field max magnitude
  itkSetMacro(MaximumBiasMagnitude, float);
  itkGetMacro(MaximumBiasMagnitude, float);

  void SetMask(MaskImageType* mask);
  void SetProbabilities(DynArray<ProbabilityImagePointer> probs);

  void SetMeans(const MatrixType& mu);
  void SetCovariances(const DynArray<MatrixType>& covars);

  // Reference class index, the mean intensity of this class is assumed
  // fix so corrected image range is similar to input image range
  // Default is 0
  itkSetMacro(ReferenceClassIndex, unsigned int);
  itkGetMacro(ReferenceClassIndex, unsigned int);

  static InternalImagePointer LogMap(InputImagePointer img);
  static InternalImagePointer ExpMap(InputImagePointer img);
  
  // Correct input images and write it to the designated output
  // fullRes flag selects whether to correct whole image or just grid points
  // defined by WorkingSpacing
  void CorrectImages(
    DynArray<InputImagePointer>& inputs,
    DynArray<InputImagePointer>& outputs,
    bool fullRes=true);

  // Obtain the bias fields after correction, must call CorrectImages() first
  DynArray<InternalImagePointer> GetLogBiasFields()
  { return m_LogBiasFields; }

protected:

  LLSBiasCorrector();
  ~LLSBiasCorrector();

  void CheckInputs();

  // Compute distributions on log transformed intensities
  void ComputeLogDistributions();

private:

  DynArray<InputImagePointer> m_InputImages;

  DynArray<ProbabilityImagePointer> m_Probabilities;

  DynArray<InternalImagePointer> m_LogBiasFields;

  bool m_DoLog;

  unsigned int m_MaxDegree;

  float m_SampleSpacing;
  float m_WorkingSpacing;

  bool m_ClampBias;
  float m_MaximumBiasMagnitude;

  MatrixType m_Means;
  DynArray<MatrixType> m_Covariances;

  MaskImagePointer m_Mask;

  MatrixType m_Basis;

  unsigned int m_ReferenceClassIndex;

  // Coordinate scaling and offset, computed from input probabilities
  // for preconditioning the polynomial basis equations
  float m_XMu[3];
  float m_XStd[3];

};

#ifndef MU_MANUAL_INSTANTIATION
#include "LLSBiasCorrector.txx"
#endif

#endif
