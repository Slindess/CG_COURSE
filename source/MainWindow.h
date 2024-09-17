#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui; // Указатель на сгенерированный класс из ui
    QGraphicsScene *scene;
    QGraphicsView *view;
};


#endif // MAINWINDOW_H
