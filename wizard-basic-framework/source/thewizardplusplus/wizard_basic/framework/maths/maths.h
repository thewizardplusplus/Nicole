#ifndef MATHS_H
#define MATHS_H

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {
namespace maths {

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
float MathsSin(float number);
float MathsCos(float number);
float MathsTg(float number);
float MathsArcsin(float number);
float MathsArccos(float number);
float MathsArctg(float number);
float MathsExp(float number);
float MathsLn(float number);
float MathsLg(float number);
float MathsModulus(float number);
float MathsPower(float exponent, float base);
float MathsIntegral(float number);
float MathsSquareRoot(float number);
float MathsAngle(float y, float x);
float MathsRandom(float maximum, float minimum);

}

}
}
}
}
#endif
