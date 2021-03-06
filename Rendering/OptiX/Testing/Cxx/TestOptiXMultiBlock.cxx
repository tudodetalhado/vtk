/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestOptiXMultiBlock.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// This test verifies that treatment of multiblock data is correct
//
// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit
//              In interactive mode it responds to the keys listed
//              vtkOptiXTestInteractor.h

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCompositeDataSet.h"
#include "vtkCompositePolyDataMapper2.h"
#include "vtkOptiXPass.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkTestUtilities.h"
#include "vtkXMLMultiBlockDataReader.h"

#include "vtkOptiXTestInteractor.h"

int TestOptiXMultiBlock(int argc, char* argv[])
{
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  iren->SetRenderWindow(renWin);
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  renWin->AddRenderer(renderer);

  vtkSmartPointer<vtkXMLMultiBlockDataReader> reader = vtkSmartPointer<vtkXMLMultiBlockDataReader>::New();
  const char* fileName = vtkTestUtilities::ExpandDataFileName(argc, argv,
                                                               "Data/many_blocks/many_blocks.vtm");
  reader->SetFileName(fileName);
  reader->Update();

  vtkSmartPointer<vtkCompositePolyDataMapper2> mapper=vtkSmartPointer<vtkCompositePolyDataMapper2>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor=vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(actor);
  actor->SetMapper(mapper);
  renderer->SetBackground(0.1,0.1,1.0);
  renWin->SetSize(400,400);
  renWin->Render();

  vtkCamera *cam = renderer->GetActiveCamera();
  cam->SetPosition(1.5,1.5,0.75);

  vtkSmartPointer<vtkOptiXPass> optix=vtkSmartPointer<vtkOptiXPass>::New();

  renderer->SetPass(optix);

  renWin->Render();

  vtkSmartPointer<vtkOptiXTestInteractor> style =
    vtkSmartPointer<vtkOptiXTestInteractor>::New();
  style->
    SetPipelineControlPoints((vtkOpenGLRenderer*)renderer.Get(), optix, NULL);
  iren->SetInteractorStyle(style);
  style->SetCurrentRenderer(renderer);

  iren->Start();

  return 0;
}
