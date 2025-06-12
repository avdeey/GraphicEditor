// shape.cpp
#include "shape.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtMath>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFontMetrics>
#include <QPolygonF>

Shape::Shape(ShapeType type,
             const QPointF& startPos,
             const QColor& color,
             const QFont& font,
             QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , type(type)
    , startPos(startPos)
    , endPos(startPos)
    , color(color)
    , textFont(font)
    , isEditing(false)
    , currentHandle(None)
    , isResizing(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF Shape::boundingRect() const {
    return QRectF(startPos, endPos)
    .normalized()
        .adjusted(-10, -10, +10, +10);
}

void Shape::paint(QPainter* painter,
                  const QStyleOptionGraphicsItem* /*opt*/,
                  QWidget* /*w*/)
{
    // теперь QPainter и QPen уже полноценно видны
    painter->setPen(QPen(color, 2));

    switch (type) {
    case ShapeType::Line:
        painter->drawLine(startPos, endPos);
        break;
    case ShapeType::Rectangle:
        painter->drawRect(boundingRect());
        break;
    case ShapeType::Ellipse:
        painter->drawEllipse(boundingRect());
        break;
    case ShapeType::Star: {
        QRectF r = boundingRect();
        QPointF c = r.center();
        qreal  R = qMin(r.width(), r.height()) / 2;
        QPolygonF star;
        const int pts = 5;
        for (int i = 0; i < 2*pts; ++i) {
            qreal angle = M_PI/pts * i;
            qreal rad   = (i % 2 == 0 ? R : R/2);
            star << QPointF(
                c.x() + rad * qCos(angle - M_PI_2),
                c.y() + rad * qSin(angle - M_PI_2)
                );
        }
        painter->drawPolygon(star);
        break;
    }
    case ShapeType::Text: {
        QFontMetrics fm(textFont);
        QRectF tr(startPos,
                  QSizeF(fm.horizontalAdvance(text),
                         fm.height()));
        if (isSelected()) {
            painter->save();
            painter->setBrush(QColor(0,120,215,50));
            painter->setPen(Qt::NoPen);
            painter->drawRect(tr);
            painter->restore();
        }
        painter->setFont(textFont);
        painter->drawText(startPos + QPointF(0, fm.ascent()), text);
        break;
    }
    }

    // рамка и ручки ресайза
    if (isSelected() || isEditing) {
        painter->setPen(QPen(Qt::blue,1,Qt::DashLine));
        painter->drawRect(boundingRect());
        if (type != ShapeType::Text) {
            painter->setBrush(Qt::white);
            painter->setPen(QPen(Qt::black,1));
            for (int i = 1; i <= 4; ++i)
                painter->drawRect(getHandleRect(
                    static_cast<ResizeHandle>(i)
                    ));
        }
    }
}
void Shape::setEndPos(const QPointF& ep) {
    prepareGeometryChange();
    endPos = ep;
    update();
}

void Shape::setText(const QString& t) {
    text = t;
    update();
}

QString Shape::getText() const {
    return text;
}

void Shape::setFont(const QFont& f) {
    textFont = f;
    update();
}

QFont Shape::getFont() const {
    return textFont;
}

void Shape::setColor(const QColor& c) {
    color = c;
    update();
}

QColor Shape::getColor() const {
    return color;
}

void Shape::setEditing(bool e) {
    isEditing = e;
    update();
}

ShapeType Shape::getType() const {
    return type;
}

QPointF Shape::getStartPos() const {
    return startPos;
}

QPointF Shape::getEndPos() const {
    return endPos;
}

Shape::ResizeHandle Shape::getResizeHandle(const QPointF& pos) const {
    if (type == ShapeType::Text) return None;
    for (int i = 1; i <= 4; ++i) {
        ResizeHandle h = static_cast<ResizeHandle>(i);
        if (getHandleRect(h).contains(pos))
            return h;
    }
    return None;
}

QRectF Shape::getHandleRect(ResizeHandle handle) const {
    QRectF r = QRectF(startPos, endPos).normalized();
    const qreal hs = 8;
    switch (handle) {
    case TopLeft:
        return QRectF(r.topLeft()     - QPointF(hs/2, hs/2),
                      QSizeF(hs, hs));
    case TopRight:
        return QRectF(r.topRight()    - QPointF(hs/2, hs/2),
                      QSizeF(hs, hs));
    case BottomLeft:
        return QRectF(r.bottomLeft()  - QPointF(hs/2, hs/2),
                      QSizeF(hs, hs));
    case BottomRight:
        return QRectF(r.bottomRight() - QPointF(hs/2, hs/2),
                      QSizeF(hs, hs));
    default:
        return QRectF();
    }
}

void Shape::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        currentHandle = getResizeHandle(e->pos());
        isResizing    = (currentHandle != None);
        if (isResizing) {
            resizeStartPos = startPos;
            resizeStartEnd = endPos;
        }
    }
    QGraphicsItem::mousePressEvent(e);
}

void Shape::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (isResizing && (e->buttons() & Qt::LeftButton)) {
        prepareGeometryChange();
        QPointF d = e->scenePos() - e->lastScenePos();
        switch (currentHandle) {
        case TopLeft:     startPos += d; break;
        case TopRight:
            endPos.setX(endPos.x() + d.x());
            startPos.setY(startPos.y() + d.y());
            break;
        case BottomLeft:
            startPos.setX(startPos.x() + d.x());
            endPos.setY(endPos.y() + d.y());
            break;
        case BottomRight:
            endPos += d; break;
        default: break;
        }
        update();
    } else {
        QGraphicsItem::mouseMoveEvent(e);
    }
}

void Shape::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    isResizing    = false;
    currentHandle = None;
    QGraphicsItem::mouseReleaseEvent(e);
}

void Shape::hoverMoveEvent(QGraphicsSceneHoverEvent* e) {
    ResizeHandle h = getResizeHandle(e->pos());
    switch (h) {
    case TopLeft:
    case BottomRight:
        setCursor(QCursor(Qt::SizeFDiagCursor)); break;
    case TopRight:
    case BottomLeft:
        setCursor(QCursor(Qt::SizeBDiagCursor)); break;
    default:
        setCursor(QCursor(Qt::ArrowCursor));      break;
    }
    QGraphicsItem::hoverMoveEvent(e);
}
