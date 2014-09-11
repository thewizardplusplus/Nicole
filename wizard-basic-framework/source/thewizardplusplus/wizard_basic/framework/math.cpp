#include "math.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace thewizardplusplus::wizard_basic::framework;

static bool initialize_prmg = false;

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

extern "C" float MathSin(float number) {
	return std::sin(number);
}

extern "C" float MathCos(float number) {
	return std::cos(number);
}

extern "C" float MathTg(float number) {
	return std::tan(number);
}

extern "C" float MathArcsin(float number) {
	return std::asin(number);
}

extern "C" float MathArccos(float number) {
	return std::acos(number);
}

extern "C" float MathArctg(float number) {
	return std::atan(number);
}

extern "C" float MathExponent(float number) {
	return std::exp(number);
}

extern "C" float MathLn(float number) {
	return std::log(number);
}

extern "C" float MathLg(float number) {
	return std::log10(number);
}

extern "C" float MathModulus(float number) {
	return std::fabs(number);
}

extern "C" float MathPower(float exponent, float base) {
	return std::pow(base, exponent);
}

extern "C" float MathInteger(float number) {
	return std::floor(number);
}

extern "C" float MathRandom(float maximum, float minimum) {
	if (!initialize_prmg) {
		std::srand(std::time(NULL));
		initialize_prmg = true;
	}

	return (maximum - minimum) * rand() / RAND_MAX + minimum;
}

extern "C" float MathSquareRoot(float number) {
	return std::sqrt(number);
}

extern "C" float MathAngle(float y, float x) {
	return std::atan2(y, x);
}
