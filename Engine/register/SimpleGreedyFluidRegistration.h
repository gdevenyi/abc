

/*
  Simplified greedy fluid warping with L2 norm
*/

#ifndef _SimpleGreedyFluidRegistration_h
#define _SimpleGreedyFluidRegistration_h

#include "itkImage.h"
#include "itkVector.h"

#include <vector>

template <class TPixel, unsigned int Dimension>
class SimpleGreedyFluidRegistration : public itk::Object
{

public:

  /** Standard class typedefs. */
  typedef SimpleGreedyFluidRegistration  Self;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef itk::Image<TPixel, Dimension> ImageType;
  typedef typename ImageType::Pointer ImagePointer;

  typedef itk::Vector<float, Dimension> DisplacementType;
  typedef itk::Image<DisplacementType, 3> DeformationFieldType;
  typedef typename DeformationFieldType::Pointer DeformationFieldPointer;

  void SetFixedImages(const std::vector<ImagePointer>& images);
  void SetMovingImages(const std::vector<ImagePointer>& images);

  itkSetMacro(KernelWidth, double);
  itkSetMacro(Iterations, unsigned int);
  itkSetMacro(TimeStep, double);

  DeformationFieldPointer GetDeformation();

  std::vector<ImagePointer> GetOutputImages();

  void Update();

protected:

  SimpleGreedyFluidRegistration();
  ~SimpleGreedyFluidRegistration();

  void Step();

  unsigned int m_Iterations;
  double m_TimeStep;

  std::vector<ImagePointer> m_FixedImages;
  std::vector<ImagePointer> m_MovingImages;

  std::vector<ImagePointer> m_OutputImages;

  DeformationFieldPointer m_Deformation;

  bool m_Modified;

};

#endif
