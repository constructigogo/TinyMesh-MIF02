#include "meshcolor.h"

/*!
\brief Create an empty mesh.
*/
MeshColor::MeshColor() {
}

/*!
\brief Constructor from a Mesh with color array and indices.
\param m Base mesh.
\param cols Color array.
\param carr Color indexes, should be the same size as Mesh::varray and Mesh::narray.
*/
MeshColor::MeshColor(const Mesh &m, const std::vector<Color> &cols, const std::vector<int> &carr, int acc,
                     double range) : Mesh(m), colors(cols), carray(carr) {
    Accessibility(acc, range);
}

/*!
\brief Constructor from a Mesh.
\param m the base mesh
*/
MeshColor::MeshColor(const Mesh &m) : Mesh(m) {
    colors.resize(vertices.size(), Color(1.0, 1.0, 1.0));
    carray = varray;
}

/*!
\brief Empty.
*/
MeshColor::~MeshColor() {
}

void MeshColor::DebugVertices() {
    MeshColor Origin = *this;
    Mesh OCopy = Origin;
    Mesh view(Sphere(Vector::Null, 0.2), 4);

    for (int i = 0; i < Origin.Vertexes(); ++i) {
        Mesh copy = view;
        copy.Rotate(-std::acos(Origin.Normal(i) * Vector::Z), Origin.Normal(i) / Vector::Z);
        copy.Translate(Origin.Vertex(i));
        OCopy.Merge(copy);
    }
    std::vector<Color> cols;
    cols.resize(OCopy.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    *this = MeshColor(OCopy, cols, OCopy.VertexIndexes(), 3, 4);
}


void MeshColor::Accessibility(int accuracy, double range) {
    aocolors.reserve(carray.size());
    double deltaTheta = (0.5 * Math::PI()) / ((double) (accuracy * 4));
    double deltaPhi = (2.0 * Math::PI()) / ((double) accuracy * accuracy);
    static double bias = 1.0e-1;


    for (int vertIndex = 0; vertIndex < Vertexes(); vertIndex++) {
        double theta = 0;
        double phi = 0;
        double buffer = 0;
        double count = 0;
        double rotateZ = std::acos(Normal(vertIndex) * Vector::Z);
        double rotateX = std::acos(Normal(vertIndex) * Vector::X);
        double rotateY = std::acos(Normal(vertIndex) * Vector::Y);
        Matrix rot = Matrix::rotate(-rotateZ, Normal(vertIndex) / Vector::Z);
        Matrix rotZ = (rotateY < Math::DegreeToRadian(90)) ?
                      Matrix::rotate(rotateX, Vector::Z) :
                      Matrix::rotate(-rotateX, Vector::Z);

        for (int row = 0; row < (accuracy == 1 ? 1 : accuracy * 4); ++row) {
            phi = 0;
            for (int point = 0; point < (row == 0 ? 1 : accuracy * accuracy); ++point) {
                double x = std::sin(theta) * std::cos(phi);
                double y = std::sin(theta) * std::sin(phi);
                double z = std::cos(theta);
                Vector dir(x, y, z);
                dir = Normalized(rot * (rotZ * dir));
                Ray ray(Vertex(vertIndex) + (Normal(vertIndex) * bias), dir);
                count++;
                for (int i = 0; i < Triangles(); ++i) {
                    Ray::RayHitTriangle hit;
                    if (GetTriangle(i).Intersect(ray, hit.depth, hit.u, hit.v)) {
                        if (hit.depth > 0 && hit.depth <= range) {
                            buffer += 1;
                            break;
                        }
                    }
                }
                phi += deltaPhi;
            }
            theta += deltaTheta;
        }

        assert(((double) buffer / (double) count) <= 1.0);
        double res = Math::Clamp(1.0 - (buffer / count));
        aocolors.emplace_back(res, res, res);
    }


    for (auto elem: carray) {
        aoarray.push_back(elem);
    }
}

