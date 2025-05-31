#include "MainWindow.h"
#include "GraphPlotter.h"
#include "MatrixCalculator.h"
#include <QApplication>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QMargins>
#include <QPalette>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <cmath>
#include <sstream>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central = new QWidget(this);
  this->setCentralWidget(central);
  QFont originalAppFont = qApp->font();
  QFont defaultFont("Courier New", originalAppFont.pointSize() + 1,
                    QFont::Bold);
  defaultFont.setStyleHint(QFont::Monospace);
  defaultFont.setFixedPitch(true);
  qApp->setFont(defaultFont);
  QHBoxLayout *mainLayout = new QHBoxLayout(central);
  mainLayout->setContentsMargins(15, 15, 15, 15);
  mainLayout->setSpacing(10);
  QVBoxLayout *controlsLayout = new QVBoxLayout;
  controlsLayout->setSpacing(10);
  mainLayout->addLayout(controlsLayout);
  QLabel *matrixLabel =
      new QLabel("Введите матрицу смежности для задания графа:", central);
  matrixLabel->setFont(defaultFont);
  controlsLayout->addWidget(matrixLabel);
  matrixEdit = new QPlainTextEdit(central);
  matrixEdit->setPlaceholderText(
      "Например: \n0 1 0 0\n1 0 1 0\n0 1 0 1\n0 0 1 0");
  matrixEdit->setFont(defaultFont);
  computeButton =
      new QPushButton("Применить алгоритм Флойда–Уоршелла", central);
  computeButton->setFont(defaultFont);
  outputEdit = new QPlainTextEdit(central);
  outputEdit->setFont(defaultFont);
  outputEdit->setReadOnly(true);
  outputEdit->setPlaceholderText("Здесь появится результат работы алгоритма");
  controlsLayout->addWidget(matrixEdit);
  controlsLayout->addWidget(computeButton);
  QLabel *resultLabel = new QLabel("Результат работы алгоритма:", central);
  resultLabel->setFont(defaultFont);
  controlsLayout->addWidget(resultLabel);
  controlsLayout->addWidget(outputEdit);
  QPushButton *resetButton = new QPushButton("Сбросить", central);
  resetButton->setFont(defaultFont);
  resetButton->setToolTip("Очистить поля ввода и граф");
  controlsLayout->addWidget(resetButton);
  QString commonButtonStyle = R"(
        QPushButton {
            background-color: #216fff;
            padding: 8px 18px;
            font-size: 15px;
            margin: 4px 2px;
            border-radius: 12px;
            min-height: 32px;
            font-weight: bold; /* Changed from 500 to bold */
        }
        QPushButton:hover {
            background-color: #195ada;
        }
        QPushButton:pressed {
            background-color: #1446aa;
        }
    )";
  resetButton->setStyleSheet(commonButtonStyle);
  connect(resetButton, &QPushButton::clicked, this, [this]() {
    matrixEdit->clear();
    outputEdit->clear();
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->replot();
  });

  QWidget *rightAreaWidget = new QWidget(central);
  QVBoxLayout *plotAreaLayout = new QVBoxLayout(rightAreaWidget);

  QLabel *plotLabel = new QLabel("Визуализация графа:", rightAreaWidget);
  plotLabel->setFont(defaultFont);
  plotLabel->setAlignment(Qt::AlignCenter);
  plotAreaLayout->addWidget(plotLabel);

  customPlot = new QCustomPlot(rightAreaWidget);
  customPlot->setToolTip("Визуализация графа по введённой матрице");
  plotAreaLayout->addWidget(customPlot);
  mainLayout->addWidget(rightAreaWidget);
  customPlot->setMinimumSize(550, 550);
  customPlot->setMaximumSize(550, 550);
  customPlot->axisRect()->setupFullAxesBox(true);
  customPlot->xAxis->setTicks(false);
  customPlot->yAxis->setTicks(false);
  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);
  customPlot->xAxis->grid()->setVisible(false);
  customPlot->yAxis->grid()->setVisible(false);
  customPlot->yAxis2->grid()->setVisible(false);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setTickLabels(false);

  mainLayout->setStretchFactor(plotAreaLayout, 1);
  plotAreaLayout->setStretchFactor(customPlot, 1);

  setWindowTitle("Кратчайшие пути между всеми парами вершин");
  connect(computeButton, &QPushButton::clicked, this, &MainWindow::computeFW);
  QPalette lightPalette;
  QColor mainWhite(255, 255, 255);
  lightPalette.setColor(QPalette::Window, mainWhite);
  lightPalette.setColor(QPalette::WindowText, Qt::black);
  lightPalette.setColor(QPalette::PlaceholderText, Qt::gray);
  lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
  lightPalette.setColor(QPalette::ToolTipText, Qt::black);
  lightPalette.setColor(QPalette::Text, Qt::black);
  lightPalette.setColor(QPalette::Button, mainWhite);
  lightPalette.setColor(QPalette::ButtonText, Qt::white);
  lightPalette.setColor(QPalette::BrightText, Qt::red);
  lightPalette.setColor(QPalette::Link, QColor(33, 111, 255));
  lightPalette.setColor(QPalette::Highlight, QColor(220, 230, 255));
  lightPalette.setColor(QPalette::HighlightedText, Qt::black);
  qApp->setPalette(lightPalette);
  QString textEditStyle = R"(
        QPlainTextEdit {
            border: 1.5px solid #d2d2d2;
            border-radius: 10px;
            padding: 8px;
            background-color: #fff;
            font-size: 14px;
        }
    )";
  computeButton->setStyleSheet(commonButtonStyle);
  matrixEdit->setStyleSheet(textEditStyle);
  outputEdit->setStyleSheet(textEditStyle);
  customPlot->axisRect()->setBackground(QColor(255, 249, 230));
  QPen axisPen(Qt::black);
  axisPen.setWidthF(0.7);
  customPlot->xAxis->setBasePen(axisPen);
  customPlot->xAxis->setTickPen(axisPen);
  customPlot->xAxis->setSubTickPen(axisPen);
  customPlot->xAxis->setTickLabelColor(Qt::black);
  customPlot->xAxis->setLabelColor(Qt::black);
  customPlot->yAxis->setBasePen(axisPen);
  customPlot->yAxis->setTickPen(axisPen);
  customPlot->yAxis->setSubTickPen(axisPen);
  customPlot->yAxis->setTickLabelColor(Qt::black);
  customPlot->yAxis->setLabelColor(Qt::black);
  this->resize(700, 650);

  QLabel *versionLabel = new QLabel(QString("v%1").arg(APP_VERSION), this);
  versionLabel->setFont(defaultFont);
  statusBar()->addPermanentWidget(versionLabel);

  QLabel *authorStatusLabel = new QLabel(
      "Автор: <a href=\"https://github.com/ariollex\">ariollex</a>", this);
  authorStatusLabel->setTextFormat(Qt::RichText);
  authorStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  authorStatusLabel->setOpenExternalLinks(true);
  authorStatusLabel->setFont(defaultFont);
  authorStatusLabel->setContentsMargins(10, 0, 0, 0);
  statusBar()->addWidget(authorStatusLabel);
}

void MainWindow::computeFW() {
  QString text = matrixEdit->toPlainText();
  CalculationResult calcResult = MatrixCalculator::calculateFloydWarshall(text);

  if (calcResult.success) {
    outputEdit->setPlainText(calcResult.outputText);
    outputEdit->setPalette(qApp->palette());
    QString plotError;
    GraphPlotter::plotGraph(customPlot, calcResult.matrixForPlotting,
                            plotError);
    if (!plotError.isEmpty()) {
      outputEdit->setPlainText("Ошибка построения графика: " + plotError);
      QPalette errPal = outputEdit->palette();
      errPal.setColor(QPalette::Text, Qt::red);
      outputEdit->setPalette(errPal);
    }
  } else {
    outputEdit->setPlainText(calcResult.errorMessage);
    QPalette errPal = outputEdit->palette();
    errPal.setColor(QPalette::Text, Qt::red);
    outputEdit->setPalette(errPal);
    customPlot->clearGraphs();
    customPlot->clearItems();
    customPlot->replot();
  }
}
