#include "matrix4x4.h"
#include "math.h"
#include "MyMath/mathconstants.h"
#include <iostream>


Matrix4x4::Matrix4x4()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            m_matrix[i][j] = 0.0f;
        }
    }
}

Matrix4x4 Matrix4x4::setToIdentity()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(i == j) {
                m_matrix[i][j] = 1.0f;
            } else {
                m_matrix[i][j] = 0.0f;
            }
        }
    }

    return *this;
}

const float *Matrix4x4::constData() const
{
    return &m_matrix[0][0];
}

void Matrix4x4::print()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << m_matrix[i][j];
            if(j == 3) {
                std::cout << std::endl;
            } else {
                std::cout << ", ";
            }
        }
    }
}

void Matrix4x4::perspective(float fieldOfView, float aspectRatio, float whyNear, float whyFar)
{
    float top = whyNear*std::tan(fieldOfView);
    float right = top/aspectRatio;

    m_matrix[0][0] = whyNear/right;
    m_matrix[1][1] = whyNear/top;
    m_matrix[2][2] = -(whyFar+whyNear)/(whyFar-whyNear);
    m_matrix[2][3] = -2*whyFar*whyNear/(whyFar-whyNear);
    m_matrix[3][2] = -1;
}


