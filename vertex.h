#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <QVector3D>

class Vertex {
    // Overloaded ostream operator to write vertex data on an open textfile
    friend std::ostream& operator<<(std::ostream &out, const Vertex &v) {
        out << std::fixed;
        out << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
        out << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
        out << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
        return out;
    }
    // Overloaded istream operator to read all vertex data from open textfile
    friend std::istream& operator>> (std::istream& in, Vertex& v) {
        char dum, dum2, dum3, dum4;
        in >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
        in >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
        in >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
        return in;
    }

public:
    Vertex();
    Vertex(float x, float y, float z, float r, float g, float b, float s = 0, float t = 0);
    float GetX(){return m_xyz[0];}
    float GetY(){return m_xyz[1];}
    float GetZ(){return m_xyz[2];}
    float GetR(){return m_normal[0];}
    float GetG(){return m_normal[1];}
    float GetB(){return m_normal[2];}
    float GetU(){return m_st[0];}
    float GetV(){return m_st[1];}
    QVector3D getXYZ();

    void SetXYZ(float x, float y, float z);
    void SetXYZ(QVector3D &v);
    void SetRGB(float r, float g, float b);
    void SetRGB(QVector3D &v);
    void SetST(float s, float t);


    float m_xyz[3]; // position in 3D
    float m_normal[3]; // normal in 3D or rgb colors
    float m_st[2]; // texture coordinates if used
    bool hasUVCoords = false;

};
#endif // VERTEX_H
