#include <vector>

/*
Это класс для автоматического вычисления градиента, 
основынный на использовании дуальных чисел
Все функции приходится забивать в код самостоятельно
ВСе примеры функций внизу файла
*/

template <class T>
class dual {
public:
	T real;
	T inf;

	dual<T>(T real, T inf) : real(real), inf(inf) {}

	dual<T>(T real) : real(real), inf(0) {}

	dual<T>() : real(0), inf(0) {}

	dual<T> operator+(const dual<T>& second) {
		dual<T> first(real + second.real, inf + second.inf);
		return first;
	}

	dual<T> operator-(const dual<T>& second) {
		dual<T> first(real - second.real, inf - second.inf);
		return first;
	}

	dual<T> operator-() {
		dual<T> first(-real, -inf);
		return first;
	}

	dual<T> operator*(const dual<T>& second) {
		dual<T> first(real * second.real, real * second.inf + inf * second.real);
		return first;
	}

	dual<T>& operator+=(const dual<T>& second) {
		this->real += second.real;
		this->inf += second.inf;
		return *this;
	}

	friend dual<T> operator*(const T& first, const dual<T>& second) {
		return dual<T>(first) * second;
	}

	friend dual<T> operator-(const T& first, const dual<T>& second) {
		return dual<T>(first) - second;
	}



};


template <class T>
dual<T> sin_d(const dual<T>& x) {
	dual<T> z(sin(x.real), x.inf * cos(x.real));
	return z;
}

template <class T>
dual<T> sin_d(std::vector<T>& vect) {

}

template <class T>
dual<T> exp_d(const dual<T>& x) {
	dual<T> z(exp(x.real), x.inf * exp(x.real));
	return z;
}

template <class T>
dual<T> lin_d(const dual<T>& x) {// y = x
	dual<T> z(x.real, x.inf);
	return z;
}

template <class T>
dual<T> sqr_d(const dual<T>& x) {//y = x^2
	dual<T> z(x.real * x.real, 2 * x.inf * x.real);
	return z;
}

template <class T>
dual<T> pow_d(const dual<T>& x, double pow) {
	if (x.inf == 0 && x.real == 0) {
		dual<T> z(0, 0);
		return z;
	}
	dual<T> z(std::pow(x.real, pow), pow * x.inf * std::pow(x.real, pow - 1));
	return z;
}

