#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
#include <chrono>
#include "Colliders/collider.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(const char *path, const char *texture = nullptr);
    TriangleSurface(const char *path, QVector3D color);
    ~TriangleSurface() override;
    void init(MyStructs::ShaderUniforms shaderUniforms) override;
    void draw() override;

    // Offentlige funksjoner som setter ulike translasjoner
    // Brukes av edit vinduet i MainWindow blant annet
    // Forskjellen på disse og setTranlation og setScale er at de gjøres for alle collidere også
    void setTranslationForAll(float x, float y, float z) override;
    void setScaleForAll(float x, float y, float z) override;
    void setRotationForAll(float roll, float yaw, float pitch) override;

    // Translasjonsfunksjoner
    void move(float dx, float dy, float dz) override; // Deltaflytter objektet
    void moveTo(float x, float y, float z) override; // Flytter objektet til et spesifikt punkt
    void rotate(float dTilt, float dYaw, float dRoll) override; // Deltaroterer objektet
    void scale(float dx, float dy, float dz) override; // Skalerer objektet

    // Spesielle forflytningsfunksjoner
    // moveAlongCurve() brukes av fienden til å følge bezierkurven
    // moveAboveGround() brukes blant annet av spilleren til å forflytte seg over bakken
    void moveAlongCurve(unsigned int increment, std::vector<QVector3D> points, Terrain* ground, std::vector<VisualObject*> otherObjects);
    void moveAboveGround(float x, float y, float z, Terrain* ground, std::vector<VisualObject*> otherObjects, bool adjustHeight = true);

    // Plasserer objektet oppå bakken, ved hjelp av barysentriske koordinater
    void placeOnTopOfGround(Terrain *ground, float addedHeight = 0) override;

    // Setter objektets farge
    void setColor(QVector3D color) override;

    // Skrur av eller på synligheten til alle shaderne som objektet har koblet til seg
    void renderAllColliders(bool render);

    // Brukes for å sette IDen til objektet i outlineren
    void setID(int id) override {
        mID = id;
    }

    // Kalkulerer normaler
    // Brukes ikke på noe annet objekt enn i subklassen Terrain, men kan brukes for TriangleSurface
    void calcNormals();

    // Det siste triangelet som objektet sto på
    // Brukes som en del av søket som minner om topologisk søk for barysentriske koordinater
    MyStructs::Triangles *mLastTriangle = nullptr;

    // Viser til siste retning som objektet beveget seg i
    QVector3D mMoveDirection;

private:
    //Kalkulerer objektets høyde når den lages, brukes til å enkelt lage colliders rundt objektet
    void calcInitHeight();

    void generateTexture() override; // Genererer en tekstur, hvis objektet har
    void generatePhongColors(); // Genererer en slags RGB tekstur som phongshaderen kan tolke

    // Setter translasjoner
    // add legger til verdiene som sendes inn på toppen av det som finnes fra før
    // set setter helt nye verdier og overskriver det som fantes fra før
    void addTranslation(float dx, float dy, float dz);
    void addRotation(float deltaTilt, float deltaYaw, float deltaRoll);
    void addScale(float dx, float dy, float dz);
    void setTranslation(float x, float y, float z);
    void setRotation(float tilt, float yaw, float roll);
    void setScale(float x, float y, float z);

    // Kollisjonstest mot alle objektene som finnes i otherObject
    // Denne brukes nå kun til å teste opp mot objekter som tegnes av phong shaderen,
    // men det er de eneste objektene som er interessante å kollisjonsteste med uansett
    bool validateMove(float x, float y, float z, std::vector<VisualObject *> otherObjects);

    // Kalkulerer den siste retningen som objektet har beveget seg
    void calcMoveDirection(float dy, float dx, float dz);


};

#endif // TRIANGLESURFACE_H
