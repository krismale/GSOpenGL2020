#ifndef MATRIX4X4_H
#define MATRIX4X4_H


class Matrix4x4
{
    public:
        Matrix4x4();
        Matrix4x4 setToIdentity();

        const float *constData() const;

        float m_matrix[4][4];

        void print();
        void perspective(float fieldOfView, float aspectRatio, float whyNear, float whyFar);
};

#endif // MATRIX4X4_H
