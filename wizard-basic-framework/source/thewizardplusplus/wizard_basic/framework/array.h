#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <list>
#include <string>

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {

typedef std::vector<float>      Array;
typedef std::list<Array*>       ArrayList;
typedef std::list<std::string*> StringList;

extern "C" {

float ArrayCreate(float size);
float ArrayCreateFromString(const char* string);
float ArrayCreateFromNumber(float number);
const char* ArrayConvertToString(float array_id);
float ArrayConvertToNumber(float array_id);
void ArrayDelete(float array_id);
void ArrayDeleteAll(void);
void ArrayClearMemoryAfterConvertsToStrings(void);
float ArrayAppend(float array_id2, float array_id1);
float ArrayGetSize(float array_id);
float ArrayItemGet(float index, float array_id);
void ArrayItemSet(float value, float index, float array_id);
void ArrayItemInsert(float value, float index, float array_id);
void ArrayItemAppend(float value, float array_id);
void ArrayItemRemove(float index, float array_id);
Array* ArrayGetById(float array_id);

}

}
}
}
#endif
