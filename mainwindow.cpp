// mainwindow.cpp
#include "mainwindow.h"
#include <QAction>
#include <QColorDialog>
#include <QStyle>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , view(nullptr)
    , toolBar(nullptr)
    , fontCombo(nullptr)
    , sizeCombo(nullptr)
    , boldBtn(nullptr)
    , italicBtn(nullptr)
    , underlineBtn(nullptr)
    , model(new GraphicModel(this))
    , controller(new GraphicController(model, this))
{
    setupUI();
    setupToolBar();
    setupConnections();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    view = new QGraphicsView(this);
    view->setScene(model->getScene());
    view->setRenderHint(QPainter::Antialiasing);

    // Полный режим обновления: при любом изменении сцены вьюпорт перерисовывается целиком
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    view->setDragMode(QGraphicsView::RubberBandDrag);
    setCentralWidget(view);

    toolBar = new QToolBar("Tools", this);
    addToolBar(Qt::LeftToolBarArea, toolBar);
}

void MainWindow::setupToolBar() {
    // Режимы рисования
    QAction* selectAction  = toolBar->addAction("Select");
    QAction* lineAction    = toolBar->addAction("Line");
    QAction* rectAction    = toolBar->addAction("Rectangle");
    QAction* ellipseAction = toolBar->addAction("Ellipse");
    QAction* starAction    = toolBar->addAction("Star");
    QAction* textAction    = toolBar->addAction("Text");
    toolBar->addSeparator();

    // Цвет
    QAction* colorAction = toolBar->addAction("Color");
    toolBar->addSeparator();

    // Удаление/очистка
    QAction* deleteAction = toolBar->addAction("Delete");
    QAction* clearAction  = toolBar->addAction("Clear");
    toolBar->addSeparator();

    // Шрифтовые виджеты
    fontCombo    = new QFontComboBox(this);
    sizeCombo    = new QComboBox(this);
    boldBtn      = new QToolButton(this);
    italicBtn    = new QToolButton(this);
    underlineBtn = new QToolButton(this);

    // Заполнить sizeCombo
    for (int i = 8; i <= 48; i += 2)
        sizeCombo->addItem(QString::number(i));

    // Кнопка "B"
    boldBtn->setText("B");
    boldBtn->setCheckable(true);
    {
        QFont f = boldBtn->font();
        f.setBold(true);
        boldBtn->setFont(f);
    }

    // Кнопка "I"
    italicBtn->setText("I");
    italicBtn->setCheckable(true);
    {
        QFont f = italicBtn->font();
        f.setItalic(true);
        italicBtn->setFont(f);
    }

    // Кнопка "U"
    underlineBtn->setText("U");
    underlineBtn->setCheckable(true);
    {
        QFont f = underlineBtn->font();
        f.setUnderline(true);
        underlineBtn->setFont(f);
    }

    // Добавить в тулбар
    toolBar->addWidget(fontCombo);
    toolBar->addWidget(sizeCombo);
    toolBar->addWidget(boldBtn);
    toolBar->addWidget(italicBtn);
    toolBar->addWidget(underlineBtn);
    toolBar->addSeparator();

    // Undo/Redo стрелками
    QAction* undoAct = new QAction(style()->standardIcon(QStyle::SP_ArrowBack),  "", this);
    QAction* redoAct = new QAction(style()->standardIcon(QStyle::SP_ArrowForward), "", this);
    undoAct->setToolTip("Undo");
    redoAct->setToolTip("Redo");
    toolBar->addAction(undoAct);
    toolBar->addAction(redoAct);

    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // Сигналы тулбара
    connect(selectAction,  &QAction::triggered, this, &MainWindow::onSelectAction);
    connect(lineAction,    &QAction::triggered, this, &MainWindow::onLineAction);
    connect(rectAction,    &QAction::triggered, this, &MainWindow::onRectAction);
    connect(ellipseAction, &QAction::triggered, this, &MainWindow::onEllipseAction);
    connect(starAction,    &QAction::triggered, this, &MainWindow::onStarAction);
    connect(textAction,    &QAction::triggered, this, &MainWindow::onTextAction);
    connect(colorAction,   &QAction::triggered, this, &MainWindow::onColorAction);
    connect(deleteAction,  &QAction::triggered, this, &MainWindow::onDeleteAction);
    connect(clearAction,   &QAction::triggered, this, &MainWindow::onClearAction);
    connect(undoAct,       &QAction::triggered, this, &MainWindow::onUndoAction);
    connect(redoAct,       &QAction::triggered, this, &MainWindow::onRedoAction);
}

