#include "mesh.h"

/*!
\class Mesh mesh.h

\brief Core triangle mesh class.
*/



/*!
\brief Initialize the mesh to empty.
*/
Mesh::Mesh() = default;

/*!
\brief Initialize the mesh from a list of vertices and a list of triangles.

Indices must have a size multiple of three (three for triangle vertices and three for triangle normals).

\param vertices List of geometry vertices.
\param indices List of indices wich represent the geometry triangles.
*/
Mesh::Mesh(std::vector<Vector> vertices, std::vector<int> indices) :
        vertices(std::move(vertices)),
        varray(std::move(indices)) {
    normals.resize(Mesh::vertices.size(), Vector::Z);
}

/*!
\brief Create the mesh.

\param vertices Array of vertices.
\param normals Array of normals.
\param va, na Array of vertex and normal indexes.
*/
Mesh::Mesh(std::vector<Vector> vertices,
           std::vector<Vector> normals,
           std::vector<int> va,
           std::vector<int> na) :
        vertices(std::move(vertices)),
        normals(std::move(normals)),
        varray(std::move(va)),
        narray(std::move(na)) {
}

/*!
\brief Reserve memory for arrays.
\param nv,nn,nvi,nvn Number of vertices, normals, vertex indexes and vertex normals.
*/
void Mesh::Reserve(int nv, int nn, int nvi, int nvn) {
    vertices.reserve(nv);
    normals.reserve(nn);
    varray.reserve(nvi);
    narray.reserve(nvn);
}

/*!
\brief Empty
*/
Mesh::~Mesh() = default;

/*!
\brief Smooth the normals of the mesh.

This function weights the normals of the faces by their corresponding area.
\sa Triangle::AreaNormal()
*/
void Mesh::SmoothNormals() {
    // Initialize
    normals.resize(vertices.size(), Vector::Null);

    narray = varray;

    // Accumulate normals
    for (int i = 0; i < varray.size(); i += 3) {
        Vector tn = Triangle(vertices[varray.at(i)], vertices[varray.at(i + 1)],
                             vertices[varray.at(i + 2)]).AreaNormal();
        normals[narray[i + 0]] += tn;
        normals[narray[i + 1]] += tn;
        normals[narray[i + 2]] += tn;
    }

    // Normalize
    for (auto &normal: normals) {
        Normalize(normal);
    }
}

/*!
\brief Add a smooth triangle to the geometry.
\param a, b, c Index of the vertices.
\param na, nb, nc Index of the normals.
*/
void Mesh::AddSmoothTriangle(int a, int na, int b, int nb, int c, int nc) {
    varray.push_back(a);
    narray.push_back(na);
    varray.push_back(b);
    narray.push_back(nb);
    varray.push_back(c);
    narray.push_back(nc);
}

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void Mesh::AddTriangle(int a, int b, int c, int n) {
    varray.push_back(a);
    narray.push_back(n);
    varray.push_back(b);
    narray.push_back(n);
    varray.push_back(c);
    narray.push_back(n);
}

/*!
\brief Add a smooth quadrangle to the geometry.

Creates two smooth triangles abc and acd.

\param a, b, c, d  Index of the vertices.
\param na, nb, nc, nd Index of the normal for all vertices.
*/
void Mesh::AddSmoothQuadrangle(int a, int na, int b, int nb, int c, int nc, int d, int nd) {
    // First triangle
    AddSmoothTriangle(a, na, b, nb, c, nc);

    // Second triangle
    AddSmoothTriangle(a, na, c, nc, d, nd);
}

/*!
\brief Add a quadrangle to the geometry.

\param a, b, c, d  Index of the vertices and normals.
*/
void Mesh::AddQuadrangle(int a, int b, int c, int d) {
    AddSmoothQuadrangle(a, a, b, b, c, c, d, d);
}

/*!
\brief Compute the bounding box of the object.
*/
Box Mesh::GetBox() const {
    if (vertices.empty()) {
        return Box::Null;
    }
    return Box(vertices);
}

