// commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QColor>
#include <QFont>
#include <QPointF>
#include <QList>
#include "graphicmodel.h"
#include "shape.h"

// Добавление новой фигуры
class AddShapeCommand : public QUndoCommand {
public:
    AddShapeCommand(GraphicModel* model,
                    ShapeType type,
                    const QPointF& pos,
                    const QColor& color,
                    const QFont& font,
                    QUndoCommand* parent = nullptr)
        : QUndoCommand("Add Shape", parent)
        , m_model(model)
        , m_type(type)
        , m_pos(pos)
        , m_color(color)
        , m_font(font)
        , m_shape(nullptr)
    {}

    void undo() override {
        m_model->removeExistingShape(m_shape);
    }

    void redo() override {
        if (!m_shape) {
            m_model->addShape(m_type, m_pos, m_color, m_font);
            m_shape = m_model->getShapes().last();
        } else {
            m_model->addExistingShape(m_shape);
        }
    }

private:
    GraphicModel* m_model;
    ShapeType     m_type;
    QPointF       m_pos;
    QColor        m_color;
    QFont         m_font;
    Shape*        m_shape;
};

// Удаление фигуры
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

// Перемещение фигуры
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

    void undo() override {
        m_shape->setPos(m_from);
    }

    void redo() override {
        m_shape->setPos(m_to);
    }

private:
    Shape*   m_shape;
    QPointF  m_from;
    QPointF  m_to;
};

// Смена цвета
class ColorCommand : public QUndoCommand {
public:
    ColorCommand(Shape* shape,
                 const QColor& oldColor,
                 const QColor& newColor,
                 QUndoCommand* parent = nullptr)
        : QUndoCommand("Change Color", parent)
        , m_shape(shape)
        , m_oldColor(oldColor)
        , m_newColor(newColor)
    {}

    void undo() override {
        m_shape->setColor(m_oldColor);
    }

    void redo() override {
        m_shape->setColor(m_newColor);
    }

private:
    Shape*   m_shape;
    QColor   m_oldColor;
    QColor   m_newColor;
};

// Очистить всё (мягко удаляет, чтобы можно было вернуть)
class ClearAllCommand : public QUndoCommand {
public:
    ClearAllCommand(GraphicModel* model,
                    const QList<Shape*>& shapes,
                    QUndoCommand* parent = nullptr)
        : QUndoCommand("Clear All", parent)
        , m_model(model)
        , m_shapes(shapes)
    {}

    void undo() override {
        for (Shape* s : m_shapes)
            m_model->addExistingShape(s);
    }

    void redo() override {
        for (Shape* s : m_shapes)
            m_model->removeExistingShape(s);
    }

private:
    GraphicModel*      m_model;
    QList<Shape*>      m_shapes;
};

#endif // COMMANDS_H
