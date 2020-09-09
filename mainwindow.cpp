#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QCheckBox>

#include "renderwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{

    //this sets up what's in the mainwindow.ui
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this - sorry!)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types than OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have some OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we try to get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    populateOutliner(); // legger til alle objekter i en outliner ved siden av renderWindow

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), SLOT(itemClicked(QListWidgetItem *)));
}

void MainWindow::populateOutliner()
{
    new QListWidgetItem(QString::fromUtf8(mRenderWindow->mSceneManager->mCamera.mLabel.c_str()), ui->listWidget, mRenderWindow->mSceneManager->mCamera.mID);
    for(const auto &it : mRenderWindow->mSceneManager->mSceneLights) {
        it->setID(++mRenderWindow->mSceneManager->mLastObjectID);
        new QListWidgetItem(QString::fromUtf8(it->mLabel.c_str()), ui->listWidget, it->mID);
    }
    for(const auto &it : mRenderWindow->mSceneManager->mPhongVisualObjects) {
        it->setID(++mRenderWindow->mSceneManager->mLastObjectID);
        new QListWidgetItem(QString::fromUtf8(it->mLabel.c_str()), ui->listWidget, it->mID);
    }
    for(const auto &it : mRenderWindow->mSceneManager->mTexVisualObjects) {
        it->setID(++mRenderWindow->mSceneManager->mLastObjectID);
        new QListWidgetItem(QString::fromUtf8(it->mLabel.c_str()), ui->listWidget, it->mID);
    }
    for(const auto &it : mRenderWindow->mSceneManager->mRgbVisualObjects) {
        if(it != mRenderWindow->mSceneManager->mGrid) {
            it->setID(++mRenderWindow->mSceneManager->mLastObjectID);
            new QListWidgetItem(QString::fromUtf8(it->mLabel.c_str()), ui->listWidget, it->mID);
        }
    }
}



//Example of a slot called from the button on the top of the program.
void MainWindow::on_playButton_clicked()
{
    mRenderWindow->mSceneManager->mIsPlaying = !mRenderWindow->mSceneManager->mIsPlaying;
    if(mRenderWindow->mSceneManager->mIsPlaying) {
        ui->playButton->setText("Stop");
        mRenderWindow->mSceneManager->startScene();
    } else {
        ui->playButton->setText("Play");
        mRenderWindow->mSceneManager->stopScene();
    }


}

void MainWindow::on_wireframeButton_clicked()
{
    mRenderWindow->mIsWireframeMode = !mRenderWindow->mIsWireframeMode;
    mRenderWindow->changeRenderMode();

    if(mRenderWindow->mIsWireframeMode) {
        ui->wireframeButton->setText("Turn off wireframe");
    } else {
        ui->wireframeButton->setText("Turn on wireframe");
    }


}

void MainWindow::on_axisButton_clicked()
{
    mRenderWindow->mIsAxisVisible = !mRenderWindow->mIsAxisVisible;

    if(mRenderWindow->mIsAxisVisible) {
        ui->axisButton->setText("Hide grid");
        mRenderWindow->mSceneManager->mGrid->mVisible = true;
    } else {
        ui->axisButton->setText("Show grid");
        mRenderWindow->mSceneManager->mGrid->mVisible = false;
    }
}

void MainWindow::on_moveToButton_clicked()
{
    if(mSelectedObject) {
        mRenderWindow->mSceneManager->mCamera.moveCameraToTarget(mSelectedObject); // Flytter kamera bort til objektet
    }
}

void MainWindow::on_renderCollidersCheckBox_clicked()
{
    if(mSelectedObject) {
        if(dynamic_cast<TriangleSurface*>(mSelectedObject)) {
            dynamic_cast<TriangleSurface*>(mSelectedObject)->renderAllColliders(ui->renderCollidersCheckBox->isChecked());
        }
    }
}

void MainWindow::on_resetGameButton_clicked()
{

    mRenderWindow->mSceneManager->resetGame();
}

