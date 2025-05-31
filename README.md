# Алгоритм Флойда–Уоршелла

C++ приложение на Qt для расчета и визуализации кратчайших путей в ориентированном графе, заданном матрицей смежности, с использованием алгоритма Флойда–Уоршелла. Графический интерфейс реализован на Qt Widgets и QCustomPlot.

```
Путь из вершины vᵢ в vⱼ орграфа называется кратчайшим, если он содержит наименьшее количество дуг среди всех путей из vᵢ в vⱼ.
```

## Структура проекта
```text
CMakeLists.txt
GraphPlotter.cpp
GraphPlotter.h
main.cpp
MainWindow.cpp
MainWindow.h
MatrixCalculator.cpp
MatrixCalculator.h
qcustomplot.cpp
qcustomplot.h
README.md
```

## Требования

- macOS / Windows / Linux
- Qt 6 с модулями Widgets и PrintSupport
- Компилятор C++23 (clang++)
- CMake >= 4.0.2

## Сборка
```bash
mkdir build
cd build
cmake ..
make
```

## Запуск

После успешной сборки в папке ```build``` будет Ваша программа.

После запуска откроется GUI, в котором вы можете задать матрицу смежности (включая взвешенные данные) и запустить выполнение алгоритма.

## Зависимости

- Qt (https://www.qt.io/) — GUI фреймворк
- QCustomPlot (https://www.qcustomplot.com/) — библиотека для построения графиков

## Лицензия

Этот проект распространяется под лицензией GNU GPL v3.
См. файл [COPYING](COPYING) для подробностей.
