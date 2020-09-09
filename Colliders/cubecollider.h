#ifndef CUBECOLLIDER_H
#define CUBECOLLIDER_H

#include "Colliders/collider.h"
#include "spherecollider.h"

class CubeCollider : public Collider
{
    public:
        CubeCollider(float height, float depth, float width);
        void init(MyStructs::ShaderUniforms shaderUniforms) override;
        void draw() override;

        // Se kommentar i collider.h
        bool checkBlockCollision(VisualObject* otherObject) override;
        bool checkTriggerCollision(VisualObject* otherObject) override;


    private:
        // Sjekker kollisjon på en spesifikk dimensjon om gangen
        // Brukes både som en del av BlockCollision og TriggerCollision-testene
        bool checkAxisCollision(float thisDimensionPosition, float thisDimensionMeasurement, float otherDimensionPosition, float otherDimensionMeasurement);

        // En unik funksjon for å teste en CubeCollider mot en SphereCollider
        bool checkCollidingWithSphere(SphereCollider *otherCollider);
};

#endif // CUBECOLLIDER_H
