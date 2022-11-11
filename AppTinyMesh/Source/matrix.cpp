//
// Created by constantin on 26/09/22.
//

#include "mathematics.h"

/**
 * Diagonal matrix helper
 */
const Matrix Matrix::Idendity = Matrix(1, 0, 0,
                                       0, 1, 0,
                                       0, 0, 1);

Matrix::Matrix(double a, double b, double c,
               double d, double e, double f,
               double g, double h, double i) : c({a, b, c, d, e, f, g, h, i}) {

}

/**
 * Generate a scale matrix
 * @param x
 * @param y
 * @param z
 * @return
 */
Matrix Matrix::scale(double x, double y, double z) {
    return {x, 0, 0,
            0, y, 0,
            0, 0, z};
}

/**
 * rotate by angle around UP
 * @param angle
 * @param up
 * @return
 */
Matrix Matrix::rotate(double angle, const Vector &up) {
    double s = sin(angle);
    double c = cos(angle);
    double cb = 1 - c;
    return {((up[0] * up[0]) * cb) + c, (up[0] * up[1] * cb) - (s * up[2]), (up[0] * up[2] * cb) + (s * up[1]),
            (up[0] * up[1] * cb) + (s * up[2]), ((up[1] * up[1]) * cb) + c, (up[1] * up[2] * cb) - (s * up[0]),
            (up[0] * up[2] * cb) - (s * up[1]), (up[1] * up[2] * cb) + (s * up[0]), ((up[2] * up[2]) * cb) + c};
}

/**
 * Helper function to rotate around X
 * @param angle radian
 * @return rotation matrix
 */
Matrix Matrix::rotateAroundX(double angle) {
    return rotate(angle, Vector::X);
}

/**
 * Helper function to rotate around Y
 * @param angle radian
 * @return rotation matrix
 */
Matrix Matrix::rotateAroundY(double angle) {
    return rotate(angle, Vector::Y);
}

/**
 * Helper function to rotate around Z
 * @param angle radian
 * @return rotation matrix
 */
Matrix Matrix::rotateAroundZ(double angle) {
    return rotate(angle, Vector::Z);
}

/**
 * Compute the transpose of a matrix
 * @param m
 * @return Transpose of m
 */
Matrix Transpose(const Matrix &m) {
    return {m[0], m[3], m[6],
            m[1], m[4], m[7],
            m[2], m[5], m[8]};
}

/**
 * Compute the Inverse of a matrix
 * @param m
 * @return Inverse of m
 */
Matrix Inverse(const Matrix &m) {
    return (1 / Det(m)) * Matrix(
            m[4] * m[7] - m[5] * m[7], -(m[3] * m[8] - m[5] * m[6]), m[3] * m[7] - m[4] * m[6],
            -(m[1] * m[8] - m[2] * m[7]), m[0] * m[8] - m[2] * m[6], -(m[0] * m[7] - m[1] * m[6]),
            m[1] * m[5] - m[2] * m[4], -(m[0] * m[5] - m[2] * m[3]), m[0] * m[4] - m[1] * m[3]
    );
}

/**
 * Get the determinant of a matrix
 * @param m
 * @return det of m
 */
double Det(const Matrix &m) {
    return m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]
           - (m[2] * m[4] * m[6] + m[1] * m[3] * m[8] + m[0] * m[5] * m[7]);
}

