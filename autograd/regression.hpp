#include <vector>
#include <random>
#include <fstream>
//#include "auto_grad.hpp"
#include "gradient_descent.hpp"
#include <fstream>


/*
Любая комбинация, я взял самую простую
*/
template <class T>
T combination(std::vector<T> w, std::vector<std::vector<T>> x, int i) {
	T sum = T(0);
	for (int j = 0; j < w.size(); ++j) {
		sum += x[j][i] * w[j];
	}
	//T sum = x[0][i] * w[0] + x[1][i] * w[1] + x[2][i] * w[2];
	return sum;
}






template <class T>
dual<T> dLoss(std::vector<T> object, T target, std::vector<dual<T>> w) {
	dual<T> sum = combination_my(object, w) - dual<T>(target);
	sum = sqr_d(sum);
	return sum;
}

template <class T>
dual<T> combination_my(std::vector<T> object, std::vector<dual<T>> w) {
	dual<T> sum = T(0);
	for (int i = 0; i < object.size(); ++i) {
		sum += object[i] * w[i];
	}
	return sum;
}

template <class T>
T combination_my(std::vector<T> object, std::vector<T> w) {
	T sum = T(0);
	for (int i = 0; i < object.size(); ++i) {
		sum += object[i] * w[i];
	}
	return sum;
}

template <class T>
T Loss_function(std::vector<T> object, T target, std::vector<T> w) {
	T sum = combination_my(object, w) - (target);
	sum *= sum;
	return sum;
}



/*
Класс создает таблицу случайных чисел, порожденных uniform_real_distribution 
num_of_features - количество фичей - первые столбцы таблицы
num_elements - количество объектов в таблице
*/

void Xy_axis() {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0,1); // Диапазон от 0 до 1

	size_t num_elements = 1000;
	size_t num_of_features = 3;


	//матрица фичи\обьекты
	std::vector<std::vector<double>> values(num_of_features, std::vector<double>(num_elements));


	// Заполняем матрицу случайными числами
	for (int j = 0; j < num_of_features; ++j) {
		for (size_t i = 0; i < num_elements; ++i) {
			values[j][i] = dis(gen);
		}
	}

	/*

	std::ifstream fin("hello.txt");
	char c;
			
	for (int i = 0; i < num_of_features; ++i) {
		for (int j = 0; j < num_elements; ++j) {
			fin >> values[i][j] >> c;
		}
	}
	*/
	
	//веса, с которыми я взвешиваю фичи
	std::vector<double> w_true = { 3.12,5.03,-7.93 };


	std::vector<double> y_values(num_elements);

	/*
	наполняем таргет
	*/
	for (size_t i = 0; i < num_elements; ++i) {
		y_values[i] = (combination(w_true, values,i));
	}

	std::vector<double> nw = SG2(values, y_values, Loss_function);

	for (auto x : nw) {
		std::cout << x << " ";
	}

	std::cout << "\n\n";

	int count = 0;

	std::vector<double> object(num_of_features);
	for (int i = 0; i < num_elements; ++i) {
		double sum = 0;

		sum = combination(nw, values, i);
		//for (size_t i = 0; i < num_elements; ++i) {
			//y_values[i] = (combination(w_true, values, i));
		//}
		count += abs(combination(nw, values, i) - y_values[i]) < 0.1;
	}
	std::cout << count;
		

}

