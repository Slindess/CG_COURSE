#include "MainWindow.h"
#include "Drawer/QtDrawer.h"
#include "Manager/Manager.h"
#include <iostream>
#include <thread>
#include <chrono>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = std::make_shared<QGraphicsScene>(this);
    ui->graphicsView->setScene(scene.get());
    auto cont = ui->graphicsView->contentsRect();

    scene->setSceneRect(0, 0, cont.width(), cont.height());
    drawer = std::make_shared<QtDrawer>(scene);
    drawer->setColor(255, 0, 0);

    manager = std::make_shared<Manager>();
    manager->SetDrawer(drawer);
    scene->clear();
    //manager->Manage();

    //drawer->drawLine(0, 0, 400, 400);
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Plus)
    {
        scene->clear();
        manager->CamPlus();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_Minus)
    {
        scene->clear();
        manager->CamMinus();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_Up)
    {
        scene->clear();
        manager->CamUp();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_Down)
    {
        scene->clear();
        manager->CamDown();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_Left)
    {
        scene->clear();
        manager->CamLeft();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_Right)
    {
        scene->clear();
        manager->CamRight();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_W)
    {
        scene->clear();
        manager->CamPitchUp();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_S)
    {
        scene->clear();
        manager->CamPitchDown();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_A)
    {
        scene->clear();
        manager->CamYawLeft();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_D)
    {
        scene->clear();
        manager->CamYawRight();
        manager->Manage();
    }
    else if (event->key() == Qt::Key_M)
    {
        connect(timer, &QTimer::timeout, this, &MainWindow::melt);
        timer->start(5000);
    }

    QWidget::keyPressEvent(event);

}

void MainWindow::on_leftButton_clicked()
{
    scene->clear();
    manager->CamLeft();
    manager->Manage();
}

void MainWindow::on_rightButton_clicked()
{
    scene->clear();
    manager->CamRight();
    manager->Manage();
}

void MainWindow::on_upButton_clicked()
{
    scene->clear();
    manager->CamUp();
    manager->Manage();
}


void MainWindow::on_downButton_clicked()
{
    scene->clear();
    manager->CamDown();
    manager->Manage();
}

void MainWindow::on_minusButton_clicked()
{
    scene->clear();
    manager->CamMinus();
    manager->Manage();
}

void MainWindow::on_plusButton_clicked()
{
    scene->clear();
    manager->CamPlus();
    manager->Manage();
}

void MainWindow::on_firstButton_clicked()
{
    manager->SetCameraPosition(1);
}

void MainWindow::on_secondButton_clicked()
{
    manager->SetCameraPosition(2);
}

void MainWindow::on_thirdButton_clicked()
{
    manager->SetCameraPosition(3);
}

void MainWindow::on_fourthButton_clicked()
{
    manager->SetCameraPosition(4);
}

void MainWindow::on_renderButton_clicked()
{
    scene->clear();
    manager->Manage();
}

void MainWindow::on_rockButton_clicked()
{
    manager->ResetLandScape(1);
}


void MainWindow::on_sandButton_clicked()
{
    manager->ResetLandScape(2);
}

void MainWindow::on_jungleButton_clicked()
{
    manager->ResetLandScape(3);
}

void MainWindow::on_heightButton_clicked()
{
    bool ok;
    int value = ui->height->text().toInt(&ok);
    if (ok)
    {
        manager->ResetHeight(value);
    }
}


void MainWindow::on_sourceButton_clicked()
{
    manager->Source(ui->source->text().toStdString());
}

void MainWindow::on_snowButton_clicked()
{
    bool ok;
    int value = ui->snow->text().toInt(&ok);
    if (ok)
    {
        melt_coef = value;
        manager->Snow(value);
    }
}

void MainWindow::melt()
{
    if (melt_coef == 10)
        melt_coef = 6;
    manager->Snow(melt_coef, 1);
    on_renderButton_clicked();
    
    melt_coef -= 2;
    if (melt_coef < 0)
    {
        manager->Snow(0);
        on_renderButton_clicked();
        timer->stop();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}



