#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QKeyEvent>
#include <string>
#include "point3d.h"
#include "sphere.h"
#include "transform.h"
#include "cube.h"
#include "windows.h"
#include "light.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void game();

private:
    int score = 0;
    int lives = 3;
    Ui::MainWindow *ui;
    QImage *img;
    Texture cubeTexture, sphereTexture;
    QImage *sphereTextureImage = new QImage(":/images/sphereTexture.jpg");
    QImage *cubeTextureImage = new QImage(":/images/box.jpg");
    QTimer *timer;
    Draw draw;

    Sphere sphere;
    Cube cube;
    Transform transform_sphere, transform_cube;

    Point3D POV;
    Light sphereLight, cubeLight;

    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool drawEdges = false;
    bool drawTextures = true;

    double cubePositionX = 0;
    double cubePositionY = -50;
    double valueCube = 0;
    double valueSphere = 0;

    double lightSpeed = 0.07;
    double spherePositionSpeed = 7;
    double cubeRotationSpeed = 2;
    double cubePositionSpeed = 7;

    const int img_width = 800;
    const int img_height = 600;
    int f_ogniskowa = 150;

    void setup();
    void insertPixel(int x, int y, QColor c);

private slots:
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void paintEvent(QPaintEvent*);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_startButton_clicked();
};
#endif // MAINWINDOW_H
