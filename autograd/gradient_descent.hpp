#include "auto_grad.hpp"



//вычиселие градиента функции с помощью дуальных чисел
template <class T>
std::vector<T> gradf(std::vector<T> object, T target, std::vector<T> w) {
	std::vector<T> ans;
	std::vector<dual<T>> diff;

	for (int i = 0; i < object.size(); ++i) {
		diff.push_back(dual<T>(w[i]));
	}

	for (int i = 0; i < object.size(); ++i) {
		diff[i].inf = 1;
		ans.push_back(dLoss(object, target, diff).inf);
		diff[i].inf = 0;
	}

	return ans;
}

template <class T>
std::vector<T> grad2(std::vector<T> points, dual<T>(*func)(std::vector<dual<T>> vect)) {

	std::vector<dual<T>> vect;
	std::vector<T> ans;

	for (int i = 0; i < points.size(); ++i) {
		vect.push_back(dual<T>(points[i], 0));
	}

	for (int i = 0; i < vect.size(); ++i) {
		vect[i].inf = 1;
		ans.push_back(func(vect).inf);

		vect[i].inf = 0;
	}

	return ans;
}

template <class T>
std::vector<T> gradient_descent(int n, dual<T>(*func)(std::vector<dual<T>> vect)) {

	std::vector<T> vect(n, 1);

	T alpha = T(0.1);
	T epsilon = T(0.0001);
	T sum = T(0);

	do {
		sum = T(0);
		std::vector<T> gradient; //= grad2(vect, func);
		for (int i = 0; i < 3; ++i) {
			gradient.push_back(2 * (5 * (i==0) + 4 * (i == 1) + 3 * (i == 2)) * (5 * vect[0] + 4 * vect[1] + 3 * vect[2] - 10));
		}

		for (int i = 0; i < n; ++i) {
			gradient[i] *= alpha;
			sum += gradient[i];
		}

		for (int i = 0; i < n; ++i) {
			vect[i] -= gradient[i];
		}
	} while (abs(sum) > epsilon);

	return vect;
}


/*
Stochatic Gradient for linear regression, 
reqiuerse matrix of object/fearutes, vector of targets, Loss function
returns the vector of weights
*/

template <class T>
std::vector<T> SG2(std::vector<std::vector<T>> matrix, std::vector<T> target, T(*Loss)(std::vector<T> object, T target, std::vector<T> w)) {

	int m = matrix.size();
	int n = matrix[0].size();

	T alpha = T(0.01);//for gradient descent
	T lambda = T(0.05);//weight for Q - Empirical risk


	T beta = T(0.5);

	std::vector<T> w(m, 0);

	/*зададим начальные веса, как решение в МНК:
	Xw = target
	w = (X^(-1))*target
	wj = <target, xj> / <xj, xj>
	*/

	for (int i = 0; i < m; ++i) {
		T w_sum = T(0); // <y, xj>
		T n_sum = T(0); // normalization <xj,xj>
		for (int j = 0; j < n; ++j) {
			w_sum += target[j] * matrix[i][j];
			n_sum += matrix[i][j] * matrix[i][j];
		}
		w[i] = w_sum / n_sum;
	}

	//offset for change of vector w
	T offset = T(1);

	//object is a row of matrix
	std::vector<T> object(m);

	//weighted sum of Loss function
	//Empirical risk
	T Q = T(0);



	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			object[j] = matrix[j][i];
		}
		Q += Loss(object, target[i], w);
	}

	Q /= n;

	//Qp for tracking change of Empirical risk
	T Qp = Q + T(1);

	T loss = T(0); // loss function of a stochastic object

	std::vector<T> gradient;

	while ((abs(offset) > 0.00000001) || (abs(Q - Qp) > 0.00000001)) {
		offset = T(0);


		//take a random object for stochastic gradient
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n - 1);

		std::size_t k = dist6(rng);

		for (int i = 0; i < m; ++i) {
			object[i] = matrix[i][k];
		}

		loss = Loss(object, target[k], w);//compute loss

		//compute gradient
		gradient = gradf(object, target[k], w);

		//compute new step of w
		for (int i = 0; i < m; ++i) {
			offset += gradient[i];
			w[i] = std::fma(-alpha, gradient[i], w[i]);
		}

		//compute Q
		Qp = Q;
		Q = lambda * loss + (1 - lambda) * loss;

	}
	return w;

}


template <class T>
std::vector<T> SG(std::vector<std::vector<T>> matrix, std::vector<T> target, T(*Loss)(std::vector<T> object, T target, std::vector<T> w)) {

	int m = matrix.size();
	int n = matrix[0].size();

	T alpha_zero = T(50);//for gradient descent
	T alpha;
	T c1 = T(0.0001);
	T lambda = T(0.05);//weight for Q - Empirical risk


	T beta = T(0.5);

	std::vector<T> w(m, 0);

	/*зададим начальные веса, как решение в МНК:
	Xw = target
	w = (X^(-1))*target
	wj = <target, xj> / <xj, xj>
	*/

	for (int i = 0; i < m; ++i) {
		T w_sum = T(0); // <y, xj>
		T n_sum = T(0); // normalization <xj,xj>
		for (int j = 0; j < n; ++j) {
			w_sum += target[j] * matrix[i][j];
			n_sum += matrix[i][j] * matrix[i][j];
		}
		w[i] = w_sum / n_sum;
	}

	//offset for change of vector w
	T offset = T(1);

	//object is a row of matrix
	std::vector<T> object(m);

	//weighted sum of Loss function
	//Empirical risk
	T Q = T(0);



	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			object[j] = matrix[j][i];
		}
		Q += Loss(object, target[i], w);
	}

	Q /= n;

	//Qp for tracking change of Empirical risk
	T Qp = Q + T(1);

	T loss = T(0); // loss function of a stochastic object

	std::vector<T> gradient;
	std::vector<T> last_gradient(m, T(0));

	while ((abs(offset) > 0.00000001) || (abs(Q - Qp) > 0.00000001)) {
		offset = T(0);
		alpha = T(0);


		//take a random object for stochastic gradient
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, n - 1);

		std::size_t k = dist6(rng);

		for (int i = 0; i < m; ++i) {
			object[i] = matrix[i][k];
		}

		loss = Loss(object, target[k], w);//compute loss

		//compute gradient
		gradient = gradf(object, target[k], w);

		/*for (int i = 0; i < m; ++i) {
			last_gradient[i] *= last_gradient[i];
		}
		if (Loss(object, target, w) > alphaLoss(object, target, w))

		last_gradient = gradient;*/

		//compute new step of w
		for (int i = 0; i < m; ++i) {
			offset += gradient[i];
			w[i] = std::fma(-alpha, gradient[i], w[i]);
		}

		//compute Q
		Qp = Q;
		Q = lambda * loss + (1 - lambda) * loss;

	}
	return w;

}