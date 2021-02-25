#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gameOver->setVisible(false);
    img = new QImage(img_width, img_height, QImage::Format_RGB32);
    srand (time(NULL));
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(game()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(0, 0, *img);
    update();
}

void MainWindow::setup()
{
    // reset
    score = 0;
    lives = 3;
    cubePositionSpeed = 7;

    ui->scoreLabel->setText(QString::number(score));
    ui->livesLabel->setText(QString::number(lives));
    POV = Point3D(0, 0, -1);

    sphereLight = Light(0, 0, -1, 1);
    cubeLight = Light(0, 0, -1, 1);

    cubeTexture = Texture(cubeTextureImage, 2);
    sphereTexture = Texture(sphereTextureImage, 2);

    int randomPosition = rand() % 600 + 50;

    cube = Cube(50, randomPosition, cubePositionY);
    sphere = Sphere(img_width/2, img_height/2 + 233);

    transform_cube = Transform(230);
    transform_sphere = Transform(2.5);

    cube.create(2);
    sphere.create(3);
}

void MainWindow::game()
{
    draw.imgClear(img);

    if(leftKeyPressed)
    {
        if(sphere.positionX >= 80) // lewa krawedz widocznosci kuli
        {
            sphere.positionX -= spherePositionSpeed;
            valueSphere -= 3;
            sphereLight.x += lightSpeed;
        }
    }
    else if(rightKeyPressed)
    {
        if(sphere.positionX <= img->size().width() - 80) // prawa krawedz widocznosci kuli
        {
            sphere.positionX += spherePositionSpeed;
            valueSphere += 3;
            sphereLight.x -= lightSpeed;
        }
    }

/* KULA */
    transform_sphere.rotationz(0,0) =  cos((valueSphere * M_PI)/180.0);
    transform_sphere.rotationz(0,1) = -sin((valueSphere * M_PI)/180.0);
    transform_sphere.rotationz(1,0) =  sin((valueSphere * M_PI)/180.0);
    transform_sphere.rotationz(1,1) =  cos((valueSphere * M_PI)/180.0);

/* SZEŚCIAN */
    valueCube += cubeRotationSpeed;
    cube.positionY += cubePositionSpeed;
    transform_cube.rotationx(1,1) =  cos((valueCube * M_PI)/180.0);
    transform_cube.rotationx(1,2) = -sin((valueCube * M_PI)/180.0);
    transform_cube.rotationx(2,1) =  sin((valueCube * M_PI)/180.0);
    transform_cube.rotationx(2,2) =  cos((valueCube * M_PI)/180.0);

/* RYSOWANIE */
    cube.draw(img, &cubeTexture, f_ogniskowa, POV, cubeLight, transform_cube, drawEdges, drawTextures);
    sphere.draw(img, &sphereTexture, f_ogniskowa, POV, sphereLight, transform_sphere, drawEdges, drawTextures);
    /*for(auto &t : cubeTexture.triangles)
    {
        draw.drawLine(t.p1, t.p2, img, QColor(255,0,0));
        draw.drawLine(t.p2, t.p3, img, QColor(255,0,0));
        draw.drawLine(t.p3, t.p1, img, QColor(255,0,0));
    }*/

/* ZMIANA POZYCJI SZEŚCIANU */
    int range = 50;
    if(cube.positionY >= 460 && cube.positionY < 660) // poczatek kuli <-> dol ekranu
    {
        if(sphere.positionX - range <= cube.positionX && sphere.positionX + range >= cube.positionX)
        {
            int randomPosition = rand() % 600 + 50;
            cube.positionX = randomPosition;
            cube.positionY = cubePositionY;
            score++;
            ui->scoreLabel->setText(QString::number(score));
            if(score % 5 == 0)
                cubePositionSpeed++;
        }
    }
    else if(cube.positionY >= 660) // dol ekranu
    {
        int randomPosition = rand() % 600 + 50;
        cube.positionX = randomPosition;
        cube.positionY = cubePositionY;
        lives--;
        ui->livesLabel->setText(QString::number(lives));
        if(lives == 0)
        {
            ui->gameOver->setVisible(true);
            timer->stop();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Left)
        leftKeyPressed = true;
    else if(event->key() == Qt::Key_Right)
        rightKeyPressed = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Left)
        leftKeyPressed = false;
    else if(event->key() == Qt::Key_Right)
        rightKeyPressed = false;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    drawEdges = checked;
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    drawTextures = checked;
}

void MainWindow::on_startButton_clicked()
{
    Sleep(500);
    ui->gameOver->setVisible(false);
    timer->start(20);
    setup();
}
