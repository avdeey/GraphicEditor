// shape.h
#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QColor>
#include <QFont>
#include <QString>
#include <QPointF>

enum class ShapeType { Line, Rectangle, Ellipse, Text, Star };

class Shape : public QGraphicsItem {
public:
    Shape(ShapeType type,
          const QPointF& startPos,
          const QColor& color,
          const QFont& font = QFont(),
          QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // Геометрия
    QPointF getStartPos() const;
    QPointF getEndPos()   const;
    void    setEndPos(const QPointF& endPos);

    // Текст
    void    setText(const QString& text);
    QString getText() const;

    void    setFont(const QFont& font);
    QFont   getFont() const;

    // Цвет
    void   setColor(const QColor& color);
    QColor getColor() const;

    // Режим редактирования текста
    void    setEditing(bool editing);

    // Тип фигуры
    ShapeType getType() const;

protected:
    void mousePressEvent  (QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent   (QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent   (QGraphicsSceneHoverEvent* event) override;

private:
    enum ResizeHandle { None, TopLeft, TopRight, BottomLeft, BottomRight };
    ResizeHandle getResizeHandle(const QPointF& pos) const;
    QRectF        getHandleRect(ResizeHandle handle) const;

    ShapeType type;
    QPointF   startPos;
    QPointF   endPos;
    QColor    color;

    QString   text;
    QFont     textFont;
    bool      isEditing;

    ResizeHandle currentHandle;
    bool         isResizing;
    QPointF      resizeStartPos;
    QPointF      resizeStartEnd;
};

#endif // SHAPE_H
