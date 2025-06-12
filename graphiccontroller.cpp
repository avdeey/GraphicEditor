// graphiccontroller.cpp
#include "graphiccontroller.h"
#include "commands.h"
#include <QInputDialog>

GraphicController::GraphicController(GraphicModel* model, QObject* parent)
    : QObject(parent)
    , m_model(model)
    , m_undoStack(new QUndoStack(this))
    , m_mode(EditorMode::Select)
    , m_currentColor(Qt::black)
    , m_currentFont(QFont())
    , m_currentShape(nullptr)
    , m_selectedShape(nullptr)
    , m_isDrawing(false)
    , m_isMoving(false)
{ }

void GraphicController::setEditorMode(EditorMode mode) {
    m_mode = mode;
}

void GraphicController::setCurrentColor(const QColor& c) {
    m_currentColor = c;
}
QColor GraphicController::getCurrentColor() const {
    return m_currentColor;
}

void GraphicController::setCurrentFont(const QFont& f) {
    m_currentFont = f;
}
QFont GraphicController::getCurrentFont() const {
    return m_currentFont;
}

void GraphicController::changeSelectedItemsFont(const QFont& f) {
    for (Shape* s : m_model->getShapes())
        if (s->isSelected() && s->getType() == ShapeType::Text)
            s->setFont(f);
}

void GraphicController::changeSelectedItemsColor(const QColor& c) {
    for (Shape* s : m_model->getShapes()) {
        if (s->isSelected()) {
            QColor old = s->getColor();
            m_undoStack->push(new ColorCommand(s, old, c));
        }
    }
}

void GraphicController::undo() { m_undoStack->undo(); }
void GraphicController::redo() { m_undoStack->redo(); }

void GraphicController::mousePressed(const QPointF& pos) {
    if (m_mode == EditorMode::Select) {
        for (auto* it : m_model->getScene()->items(pos)) {
            if (auto* s = dynamic_cast<Shape*>(it)) {
                m_isMoving      = true;
                m_selectedShape = s;
                m_moveStartPos  = s->pos();
                return;
            }
        }
    }
    switch (m_mode) {
    case EditorMode::CreateLine:
        m_undoStack->push(new AddShapeCommand(m_model, ShapeType::Line, pos, m_currentColor, m_currentFont));
        break;
    case EditorMode::CreateRect:
        m_undoStack->push(new AddShapeCommand(m_model, ShapeType::Rectangle, pos, m_currentColor, m_currentFont));
        break;
    case EditorMode::CreateEllipse:
        m_undoStack->push(new AddShapeCommand(m_model, ShapeType::Ellipse, pos, m_currentColor, m_currentFont));
        break;
    case EditorMode::CreateStar:
        m_undoStack->push(new AddShapeCommand(m_model, ShapeType::Star, pos, m_currentColor, m_currentFont));
        break;
    case EditorMode::CreateText: {
        bool ok;
        QString txt = QInputDialog::getText(nullptr, "Enter Text", "Text:", QLineEdit::Normal, "", &ok);
        if (ok && !txt.isEmpty()) {
            m_undoStack->push(new AddShapeCommand(m_model, ShapeType::Text, pos, m_currentColor, m_currentFont));
            Shape* s = m_model->getShapes().last();
            s->setText(txt);
        }
        return;
    }
    default:
        return;
    }
    m_currentShape = m_model->getShapes().last();
    m_isDrawing    = true;
}

void GraphicController::mouseMoved(const QPointF& pos) {
    if (m_isMoving && m_selectedShape) {
        m_selectedShape->setPos(pos - m_selectedShape->boundingRect().center());
    } else if (m_isDrawing && m_currentShape) {
        m_currentShape->setEndPos(pos);
    }
}

void GraphicController::mouseReleased() {
    if (m_isMoving && m_selectedShape) {
        QPointF newPos = m_selectedShape->pos();
        if (newPos != m_moveStartPos)
            m_undoStack->push(new MoveShapeCommand(m_selectedShape, m_moveStartPos, newPos));
    }
    m_isMoving      = false;
    m_isDrawing     = false;
    m_selectedShape = nullptr;
    m_currentShape  = nullptr;
}

void GraphicController::deleteSelectedItems() {
    for (Shape* s : m_model->getShapes())
        if (s->isSelected())
            m_undoStack->push(new DeleteShapeCommand(m_model, s));
}

void GraphicController::clearAll() {
    m_undoStack->push(new ClearAllCommand(m_model, m_model->getShapes()));
    m_model->clear();
}
