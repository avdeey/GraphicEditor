// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QToolBar>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QKeyEvent>
#include "graphicmodel.h"
#include "graphiccontroller.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onSelectAction();
    void onLineAction();
    void onRectAction();
    void onEllipseAction();
    void onStarAction();
    void onTextAction();
    void onColorAction();
    void onDeleteAction();
    void onClearAction();
    void onUndoAction();
    void onRedoAction();

    void onFontChanged(const QFont& font);
    void onSizeChanged(int index);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onUnderlineToggled(bool checked);

    void handleMousePressed (const QPointF& pos);
    void handleMouseMoved   (const QPointF& pos);
    void handleMouseReleased();

private:
    void setupUI();
    void setupToolBar();
    void setupConnections();

    QGraphicsView* view;
    QToolBar*      toolBar;

    QFontComboBox* fontCombo;
    QComboBox*     sizeCombo;
    QToolButton*   boldBtn;
    QToolButton*   italicBtn;
    QToolButton*   underlineBtn;

    GraphicModel*      model;
    GraphicController* controller;
};

#endif // MAINWINDOW_H
