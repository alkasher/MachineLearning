#include <vector>


template <class T>
std::vector<T> grad2(std::vector<T>& w, std::vector<std::vector<T>>& matrix, std::vector<T>& target, int k) {

	std::vector<dual<T>> vect;
	std::vector<T> ans;

	for (int i = 0; i < w.size(); ++i) {
		vect.push_back(dual<T>(w[i], 0));
	}

	for (int i = 0; i < vect.size(); ++i) {
		vect[i].inf = 1;
		ans.push_back(Loss_function(vect, matrix, target, k).inf);// w, duals

		vect[i].inf = 0;
	}

	return ans;
}
/*
Любая комбинация, я взял самую простую
*/
template <class T>
T combination(std::vector<T> w, std::vector<std::vector<T>> x, int i) {
	T sum = x[0][i] * w[0] + x[1][i] * w[1] + x[2][i] * w[2];
	return sum;
}

template <class T>
double Loss_function(std::vector<std::vector<T>>& matrix, std::vector<T>& y, std::vector<T>& w, int i) {
	T sum = combination(w, matrix, i) - y[i];
	sum = std::pow(sum, 2); //mse
	return sum;
}

template <class T>
dual<T> combination(std::vector<dual<T>> w, std::vector<std::vector<T>> x, int i) {
	return x[0][i] * w[0] + x[1][i] * w[1] + x[2][i] * w[2];
}

template <class T>
dual<T> Loss_function(std::vector<dual<T>>& w, std::vector<std::vector<T>>& matrix, std::vector<T>& target, int k) {
	dual<T> z = sqr_d((combination(w, matrix, k) - target[k]));
	return z;
}

template <class T>
std::vector<T> SG(std::vector<std::vector<T>> matrix, std::vector<T> target) {

	std::vector<T> vect(matrix.size(), 1);

	T alpha = T(0.01);
	T epsilon = T(0.0001);
	T sum;
	T loss;
	T lambda = 0.05;

	T Q = T(0); // функционал для обучения по лоссу

	/*зададим начальные веса, как решение в МНК:
	wj = <target, xj> / <xj, xj>
	*/

	for (int i = 0; i < matrix.size(); ++i) {
		T w_sum = T(0); // <y, xj>
		T n_sum = T(0); // normalization <xj,xj>
		for (int j = 0; j < matrix[0].size(); ++j) {
			w_sum = target[j] * matrix[i][j];
			n_sum = matrix[i][j] * matrix[i][j];
		}
		vect[i] = w_sum / n_sum;
	}

	for (int i = 0; i < matrix[0].size(); ++i) {
		Q += Loss_function(matrix, target, vect, i);
	}

	Q /= matrix[0].size();

	int s = 0;


	do {
		srand(time(0));

		sum = T(0);
		std::size_t k = rand() % matrix[0].size();

		loss = Loss_function(matrix, target, vect, k); // stochastic

		std::vector<T> gradient = grad2(vect, matrix, target, k);

		for (int i = 0; i < vect.size(); ++i) {
			gradient[i] *= alpha;
			sum += gradient[i];
		}

		for (int i = 0; i < matrix.size(); ++i) {
			vect[i] -= gradient[i];
		}

		Q = lambda * loss + (1 - lambda) * Q;

	} while (s++ < 150000);

	return vect;
}