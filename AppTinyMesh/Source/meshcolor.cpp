#include "meshcolor.h"

/*!
\brief Create an empty mesh.
*/
MeshColor::MeshColor()
{
}

/*!
\brief Constructor from a Mesh with color array and indices.
\param m Base mesh.
\param cols Color array.
\param carr Color indexes, should be the same size as Mesh::varray and Mesh::narray.
*/
MeshColor::MeshColor(const Mesh& m, const std::vector<Color>& cols, const std::vector<int>& carr) : Mesh(m), colors(cols), carray(carr)
{
    Accecibility();
}

/*!
\brief Constructor from a Mesh.
\param m the base mesh
*/
MeshColor::MeshColor(const Mesh& m) : Mesh(m)
{
	colors.resize(vertices.size(), Color(1.0, 1.0, 1.0));
	carray = varray;
}

/*!
\brief Empty.
*/
MeshColor::~MeshColor()
{
}

void MeshColor::DebugVertices() {
    MeshColor Origin = *this;
    Mesh OCopy = Origin;
    Mesh view(Sphere(Vector::Null,0.2),3);
    for (const auto & vert: Origin.vertices) {
        Mesh copy = view;
        copy.Translate(vert);
        OCopy.Merge(copy);
    }

    std::vector<Color> cols;
    cols.resize(OCopy.Vertexes());
    for (auto &col: cols) {
        col = Color(0.8, 0.8, 0.8);
    }
    *this = MeshColor(OCopy,cols,OCopy.VertexIndexes());
}
