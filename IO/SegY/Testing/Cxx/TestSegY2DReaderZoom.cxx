/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestSegY2DReaderZoom.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME Test of vtkSegY2DReader
// .SECTION Description
//

#include "vtkSegY2DReader.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkDataSetMapper.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkMathUtilities.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkStructuredGrid.h"
#include "vtkTestUtilities.h"

int TestSegY2DReaderZoom(int argc, char* argv[])
{
  // Basic visualisation.
  vtkNew<vtkRenderWindow> renWin;
  renWin->SetMultiSamples(0);
  renWin->SetSize(300, 300);
  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin.GetPointer());

  // Read file name.
  char* fname =
    vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/SegY/lineA.sgy");

  vtkNew<vtkSegY2DReader> reader;
  vtkNew<vtkDataSetMapper> mapper;
  vtkNew<vtkActor> actor;

  reader->SetFileName(fname);
  reader->Update();
  delete[] fname;

  int retVal = 0;

  double range[2];
  vtkStructuredGrid* output = reader->GetOutput();

  output->GetScalarRange(range);

  // Test against expected scalar range
  if (!vtkMathUtilities::FuzzyCompare<float>(range[0], -6.421560287))
  {
    std::cerr << "Error: Range[0] != -6.42156" << std::endl
              << "Range[0] = " << range[0] << std::endl;
    retVal++;
  }
  if (!vtkMathUtilities::FuzzyCompare<float>(range[1], 6.616714478))
  {
    std::cerr << "Error: Range[1] != 6.61671" << std::endl
              << "Range[1] = " << range[1] << std::endl;
    retVal++;
  }

  // Test some scalar values
  vtkFloatArray* scalars =
    vtkFloatArray::SafeDownCast(output->GetPointData()->GetScalars());
  float scalar = scalars->GetVariantValue(390 * 39).ToFloat();
  if (!vtkMathUtilities::FuzzyCompare<float>(scalar, 0.0676235f))
  {
    std::cerr << "Error: Trace value for 39th sample is wrong." << std::endl
              << "trace[390*39] = " << std::setprecision(10) << scalar
              << std::endl
              << "Expected trace[390*39] = 0.0676235f" << std::endl;
    retVal++;
  }

  scalar = scalars->GetVariantValue(390 * 390).ToFloat();
  if (!vtkMathUtilities::FuzzyCompare<float>(scalar, 0.6201947331f))
  {
    std::cerr << "Error: Trace value for 390th sample is wrong." << std::endl
              << "trace[390*390] = " << std::setprecision(10) << scalar
              << std::endl
              << "Expected trace[390*390] = 0.620195f" << std::endl;
    retVal++;
  }

  double midrange = 0.5 * (range[0] + range[1]);

  vtkNew<vtkColorTransferFunction> lut;
  lut->AddRGBPoint(range[0], 0.23, 0.30, 0.75);
  lut->AddRGBPoint(midrange, 0.86, 0.86, 0.86);
  lut->AddRGBPoint(range[1], 0.70, 0.02, 0.15);

  mapper->SetInputConnection(reader->GetOutputPort());
  mapper->SetColorModeToMapScalars();
  mapper->SetLookupTable(lut.GetPointer());

  actor->SetMapper(mapper.GetPointer());

  ren->AddActor(actor.GetPointer());
  ren->ResetCamera();

  ren->GetActiveCamera()->Azimuth(90);
  ren->GetActiveCamera()->Zoom(45.0);

  // interact with data
  renWin->Render();

  int regRetVal = vtkRegressionTestImage(renWin);

  if (regRetVal == vtkRegressionTester::DO_INTERACTOR)
  {
    iren->Start();
  }

  retVal += !regRetVal;
  return retVal;
}
