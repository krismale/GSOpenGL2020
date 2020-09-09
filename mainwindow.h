#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "visualobject.h"
#include "camera.h"

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Knapper og item clicks
    void on_playButton_clicked();
    void on_wireframeButton_clicked();
    void on_axisButton_clicked();
    void itemClicked(QListWidgetItem *item);
    void on_moveToButton_clicked();
    void on_renderCollidersCheckBox_clicked();
    void on_resetGameButton_clicked();

    // Transform QLineEdit-boxes
    void on_TranslateXValues_textChanged();
    void on_TranslateYValues_textChanged();
    void on_TranslateZValues_textChanged();
    void on_RotateRollValues_textChanged();
    void on_RotateYawValues_textChanged();
    void on_RotatePitchValues_textChanged();
    void on_ScaleXValues_textChanged();
    void on_ScaleYValues_textChanged();
    void on_ScaleZValues_textChanged();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    VisualObject * mSelectedObject = nullptr; // Hvilket objekt som er valgt i outlineren
    Camera * mSelectedCamera = nullptr; // Fylles inn dersom kameraet blir valgt

    void populateOutliner(); // Fyller outlineren med objekter
    void populateEditWindow(); // Fyller inn edit vinduet, med transforms etc.
};

#endif // MAINWINDOW_H
