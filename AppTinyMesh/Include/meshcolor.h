#ifndef __MeshColor__
#define __MeshColor__

#include "color.h"
#include "mesh.h"

class MeshColor : public Mesh {
protected:
    std::vector<Color> colors; //!< Array of colors.
    std::vector<int> carray;  //!< Indexes.
    std::vector<Color> aocolors; //!< Array of colors.
    std::vector<int> aoarray;  //!< Indexes.

public:
    explicit MeshColor();

    explicit MeshColor(const Mesh &);

    explicit MeshColor(const Mesh &, const std::vector<Color> &, const std::vector<int> &);

    ~MeshColor();

    void DebugVertices() override;

    Color GetColor(int) const;

    std::vector<Color> GetColors() const;

    std::vector<int> ColorIndexes() const;

    Color GetAO(int) const;

    std::vector<int> AOIndexes() const;

    void Accecibility();
};

/*!
\brief Get a color.
\param i The index of the desired color.
\return The color.
*/
inline Color MeshColor::GetColor(int i) const {
    return colors[i];
}

/*!
\brief Get the array of colors.
*/
inline std::vector<Color> MeshColor::GetColors() const {
    return colors;
}

/*!
\brief Return the set of color indices.
*/
inline std::vector<int> MeshColor::ColorIndexes() const {
    return carray;
}


inline Color MeshColor::GetAO(int i) const {
    return aocolors[i];
}


inline std::vector<int> MeshColor::AOIndexes() const {
    return aoarray;
}

inline void MeshColor::Accecibility() {
    aocolors.reserve(carray.size());

    for (int i = 0; i < carray.size(); ++i) {
        aocolors.emplace_back(0.1, 0.8, 0.1);// l'AO
    }

    for (auto elem: carray) {
        aoarray.push_back(elem);
    }
}

#endif
