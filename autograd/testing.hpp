#include <iostream>
#include <vector>

// Функция для вычисления значения функции f(x, y) = x^2 + y^2
double function(double x, double y) {
    return x * x + y * y;
}

// Функция для вычисления градиента f'(x, y) = (2x, 2y)
std::vector<double> gradient(double x, double y) {
    return { 2 * x, 2 * y };
}

// Функция для выполнения градиентного спуска
void gradientDescent(double learningRate, int numIterations) {
    double x = 10.0;  // Начальное значение x
    double y = 10.0;  // Начальное значение y
    std::cout << "Initial x: " << x << ", y: " << y << std::endl;

    for (int i = 0; i < numIterations; ++i) {
        std::vector<double> grad = gradient(x, y);  // Вычисление градиента
        x = x - learningRate * grad[0];  // Обновление значения x
        y = y - learningRate * grad[1];  // Обновление значения y

        // Вывод текущих значений x, y и функции f(x, y)
        std::cout << "Iteration " << i + 1 << ": x = " << x
            << ", y = " << y
            << ", f(x, y) = " << function(x, y) << std::endl;
    }
}
