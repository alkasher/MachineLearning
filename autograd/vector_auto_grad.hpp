#include <vector>


template <class T>
class variables {
public:
	int i;

};

template <class T>
class dual {
public:
	T real;
	std::vector<T> infs;

	dual<T> operator*(const dual<T>& second) {
		std::vector<T> first_infs(infs.size());
		for (int i = 0; i < infs.size(); ++i) {
			first_infs[i] = real* second.infs[i] + second.real * infs[i];
		}
		dual<T> first(real * second.real, first_infs);
		return first;
	}
};

template <class T>
dual<T> sin_d(T x0, T x1) {
	
}