void MainWindow::setupConnections() {
    // Обновлять вьюпорт при любом изменении модели
    connect(model, &GraphicModel::sceneUpdated,
            view->viewport(), QOverload<>::of(&QWidget::update));

    // Сцена мыши
    auto sc = model->getScene();
    connect(sc, &CustomGraphicsScene::sceneMousePressed,  this, &MainWindow::handleMousePressed);
    connect(sc, &CustomGraphicsScene::sceneMouseMoved,    this, &MainWindow::handleMouseMoved);
    connect(sc, &CustomGraphicsScene::sceneMouseReleased, this, &MainWindow::handleMouseReleased);

    // Шрифтовые элементы
    connect(fontCombo, &QFontComboBox::currentFontChanged,          this, &MainWindow::onFontChanged);
    connect(sizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSizeChanged);
    connect(boldBtn,      &QToolButton::toggled, this, &MainWindow::onBoldToggled);
    connect(italicBtn,    &QToolButton::toggled, this, &MainWindow::onItalicToggled);
    connect(underlineBtn, &QToolButton::toggled, this, &MainWindow::onUnderlineToggled);
}

void MainWindow::onSelectAction()  { controller->setEditorMode(EditorMode::Select);    view->setDragMode(QGraphicsView::RubberBandDrag); }
void MainWindow::onLineAction()    { controller->setEditorMode(EditorMode::CreateLine); view->setDragMode(QGraphicsView::NoDrag); }
void MainWindow::onRectAction()    { controller->setEditorMode(EditorMode::CreateRect); view->setDragMode(QGraphicsView::NoDrag); }
void MainWindow::onEllipseAction() { controller->setEditorMode(EditorMode::CreateEllipse); view->setDragMode(QGraphicsView::NoDrag); }
void MainWindow::onStarAction()    { controller->setEditorMode(EditorMode::CreateStar); view->setDragMode(QGraphicsView::NoDrag); }
void MainWindow::onTextAction()    { controller->setEditorMode(EditorMode::CreateText); view->setDragMode(QGraphicsView::NoDrag); }

void MainWindow::onColorAction() {
    QColor c = QColorDialog::getColor(controller->getCurrentColor(), this, "Select Color");
    if (c.isValid()) {
        controller->setCurrentColor(c);
        controller->changeSelectedItemsColor(c);
    }
}

void MainWindow::onDeleteAction() { controller->deleteSelectedItems(); }
void MainWindow::onClearAction()  { controller->clearAll();          }
void MainWindow::onUndoAction()   { controller->undo();              }
void MainWindow::onRedoAction()   { controller->redo();              }

void MainWindow::onFontChanged(const QFont& font) {
    QFont f = controller->getCurrentFont();
    f.setFamily(font.family());
    controller->setCurrentFont(f);
    controller->changeSelectedItemsFont(f);
}

void MainWindow::onSizeChanged(int index) {
    int size = sizeCombo->itemText(index).toInt();
    QFont f = controller->getCurrentFont();
    f.setPointSize(size);
    controller->setCurrentFont(f);
}

void MainWindow::onBoldToggled(bool checked) {
    QFont f = controller->getCurrentFont();
    f.setBold(checked);
    controller->setCurrentFont(f);
    controller->changeSelectedItemsFont(f);
}

void MainWindow::onItalicToggled(bool checked) {
    QFont f = controller->getCurrentFont();
    f.setItalic(checked);
    controller->setCurrentFont(f);
    controller->changeSelectedItemsFont(f);
}

void MainWindow::onUnderlineToggled(bool checked) {
    QFont f = controller->getCurrentFont();
    f.setUnderline(checked);
    controller->setCurrentFont(f);
    controller->changeSelectedItemsFont(f);
}

void MainWindow::handleMousePressed(const QPointF& pos)  { controller->mousePressed(pos);  }
void MainWindow::handleMouseMoved  (const QPointF& pos)  { controller->mouseMoved(pos);    }
void MainWindow::handleMouseReleased()                  { controller->mouseReleased();    }

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete)
        controller->deleteSelectedItems();
    QMainWindow::keyPressEvent(event);
}
