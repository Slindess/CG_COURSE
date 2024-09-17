#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Drawer/QtDrawer.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui; // Указатель на сгенерированный класс из ui
    std::shared_ptr<QGraphicsScene> scene;
    std::shared_ptr<QtDrawer> drawer;
};


#endif // MAINWINDOW_H
