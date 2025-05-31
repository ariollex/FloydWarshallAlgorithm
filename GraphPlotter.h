#include "qcustomplot.h"
#include <QString>
#include <vector>

class GraphPlotter {
public:
  static void plotGraph(QCustomPlot *customPlot,
                        const std::vector<std::vector<long long>> &matrix,
                        QString &errorOutput);
};
