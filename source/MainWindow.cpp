#include "MainWindow.h"
#include "Drawer/QtDrawer.h"

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
    drawer->drawLine(0, 0, 400, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}



