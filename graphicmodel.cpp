// graphicmodel.cpp
#include "graphicmodel.h"

GraphicModel::GraphicModel(QObject* parent)
    : QObject(parent)
    , scene(new CustomGraphicsScene(this))
{
    scene->setSceneRect(-500, -500, 1000, 1000);
}

void GraphicModel::addShape(ShapeType type,
                            const QPointF& pos,
                            const QColor& color,
                            const QFont& font)
{
    Shape* s = new Shape(type, pos, color, font);
    shapes.append(s);
    scene->addItem(s);
    emit sceneUpdated();
}

void GraphicModel::removeShape(Shape* s) {
    if (shapes.removeOne(s)) {
        scene->removeItem(s);
        delete s;
        emit sceneUpdated();
    }
}

void GraphicModel::clear() {
    for (Shape* s : shapes) {
        scene->removeItem(s);
        delete s;
    }
    shapes.clear();
    emit sceneUpdated();
}

QList<Shape*> GraphicModel::getShapes() const {
    return shapes;
}

CustomGraphicsScene* GraphicModel::getScene() const {
    return scene;
}

void GraphicModel::addExistingShape(Shape* s) {
    if (!shapes.contains(s)) {
        shapes.append(s);
        scene->addItem(s);
        emit sceneUpdated();
    }
}

void GraphicModel::removeExistingShape(Shape* s) {
    if (shapes.removeOne(s)) {
        scene->removeItem(s);
        emit sceneUpdated();
    }
}

void GraphicModel::setShapes(const QVector<Shape*>& arr) {
    clear();
    for (Shape* s : arr) {
        shapes.append(s);
        scene->addItem(s);
    }
    emit sceneUpdated();
}
