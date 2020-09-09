#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "trianglesurface.h"
#include <QVector2D>

class InteractiveObject : public TriangleSurface // Arver alle nødvendige funksjoner fra TriangleSurface
{
public:
    InteractiveObject();
    InteractiveObject(const char *path, const char *texture);
    // Bruker kun den under, men objektet skal kunne ha en texture også
    InteractiveObject(const char *path, QVector3D color);

    // Plasserer boksen tilbake til starten. Kalles når spillet resettes
    void setToGameStart(Terrain *ground);

    // Funksjonen som faktisk får objektet til å forflytte seg etter å ha blitt dyttet
    void moveBetweenTwoPoints(QVector3D pushedFrom, Terrain* ground, std::vector<VisualObject*> otherObjects);

    // Setter translasjonen for objektet og dens colliders
    void setTranslationForAll(float x, float y, float z) override;

    // Variabler som har med push-funksjonaliteten å gjøre
    QVector3D mMoveTo; // Hvilken posisjon den skal til etter å ha blitt dyttet
    float mPushDistance = 2; // Hvor langt den skal dyttes
    bool mJustPushed = false; // Settes til true rett etter at den har blitt dyttet
    QVector3D mInitPos; // Hvor den startet å bli dyttet fra
    float mStep; // Hvor langt objektet har kommet
    float mStepIncrement; // Er lik verdien mStep har fra starten, inkrementerer mStep
    float mSpeed = 0.1f; // Hvor raskt boksen forflytter seg
    bool mMoving = false; // Om boksen beveger seg eller ikke

    QVector3D mGameStartPos; // Startposisjonen til boksen når man starter spillet



};

#endif // INTERACTIVEOBJECT_H
