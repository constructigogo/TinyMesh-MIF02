#include "qte.h"
#include "implicits.h"
#include "../UI/ui_interface.h"
#include "timer.h"

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets) {
    // Chargement de l'interface
    uiw->setupUi(this);

    // Chargement du GLWidget
    meshWidget = new MeshWidget;
    QGridLayout * GLlayout = new QGridLayout;
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
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(DemoExample()));
    connect(uiw->sphereMesh, SIGNAL(clicked()), this, SLOT(SphereMeshExample()));
    connect(uiw->cylinderMesh, SIGNAL(clicked()), this, SLOT(CylinderMeshExample()));
    connect(uiw->capsuleMesh, SIGNAL(clicked()), this, SLOT(CapsuleMeshExample()));
    connect(uiw->torusMesh, SIGNAL(clicked()), this, SLOT(TorusMeshExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_3, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

    // Widget edition
    connect(meshWidget, SIGNAL(_signalEditSceneLeft(
                                       const Ray&)), this, SLOT(editingSceneLeft(
                                                                        const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(
                                       const Ray&)), this, SLOT(editingSceneRight(
                                                                        const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray &) {
}

void MainWindow::editingSceneRight(const Ray &) {
}

void MainWindow::BoxMeshExample() {
    Timer timer;
    timer.Start();
    Mesh boxMesh = Mesh(Box(1.0));

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
    std::vector<Color> cols;
    cols.resize(boxMesh.Vertexes());
    for (int i = 0; i < cols.size(); ++i) {
        cols[i] = Color(0.8,0.8,0.8);
    }

    meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    timer.Stop();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));

}

void MainWindow::DemoExample() {
    Timer timer;
    timer.Start();

    Mesh base(Sphere(Vector(0, 0, 5), 3), 32);

    Mesh ring1(Torus(Vector(0, 0, 0), 4, 10), 32);
    ring1.Scale(1, 1, 0.05);
    ring1.Rotate(25, Vector::Y);
    ring1.Translate(Vector(0, 0, 5));

    Mesh ring2(Torus(Vector(0, 0, 0), 2, 10), 32);
    ring2.Scale(1, 1, 0.175);
    ring2.Translate(Vector(0, 0, 1.5));
    ring2.Rotate(25, Vector::Y);
    ring2.Translate(Vector(0, 0, 5));

    Mesh sat(Sphere(Vector(5, 0, 5), 1), 32);

    Mesh pillar(Cylinder(Vector(), 0.8, 8), 32);
    Mesh pillar2(Cylinder(Vector(), 2, 1), 32, 16);

    pillar2.Rotate(Math::DegreeToRadian(90), Vector::X);
    pillar2.Translate(Vector(5, 0, 2));

    Mesh bottom(Sphere(Vector(0, 0, 0), 8), 32);
    bottom.Scale(1,1,0.2);
    bottom.Translate(Vector(0,0,-4.2));
    bottom.SphereWarp(Sphere(Vector(0, 0, -7), 4),Vector(0,0,-2));
    bottom.SphereWarp(Sphere(Vector(0, 0, 0), 7),Vector(0,0,-2));
    bottom.Translate(Vector(0,0,2));


    base.Merge(ring1);
    base.Merge(ring2);
    base.Merge(sat);
    base.Merge(pillar);
    base.Merge(pillar2);
    base.Merge(bottom);

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
    std::vector<Color> cols;
    cols.resize(base.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(base, cols, base.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
}

void MainWindow::UpdateGeometry() {
    Timer renTime;
    renTime.Start();
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
    else if (uiw->radioShadingButton_2->isChecked())
        meshWidget->SetMaterialGlobal(MeshMaterial::Color);
    else if (uiw->radioShadingButton_3->isChecked())
        meshWidget->SetMaterialGlobal(MeshMaterial::AO);
}

void MainWindow::ResetCamera() {
    meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}

void MainWindow::SphereMeshExample() {
    Timer timer;
    timer.Start();
    Mesh mesh(Sphere(Vector(), uiw->sphereSize->text().toDouble()), uiw->sphereAcc->text().toUInt());

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));


    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    timer.Stop();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
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

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    timer.Stop();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
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


    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    timer.Stop();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
}

void MainWindow::TorusMeshExample() {
    Timer timer;
    timer.Start();
    Mesh mesh(Torus(Vector::Null, uiw->torusAlpha->text().toDouble(), uiw->torusBeta->text().toDouble()),
              uiw->torusAccuracy->text().toUInt());

    //mesh.Rotate(25,Vector::Z);

    uiw->lineGenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    int count = 1;
    for (auto &col: cols) {
        if ((count + uiw->torusAccuracy->text().toUInt() / 4) % uiw->torusAccuracy->text().toUInt() >
            uiw->torusAccuracy->text().toUInt() / 2) {
            col = Color(253, 245, 191);
        } else {
            col = Color(255, 213, 255);

        }
        count++;
    }
    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes(), uiw->lineAOAcc->text().toUInt(), uiw->lineAORange->text().toDouble());
    UpdateGeometry();
    timer.Stop();
    uiw->lineRenTime->setText(QString::number(timer.ElapsedMilliSeconds(), 'G'));
}