void MainWindow::on_TranslateXValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setTranslationForAll(ui->TranslateXValues->text().toFloat(),
                                              mSelectedObject->mTransformation.translation.y(),
                                              mSelectedObject->mTransformation.translation.z());

        mRenderWindow->mSceneManager->mPawnSpawnLocation = mSelectedObject->mTransformation.translation.toVector3D();
        mSelectedObject->move(0,0,0);
    } else if(mSelectedCamera) {
        mSelectedCamera->updateGameStartTransform(QVector3D(ui->TranslateXValues->text().toFloat(),
                                                            mSelectedCamera->mGameStartTransformation.translation.y(),
                                                            mSelectedCamera->mGameStartTransformation.translation.z()),
                                                  QVector3D(mSelectedCamera->mGameStartTransformation.rotation.x(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.y(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.z()));
    }
}

void MainWindow::on_TranslateYValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setTranslationForAll(mSelectedObject->mTransformation.translation.x(),
                                              ui->TranslateYValues->text().toFloat(),
                                              mSelectedObject->mTransformation.translation.z());
        mRenderWindow->mSceneManager->mPawnSpawnLocation = mSelectedObject->mTransformation.translation.toVector3D();
        mSelectedObject->move(0,0,0);
    } else if(mSelectedCamera) {
        mSelectedCamera->updateGameStartTransform(QVector3D(mSelectedCamera->mGameStartTransformation.translation.x(),
                                                            ui->TranslateYValues->text().toFloat(),
                                                            mSelectedCamera->mGameStartTransformation.translation.z()),
                                                  QVector3D(mSelectedCamera->mGameStartTransformation.rotation.x(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.y(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.z()));
    }
}

void MainWindow::on_TranslateZValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setTranslationForAll(mSelectedObject->mTransformation.translation.x(),
                                              mSelectedObject->mTransformation.translation.y(),
                                              ui->TranslateZValues->text().toFloat());
        mRenderWindow->mSceneManager->mPawnSpawnLocation = mSelectedObject->mTransformation.translation.toVector3D();
        mSelectedObject->move(0,0,0);
    } else if(mSelectedCamera) {
        mSelectedCamera->updateGameStartTransform(QVector3D(mSelectedCamera->mGameStartTransformation.translation.x(),
                                                            mSelectedCamera->mGameStartTransformation.translation.y(),
                                                            ui->TranslateZValues->text().toFloat()),
                                                  QVector3D(mSelectedCamera->mGameStartTransformation.rotation.x(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.y(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.z()));
    }
}

void MainWindow::on_RotateRollValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setRotationForAll(mSelectedObject->mTransformation.rotation.x(),
                                              mSelectedObject->mTransformation.rotation.y(),
                                              ui->RotateRollValues->text().toFloat());
        mSelectedObject->rotate(0,0,0);
    }
}

void MainWindow::on_RotateYawValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setRotationForAll(mSelectedObject->mTransformation.rotation.x(),
                                              ui->RotateYawValues->text().toFloat(),
                                              mSelectedObject->mTransformation.rotation.z());
        mSelectedObject->rotate(0,0,0);
    } else if(mSelectedCamera) {
        mSelectedCamera->updateGameStartTransform(QVector3D(mSelectedCamera->mGameStartTransformation.translation.x(),
                                                            mSelectedCamera->mGameStartTransformation.translation.y(),
                                                            mSelectedCamera->mGameStartTransformation.translation.z()),
                                                  QVector3D(mSelectedCamera->mGameStartTransformation.rotation.x(),
                                                            ui->RotateYawValues->text().toFloat(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.z()));
    }
}

void MainWindow::on_RotatePitchValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setRotationForAll(ui->RotatePitchValues->text().toFloat(),
                                           mSelectedObject->mTransformation.rotation.y(),
                                              mSelectedObject->mTransformation.rotation.z());
        mSelectedObject->rotate(0,0,0);
    } else if(mSelectedCamera) {
        mSelectedCamera->updateGameStartTransform(QVector3D(mSelectedCamera->mGameStartTransformation.translation.x(),
                                                            mSelectedCamera->mGameStartTransformation.translation.y(),
                                                            mSelectedCamera->mGameStartTransformation.translation.z()),
                                                  QVector3D(ui->RotatePitchValues->text().toFloat(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.y(),
                                                            mSelectedCamera->mGameStartTransformation.rotation.z()));
    }
}

void MainWindow::on_ScaleXValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setScaleForAll(ui->ScaleXValues->text().toFloat(),
                                        mSelectedObject->mTransformation.scale.y(),
                                        mSelectedObject->mTransformation.scale.z());
        mSelectedObject->scale(mSelectedObject->mTransformation.scale.x(),
                               mSelectedObject->mTransformation.scale.y(),
                               mSelectedObject->mTransformation.scale.z());
    }
}

