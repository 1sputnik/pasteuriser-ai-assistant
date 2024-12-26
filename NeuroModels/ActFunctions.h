#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#define M_PI 3.1415926535

using std::pair;
using std::pow;
using std::tanh;
using std::sin;
using std::cos;
using std::exp;
using std::log;
using std::fabs;
using std::asin;
using std::atanh;
using std::acos;

typedef std::vector<double> double_vector;

class AbstractActivator {
public:
	virtual double activate(double x) = 0;
	virtual double dif_activate(double x) = 0;
	virtual double dif_activate_per_activate(double y) = 0;
	virtual double inverse(double y, double dif_value = 0) = 0;
	virtual double_vector cooficients() = 0;
	double operator()(double x) {
		return this->activate(x);
	}
	double operator[](double x) {
		return this->dif_activate(x);
	}
};
typedef AbstractActivator ActivateFunc;
typedef AbstractActivator AbstAct;

class Identical : public AbstractActivator {
public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
};

class Linear : public AbstractActivator {
	double a;

public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
	Linear(double a);
};
typedef Linear Lin;

class BiasLinear : public AbstractActivator {
	double a;
	double b;

public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
	BiasLinear(double a, double b);
};
typedef BiasLinear BLin;
typedef BiasLinear BiasL;

class ThresholdLinear : public AbstractActivator {
	double a;
	double b;

public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
	ThresholdLinear(double a, double b);
};
typedef ThresholdLinear TLin;
typedef ThresholdLinear ThresholdL;

class Logistic : public AbstractActivator {
public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
};
typedef Logistic Logist;

class BipolarLogistic : public AbstractActivator {
public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
};
typedef BipolarLogistic BipolLogist;

class Sigmoid : public AbstractActivator {
	double a;

public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
	Sigmoid(double a);
};
typedef Sigmoid Sigm;

class BipolarSigmoid : public AbstractActivator {
	double a;

public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
	BipolarSigmoid(double a);
};
typedef BipolarSigmoid BipolSigm;

class HyperbolicTangent : public AbstractActivator {
public:
	virtual double activate(double x) override;
	virtual double dif_activate(double x) override;
	virtual double dif_activate_per_activate(double y) override;
	virtual double inverse(double y, double dif_value = 0) override;
	virtual double_vector cooficients() override;
};
typedef HyperbolicTangent TypTan;
typedef HyperbolicTangent Tanh;
typedef HyperbolicTangent Th;

// TODO
//class SoftSign : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		return x / (1 + fabs(x));
//	}
//	virtual double dif_activate(double x) {
//		return 1 / pow((1 + fabs(x)), 2);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		if (y == 0)
//			return 1;
//		else
//			return pow(y, 2) / pow(this->inverse(y), 2);
//	}
//	virtual double inverse(double y, double dif_value = 0) {
//		if (y <= 0)
//			return y / (1 + y);
//		else
//			return y / (1 - y);
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//
//class CleanSinus : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		return sin(x);
//	}
//	virtual double dif_activate(double x) {
//		return cos(x);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		return pow(1 - pow(y, 2), 0.5);
//	}
//	virtual double inverse(double y, double dif_value = 0) {
//		return asin(y);
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//typedef CleanSinus Sinus;
//typedef CleanSinus Sin;
//
//class LimitedSinus : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		if (x >= M_PI / 2)
//			return 1;
//		else if (x <= M_PI / 2)
//			return -1;
//		else
//			return cos(x);
//	}
//	virtual double dif_activate(double x) {
//		if (x >= M_PI || x <= M_PI)
//			return 0;
//		else
//			return -sin(x);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		if (y == 1 || y == -1)
//			return 0;
//		else
//			return -pow(1 - pow(y, 2), 0.5);
//	}
//	virtual double inverse(double y, double dif_value = -1) {
//		double inverse_value = asin(y);
//		if (dif_value > 0)
//			inverse_value *= -1;
//		return inverse_value;
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//typedef LimitedSinus LSinus;
//typedef LimitedSinus LSin;
//
//class CleanCosinus : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		return cos(x);
//	}
//	virtual double dif_activate(double x) {
//		return -sin(x);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		return -pow(1 - pow(y, 2), 0.5);
//	}
//	virtual double inverse(double y, double dif_value = -1) {
//		double inverse_value = acos(y);
//		if (dif_value > 0)
//			inverse_value *= -1;
//		return inverse_value;
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//typedef CleanCosinus Cosinus;
//typedef CleanCosinus Cos;
//
//class LimitedCosinus : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		if (x >= M_PI || x <= M_PI)
//			return -1;
//		else
//			return cos(x);
//	}
//	virtual double dif_activate(double x) {
//		if (x > M_PI || x < M_PI)
//			return 0;
//		else
//			return -sin(x);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		return -pow(1 - pow(y, 2), 0.5);
//	}
//	virtual double inverse(double y, double dif_value = -1) {
//		double inverse_value = acos(y);
//		if (dif_value > 0)
//			inverse_value *= -1;
//		return inverse_value;
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//typedef LimitedCosinus LCosinus;
//typedef LimitedCosinus LCos;
//
//class Sinc : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		if (x == 0)
//			return 1;
//		else
//			return sin(x) / x;
//	}
//	virtual double dif_activate(double x) {
//		if (x == 0)
//			return 0;
//		else
//			return cos(x) / x - sin(x) / pow(x, 2);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		return -1; // TODO
//	}
//	virtual double inverse(double y, double dif_value = 0) {
//		if (y == 1)
//			return 0;
//		else
//			return -1; // TODO
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//
//class Gaussian : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		return exp(-pow(x, 2));
//	}
//	virtual double dif_activate(double x) {
//		return 2 * x * this->activate(x);
//	}
//	virtual double dif_activate_per_activate(double y) {
//		return 2 * y * this->inverse(y);
//	}
//	virtual double inverse(double y, double dif_value = 1) {
//		double inverse_value = pow(-log(y), 0.5);
//		if (dif_value < 0)
//			inverse_value *= -1;
//		return inverse_value;
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//};
//
//class ReLu : public AbstractActivator {
//public:
//	virtual double activate(double x) {
//		if (x > 0)
//			return x;
//		else
//			return 0;
//	}
//	virtual double dif_activate(double x) {
//		if (x > 0)
//			return 1;
//		else
//			return Random();
//	}
//	virtual double dif_activate_per_activate(double y) {
//		if (y > 0)
//			return 1;
//		else
//			return Random();
//	}
//	virtual double inverse(double y, double dif_value = 0) {	
//			return -y; 
//	}
//	pair<double, double> cooficients() {
//		return  pair<double, double>(1.0, 1.0);
//	}
//private:
//	double Random()
//	{
//		double min = 0.01;
//		double max = 0.05;
//		double value;
//		value = rand();
//
//		while (value > max || value < min)
//			value /= 10;
//		// получить вещественное число
//		return value;
//	
//	}
//};
