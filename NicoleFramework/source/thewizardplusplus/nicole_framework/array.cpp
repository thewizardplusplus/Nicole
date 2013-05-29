#include "array.h"
#include "utils.h"

using namespace thewizardplusplus::nicole_framework;

static ArrayList  arrays;
static StringList converted_strings;

extern "C" float ArrayCreate(float size) {
	Array* array = new Array(size);
	arrays.push_back(array);
	return arrays.size() - 1;
}

extern "C" float ArrayCreateFromString(const char* string) {
	std::string source = string;
	float array_id = ArrayCreate(source.size());

	Array* array = ArrayGetById(array_id);
	for (size_t i = 0; i < array->size(); i++) {
		(*array)[i] = source[i];
	}

	return array_id;
}

extern "C" float ArrayCreateFromNumber(float number) {
	std::ostringstream out;
	out << number;

	float array_id = ArrayCreateFromString(out.str().c_str());
	return array_id;
}

extern "C" const char* ArrayConvertToString(float array_id) {
	std::string* string = new std::string();
	converted_strings.push_back(string);

	Array* array = ArrayGetById(array_id);
	for (size_t i = 0; i < array->size(); i++) {
		*string += (*array)[i];
	}

	return string->c_str();
}

extern "C" void ArrayDelete(float array_id) {
	if (array_id >= 0 && array_id < arrays.size()) {
		ArrayList::iterator index = arrays.begin();
		std::advance(index, array_id);

		if (*index != NULL) {
			delete *index;
			*index = NULL;
		}

		return;
	}

	ProcessError("Invalid array ID " + ConvertToString(array_id) + ".");
}

extern "C" void ArrayDeleteAll(void) {
	for (size_t i = 0; i < arrays.size(); i++) {
		ArrayDelete(i);
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
	size_t array1_size = array1->size();
	Array* array2 = ArrayGetById(array_id2);

	size_t result_size = array1_size + array2->size();
	float result_id = ArrayCreate(result_size);
	Array* result = ArrayGetById(result_id);

	for (size_t i = 0; i < result_size; i++) {
		if (i < array1_size) {
			(*result)[i] = (*array1)[i];
		} else {
			(*result)[i] = (*array2)[i - array1_size];
		}
	}

	return result_id;
}

extern "C" float ArrayGetSize(float array_id) {
	Array* array = ArrayGetById(array_id);
	return array->size();
}

extern "C" float ArrayItemGet(float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	if (index >= 0 && index < array->size()) {
		return (*array)[index];
	} else {
		float array_size = ArrayGetSize(array_id);
		ProcessError("Invalid index " + ConvertToString(index) + " for array "
			"with ID " + ConvertToString(array_id) + ". This array has " +
			ConvertToString(array_size) + " element" + (array_size > 1 ? "s" :
			"") + ".");
		return 0.0f;
	}
}

extern "C" void ArrayItemSet(float value, float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	if (index >= 0 && index < array->size()) {
		(*array)[index] = value;
	} else {
		float array_size = ArrayGetSize(array_id);
		ProcessError("Invalid index " + ConvertToString(index) + " for array "
			"with ID " + ConvertToString(array_id) + ". This array has " +
			ConvertToString(array_size) + " element" + (array_size > 1 ? "s" :
			"") + ".");
	}
}

extern "C" void ArrayItemInsert(float value, float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	if (index >= 0 && index < array->size()) {
		Array::iterator position = array->begin();
		std::advance(position, index);
		array->insert(position, value);
	} else {
		float array_size = ArrayGetSize(array_id);
		ProcessError("Invalid index " + ConvertToString(index) + " for array "
			"with ID " + ConvertToString(array_id) + ". This array has " +
			ConvertToString(array_size) + " element" + (array_size > 1 ? "s" :
			"") + ".");
	}
}

extern "C" void ArrayItemAppend(float value, float array_id) {
	Array* array = ArrayGetById(array_id);
	array->push_back(value);
}

extern "C" void ArrayItemRemove(float index, float array_id) {
	Array* array = ArrayGetById(array_id);
	if (index >= 0 && index <= array->size()) {
		Array::iterator position = array->begin();
		std::advance(position, index);
		array->erase(position);
	} else {
		float array_size = ArrayGetSize(array_id);
		ProcessError("Invalid index " + ConvertToString(index) + " for array "
			"with ID " + ConvertToString(array_id) + ". This array has " +
			ConvertToString(array_size) + " element" + (array_size > 1 ? "s" :
			"") + ".");
	}
}

extern "C" Array* ArrayGetById(float array_id) {
	if (array_id >= 0 && array_id < arrays.size()) {
		ArrayList::const_iterator index = arrays.begin();
		std::advance(index, array_id);

		Array* array = *index;
		if (array != NULL) {
			return array;
		}
	}

	ProcessError("Invalid array ID " + ConvertToString(array_id) + ".");
	return NULL;
}
