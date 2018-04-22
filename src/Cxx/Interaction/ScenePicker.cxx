#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkConeSource.h>
#include <vtkRendererCollection.h>
#include <vtkCellArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkScenePicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// Handle mouse events
// class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
// {
// public:
//   static MouseInteractorHighLightActor* New();
//   vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);
//
//   MouseInteractorHighLightActor()
//   {
//     LastPickedActor = NULL;
//     LastPickedProperty = vtkProperty::New();
//   }
//   virtual ~MouseInteractorHighLightActor()
//   {
//     LastPickedProperty->Delete();
//   }
//   virtual void OnLeftButtonDown()
//   {
//     int* clickPos = this->GetInteractor()->GetEventPosition();
//
//     // Pick from this location.
//     vtkSmartPointer<vtkPropPicker>  picker =
//       vtkSmartPointer<vtkPropPicker>::New();
//     picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
//
//     // If we picked something before, reset its property
//     if (this->LastPickedActor)
//       {
//       this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
//       }
//     this->LastPickedActor = picker->GetActor();
//     if (this->LastPickedActor)
//       {
//       // Save the property of the picked actor so that we can
//       // restore it next time
//       this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
//       // Highlight the picked actor by changing its properties
//       this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
//       this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
//       this->LastPickedActor->GetProperty()->SetSpecular(0.0);
//       }
//
//     // Forward events
//     vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
//   }
//
// private:
//   vtkActor    *LastPickedActor;
//   vtkProperty *LastPickedProperty;
// };


// Handle mouse events
class MouseInteractorStyle3 : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorStyle3* New();
  vtkTypeMacro(MouseInteractorStyle3, vtkInteractorStyleTrackballCamera);

  MouseInteractorStyle3()
  {
    LastPickedActor = NULL;
    LastPickedProperty = vtkProperty::New();
  }

  virtual ~MouseInteractorStyle3()
  {
    LastPickedProperty->Delete();
  }

  virtual void OnMouseMove()
  {
    int* clickPos = this->GetInteractor()->GetEventPosition();

    // Pick from this location.
    vtkSmartPointer<vtkScenePicker>  picker = vtkSmartPointer<vtkScenePicker>::New();
    picker->EnableVertexPickingOff();
    picker->SetRenderer(this->GetDefaultRenderer());

    int e[2];
    this->GetInteractor()->GetEventPosition(e);
    cout << "DisplayPosition : (" << e[0] << "," << e[1] << ")" << endl
    << " Prop: "     << picker->GetViewProp(e) << endl
    << " CellId: "   << picker->GetCellId(e) << endl
    << endl;

    // If we picked something before, reset its property
    if (this->LastPickedActor)
    {
      this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
      this->LastPickedActor = NULL;
    }
    this->LastPickedActor = vtkActor::SafeDownCast (picker->GetViewProp(e));
    if (this->LastPickedActor)
    {
      // Save the property of the picked actor so that we can
      // restore it next time
      this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
      // Highlight the picked actor by changing its properties
      this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
      this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
      this->LastPickedActor->GetProperty()->SetSpecular(0.0);
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }

private:
  vtkActor    *LastPickedActor;
  vtkProperty *LastPickedProperty;

};

vtkStandardNewMacro(MouseInteractorStyle3);

// Execute application.
int main(int, char *[])
{
  // create a sphere source, mapper, and actor
  vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphere->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);

  // create the spikes by glyphing the sphere with a cone.  Create the
  // mapper and actor for the glyphs.
  vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
  vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
  glyph->SetInputConnection(sphere->GetOutputPort());
  glyph->SetSourceConnection(cone->GetOutputPort());
  glyph->SetVectorModeToUseNormal();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(0.25);
  vtkSmartPointer<vtkPolyDataMapper> spikeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  spikeMapper->SetInputConnection(glyph->GetOutputPort());
  vtkSmartPointer<vtkActor> spikeActor = vtkSmartPointer<vtkActor>::New();
  spikeActor->SetMapper(spikeMapper);

  // A renderer and render window
  vtkSmartPointer<vtkRenderer> renderer =
  vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
  vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer ( renderer );

  // An interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow ( renderWindow );

  // Set the custom stype to use for interaction.
  vtkSmartPointer<MouseInteractorStyle3> style =
  vtkSmartPointer<MouseInteractorStyle3>::New();
  style->SetDefaultRenderer(renderer);

  renderWindowInteractor->SetInteractorStyle( style );

  // Add the actors to the scene
  renderer->AddActor ( sphereActor );
  renderer->AddActor ( spikeActor );
  renderer->SetBackground ( 0,0,1 );

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
