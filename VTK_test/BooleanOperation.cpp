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

#define VTK_DATA_ROOT "E:\\myStudySpace\\vtk\\examples\\resource\\STL\\"

vtkActor* GetBooleanOperationActor(double x, int operation)
{
	//��ȡstl����
	vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
	reader1->SetFileName(VTK_DATA_ROOT "ear.stl");
	reader1->Update();
	vtkSmartPointer<vtkSTLReader> reader2 = vtkSmartPointer<vtkSTLReader>::New();
	reader2->SetFileName(VTK_DATA_ROOT "̫��.stl");
	reader2->Update();

	//double centerSeparation = 0.20;
	//vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper1->SetInputConnection(reader1->GetOutputPort());
	//vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	//actor1->SetMapper(mapper1);
	//actor1->SetPosition(x + centerSeparation, 0, 0);
	//vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputConnection(reader2->GetOutputPort());
	//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(mapper2);
	//actor2->SetPosition(x - centerSeparation, 0, 0);

	vtkSmartPointer<vtkTriangleFilter> triFilter1 = vtkSmartPointer<vtkTriangleFilter>::New();
	triFilter1->SetInputData(reader1->GetOutput());
	triFilter1->Update();
	vtkSmartPointer<vtkTriangleFilter> triFilter2 = vtkSmartPointer<vtkTriangleFilter>::New();
	triFilter2->SetInputData(reader2->GetOutput());
	triFilter2->Update();

	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> boolFilter = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
	boolFilter->SetInputConnection(0, triFilter1->GetOutputPort());//boolFilter��������reader1��reader2�����
	boolFilter->SetInputConnection(1, triFilter2->GetOutputPort());
	boolFilter->SetOperation(operation);
	boolFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();//������������Ⱦ����εļ�������
	mapper->SetInputConnection(boolFilter->GetOutputPort());//mapper��������boolFilter�����
	mapper->ScalarVisibilityOff();

	vtkActor* actor = vtkActor::New();
	actor->SetMapper(mapper);

	return actor;
}

int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();//������Ⱦ����
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renWinInteractor->SetRenderWindow(renWin);//������꣬���̼���

	//vtkActor* unionActor = GetBooleanOperationActor(-2.0, vtkBooleanOperationPolyDataFilter::VTK_UNION);//��
	//renderer->AddActor(unionActor);//�������Ӳ�������
	//unionActor->Delete();

	//vtkActor* intersectionActor = GetBooleanOperationActor(0.0, vtkBooleanOperationPolyDataFilter::VTK_INTERSECTION);//��
	//renderer->AddActor(intersectionActor);//�������ӽ�������
	//intersectionActor->Delete();

	vtkActor* differenceActor = GetBooleanOperationActor(2.0, vtkBooleanOperationPolyDataFilter::VTK_DIFFERENCE);//��
	renderer->AddActor(differenceActor);//�������Ӳ����
	differenceActor->Delete();

	renWin->Render();
	renWinInteractor->Start();//�����¼�ѭ����Ӧ

	return EXIT_SUCCESS;
}