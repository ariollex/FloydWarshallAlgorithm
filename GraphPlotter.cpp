#include "GraphPlotter.h"
#include <QColor>
#include <QFont>
#include <QPointF>
#include <QVector>
#include <cmath>

void GraphPlotter::plotGraph(QCustomPlot *customPlot,
                             const std::vector<std::vector<long long>> &matrix,
                             QString &errorOutput) {
  int n = matrix.size();
  if (n <= 0) {
    errorOutput = "Ошибка: неверный размер N для построения графика.";
    return;
  }

  std::vector<std::vector<int>> adj(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      adj[i][j] = (matrix[i][j] != 0);
    }
  }
  customPlot->clearGraphs();
  customPlot->clearItems();
  customPlot->xAxis->setRange(-1.5, 1.5);
  customPlot->yAxis->setRange(-1.5, 1.5);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

  QVector<QPointF> pos(n);
  double R = 1.0, r = 0.15;

  QVector<QColor> nodeColorPalette = {
      QColor(52, 152, 219),  // Синий
      QColor(46, 204, 113),  // Зеленый
      QColor(231, 76, 60),   // Красный
      QColor(155, 89, 182),  // Пурпурный
      QColor(241, 196, 15),  // Желтый
      QColor(26, 188, 156),  // Бирюзовый
      QColor(230, 126, 34),  // Оранжевый
      QColor(236, 240, 241), // Светло-серый
      QColor(149, 165, 166), // Серый
      QColor(243, 156, 18),  // Оранжево-желтый
      QColor(52, 73, 94),    // Темно-синий-серый
      QColor(39, 174, 96),   // Изумрудный
      QColor(142, 68, 173),  // Аметист
      QColor(192, 57, 43),   // Гранат
      QColor(22, 160, 133)   // Морская волна
  };
  int nodeColorIndex = 0;

  for (int i = 0; i < n; ++i) {
    double a = 2 * M_PI * i / n;
    pos[i] = QPointF(R * cos(a), R * sin(a));
    auto *c = new QCPItemEllipse(customPlot);
    c->topLeft->setCoords(pos[i].x() - r, pos[i].y() - r);
    c->bottomRight->setCoords(pos[i].x() + r, pos[i].y() + r);
    c->setPen(QPen(Qt::black));
    QColor currentNodeColor =
        nodeColorPalette[nodeColorIndex % nodeColorPalette.size()];
    nodeColorIndex++;
    c->setBrush(QBrush(currentNodeColor));
    auto *t = new QCPItemText(customPlot);
    QFont labelFont = customPlot->font();
    labelFont.setPointSize(labelFont.pointSize() + 12);
    t->setFont(labelFont);
    t->setPositionAlignment(Qt::AlignCenter);
    t->position->setCoords(pos[i]);
    t->setText(QString::number(i + 1));
    if (currentNodeColor.lightnessF() < 0.6) {
      t->setColor(Qt::white);
    } else {
      t->setColor(Qt::black);
    }
  }

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      if (adj[i][j]) {
        if (i == j) {
          QCPItemCurve *loop = new QCPItemCurve(customPlot);
          double a = 2 * M_PI * i / n;
          QPointF c = pos[i];
          double d = r * 1.2;
          QPointF p0 = c + QPointF(r * cos(a - 0.3), r * sin(a - 0.3));
          QPointF p3 = c + QPointF(r * cos(a + 0.3), r * sin(a + 0.3));
          QPointF p1 =
              c + QPointF((r + d) * cos(a - 0.6), (r + d) * sin(a - 0.6));
          QPointF p2 =
              c + QPointF((r + d) * cos(a + 0.6), (r + d) * sin(a + 0.6));
          loop->start->setCoords(p0);
          loop->startDir->setCoords(p1);
          loop->endDir->setCoords(p2);
          loop->end->setCoords(p3);
          loop->setPen(QPen(Qt::black));
          QCPLineEnding head;
          head.setStyle(QCPLineEnding::esSpikeArrow);
          loop->setHead(head);
          auto *wt = new QCPItemText(customPlot);
          QFont weightFont = customPlot->font();
          weightFont.setPointSize(weightFont.pointSize() + 2);
          wt->setFont(weightFont);
          wt->setPositionAlignment(Qt::AlignCenter);
          QPointF midLoop((p1.x() + p2.x()) / 2.0, (p1.y() + p2.y()) / 2.0);
          QPointF dirLoop = midLoop - c;
          double lenLoop =
              std::sqrt(dirLoop.x() * dirLoop.x() + dirLoop.y() * dirLoop.y());
          QPointF uLoop = (lenLoop > 1e-6) ? dirLoop / lenLoop : QPointF(0, 0);
          double offsetLoop = r * 0.2;
          QPointF posLoop = midLoop + uLoop * offsetLoop;
          wt->position->setCoords(posLoop);
          wt->setText(QString::number(matrix[i][j]));
          wt->setColor(Qt::black);
        } else {
          if (matrix[j][i] != 0) {
            QPointF v = pos[j] - pos[i];
            double len = std::sqrt(v.x() * v.x() + v.y() * v.y());
            QPointF u = (len > 1e-6) ? v / len : QPointF(0, 0);
            QPointF perp(-u.y(), u.x());
            double curveOff = r * 0.5;
            QPointF p0 = pos[i] + u * r;
            QPointF p3 = pos[j] - u * r;
            double tangentInfluenceFactor = 0.3;
            double tangentInfluence = len * tangentInfluenceFactor;
            QPointF p1 = p0 + u * tangentInfluence + perp * curveOff;
            QPointF p2 = p3 - u * tangentInfluence + perp * curveOff;

            QCPItemCurve *curve = new QCPItemCurve(customPlot);
            curve->start->setCoords(p0);
            curve->startDir->setCoords(p1);
            curve->endDir->setCoords(p2);
            curve->end->setCoords(p3);
            curve->setPen(QPen(Qt::black));
            QCPLineEnding head;
            head.setStyle(QCPLineEnding::esSpikeArrow);
            curve->setHead(head);

            QCPItemText *wt = new QCPItemText(customPlot);
            QFont wf = customPlot->font();
            wf.setPointSize(wf.pointSize() + 2);
            wt->setFont(wf);
            wt->setPositionAlignment(Qt::AlignCenter);
            QPointF controlPointsMid = (p1 + p2) * 0.5;
            QPointF perpTextOffset = perp * (r * 0.2);
            double backwardShiftFactor = 0.2;
            QPointF backwardShift = u * (r * backwardShiftFactor);

            QPointF textLabelMid =
                controlPointsMid + perpTextOffset - backwardShift;

            wt->position->setCoords(textLabelMid);
            wt->setText(QString::number(matrix[i][j]));
            wt->setColor(Qt::black);
          } else {
            QCPItemLine *arrow = new QCPItemLine(customPlot);
            QPointF v = pos[j] - pos[i];
            double l = std::sqrt(v.x() * v.x() + v.y() * v.y());
            QPointF u = (l > 1e-6) ? v / l : QPointF(0, 0);
            arrow->start->setCoords(pos[i] + u * r);
            arrow->end->setCoords(pos[j] - u * r);
            arrow->setPen(QPen(Qt::black));
            QCPLineEnding head;
            head.setStyle(QCPLineEnding::esSpikeArrow);
            arrow->setHead(head);
            QCPItemText *wt = new QCPItemText(customPlot);
            QFont wf = customPlot->font();
            wf.setPointSize(wf.pointSize() + 2);
            wt->setFont(wf);
            wt->setPositionAlignment(Qt::AlignCenter);
            QPointF mid = (pos[i] + pos[j]) * 0.5;
            QPointF perp2(-v.y(), v.x());
            double pl =
                std::sqrt(perp2.x() * perp2.x() + perp2.y() * perp2.y());
            QPointF uperp = (pl > 1e-6) ? perp2 / pl : QPointF(0, 0);
            wt->position->setCoords(mid - uperp * (r * 0.2));
            wt->setText(QString::number(matrix[i][j]));
            wt->setColor(Qt::black);
          }
        }
      }
  customPlot->replot();
}
