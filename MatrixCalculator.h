#include <QString>
#include <limits>
#include <string>
#include <vector>

const long long INF = std::numeric_limits<long long>::max();

struct CalculationResult {
  QString outputText;
  std::vector<std::vector<long long>> matrixForPlotting;
  bool success;
  QString errorMessage;
};

class MatrixCalculator {
public:
  static CalculationResult calculateFloydWarshall(const QString &matrixText);
};
