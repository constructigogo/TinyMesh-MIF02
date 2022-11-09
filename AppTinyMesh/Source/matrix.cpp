//
// Created by constantin on 26/09/22.
//

#include "mathematics.h"

const Matrix Matrix::Idendity = Matrix(1, 0, 0,
                                       0, 1, 0,
                                       0, 0, 1);

Matrix::Matrix(double a, double b, double c,
               double d, double e, double f,
               double g, double h, double i) : c({a, b, c, d, e, f, g, h, i}) {

}

Matrix Matrix::scale(double x, double y, double z) {
    return {x, 0, 0,
            0, y, 0,
            0, 0, z};
}

Matrix Matrix::rotate(double angle, const Vector &up) {
    double s = sin(angle);
    double c = cos(angle);
    double cb = 1 - c;
    return {((up[0] * up[0]) * cb) + c, (up[0] * up[1] * cb) - (s * up[2]), (up[0] * up[2] * cb) + (s * up[1]),
            (up[0] * up[1] * cb) + (s * up[2]), ((up[1] * up[1]) * cb) + c, (up[1] * up[2] * cb) - (s * up[0]),
            (up[0] * up[2] * cb) - (s * up[1]), (up[1] * up[2] * cb) + (s * up[0]), ((up[2] * up[2]) * cb) + c};
}

Matrix Matrix::rotateAroundX(double angle) {
    return rotate(angle, Vector::X);
}

Matrix Matrix::rotateAroundY(double angle) {
    return rotate(angle, Vector::Y);
}

Matrix Matrix::rotateAroundZ(double angle) {
    return rotate(angle, Vector::Z);
}

Matrix Transpose(const Matrix &m) {
    return {m[0], m[3], m[6],
            m[1], m[4], m[7],
            m[2], m[5], m[8]};
}


Matrix Inverse(const Matrix &m) {
    return (1 / Det(m)) * Matrix(
            m[4] * m[7] - m[5] * m[7], -(m[3] * m[8] - m[5] * m[6]), m[3] * m[7] - m[4] * m[6],
            -(m[1] * m[8] - m[2] * m[7]), m[0] * m[8] - m[2] * m[6], -(m[0] * m[7] - m[1] * m[6]),
            m[1] * m[5] - m[2] * m[4], -(m[0] * m[5] - m[2] * m[3]), m[0] * m[4] - m[1] * m[3]
    );
}

double Det(const Matrix &m) {
    return m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]
           - (m[2] * m[4] * m[6] + m[1] * m[3] * m[8] + m[0] * m[5] * m[7]);
}

