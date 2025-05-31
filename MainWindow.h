#include "qcustomplot.h"
#include <QMainWindow>
class QPlainTextEdit;
class QPushButton;

class MainWindow : public QMainWindow {
public:
  MainWindow(QWidget *parent = nullptr);

private:
  void computeFW();
  QPlainTextEdit *matrixEdit;
  QPlainTextEdit *outputEdit;
  QPushButton *computeButton;
  QCustomPlot *customPlot;
};
