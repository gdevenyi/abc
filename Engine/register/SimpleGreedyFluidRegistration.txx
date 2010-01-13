
#ifndef _SimpleGreedyFluidRegistration_txx
#define _SimpleGreedyFluidRegistration_txx

#include "itkDerivativeImageFilter.h"
#include "itkImageDuplicator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkWarpImageFilter.h"
#include "itkWarpVectorImageFilter.h"

#include "VectorBlurImageFilter.h"

template <class TPixel, unsigned int Dimension>
SimpleGreedyFluidRegistration<TPixel, Dimension>
::SimpleGreedyFluidRegistration()
{
  m_Iterations = 10;
  m_MaxStep = 0.5;
  m_Delta = 0.0;
  m_KernelWidth = 1.0;
  m_Modified = false;
}

template <class TPixel, unsigned int Dimension>
SimpleGreedyFluidRegistration<TPixel, Dimension>
::~SimpleGreedyFluidRegistration()
{
}

template <class TPixel, unsigned int Dimension>
void
SimpleGreedyFluidRegistration<TPixel, Dimension>
::SetFixedImages(const DynArray<ImagePointer>& images)
{
  m_FixedImages = images;
  m_Modified = true;
}

template <class TPixel, unsigned int Dimension>
void
SimpleGreedyFluidRegistration<TPixel, Dimension>
::SetMovingImages(const DynArray<ImagePointer>& images)
{
  m_MovingImages = images;
  m_Modified = true;
}

template <class TPixel, unsigned int Dimension>
void
SimpleGreedyFluidRegistration<TPixel, Dimension>
::SetMask(MaskPointer m)
{
  m_Mask = m;
  m_Modified = true;
}

template <class TPixel, unsigned int Dimension>
void
SimpleGreedyFluidRegistration<TPixel, Dimension>
::Update()
{
  if (!m_Modified)
    return;

  // Initialize out = input moving
  m_OutputImages.Clear();
  for (unsigned int c = 0; c < m_MovingImages.GetSize(); c++)
  {
    typedef itk::ImageDuplicator<ImageType> DuperType;
    typename DuperType::Pointer dupef = DuperType::New();
    dupef->SetInputImage(m_MovingImages[c]);
    dupef->Update();
    m_OutputImages.Append(dupef->GetOutput());
  }

  // Initialize phi(x) = x
  m_DeformationField = DeformationFieldType::New();
  //m_DeformationField->CopyInformation(m_FixedImages[0]);
  m_DeformationField->SetDirection(m_FixedImages[0]->GetDirection());
  m_DeformationField->SetOrigin(m_FixedImages[0]->GetOrigin());
  m_DeformationField->SetSpacing(m_FixedImages[0]->GetSpacing());
  m_DeformationField->SetRegions(m_FixedImages[0]->GetLargestPossibleRegion());
  m_DeformationField->Allocate();

  DisplacementType zerov;
  zerov.Fill(0.0);
  m_DeformationField->FillBuffer(zerov);

  typedef itk::ImageRegionIteratorWithIndex<DeformationFieldType> IteratorType;
  IteratorType it(m_DeformationField, m_DeformationField->GetLargestPossibleRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    ImagePointType p;
    m_DeformationField->TransformIndexToPhysicalPoint(it.GetIndex(), p);

    DisplacementType v;
    for (unsigned int i = 0; i < Dimension; i++)
      v[i] = p[i];
    it.Set(v);
  }

  // Store displacement field as well
  m_DisplacementField = DeformationFieldType::New();
  //m_DisplacementField->CopyInformation(m_FixedImages[0]);
  m_DisplacementField->SetDirection(m_FixedImages[0]->GetDirection());
  m_DisplacementField->SetOrigin(m_FixedImages[0]->GetOrigin());
  m_DisplacementField->SetSpacing(m_FixedImages[0]->GetSpacing());
  m_DisplacementField->SetRegions(m_FixedImages[0]->GetLargestPossibleRegion());
  m_DisplacementField->Allocate();
  m_DisplacementField->FillBuffer(zerov);

  // Initialize delta
  m_Delta = 0.0;

  // Greedy optimization
  for (unsigned int i = 0; i < m_Iterations; i++)
  {
    this->Step();
  }

  m_Modified = false;
}