/*!
\brief Creates an axis aligned box.

The object has 8 vertices, 6 normals and 12 triangles.
\param box The box.
*/
Mesh::Mesh(const Box &box) {
    // Vertices
    vertices.resize(8);

    for (int i = 0; i < 8; i++) {
        vertices[i] = box.Vertex(i);
    }

    // Normals
    normals.emplace_back(-1, 0, 0);
    normals.emplace_back(1, 0, 0);
    normals.emplace_back(0, -1, 0);
    normals.emplace_back(0, 1, 0);
    normals.emplace_back(0, 0, -1);
    normals.emplace_back(0, 0, 1);

    // Reserve space for the triangle array
    varray.reserve(12 * 3);
    narray.reserve(12 * 3);

    AddTriangle(0, 2, 1, 4);
    AddTriangle(1, 2, 3, 4);

    AddTriangle(4, 5, 6, 5);
    AddTriangle(5, 7, 6, 5);

    AddTriangle(0, 4, 2, 0);
    AddTriangle(4, 6, 2, 0);

    AddTriangle(1, 3, 5, 1);
    AddTriangle(3, 7, 5, 1);

    AddTriangle(0, 1, 5, 2);
    AddTriangle(0, 5, 4, 2);

    AddTriangle(3, 2, 7, 3);
    AddTriangle(6, 7, 2, 3);
}

Mesh::Mesh(const Sphere &sphere, int accuracy) {
    vertices.emplace_back(sphere.getC() + Vector(0, 0, sphere.getR())); // Top vertice
    normals.emplace_back(Normalized((Vector(0, 0, sphere.getR()))));

    Ray::RayHitTriangle hitTriangle;
    Ray ray(Vector(2,0,0),Vector(1,0,0));
    Ray ray2(Vector(-2,0,0),Vector(1,0,0));
    Ray ray3(Vector(-0.5,0,0),Vector(1,0,0));
    Ray ray4(Vector(0.5,0,0),Vector(1,0,0));

    double theta = 0;
    double phi = 0;
    double deltaTheta = Math::PI() / accuracy;
    double deltaPhi = (2 * Math::PI()) / accuracy;

    for (int band = 0; band < accuracy; ++band) {
        phi += deltaTheta;
        for (int point = 0; point < accuracy; ++point) {
            theta += deltaPhi;
            double x = std::sin(phi) * std::cos(theta);
            double y = std::sin(phi) * std::sin(theta);
            double z = std::cos(phi);
            vertices.emplace_back((Vector(x, y, z) * sphere.getR())+sphere.getC());
            normals.emplace_back(Normalized(Vector(x, y, z)));
        }
    }


    vertices.emplace_back(sphere.getC() - Vector(0, 0, sphere.getR())); // Bottom vertice
    normals.emplace_back(Normalized((-Vector(0, 0, sphere.getR()))));

    // add top / bottom triangles
    for (int i = 0; i < accuracy; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % accuracy + 1;
        AddSmoothTriangle(0, 0, i1, i1, i0, i0);
        i0 = i + accuracy * (accuracy - 2) + 1;
        i1 = (i + 1) % accuracy + accuracy * (accuracy - 2) + 1;
        AddSmoothTriangle(vertices.size() - 1, vertices.size() - 1, i1, i1, i0, i0);
    }

    // add quads per stack / slice
    for (int j = 0; j < accuracy - 2; j++) {
        auto j0 = j * accuracy + 1;
        auto j1 = (j + 1) * accuracy + 1;
        for (int i = 0; i < accuracy; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % accuracy;
            auto i2 = j1 + (i + 1) % accuracy;
            auto i3 = j1 + i;
            Mesh::AddQuadrangle(i0, i1, i2, i3);
        }
    }
}

/*!
\brief Scale the mesh.
\param s Scaling factor.
*/
void Mesh::ScaleUniform(double s) {
    // Vertexes
    for (auto &vertice: vertices) {
        vertice *= s;
    }

    if (s < 0.0) {
        // Normals
        for (auto &normal: normals) {
            normal = -normal;
        }
    }
}


#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegularExpression>
#include <utility>
#include <QtCore/qstring.h>

