#ifndef SIMPLE_LOW_PASS_FILTER
#define SIMPLE_LOW_PASS_FILTER

#include "Arduino.h"
#include "filter.h"

template <class T>
class SimpleLowPassFilter : public Filter<T> {
	uint16_t timeConstant;
public:
	SimpleLowPassFilter(uint16_t timeConstant);
	T currentValue = 0;
	void addValue(T value);
	T getResultValue();
};

template <class T>
void SimpleLowPassFilter<T>::addValue(T value) {
	currentValue = (currentValue * (timeConstant - 1) + value) / timeConstant;
}

template <class T>
T SimpleLowPassFilter<T>::getResultValue() {
	return currentValue;
}

template <class T>
SimpleLowPassFilter<T>::SimpleLowPassFilter(uint16_t timeConstant)
	:timeConstant(timeConstant) {
}


#endif