// graphicmodel.h
#ifndef GRAPHICMODEL_H
#define GRAPHICMODEL_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QFont>
#include "customgraphicsscene.h"
#include "shape.h"

class GraphicModel : public QObject {
    Q_OBJECT
public:
    explicit GraphicModel(QObject* parent = nullptr);

    void addShape(ShapeType type,
                  const QPointF& pos,
                  const QColor& color,
                  const QFont& font = QFont());

    void removeShape(Shape* shape);
    void clear();

    QList<Shape*> getShapes() const;
    CustomGraphicsScene* getScene() const;

    // Для Undo/Redo:
    void addExistingShape(Shape* shape);
    void removeExistingShape(Shape* shape);
    void setShapes(const QVector<Shape*>& shapes);

signals:
    void sceneUpdated();

private:
    CustomGraphicsScene* scene;
    QList<Shape*>        shapes;
};

#endif // GRAPHICMODEL_H
