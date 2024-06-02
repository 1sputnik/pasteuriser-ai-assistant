#include "ActFunctions.h"

double Identical::activate(double x) {
	return x;
}
double Identical::dif_activate(double x) {
	return 1.0;
}
double Identical::dif_activate_per_activate(double y) {
	return 1.0;
}
double Identical::inverse(double y, double dif_value) {
	return y;
}
double_vector Identical::cooficients() {
	double_vector coof;
	return  coof;
}


double Linear::activate(double x) {
	return a * x;
};
double Linear::dif_activate(double x) {
	return a;
};
double Linear::dif_activate_per_activate(double y) {
	return a;
};
double Linear::inverse(double y, double dif_value) {
	return y / a;
}
double_vector Linear::cooficients() {
	double_vector coof{ this->a };
	return  coof;
}
Linear::Linear(double a) {
	this->a = a;
}


double BiasLinear::activate(double x) {
	return a * x + b;
}
double BiasLinear::dif_activate(double x) {
	return a;
}
double BiasLinear::dif_activate_per_activate(double y) {
	return a;
}
double BiasLinear::inverse(double y, double dif_value) {
	return (y - b) / a;
}
double_vector BiasLinear::cooficients() {
	double_vector coof{ this->a, this->b };
	return  coof;
}
BiasLinear::BiasLinear(double a, double b) {
	this->a = a;
	this->b = b;
}


double ThresholdLinear::activate(double x) {
	return a * x - b;
}
double ThresholdLinear::dif_activate(double x) {
	return a;
}
double ThresholdLinear::dif_activate_per_activate(double y) {
	return a;
}
double ThresholdLinear::inverse(double y, double dif_value) {
	return (y + b) / a;
}
double_vector ThresholdLinear::cooficients() {
	double_vector coof{ this->a, this->b };
	return  coof;
}
ThresholdLinear::ThresholdLinear(double a, double b) {
	this->a = a;
	this->b = b;
}


double Logistic::activate(double x) {
	return 1.0 / (1.0 + exp(-x));
}
double Logistic::dif_activate(double x) {
	return exp(-x) / pow((1.0 + exp(-x)), 2.0);
}
double Logistic::dif_activate_per_activate(double y) {
	return y * (1.0 - y);
}
double Logistic::inverse(double y, double dif_value) {
	return -log(1.0 / y - 1.0);
}
double_vector Logistic::cooficients() {
	double_vector coof;
	return  coof;
}


double BipolarLogistic::activate(double x) {
	return 2.0 / (1.0 + exp(-x)) - 1.0;
}
double BipolarLogistic::dif_activate(double x) {
	return 2.0 * exp(x) / pow((1.0 + exp(x)), 2.0);
}
double BipolarLogistic::dif_activate_per_activate(double y) {
	return 0.5 * y * (0.0 - y) + 0.5;
}
double BipolarLogistic::inverse(double y, double dif_value) {
	return -log(2.0 / (y + 1.0) - 1.0);
}
double_vector BipolarLogistic::cooficients() {
	double_vector coof;
	return  coof;
}


double Sigmoid::activate(double x) {
	return 1.0 / (1.0 + exp(-a * x));
}
double Sigmoid::dif_activate(double x) {
	return a * exp(-a * x) / pow((1.0 + exp(-a * x)), 2.0);
}
double Sigmoid::dif_activate_per_activate(double y) {
	return a * y * (1.0 - y);
}
double Sigmoid::inverse(double y, double dif_value) {
	return (-log(1.0 / y - 1.0)) / a;
}
double_vector Sigmoid::cooficients() {
	double_vector coof{ this->a };
	return  coof;
}
Sigmoid::Sigmoid(double a) {
	this->a = a;
}


double BipolarSigmoid::activate(double x) {
	return 2.0 / (1.0 + exp(-a * x)) - 1.0;
}
double BipolarSigmoid::dif_activate(double x) {
	return 2.0 * a * exp(-a * x) / pow((1.0 + exp(-a * x)), 2.0);
}
double BipolarSigmoid::dif_activate_per_activate(double y) {
	return 0.5 * a * y * (0.0 - y) + 0.5 * a;
}
double BipolarSigmoid::inverse(double y, double dif_value) {
	return (-log(2.0 / (y + 1.0) - 1.0)) / a;
}
double_vector BipolarSigmoid::cooficients() {
	double_vector coof{ this->a };
	return  coof;
}
BipolarSigmoid::BipolarSigmoid(double a) {
	this->a = a;
}


double HyperbolicTangent::activate(double x) {
	return tanh(x);
};
double HyperbolicTangent::dif_activate(double x) {
	return 1 / pow(cosh(x), 2);
};
double HyperbolicTangent::dif_activate_per_activate(double y) {
	return 1 - pow(y, 2);
};
double HyperbolicTangent::inverse(double y, double dif_value) {
	return atanh(y);
}
double_vector HyperbolicTangent::cooficients() {
	double_vector coof;
	return  coof;
}