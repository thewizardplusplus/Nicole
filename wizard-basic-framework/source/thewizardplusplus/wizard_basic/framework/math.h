#ifndef MATH_H
#define MATH_H

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {

extern "C" {

float NumberAdd(float number2, float number1);
float NumberSub(float number2, float number1);
float NumberMul(float number2, float number1);
float NumberDiv(float number2, float number1);
float NumberLs(float number2, float number1);
float NumberGt(float number2, float number1);
float NumberEq(float number2, float number1);
float NumberAnd(float number2, float number1);
float NumberOr(float number2, float number1);
float NumberNot(float number);
float MathSin(float number);
float MathCos(float number);
float MathTg(float number);
float MathArcsin(float number);
float MathArccos(float number);
float MathArctg(float number);
float MathExponent(float number);
float MathLn(float number);
float MathLg(float number);
float MathModulus(float number);
float MathPower(float exponent, float base);
float MathInteger(float number);
float MathRandom(float maximum, float minimum);
float MathSquareRoot(float number);
float MathAngle(float y, float x);

}

}
}
}
#endif
