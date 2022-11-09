#include "qte.h"
#include "implicits.h"
#include "../UI/ui_interface.h"
#include "timer.h"

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets) {
    // Chargement de l'interface
    uiw->setupUi(this);

    // Chargement du GLWidget
    meshWidget = new MeshWidget;
    QGridLayout *GLlayout = new QGridLayout;
    GLlayout->addWidget(meshWidget, 0, 0);
    GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

    // Creation des connect
    CreateActions();

    meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow() {
    delete meshWidget;
}

void MainWindow::CreateActions() {
    // Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->sphereMesh, SIGNAL(clicked()), this, SLOT(SphereMeshExample()));
    connect(uiw->cylinderMesh, SIGNAL(clicked()), this, SLOT(CylinderMeshExample()));
    connect(uiw->capsuleMesh, SIGNAL(clicked()), this, SLOT(CapsuleMeshExample()));
    connect(uiw->torusMesh, SIGNAL(clicked()), this, SLOT(TorusMeshExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

    // Widget edition
    connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray &) {
}

void MainWindow::editingSceneRight(const Ray &) {
}

void MainWindow::BoxMeshExample() {
    Mesh boxMesh = Mesh(Box(1.0));

    //TEST AREA
    AnalyticScalarField implicit;

    Mesh implicitMesh;
    implicit.Polygonize(31, implicitMesh, Box(2.0));

    boxMesh.Scale(2, 1, 1);
    boxMesh.Rotate(Math::DegreeToRadian(32), Vector::Y);
    boxMesh.Translate(Vector(4, 0, 0));
    boxMesh.Merge(implicitMesh);
    Sphere warp = Sphere(Vector(2, 0, -2), 3);
    //boxMesh.SphereWarp(Sphere(Vector(0, 0, 2), 1.5), Normalized(Vector(0, 0, 1)));
    std::vector<double> buff = boxMesh.SphereWarp(warp, Normalized(Vector(1, 0, 1)) * 2);


    //

    std::vector<Color> cols;
    cols.resize(boxMesh.Vertexes());
    /*
    for (int i = 0; i < cols.size(); i++)
        cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);
        */
    /*
    for (auto &col: cols)
        col = Color(0.8, 0.8, 0.8);
    */

    for (int i = 0; i < cols.size(); ++i) {
        cols[i] = Color(buff[i], buff[i], buff[i]);
    }

    meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::SphereImplicitExample() {
    Mesh implicitMesh(Sphere(Vector(), 3), 64);
    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::UpdateGeometry() {
    meshWidget->ClearAll();
    if (uiw->generateDebug->isChecked()) {
        meshColor.DebugVertices();
    }
    meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

    UpdateMaterial();
}

void MainWindow::UpdateMaterial() {
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
        meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
    else
        meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera() {
    meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}

void MainWindow::SphereMeshExample() {
    Mesh mesh(Sphere(Vector(), uiw->sphereSize->text().toDouble()), uiw->sphereAcc->text().toUInt());
    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::CylinderMeshExample() {
    Timer timer;
    timer.Start();
    Mesh mesh(Cylinder(
            Vector(),
    uiw->cylinderHeight->text().toDouble(),
            uiw->cylinderRadius->text().toDouble()),
    uiw->cylinderAccuracy->text().toUInt(),
            uiw->cylinderFloors->text().toUInt());
    timer.Stop();
    uiw->lineGenTime->setText(QString::number(timer.ElapsedMicroSeconds(), 'G', 6));

    //uiw->lineGenTime->setText(QString::number(0.542));

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::CapsuleMeshExample() {
    Timer timer;
    timer.Start();
    Mesh mesh(Capsule(
            Vector(),
    uiw->capsuleHeight->text().toDouble(),
            uiw->capsuleRadius->text().toDouble()),
    uiw->capsuleAccuracy->text().toUInt(),
            uiw->capsuleFloors->text().toUInt());
    timer.Stop();

    //mesh.DebugVertices();

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMicroSeconds(), 'G', 6));

    //uiw->lineGenTime->setText(QString::number(0.542));

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TorusMeshExample() {
    Mesh mesh(Torus(Vector::Null, uiw->torusAlpha->text().toDouble(), uiw->torusBeta->text().toDouble()),
              uiw->torusAccuracy->text().toUInt());

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}
