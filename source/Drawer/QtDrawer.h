#ifndef SOURCE_QTDRAWER_H
#define SOURCE_QTDRAWER_H

#include <QGraphicsScene>
#include <memory>

class QtDrawer
{
public:
    QtDrawer() = default;
    explicit QtDrawer(std::shared_ptr<QGraphicsScene> scene);
    virtual void drawPoint(double x1, double y1);
    virtual void drawLine(double x1, double y1, double x2, double y2);
    virtual void drawCircle(double x, double y, double radius);
    virtual void drawRectangle(double x, double y, double width, double height);
    virtual void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
    virtual void drawText(double x, double y, std::string text);
    virtual void setColor(int r, int g, int b);
    virtual void setLineWidth(int width);
    virtual void clear();
    void set_scene(std::shared_ptr<QGraphicsScene> scene) {_scene = scene;}
    ~QtDrawer() = default;
private:
    int color_r;
    int color_g;
    int color_b;
    int width;
    std::shared_ptr<QGraphicsScene> _scene;
};

#endif //SOURCE_QTDRAWER_H
