#include "maths.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace thewizardplusplus::wizard_basic::framework::maths;

static bool initialize_prng = false;

extern "C" float NumberAdd(float number2, float number1) {
	return number1 + number2;
}

extern "C" float NumberSub(float number2, float number1) {
	return number1 - number2;
}

extern "C" float NumberMul(float number2, float number1) {
	return number1 * number2;
}

extern "C" float NumberDiv(float number2, float number1) {
	return number1 / number2;
}

extern "C" float NumberLs(float number2, float number1) {
	return number1 < number2;
}

extern "C" float NumberGt(float number2, float number1) {
	return number1 > number2;
}

extern "C" float NumberEq(float number2, float number1) {
	return number1 == number2;
}

extern "C" float NumberAnd(float number2, float number1) {
	return number1 && number2;
}

extern "C" float NumberOr(float number2, float number1) {
	return number1 || number2;
}

extern "C" float NumberNot(float number) {
	return !number;
}

extern "C" float MathsSin(float number) {
	return std::sin(number);
}

extern "C" float MathsCos(float number) {
	return std::cos(number);
}

extern "C" float MathsTg(float number) {
	return std::tan(number);
}

extern "C" float MathsArcsin(float number) {
	return std::asin(number);
}

extern "C" float MathsArccos(float number) {
	return std::acos(number);
}

extern "C" float MathsArctg(float number) {
	return std::atan(number);
}

extern "C" float MathsExp(float number) {
	return std::exp(number);
}

extern "C" float MathsLn(float number) {
	return std::log(number);
}

extern "C" float MathsLg(float number) {
	return std::log10(number);
}

extern "C" float MathsModulus(float number) {
	return std::fabs(number);
}

extern "C" float MathsPower(float exponent, float base) {
	return std::pow(base, exponent);
}

extern "C" float MathsIntegral(float number) {
	float integral = 0.0f;
	std::modf(number, &integral);

	return integral;
}

extern "C" float MathsSquareRoot(float number) {
	return std::sqrt(number);
}

extern "C" float MathsAngle(float y, float x) {
	return std::atan2(y, x);
}

extern "C" float MathsRandom(float maximum, float minimum) {
	if (!initialize_prng) {
		std::srand(std::time(NULL));
		initialize_prng = true;
	}

	return (maximum - minimum) * rand() / RAND_MAX + minimum;
}
