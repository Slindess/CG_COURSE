#include "MainWindow.h"
#include "Drawer/QtDrawer.h"
#include "Manager/Manager.h"

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
    manager->Manage();

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

    QWidget::keyPressEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}



