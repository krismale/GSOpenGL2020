#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "xyz.h"
#include "trianglesurface.h"
#include "interactiveobject.h"
#include "octahedronball.h"
#include "beziercurve.h"
#include "camera.h"
#include "mainwindow.h"
#include "shader.h"
#include "light.h"
#include "MyStructs.h"
#include "terrain.h"
#include "billboard.h"
#include "ui.h"
#include "skybox.h"

class SceneManager : public QOpenGLFunctions_4_1_Core
{
public:
    SceneManager();
    void init(MyStructs::ShaderUniforms rgbShaderUniforms, MyStructs::ShaderUniforms texShaderUniforms,
              MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms,
              MyStructs::ShaderUniforms skyboxShaderUniforms);

    // Funksjoner for å tegne alle objekter, basert på shadere
    void drawRGBObjects(MainWindow *mainWindow, Shader* shaderProgram);
    void drawTexObjects(MainWindow *mainWindow, Shader* texShaderProgram);
    void drawPhongObjects(MainWindow *mainWindow, Shader* phongShaderProgram);
    void drawBillboardObjects(MainWindow *mainWindow, Shader *billboardShaderProgram);
    void drawSkybox(MainWindow *mainWindow, Shader* skyboxShaderProgram);

    // Logikken til spillet
    void currentSceneLogic();

    // Funksjoner for håndtering av kontroller og debug-kamerabevegelse, samt game-camera movement
    void playerControls(QKeyEvent *event, bool gameIsPlaying);
    void resetControls();
    void cameraMovement(); // Debug camera movement
    void moveCameraAfterPawn(); // Game camera movement

    // Spiller og fiendebevegelse
    void enemyMovement();
    void playerMovement();

    // Funksjoner for å fryse og starte logikken i spillet, samt resette logikken
    void startScene();
    void stopScene();
    void resetGame();

    // Skrur av og på alle hjelpeobjekter. Brukes i startScene og stopScene
    void renderAllHelpObjects(bool render);

    // Søker gjennom alle objekter i scenen, på leting etter den med mID == id
    VisualObject* searchAllObjects(int id);

    // Variabler og pekere som er viktige i spillet og scenen
    Camera mCamera; // Kameraet i scenen
    TriangleSurface* mCameraMesh = nullptr; // Meshen som viser til hvor kameraet skal starte når spillet starter
    XYZ* mGrid = nullptr; // Hjelpegriden i origo
    TriangleSurface* mPawn = nullptr; // Spillerens kontrollerbare pawn
    TriangleSurface* mEnemy = nullptr; // Fienden i spillet
    Terrain* mGround = nullptr; // Bakken, generert av et heightmap
    BezierCurve* mCurveToMoveAlong = nullptr; // Bezierkurven som fienden beveger seg langs
    InteractiveObject* mPushObject = nullptr; // Objektet som spilleren kan dytte på
    std::vector<UI*> mUIElements; // En samling av UI elementer som brukes i spillet, minus mWinUI og mLoseUI
    UI* mWinUI = nullptr; // Et UI element som blir synlig når man vinner spillet
    UI* mLoseUI = nullptr; // Et UI element som blir synlig når man taper spillet
    std::vector<TriangleSurface*> mTrophies; // Alle trofeene i scenen

    // Disse holder på objektene som skal tegnes i scenen
    std::vector<VisualObject*> mRgbVisualObjects; // Alle objekter som skal rendres av RGB shaderen
    std::vector<VisualObject*> mTexVisualObjects; // Alle objekter som skal rendres av Tex shaderen
    std::vector<VisualObject*> mPhongVisualObjects; // Alle objekter som skal rendres av Phong shaderen
    std::vector<Light*> mSceneLights; // Alle lyskilder i scenen
    std::vector<Billboard*> mBillboardVisualObjects; // Alle billboard elementer som skal rendre av Billboard shaderen
    Skybox* mSkyboxVisualObject; // Skybox-objektet, som har sin helt egne shader

    // En samling av posisjoner for trofeene.
    QVector3D mTrophyPositions[4] {
        QVector3D(-5, 0.3f, -10),
        QVector3D(4, 0.3f, -15),
        QVector3D(0, 0.3f, -15),
        QVector3D(0, 0.3f, -20),
    };
    int mGatheredTrophies = 0; // Hvor mange trofeer spilleren har samlet til nå

    QVector3D mPawnSpawnLocation = QVector3D(0,0,0); // hvor pawnen skal spawne på starten av spillet

    int mEnemyMovementSpeed = 2; // Hvor rask fienden er i bevegelsen

    // Variabler som setter opp lyset for billboard og phong shaderne
    float mAmbientStrength = 0.3f; // Hvor sterkt ambient lyset er
    QVector3D mAmbientCol = QVector3D(0.8f, 0.8f, 0.3f); // Hva slags fargetone ambientlyset skal ha

    MyStructs::PlayerInput mPlayerInput; // Structen for player input

    // Variabler for å vise til om spillet er i gang eller ikke
    bool mIsPlaying = false; // Stopper spillet
    bool mFreezeAllControls = false; // Gjør at brukeren ikke kan gjøre noe annet enn å trykke på knapper

    // Shader uniforms
    // Har mange structs her fordi jeg vil ha et tydelig skille mellom shaderne
    // Jeg får bedre oversikt enn om jeg skulle ha puttet disse inn i et array, når jeg har individuelle navn
    MyStructs::ShaderUniforms mPhongShaderUniforms;
    MyStructs::ShaderUniforms mBillboardShaderUniforms;

    int mLastObjectID = 1000; // Viser til IDen til det siste objektet som er dyttet inn i outlineren

private:
    // Oppsett av scenen
    void SetupScene1(); // Generelt oppsett av scenen
    void SetupPawn(); // Setter opp pawnen
    void setupEnemy(); // Setter opp fienden
    void setupTrophies(); // setter opp trofeene som skal samles inn
    void setupCameraMesh(); // Setter opp placeholder cameraMeshen
};

#endif // SCENEMANAGER_H
