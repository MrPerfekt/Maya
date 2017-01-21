#ifndef FILTER
#define FILTER

template <class T>
class Filter {
public:
	virtual void addValue(T value) = 0;
	virtual T getResultValue() = 0;
	virtual T compute(T value);
};

template <class T>
T Filter<T>::compute(T value) {
	addValue(value);
	return getResultValue();
}

#endif