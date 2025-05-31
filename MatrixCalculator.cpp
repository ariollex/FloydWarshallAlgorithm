#include "MatrixCalculator.h"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

CalculationResult
MatrixCalculator::calculateFloydWarshall(const QString &matrixText) {
  CalculationResult result;
  result.success = false;

  // Считывание матрицы из строки
  std::vector<std::vector<long long>> A;
  std::istringstream iss(matrixText.toStdString());
  std::string line;
  while (std::getline(iss, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream rowStream(line);
    std::vector<long long> row;
    long long val;
    while (rowStream >> val) {
      row.push_back(val);
    }
    if (!row.empty()) {
      A.push_back(row);
    }
  }
  if (A.empty()) {
    result.errorMessage = "Ошибка: матрица не может быть пустой.";
    return result;
  }

  // Определение размера матрицы
  int n = A.size();
  bool isSquare = true;
  for (const auto &row : A) {
    if ((int)row.size() != n) {
      isSquare = false;
      break;
    }
  }
  if (!isSquare) {
    result.errorMessage = "Ошибка: матрица должна быть квадратной.";
    return result;
  }

  result.matrixForPlotting = A;
  std::vector<std::vector<long long>> D(n, std::vector<long long>(n));

  // метод Флойда (по В. Липскому)
  // Составляем D^(0)
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == j) {
        D[i][j] = 0;
      } else if (A[i][j] == 0) {
        D[i][j] = INF;
      } else {
        D[i][j] = 1;
      }
    }
  }
  for (int m = 0; m < n; ++m) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (D[i][m] < INF && D[m][j] < INF) {
          D[i][j] = std::min(D[i][j], D[i][m] + D[m][j]);
        }
      }
    }
  }

  // Вывод результата
  std::vector<std::vector<std::string>> S(n, std::vector<std::string>(n));
  size_t num_width = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (D[i][j] >= INF) {
        S[i][j] = "∞";
      } else {
        S[i][j] = std::to_string(D[i][j]);
        num_width = std::max(num_width, S[i][j].length());
      }
    }
  }
  size_t effective_width = std::max(num_width, (size_t)1);

  std::ostringstream oss;
  for (int i = 0; i < n; ++i) {
    oss << S[i][0];
    for (int j = 1; j < n; ++j) {
      oss << ' ';
      if (S[i][j] == "∞") {
        for (size_t k = 0; k < effective_width - 1; ++k) {
          oss << ' ';
        }
        oss << S[i][j];
      } else {
        oss << std::setw(effective_width) << S[i][j];
      }
    }
    oss << '\n';
  }

  result.outputText = QString::fromStdString(oss.str());
  result.success = true;
  return result;
}
