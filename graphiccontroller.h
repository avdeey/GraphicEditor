// graphiccontroller.h
#ifndef GRAPHICCONTROLLER_H
#define GRAPHICCONTROLLER_H

#include <QObject>
#include <QUndoStack>
#include <QColor>
#include <QFont>
#include <QPointF>
#include "graphicmodel.h"
#include "shape.h"

enum class EditorMode {
    Select,
    CreateLine,
    CreateRect,
    CreateEllipse,
    CreateText,
    CreateStar
};

class GraphicController : public QObject {
    Q_OBJECT
public:
    explicit GraphicController(GraphicModel* model, QObject* parent = nullptr);

    void setEditorMode(EditorMode mode);

    void setCurrentColor(const QColor& c);
    QColor getCurrentColor() const;

    void setCurrentFont(const QFont& f);
    QFont getCurrentFont() const;

    void changeSelectedItemsFont(const QFont& f);
    void changeSelectedItemsColor(const QColor& c);

    void mousePressed(const QPointF& pos);
    void mouseMoved  (const QPointF& pos);
    void mouseReleased();

    void deleteSelectedItems();
    void clearAll();

    void undo();
    void redo();
    QUndoStack* undoStack() const { return m_undoStack; }

private:
    GraphicModel* m_model;
    QUndoStack*   m_undoStack;

    EditorMode    m_mode;
    QColor        m_currentColor;
    QFont         m_currentFont;

    Shape*        m_currentShape;
    Shape*        m_selectedShape;
    bool          m_isDrawing;
    bool          m_isMoving;
    QPointF       m_moveStartPos;
};

#endif // GRAPHICCONTROLLER_H
