
#include <vtkActor.h>
#include <vtkAxes.h>
#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtksys/SystemTools.hxx>

#include <vtkNamedColors.h>

namespace
{
vtkSmartPointer<vtkPolyData> ReadPolyData(const char* fileName);
void RotateX(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor);
void RotateY(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor);
void RotateZ(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor);
void RotateXY(vtkSmartPointer<vtkRenderWindow>& renWin,
              vtkSmartPointer<vtkActor>& actor);
}

int main(int argc, char* argv[])
{
  /*
  To match the illustrations in VTKTextbook.pdf, use BkgColor as the background
    and Wheat as the cow colour.
  Also comment out the lines:
    modelActor->GetProperty()->SetSpecular(.6);
    modelActor->GetProperty()->SetSpecularPower(30);
  and use cow.g as the inout data.
  */

  auto Scale = [](std::vector<double>& v, double scale) {
    std::transform(std::begin(v), std::end(v), std::begin(v),
                   [=](double const& n) { return n / scale; });
    return;
  };

  int figure = 0;
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " filename [figure]" << std::endl;
    std::cout << "where: filename is the file cow.obj" << std::endl;
    std::cout << "       figure is 0, 1, 2, or 3, default 0" << std::endl;
    return EXIT_FAILURE;
  }

  std::string fileName = argv[1];
  if (argc == 3)
  {
    figure = atoi(argv[2]);
  }

  // Create renderer stuff
  //
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();

  // Set the background color. Match those in VTKTextbook.pdf.
  auto scale = 256.0;
  std::vector<double> bkg = {60, 93, 144};

  Scale(bkg, scale);
  colors->SetColor("BkgColor", bkg[0], bkg[1], bkg[2]);

  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  // create pipeline
  //
  vtkSmartPointer<vtkPolyData> polyData = ReadPolyData(fileName.c_str());

  vtkSmartPointer<vtkPolyDataMapper> modelMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  modelMapper->SetInputData(polyData);

  vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();
  modelActor->SetMapper(modelMapper);
  // modelActor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Wheat").GetData());
  modelActor->GetProperty()->SetDiffuseColor(
    colors->GetColor3d("Crimson").GetData());
  modelActor->GetProperty()->SetSpecular(.6);
  modelActor->GetProperty()->SetSpecularPower(30);

  vtkSmartPointer<vtkAxes> modelAxesSource = vtkSmartPointer<vtkAxes>::New();
  modelAxesSource->SetScaleFactor(10);
  modelAxesSource->SetOrigin(0, 0, 0);

  vtkSmartPointer<vtkPolyDataMapper> modelAxesMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  modelAxesMapper->SetInputConnection(modelAxesSource->GetOutputPort());

  vtkSmartPointer<vtkActor> modelAxes = vtkSmartPointer<vtkActor>::New();
  modelAxes->SetMapper(modelAxesMapper);

  ren1->AddActor(modelAxes);
  modelAxes->VisibilityOff();

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(modelActor);
  // ren1->SetBackground(colors->GetColor3d("BkgColor").GetData());
  ren1->SetBackground(colors->GetColor3d("Silver").GetData());
  renWin->SetSize(640, 480);
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Azimuth(0);
  ren1->GetActiveCamera()->SetClippingRange(.1, 1000.0);

  modelAxes->VisibilityOn();

  renWin->Render();
  renWin->Render();

  switch (figure)
  {
    default:
    case 0:
      RotateXY(renWin, modelActor);
      break;
    case 1:
      RotateX(renWin, modelActor);
      break;
    case 2:
      RotateY(renWin, modelActor);
      break;
    case 3:
      RotateZ(renWin, modelActor);
      break;
  }
  renWin->EraseOff();
  iren->Start();

  return EXIT_SUCCESS;
}

namespace
{
//
void RotateX(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor)
{
  actor->SetOrientation(0, 0, 0);
  renWin->Render();
  renWin->Render();
  renWin->EraseOff();

  for (int i = 0; i < 6; ++i)
  {
    actor->RotateX(60);
    renWin->Render();
    renWin->Render();
  }
  renWin->EraseOn();
}
void RotateY(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor)
{
  actor->SetOrientation(0, 0, 0);
  renWin->Render();
  renWin->EraseOff();

  for (int i = 0; i < 6; ++i)
  {
    actor->RotateY(60);
    renWin->Render();
    renWin->Render();
  }
  renWin->EraseOn();
}
void RotateZ(vtkSmartPointer<vtkRenderWindow>& renWin,
             vtkSmartPointer<vtkActor>& actor)
{
  actor->SetOrientation(0, 0, 0);
  renWin->Render();
  renWin->EraseOff();

  for (int i = 0; i < 6; ++i)
  {
    actor->RotateZ(60);
    renWin->Render();
    renWin->Render();
  }
  renWin->EraseOn();
}
void RotateXY(vtkSmartPointer<vtkRenderWindow>& renWin,
              vtkSmartPointer<vtkActor>& actor)
{
  actor->SetOrientation(0, 0, 0);
  actor->RotateX(60);
  renWin->Render();
  renWin->Render();
  renWin->EraseOff();

  for (int i = 0; i < 6; ++i)
  {
    actor->RotateY(60);
    renWin->Render();
    renWin->Render();
  }
  renWin->EraseOn();
}
vtkSmartPointer<vtkPolyData> ReadPolyData(const char* fileName)
{
  vtkSmartPointer<vtkPolyData> polyData;
  std::string extension =
    vtksys::SystemTools::GetFilenameExtension(std::string(fileName));
  if (extension == ".ply")
  {
    vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtp")
  {
    vtkSmartPointer<vtkXMLPolyDataReader> reader =
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".obj")
  {
    vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".stl")
  {
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtk")
  {
    vtkSmartPointer<vtkPolyDataReader> reader =
      vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".g")
  {
    vtkSmartPointer<vtkBYUReader> reader = vtkSmartPointer<vtkBYUReader>::New();
    reader->SetGeometryFileName(fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else
  {
    vtkSmartPointer<vtkSphereSource> source =
      vtkSmartPointer<vtkSphereSource>::New();
    source->Update();
    polyData = source->GetOutput();
  }
  return polyData;
}
}