template <class TPixel, unsigned int Dimension>
void
SimpleGreedyFluidRegistration<TPixel, Dimension>
::Step()
{
  unsigned int numChannels = m_FixedImages.GetSize();

  // Find scale adjustment
  ImageSpacingType spacing = m_FixedImages[0]->GetSpacing();
  double minSpacing = spacing[0];
  for (unsigned int i = 1; i < Dimension; i++)
    if (spacing[i] < minSpacing)
      minSpacing = spacing[i];

  DisplacementType zerov;
  zerov.Fill(0.0);

  // Compute velocity field
  // v = sum_c { (fixed_c - moving_c) * grad(moving_c) }
  DeformationFieldPointer velocF = DeformationFieldType::New();
  //velocF->CopyInformation(m_FixedImages[0]);
  velocF->SetDirection(m_FixedImages[0]->GetDirection());
  velocF->SetOrigin(m_FixedImages[0]->GetOrigin());
  velocF->SetSpacing(m_FixedImages[0]->GetSpacing());
  velocF->SetRegions(m_FixedImages[0]->GetLargestPossibleRegion());
  velocF->Allocate();
  velocF->FillBuffer(zerov);

  typedef itk::ImageRegionIteratorWithIndex<DeformationFieldType> IteratorType;
  IteratorType it(velocF, velocF->GetLargestPossibleRegion());

  for (unsigned int ichan = 0; ichan < numChannels; ichan++)
  {
    typedef itk::DerivativeImageFilter<ImageType, ImageType>
      DerivativeFilterType;

    for (unsigned int dim = 0; dim < Dimension; dim++)
    {
      typename DerivativeFilterType::Pointer derivf = DerivativeFilterType::New();
      derivf->SetInput(m_OutputImages[ichan]);
      derivf->SetDirection(dim);
      derivf->SetUseImageSpacingOn();
      derivf->Update();

      ImagePointer grad_d = derivf->GetOutput();

// TODO: use mask?

      for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
        DisplacementType v = it.Get();

        ImageIndexType ind = it.GetIndex();

        double w =
          m_FixedImages[ichan]->GetPixel(ind) -
          m_OutputImages[ichan]->GetPixel(ind);

        v[dim] += w * grad_d->GetPixel(ind);

        it.Set(v);
      }
    } // for dim
  } // for ichan

  if (m_Delta == 0.0)
  {
    double maxVeloc = 0.0;
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
      DisplacementType v = it.Get();
      double d = v.GetNorm();
      if (d > maxVeloc)
        maxVeloc = d;
    }
    m_Delta = m_MaxStep * minSpacing / maxVeloc;
  }

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    it.Set(it.Get() * m_Delta);
  }

  // Apply Green's kernel to velocity field
  typedef VectorBlurImageFilter<DeformationFieldType, DeformationFieldType>
    DeformationSmootherType;

  double adjustedWidth = m_KernelWidth * minSpacing;

  typename DeformationSmootherType::Pointer defsmoother = DeformationSmootherType::New();
  typename DeformationFieldType::SizeType radii;
  radii.Fill(2);
  defsmoother->SetRadius(radii);
  defsmoother->SetVariance(adjustedWidth * adjustedWidth);
  defsmoother->SetInput(velocF);
  defsmoother->Update();

  velocF = defsmoother->GetOutput();

  // Compose velocity field
  // h(x) = h( g(x) ) where g(x) = x + v
  typedef itk::WarpVectorImageFilter<
    DeformationFieldType, DeformationFieldType, DeformationFieldType>
    ComposerType;
  typename ComposerType::Pointer compf = ComposerType::New();
  compf->SetInput(m_DeformationField);
  compf->SetDeformationField(velocF);
  compf->SetOutputDirection(m_DeformationField->GetDirection());
  compf->SetOutputOrigin(m_DeformationField->GetOrigin());
  compf->SetOutputSpacing(m_DeformationField->GetSpacing());
  compf->Update();

  m_DeformationField = compf->GetOutput();

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    ImageIndexType ind = it.GetIndex();

    ImagePointType p;
    m_DeformationField->TransformIndexToPhysicalPoint(ind, p);

    DisplacementType u;
    for (unsigned int i = 0; i < Dimension; i++)
      u[i] = p[i];
    m_DisplacementField->SetPixel(ind,
      m_DeformationField->GetPixel(ind) - u);
  }

  // Warp images
  m_OutputImages.Clear();
  for (unsigned int ichan = 0; ichan < numChannels; ichan++)
  {
    typedef itk::WarpImageFilter<
      ImageType, ImageType, DeformationFieldType>
      WarperType;
    typename WarperType::Pointer warpf = WarperType::New();
    warpf->SetInput(m_MovingImages[ichan]);
    warpf->SetDeformationField(m_DisplacementField);
    warpf->SetOutputDirection(m_DeformationField->GetDirection());
    warpf->SetOutputOrigin(m_DeformationField->GetOrigin());
    warpf->SetOutputSpacing(m_DeformationField->GetSpacing());
    warpf->Update();
    m_OutputImages.Append(warpf->GetOutput());
  }
}

#endif
