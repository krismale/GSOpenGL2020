#include "vertex.h"


Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
    m_st[0] = s;
    m_st[1] = t;
}

void Vertex::SetXYZ(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

void Vertex::SetXYZ(QVector3D &v)
{
    SetXYZ(v.x(), v.y(), v.z());
}

void Vertex::SetRGB(float r, float g, float b)
{
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
}

void Vertex::SetRGB(QVector3D &v)
{
    SetRGB(v.x(), v.y(), v.z());
}

void Vertex::SetST(float s, float t)
{
    m_st[0] = s;
    m_st[1] = t;
}

QVector3D Vertex::getXYZ()
{
    QVector3D v;
    v.setX(m_xyz[0]);
    v.setY(m_xyz[1]);
    v.setZ(m_xyz[2]);

    return v;
}
