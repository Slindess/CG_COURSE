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
#include "ui_mainWindow.h"
#include "Manager/Manager.h"
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    QTimer *timer = new QTimer(this);
    int melt_coef = 0;

private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_upButton_clicked();
    void on_downButton_clicked();
    void on_minusButton_clicked();
    void on_plusButton_clicked();
    void on_firstButton_clicked();
    void on_secondButton_clicked();
    void on_thirdButton_clicked();
    void on_fourthButton_clicked();
    void on_renderButton_clicked();
    void on_rockButton_clicked();
    void on_sandButton_clicked();
    void on_jungleButton_clicked();
    void on_heightButton_clicked();
    void on_sourceButton_clicked();
    void on_snowButton_clicked();
    void melt();

private:
    Ui::MainWindow *ui; // Указатель на сгенерированный класс из ui
    std::shared_ptr<QGraphicsScene> scene;
    std::shared_ptr<QtDrawer> drawer;
    std::shared_ptr<Manager> manager;
};


#endif // MAINWINDOW_H
