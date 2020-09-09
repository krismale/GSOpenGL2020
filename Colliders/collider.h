#ifndef COLLIDER_H
#define COLLIDER_H

#include "visualobject.h"

class Collider : public VisualObject
{
    public:
        Collider();
        virtual void init(MyStructs::ShaderUniforms shaderUniforms) override;
        virtual void draw() override;

        // Translasjonsfunksjoner for colliderne
        void move(float dx, float dy, float dz) override;
        void moveTo(float x, float y, float z) override;
        void setTranslation(float x, float y, float z);
        void setScale(float dx, float dy, float dz);
        void scale(float dx, float dy, float dz) override;
        void rotate(float dTilt, float dYaw, float dRoll) override;

        // Kalkulerer høyden på collideren
        void calcInitHeight();


        // Har to ulike kollisjonsfunksjoner, en for blockCollision og en for triggerCollision
        virtual bool checkBlockCollision(VisualObject* otherObject) {}
        virtual bool checkTriggerCollision(VisualObject* otherObject) {}

        // Istedenfor å kjøre dynamic_cast for å sjekke hvilken collider en skal teste mot, som er en krevende prosess
        // så har jeg valgt å sette en enum verdi for de to typene jeg har laget
        // Sphere collider er riktignok ikke implementert ordentlig, så den brukes ikke
        enum ColliderType {
            CUBECOLLIDER,
            SPHERECOLLIDER
        };
        ColliderType mColliderType;

        // Setter en ID for objektet for outlineren
        void setID(int id) override {
            mID = id;
        }

        bool mCollisionOn{true}; // Dersom denne er true så er collision skrudd på for collideren
        bool mIsTrigger{false}; // Er denne true så vil checkTriggerCollision fungere istedenfor CheckBlockCollision

    private:
        void generatePhongColors(); // genererer en farge til collideren
};

#endif // COLLIDER_H