/*!
\brief Import a mesh from an .obj file.
\param filename File name.
*/
void Mesh::Load(const QString &filename) {
    vertices.clear();
    normals.clear();
    varray.clear();
    narray.clear();

    QFile data(filename);

    if (!data.open(QFile::ReadOnly))
        return;
    QTextStream in(&data);

    // Set of regular expressions : Vertex, Normal, Triangle
    QRegularExpression rexv("v\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
    QRegularExpression rexn("vn\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
    QRegularExpression rext("f\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)");
    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = rexv.match(line);
        QRegularExpressionMatch matchN = rexn.match(line);
        QRegularExpressionMatch matchT = rext.match(line);
        if (match.hasMatch())//rexv.indexIn(line, 0) > -1)
        {
            Vector q = Vector(match.captured(1).toDouble(), match.captured(2).toDouble(), match.captured(3).toDouble());
            vertices.push_back(q);
        } else if (matchN.hasMatch())//rexn.indexIn(line, 0) > -1)
        {
            Vector q = Vector(matchN.captured(1).toDouble(), matchN.captured(2).toDouble(),
                              matchN.captured(3).toDouble());
            normals.push_back(q);
        } else if (matchT.hasMatch())//rext.indexIn(line, 0) > -1)
        {
            varray.push_back(matchT.captured(1).toInt() - 1);
            varray.push_back(matchT.captured(3).toInt() - 1);
            varray.push_back(matchT.captured(5).toInt() - 1);
            narray.push_back(matchT.captured(2).toInt() - 1);
            narray.push_back(matchT.captured(4).toInt() - 1);
            narray.push_back(matchT.captured(6).toInt() - 1);
        }
    }
    data.close();
}

/*!
\brief Save the mesh in .obj format, with vertices and normals.
\param url Filename.
\param meshName %Mesh name in .obj file.
*/
void Mesh::SaveObj(const QString &url, const QString &meshName) const {
    QFile data(url);
    if (!data.open(QFile::WriteOnly))
        return;
    QTextStream out(&data);
    out << "g " << meshName << Qt::endl;
    for (const auto &vertice: vertices)
        out << "v " << vertice[0] << " " << vertice[1] << " " << vertice[2] << QString('\n');
    for (const auto &normal: normals)
        out << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << QString('\n');
    for (int i = 0; i < varray.size(); i += 3) {
        out << "f " << varray.at(i) + 1 << "//" << narray.at(i) + 1 << " "
            << varray.at(i + 1) + 1 << "//" << narray.at(i + 1) + 1 << " "
            << varray.at(i + 2) + 1 << "//" << narray.at(i + 2) + 1 << " "
            << "\n";
    }
    out.flush();
    data.close();
}


void Mesh::Rotate(double Angle, const Vector &Up) {
    Matrix rot = Matrix::rotate(Angle, Up);
    // Vertexes
    for (auto &vertice: vertices) {
        vertice = rot * vertice;
    }

    Matrix inv = Transpose(Inverse(rot));
    // Normals
    for (auto &normal: normals) {
        normal = rot * normal;
    }

}

void Mesh::Scale(double x, double y, double z) {
    Matrix rot = Matrix::scale(x, y, z);
    // Vertexes
    for (auto &vertice: vertices) {
        vertice = rot * vertice;
    }

    if (x < 0.0) {
        // Normals
        for (auto &normal: normals) {
            normal[0] = -normal[0];
        }
    }
    if (y < 0.0) {
        // Normals
        for (auto &normal: normals) {
            normal[1] = -normal[1];
        }
    }
    if (z < 0.0) {
        // Normals
        for (auto &normal: normals) {
            normal[2] = -normal[2];
        }
    }
}

void Mesh::Translate(const Vector &t) {
    // Vertexes
    for (auto &vertice: vertices) {
        vertice += t;
    }
}

void Mesh::Merge(const Mesh &m) {
    int preMergeCountVertex = Mesh::vertices.size();
    int preMergeCountNormal = Mesh::normals.size();
    for (auto &vert: m.vertices) {
        Mesh::vertices.push_back(vert);
    }

    for (auto &norm: m.normals) {
        Mesh::normals.push_back(norm);
    }

    for (auto &val: m.varray) {
        Mesh::varray.push_back(val + preMergeCountVertex);
    }

    for (auto &nval: m.narray) {
        Mesh::narray.push_back(nval + preMergeCountNormal);
    }
}

std::vector<double> Mesh::SphereWarp(const Sphere &s, const Vector &dir) {
    std::vector<double> buff;
    for (auto &vert: Mesh::vertices) {
        double ratio = s.OneMinusPercentToCenter(vert);
        buff.push_back(ratio);
        vert += dir * Math::Cubic(0, 1, ratio);
    }
    return buff;
}


Mesh::Mesh(const Cylinder &cylinder, int accuracy, unsigned int iFloors) {
    vertices.emplace_back(cylinder.getC() + Vector(0, 0, cylinder.getH())); // Top vertice
    normals.emplace_back(Normalized((cylinder.getC() + Vector(0, 0, cylinder.getH()))));

    double theta = 0;
    double phi = 0;
    double deltaTheta = Math::PI() / accuracy;
    double deltaPhi = (2 * Math::PI()) / accuracy;

    for (int band = 0; band < iFloors; ++band) {
        phi += deltaTheta;
        for (int point = 0; point < accuracy; ++point) {
            theta += deltaPhi;
            double x = std::cos(theta) * cylinder.getR();
            double y = std::sin(theta) * cylinder.getR();
            double z = Math::Lerp(cylinder.getH(),
                                  -cylinder.getH(),
                                  (((double) band) / ((double) (iFloors - 1))));
            vertices.emplace_back(x, y, z);
            normals.emplace_back(Normalized(Vector(x, y, 0)));
        }
    }

    vertices.emplace_back(cylinder.getC() - Vector(0, 0, cylinder.getH())); // Bottom vertice
    normals.emplace_back(Normalized((cylinder.getC() - Vector(0, 0, cylinder.getH()))));


    // add top / bottom triangles
    for (int i = 0; i < accuracy; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % accuracy + 1;
        AddTriangle(0, i1, i0, 0);
        i0 = i + ((iFloors - 1) * accuracy) + 1;
        i1 = (i + 1) % accuracy + ((iFloors - 1) * (accuracy)) + 1;
        AddTriangle(vertices.size() - 1, i1, i0, vertices.size() - 1);
    }

    // add quads per stack / slice
    for (int j = 0; j < iFloors - 1; j++) {
        auto j0 = j * accuracy + 1;
        auto j1 = (j + 1) * accuracy + 1;
        for (int i = 0; i < accuracy; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % accuracy;
            auto i2 = j1 + (i + 1) % accuracy;
            auto i3 = j1 + i;
            Mesh::AddQuadrangle(i0, i1, i2, i3);
        }
    }
}

Mesh::Mesh(const Capsule &capsule, int accuracy, unsigned int iFloors) {
    vertices.emplace_back(capsule.getC() + Vector(0, 0, capsule.getH() + capsule.getR())); // Top vertice
    normals.emplace_back(Normalized((capsule.getC() + Vector(0, 0, capsule.getH()))));

    double theta = 0;
    double phi = 0;
    double deltaTheta = Math::PI() / (accuracy + (accuracy % 2 == 0 ? 1 : 0));
    double deltaPhi = (2 * Math::PI()) / (accuracy);

    for (int band = 0; band < accuracy + iFloors - 2; ++band) {
        //phi += deltaTheta;
        theta += band <= accuracy / 2 - 1 || band >= (accuracy) / 2 + iFloors - 2 ? deltaTheta : 0;


        for (int point = 0; point < accuracy; ++point) {
            phi += deltaPhi;
            double bandRatio = (Math::Clamp(band + 1,
                                            accuracy / 2,
                                            accuracy / 2 + (iFloors - 1)) - accuracy / 2) /
                               ((accuracy / 2 + (iFloors - 1)) - (accuracy / 2));
            double x = std::sin(theta) * std::cos(phi) * capsule.getR();
            double y = std::sin(theta) * std::sin(phi) * capsule.getR();
            double z = std::cos(theta) + (capsule.getH() * 1 * (band >= ((accuracy) / 2) ? -1 : 1));
            z = (band <= accuracy / 2 - 1 || band >= (accuracy) / 2 + iFloors - 2 ? std::cos(theta) * capsule.getR()
                                                                                  : 0) +
                Math::Lerp(capsule.getH(),
                           -capsule.getH(),
                           bandRatio);//+(-deltaTheta * (band >= ((accuracy) / 2) ? -1 : 0));;
            std::cout << band << " " << point << " " << z << " " << bandRatio << " " << theta << '\n';
            vertices.emplace_back(Vector(x, y, z));
            normals.emplace_back(Normalized(Vector(x, y, z)));
        }
    }

    vertices.emplace_back(capsule.getC() - Vector(0, 0, capsule.getH() + capsule.getR())); // Bottom vertice
    normals.emplace_back(Normalized((capsule.getC() - Vector(0, 0, capsule.getH()))));


    // add top / bottom triangles
    for (int i = 0; i < accuracy; ++i) {
        auto i0 = i + 1;
        auto i1 = (i + 1) % accuracy + 1;
        AddSmoothTriangle(0, 0, i1, i1, i0, i0);
        i0 = i + 1 + (accuracy * (((accuracy + (accuracy % 2 == 0 ? 0 : -1)) + iFloors - 2) - 1));
        i1 = (i + 1) % accuracy + 1 + (accuracy * (((accuracy + (accuracy % 2 == 0 ? 0 : -1)) + iFloors - 2) - 1));
        AddSmoothTriangle(vertices.size() - 1, vertices.size() - 1, i1, i1, i0, i0);
    }

    //AddTriangle(vertices.size() - 1, vertices.size() - 2, vertices.size() - 3, vertices.size() - 1);

    // add quads per stack / slice
    for (int j = 0; j < accuracy - (accuracy % 2 == 0 ? 0 : 1) + iFloors - 3; j++) {
        auto j0 = j * accuracy + 1;
        auto j1 = (j + 1) * accuracy + 1;
        for (int i = 0; i < accuracy; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % accuracy;
            auto i2 = j1 + (i + 1) % accuracy;
            auto i3 = j1 + i;
            Mesh::AddQuadrangle(i0, i1, i2, i3);
        }
    }
}

void Mesh::DebugVertices() {
    Mesh self = *this;
    Mesh view(Sphere(Vector::Null, 0.2), 3);
    for (const auto &vert: self.vertices) {
        Mesh copy = view;
        copy.Translate(vert);
        Merge(copy);
    }
}

Mesh::Mesh(const Torus &torus, int accuracy) {

    double theta = 0;
    double phi = 0;
    double deltaTheta = (2.0 * Math::PI()) / (double)(accuracy);
    double deltaPhi = (2.0 * Math::PI()) / (double)(accuracy);

    for (int circle = 0; circle < accuracy; ++circle) {
        theta += deltaTheta;
        phi = 0;
        double xc = std::cos(theta);
        double yc = std::sin(theta);
        for (int point = 0; point < accuracy; ++point) {
            phi += deltaPhi;
            double x = std::sin(phi) * xc;
            double y = std::sin(phi) * yc;
            double z = std::cos(phi);
            vertices.emplace_back(((Vector(x, y, z) * torus.getA()) + (Vector(xc, yc, 0) * torus.getB()))+torus.getC());
            normals.emplace_back(Normalized(Vector(x, y, z)));
        }
    }

    // add quads per stack / slice
    for (int j = 0; j < accuracy ; j++) {
        auto j0 = j * accuracy;
        auto j1 = (j + 1)%accuracy * accuracy;
        for (int i = 0; i < accuracy; i++) {
            auto i0 = j0 + i;
            auto i1 = j0 + (i + 1) % accuracy;
            auto i2 = j1 + (i + 1) % accuracy;
            auto i3 = j1 + i;
            Mesh::AddQuadrangle(i0, i1, i2, i3);
        }
    }

}

