#include "array.h"
#include "../utils/utils.h"
#include <cstring>

using namespace thewizardplusplus::wizard_basic::framework::array;
using namespace thewizardplusplus::wizard_basic::framework::utils;

static ArrayList arrays;
static StringList converted_strings;

static void testArrayIndex(float array_id, Array* array, float index) {
	if (index < 0 || index >= array->size()) {
		ProcessError(
			"Invalid index "
			+ ConvertToString(index)
			+ " for array with ID "
			+ ConvertToString(array_id)
			+ ". This array has "
			+ ConvertToString(array->size())
			+ " element"
			+ (array->size() > 1 ? "s" : "")
			+ "."
		);
	}
}

extern "C" float ArrayCreate(float size) {
	Array* array = new Array(size);
	arrays.push_back(array);

	return arrays.size() - 1;
}

extern "C" float ArrayCreateFromString(const char* string) {
	Array* array = new Array(std::strlen(string));
	for (size_t i = 0; i < array->size(); i++) {
		(*array)[i] = string[i];
	}

	arrays.push_back(array);
	return arrays.size() - 1;
}

extern "C" float ArrayCreateFromNumber(float number) {
	std::string string = ConvertToString(number);
	return ArrayCreateFromString(string.c_str());
}

extern "C" const char* ArrayConvertToString(float array_id) {
	Array* array = ArrayGetById(array_id);
	std::string* string = new std::string();
	for (size_t i = 0; i < array->size(); i++) {
		*string += (*array)[i];
	}

	converted_strings.push_back(string);
	return string->c_str();
}

extern "C" float ArrayConvertToNumber(float array_id) {
	float number = 0.0f;
	std::istringstream in(ArrayConvertToString(array_id));
	in >> number;

	ArrayClearMemoryAfterConvertsToStrings();
	return number;
}

extern "C" void ArrayDelete(float array_id) {
	if (array_id < 0 || array_id >= arrays.size()) {
		ProcessError("Invalid array ID " + ConvertToString(array_id) + ".");
	}

	delete arrays[array_id];
	arrays[array_id];
}

extern "C" void ArrayDeleteAll(void) {
	for (size_t i = 0; i < arrays.size(); i++) {
		delete arrays[i];
		arrays[i] = NULL;
	}
}

extern "C" void ArrayClearMemoryAfterConvertsToStrings(void) {
	while (!converted_strings.empty()) {
		std::string* string = converted_strings.back();
		delete string;
		string = NULL;

		converted_strings.pop_back();
	}
}

extern "C" float ArrayAppend(float array_id2, float array_id1) {
	Array* array1 = ArrayGetById(array_id1);
	Array* array2 = ArrayGetById(array_id2);

	Array* result_array = new Array(array1->size() + array2->size());
	for (size_t i = 0; i < result_array->size(); i++) {
		if (i < array1->size()) {
			(*result_array)[i] = (*array1)[i];
		} else {
			(*result_array)[i] = (*array2)[i - array1->size()];
		}
	}

	arrays.push_back(result_array);
	return arrays.size() - 1;
}

extern "C" float ArrayGetSize(float array_id) {
	return ArrayGetById(array_id)->size();
}

extern "C" float ArrayItemGet(float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	testArrayIndex(array_id, array, index);

	return (*array)[index];
}

extern "C" void ArrayItemSet(float value, float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	testArrayIndex(array_id, array, index);

	(*array)[index] = value;
}

extern "C" void ArrayItemInsert(float value, float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	testArrayIndex(array_id, array, index);

	Array::iterator position = array->begin();
	std::advance(position, index);

	array->insert(position, value);
}

extern "C" void ArrayItemAppend(float value, float array_id) {
	ArrayGetById(array_id)->push_back(value);
}

extern "C" void ArrayItemRemove(float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	testArrayIndex(array_id, array, index);

	Array::iterator position = array->begin();
	std::advance(position, index);

	array->erase(position);
}

extern "C" Array* ArrayGetById(float array_id) {
	Array* array = NULL;
	if (array_id >= 0 && array_id < arrays.size()) {
		array = arrays[array_id];
	}

	if (array == NULL) {
		ProcessError("Invalid array ID " + ConvertToString(array_id) + ".");
	}

	return array;
}
