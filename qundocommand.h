#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "graphicmodel.h"
#include "shape.h"

// 1) AddShapeCommand
class AddShapeCommand : public QUndoCommand {
public:
    AddShapeCommand(GraphicModel* model,
                    ShapeType type,
                    const QPointF& pos,
                    const QColor& color,
                    QUndoCommand* parent = nullptr)
        : QUndoCommand("Add Shape", parent)
        , m_model(model)
        , m_type(type)
        , m_pos(pos)
        , m_color(color)
        , m_shape(nullptr)
    {}

    void undo() override {
        m_model->removeExistingShape(m_shape);
    }
    void redo() override {
        if (!m_shape) {
            // первый раз: создаём новую
            m_model->addShape(m_type, m_pos, m_color);
            m_shape = m_model->getShapes().last();
        } else {
            // повторно: возвращаем старый объект
            m_model->addExistingShape(m_shape);
        }
    }

private:
    GraphicModel* m_model;
    ShapeType      m_type;
    QPointF        m_pos;
    QColor         m_color;
    Shape*         m_shape;
};

// 2) DeleteShapeCommand
class DeleteShapeCommand : public QUndoCommand {
public:
    DeleteShapeCommand(GraphicModel* model,
                       Shape* shape,
                       QUndoCommand* parent = nullptr)
        : QUndoCommand("Delete Shape", parent)
        , m_model(model)
        , m_shape(shape)
    {}

    void undo() override {
        m_model->addExistingShape(m_shape);
    }
    void redo() override {
        m_model->removeExistingShape(m_shape);
    }

private:
    GraphicModel* m_model;
    Shape*        m_shape;
};

// 3) MoveShapeCommand
class MoveShapeCommand : public QUndoCommand {
public:
    MoveShapeCommand(Shape* shape,
                     const QPointF& from,
                     const QPointF& to,
                     QUndoCommand* parent = nullptr)
        : QUndoCommand("Move Shape", parent)
        , m_shape(shape)
        , m_from(from)
        , m_to(to)
    {}

    void undo() override { m_shape->setPos(m_from); }
    void redo() override { m_shape->setPos(m_to); }

private:
    Shape*   m_shape;
    QPointF  m_from, m_to;
};

#endif // COMMANDS_H
