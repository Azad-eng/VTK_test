#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCylinder.h"
#include "vtkPlane.h"
#include "vtkImplicitBoolean.h"
#include "vtkPolyDataMapper.h"
#include "vtkSphereSource.h"
#include "vtkProperty.h"
#include "vtkClipPolyData.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkSTLReader.h>

int main()
{
 
    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetCenter(0, 0, 0);
    sphere->SetRadius(10);
    sphere->SetThetaResolution(40);
    sphere->SetPhiResolution(40);

    vtkCylinder* cylinder = vtkCylinder::New();//Ô²Öù
    cylinder->SetCenter(0, 0, 0);
    cylinder->SetRadius(3);

    vtkPlane* vPlane = vtkPlane::New();//ºá½ØÃæ
    vPlane->SetOrigin(0, 0, 0);
    vPlane->SetNormal(0, -1, 0);

    vtkImplicitBoolean* cuted_cylinder = vtkImplicitBoolean::New();
    cuted_cylinder->SetOperationTypeToIntersection();
    cuted_cylinder->AddFunction(cylinder);
    cuted_cylinder->AddFunction(vPlane);

    vtkClipPolyData* clipper = vtkClipPolyData::New();
    clipper->SetInputConnection(sphere->GetOutputPort());
    clipper->SetClipFunction(cylinder);
    clipper->GenerateClipScalarsOn();
    clipper->GenerateClippedOutputOn();
    clipper->SetValue(0.5);

    vtkTransform* transform = vtkTransform::New();
    transform->Translate(7, 0, 0);
    vtkTransformPolyDataFilter* filter = vtkTransformPolyDataFilter::New();
    filter->SetInputConnection(clipper->GetOutputPort());
    filter->SetTransform(transform);
    vtkClipPolyData* clipper2 = vtkClipPolyData::New();
    clipper2->SetInputConnection(filter->GetOutputPort());
    clipper2->SetClipFunction(cuted_cylinder);
    clipper2->GenerateClipScalarsOn();
    clipper2->GenerateClippedOutputOn();
    clipper2->SetValue(0.5);

    vtkPolyDataMapper* map = vtkPolyDataMapper::New();
    map->SetInputConnection(clipper2->GetOutputPort());
    map->ScalarVisibilityOff();

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(map);
    actor->GetProperty()->SetColor(0, 1, 1);
    //actor->GetProperty()->SetAmbientColor(0.4,0.5,0.6);
    //actor->GetProperty()->SetDiffuseColor(0.8,0.6,0.2);
    actor->RotateX(40);

    vtkRenderer* ren = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren);

    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);

    ren->AddActor(actor);
    ren->SetBackground(1, 1, 1);
    renWin->SetSize(450, 450);

    vtkInteractorStyleTrackballCamera* style = vtkInteractorStyleTrackballCamera::New();
    iren->SetInteractorStyle(style);

    iren->Initialize();
    renWin->Render();

    iren->Start();
}