void MainWindow::on_ScaleYValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setScaleForAll(mSelectedObject->mTransformation.scale.x(),
                                        ui->ScaleYValues->text().toFloat(),
                                        mSelectedObject->mTransformation.scale.z());
        mSelectedObject->scale(mSelectedObject->mTransformation.scale.x(),
                               mSelectedObject->mTransformation.scale.y(),
                               mSelectedObject->mTransformation.scale.z());
    }
}

void MainWindow::on_ScaleZValues_textChanged()
{
    if(mSelectedObject) {
        mSelectedObject->setScaleForAll(mSelectedObject->mTransformation.scale.x(),
                                        mSelectedObject->mTransformation.scale.y(),
                                        ui->ScaleZValues->text().toFloat());
        mSelectedObject->scale(mSelectedObject->mTransformation.scale.x(),
                           mSelectedObject->mTransformation.scale.y(),
                           mSelectedObject->mTransformation.scale.z());
    }
}

void MainWindow::itemClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    if(item->type() == 1000) {
        mSelectedCamera = &mRenderWindow->mSceneManager->mCamera;
        mSelectedObject = nullptr;
    } else {
        mSelectedObject = mRenderWindow->mSceneManager->searchAllObjects(item->type()); // Finner objektet basert på dens ID
        mSelectedCamera = nullptr;
    }


    if(mSelectedObject || mSelectedCamera) {
        populateEditWindow();
    }
}

void MainWindow::populateEditWindow()
{
    if(mSelectedObject) {
        // Sets the name of the selected object
        ui->ObjectName->setText(QString::fromUtf8(mSelectedObject->mLabel.c_str()));

        // Adds the transform values to the QLineEdit-boxes
        ui->TranslateXValues->setText(QVariant(mSelectedObject->mTransformation.translation.x()).toString());
        ui->TranslateYValues->setText(QVariant(mSelectedObject->mTransformation.translation.y()).toString());
        ui->TranslateZValues->setText(QVariant(mSelectedObject->mTransformation.translation.z()).toString());

        // Adds the rotate values to the QLineEdit-boxes
        ui->RotateRollValues->setText(QVariant(mSelectedObject->mTransformation.rotation.x()).toString());
        ui->RotateYawValues->setText(QVariant(mSelectedObject->mTransformation.rotation.y()).toString());
        ui->RotatePitchValues->setText(QVariant(mSelectedObject->mTransformation.rotation.z()).toString());

        // Adds the scale values to the QLineEdit-boxes
        ui->ScaleXValues->setText(QVariant(mSelectedObject->mTransformation.scale.x()).toString());
        ui->ScaleYValues->setText(QVariant(mSelectedObject->mTransformation.scale.y()).toString());
        ui->ScaleZValues->setText(QVariant(mSelectedObject->mTransformation.scale.z()).toString());

        // Disables the Render Colliders checkbox, if it's not a TriangleSurface
        if(dynamic_cast<TriangleSurface*>(mSelectedObject)) {
            ui->renderCollidersCheckBox->setCheckable(true);
            ui->renderCollidersCheckBox->setStyleSheet("");
        } else {
            ui->renderCollidersCheckBox->setCheckable(false);
            ui->renderCollidersCheckBox->setStyleSheet("QCheckBox::indicator {image: url(:/images/checkbox_indeterminate.png);}");
        }
    } else {
        // Sets the name of the selected object
        ui->ObjectName->setText(QString::fromUtf8(mSelectedCamera->mLabel.c_str()));

        // Adds the transform values to the QLineEdit-boxes
        ui->TranslateXValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.translation.x()).toString());
        ui->TranslateYValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.translation.y()).toString());
        ui->TranslateZValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.translation.z()).toString());

        // Adds the rotate values to the QLineEdit-boxes
        ui->RotateRollValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.rotation.x()).toString());
        ui->RotateYawValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.rotation.y()).toString());
        ui->RotatePitchValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.rotation.z()).toString());

        // Adds the scale values to the QLineEdit-boxes
        ui->ScaleXValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.scale.x()).toString());
        ui->ScaleYValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.scale.y()).toString());
        ui->ScaleZValues->setText(QVariant(mSelectedCamera->mGameStartTransformation.scale.z()).toString());


        ui->renderCollidersCheckBox->setCheckable(false);
        ui->renderCollidersCheckBox->setStyleSheet("QCheckBox::indicator {image: url(:/images/checkbox_indeterminate.png);}");
    }

}


