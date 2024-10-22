#include "QtDrawer.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <memory>
#include <iostream>

QtDrawer::QtDrawer(std::shared_ptr<QGraphicsScene> scene) : _scene(scene), color_r(0), color_g(0), color_b(0), width(1) {}

void QtDrawer::drawPoint(double x1, double y1)
{
    QPen pen(QColor(color_r, color_g, color_b));
    pen.setWidth(width);
    _scene->addEllipse(x1, y1, 1, 1, pen, QBrush(QColor(color_r, color_g, color_b)));
}

void QtDrawer::drawLine(double x1, double y1, double x2, double y2)
{
    QPen pen(QColor(color_r, color_g, color_b));
    pen.setWidth(width);
    _scene->addLine(x1, y1, x2, y2, pen);
}

void QtDrawer::drawCircle(double x, double y, double radius)
{
    QPen pen(QColor(color_r, color_g, color_b));
    pen.setWidth(width);
    _scene->addEllipse(x - radius, y - radius, 2 * radius, 2 * radius, pen);

}

void QtDrawer::drawRectangle(double x, double y, double width, double height)
{
    QPen pen(QColor(color_r, color_g, color_b));
    pen.setWidth(this->width);
    _scene->addRect(x, y, width, height, pen);
}

void QtDrawer::drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    // Создаем цвет на основе значений color_r, color_g, color_b
    QColor color(color_r, color_g, color_b);

    // Создаем контур с использованием цвета
    QPen pen(color);
    pen.setWidth(width);

    // Создаем кисть для заливки с тем же цветом
    QBrush brush(color);

    // Создание треугольника
    QPolygonF triangle;
    triangle << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3);

    // Добавляем треугольник с контуром и заливкой
    _scene->addPolygon(triangle, pen, brush);
}

void QtDrawer::drawText(double x, double y, std::string text)
{
    QGraphicsTextItem *textItem = _scene->addText(QString::fromStdString(text));
    textItem->setDefaultTextColor(QColor(color_r, color_g, color_b));
    textItem->setPos(x, y);
}

void QtDrawer::setColor(int r, int g, int b)
{
    color_r = r;
    color_g = g;
    color_b = b;
}

void QtDrawer::setColor(Color color)
{
    color_r = color.r;
    color_g = color.g;
    color_b = color.b;
}

void QtDrawer::setLineWidth(int width)
{
    this->width = width;
}

void QtDrawer::clear()
{
    _scene->clear();
}


