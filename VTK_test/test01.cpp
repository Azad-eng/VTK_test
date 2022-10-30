#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
#include <vtkActor.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>
#include <vtkTriangleFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkSTLWriter.h>

#define VTK_DATA_ROOT "E:\\myStudySpace\\vtk\\examples\\resource\\STL\\"

vtkActor* GetBooleanOperationActor(double x, int operation)
{

    //¶ÁÈ¡stlÊý¾Ý
    vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
    reader1->SetFileName(VTK_DATA_ROOT "¿ÕÐÄÔ²Öù1.stl");
    reader1->Update();
    vtkSmartPointer<vtkPolyData> polyData1;
    polyData1 = reader1->GetOutput();
    vtkNew<vtkTriangleFilter> tri1;
    tri1->SetInputData(polyData1);
    vtkNew<vtkCleanPolyData> clean1;
    clean1->SetInputConnection(tri1->GetOutputPort());
    clean1->Update();

    vtkSmartPointer<vtkSTLReader> reader2 = vtkSmartPointer<vtkSTLReader>::New();
    reader2->SetFileName(VTK_DATA_ROOT "¿ÕÐÄÔ²Öù2.stl");
    reader2->Update();
    vtkSmartPointer<vtkPolyData> polyData2;
    polyData2 = reader2->GetOutput();
    vtkNew<vtkTriangleFilter> tri2;
    tri2->SetInputData(polyData2);
    vtkNew<vtkCleanPolyData> clean2;
    clean2->SetInputConnection(tri2->GetOutputPort());
    clean2->Update();


    vtkSmartPointer<vtkBooleanOperationPolyDataFilter> boolFilter =
        vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
    boolFilter->SetOperation(operation);
    boolFilter->SetInputData(0, clean1->GetOutput());
    boolFilter->SetInputData(1, clean2->GetOutput());
    boolFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(boolFilter->GetOutputPort(0));
    mapper->ScalarVisibilityOff();

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);

    vtkNew<vtkSTLWriter> stlWriter;
    stlWriter->SetFileName(VTK_DATA_ROOT  "¿ÕÐÄÔ²Öù3.stl");
    stlWriter->SetInputConnection(boolFilter->GetOutputPort());
    stlWriter->Write();

    return actor;
}


int main(int argc, char* argv[])
{
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renWin =
        vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renWinInteractor->SetRenderWindow(renWin);

   /* vtkActor* unionActor =
        GetBooleanOperationActor(-2.0, vtkBooleanOperationPolyDataFilter::VTK_UNION);
    renderer->AddActor(unionActor);
    unionActor->Delete();*/

    //vtkActor* intersectionActor =
    //    GetBooleanOperationActor(0.0, vtkBooleanOperationPolyDataFilter::VTK_INTERSECTION);
    //renderer->AddActor(intersectionActor);
    //intersectionActor->Delete();

    vtkActor* differenceActor =
        GetBooleanOperationActor(2.0, vtkBooleanOperationPolyDataFilter::VTK_DIFFERENCE);
    renderer->AddActor(differenceActor);
    differenceActor->Delete();


    renWin->Render();
    renWinInteractor->Start();

    return EXIT_SUCCESS;
}