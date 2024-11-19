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
#include "Manager/Manager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui; // Указатель на сгенерированный класс из ui
    std::shared_ptr<QGraphicsScene> scene;
    std::shared_ptr<QtDrawer> drawer;
    std::shared_ptr<Manager> manager;
};


#endif // MAINWINDOW_H
