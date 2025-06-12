# GraphicEditor

Простой графический редактор на Qt C++ с использованием QGraphicsScene/QGraphicsView и кастомной отрисовкой фигур через QPainter.

## Описание

Приложение позволяет:
- Добавлять графические примитивы (линии, прямоугольники, эллипсы, звёзды)
- Перемещать, редактировать и изменять размеры фигур
- Настраивать параметры отображения (цвет, шрифт)
- Использовать Undo/Redo с помощью `QUndoStack`
- Работать в интерфейсе на основе `QMainWindow` и `QGraphicsView`

## Технологии

- C++
- Qt 6 / Qt 5
- CMake
- QGraphicsScene / QGraphicsView / QGraphicsItem
- QPainter, QPen, QBrush, QFont
- QUndoCommand, QToolBar, QAction

##  Структура проекта

```bash
.
├── mainwindow.cpp/.h       # Главное окно и тулбар
├── customgraphicsscene.*   # Сцена с обработкой событий мыши
├── shape.*                 # Базовый графический элемент
├── graphicmodel.*          # Модель хранения сцены
├── graphiccontroller.*     # Логика взаимодействия
├── addshapecommand.*       # Команда для Undo/Redo
├── CMakeLists.txt

```
Для сборки проекта потребуется установленный **Qt** (версии 5.15+ или 6) и **CMake**.
## Сборка и запуск
1. git clone https://github.com/avdeey/GraphicEditor.git
2. Открыть проект в Qt Creator
3. Выбрать коплекты для сборки (Qt 6.x.x или Qt 5.x) и сконфигурировать проект
4. Молоток или CTRL+B - сборка проекта
5. CTRL+R - запуск


