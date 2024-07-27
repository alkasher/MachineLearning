#include <iostream>
#include <random>

#include "regression.hpp"
//#include "testing.hpp"
//#include "gradient_descent.hpp"
template <class T>
T grad(T x0) {
	dual<T> x(x0, 1);
	dual<T> res = sin_d(x) * exp_d(-x * x) + lin_d(x);
	return res.inf;
}


template <class T>
dual<T> funct(std::vector<dual<T>> vect) {
	dual<T> res = sqr_d(5 * vect[0] + 4 * vect[1] + 3 * vect[2] - 10);
	return res;
}

template <class T>
dual<T> funct2(std::vector<dual<T>> x) {
	dual<T> res = sqr_d(x[0] * x[0] + x[1] - 11) + sqr_d(x[0] + x[1] * x[1] - 7);
	return res;
}

template <class T>
T target(T point) {
	return point * point;
}





int main() {

	std::vector<double> points;
	/*points.push_back(1);
	points.push_back(1);
	for (auto x : gradient_descent(3, funct<double>)) {
		std::cout << x << " ";
	}
	*/
	Xy_axis();
	//std::vector<double> object = { 1,1,1 };
	//double target = 1;

	//std::vector<double> gradient(3, 0);
	////for (int j = 0; j < batch; ++j){
	//std::vector<double> w = { 0.5186, -1.3125,  0.1920 };


	//for (int i = 0; i < 3; ++i) {
	//	gradient[i] = (2 * object[i] * (w[0] * object[0] + w[1] * object[1] + w[2] * object[2] - 1));
	//}

	//gradf(object, target, w);
	//
	//for (int i = 0; i < 3; ++i) {
	//	w[i] -= gradient[i];
	//}
	
	

}