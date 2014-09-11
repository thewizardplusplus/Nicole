#include "input_output.h"
#include "array.h"
#include "utils.h"
#include <iostream>

using namespace thewizardplusplus::wizard_basic::framework;

static const size_t FILE_READ_ALL_BUFFER_SIZE = 1024;

FileList files;

extern "C" void Show(float string_id) {
	Array* string = ArrayGetById(string_id);
	for (size_t i = 0; i < string->size(); i++) {
		std::cout << static_cast<char>((*string)[i]);
	}
	std::cout << std::flush;
}

extern "C" float FileOpen(float mode, float filename_id) {
	std::string filename = ArrayConvertToString(filename_id);
	ArrayClearMemoryAfterConvertsToStrings();

	std::fstream* file = new std::fstream();
	if (mode == FileOpenMode::READ) {
		file->open(filename.c_str(), std::ios_base::binary | std::ios_base::in);
	} else if (mode == FileOpenMode::APPEND) {
		file->open(filename.c_str(), std::ios_base::binary | std::ios_base::out
			| std::ios_base::app);
	} else if (mode == FileOpenMode::REWRITE) {
		file->open(filename.c_str(), std::ios_base::binary | std::ios_base::out
			| std::ios_base::trunc);
	}
	if (!file->is_open()) {
		return -1;
	}

	files.push_back(file);
	return files.size() - 1;
}

extern "C" void FileClose(float file_id) {
	if (file_id >= 0 && file_id < files.size()) {
		FileList::iterator index = files.begin();
		std::advance(index, file_id);

		if (*index != NULL) {
			(*index)->close();

			delete *index;
			*index = NULL;
		}

		return;
	}

	ProcessError("Invalid file ID " + ConvertToString(file_id) + ".");
}

extern "C" void FileCloseAll(void) {
	for (size_t i = 0; i < files.size(); i++) {
		FileClose(i);
	}
}

extern "C" void FileDelete(float filename_id) {
	std::string filename = ArrayConvertToString(filename_id);
	ArrayClearMemoryAfterConvertsToStrings();

	std::remove(filename.c_str());
}

extern "C" float FileRead(float number_of_bytes, float file_id) {
	std::fstream* file = FileGetById(file_id);

	char* buffer = new char[static_cast<size_t>(number_of_bytes)];
	size_t number_of_read_bytes = file->readsome(buffer, number_of_bytes);

	float array_id = ArrayCreate(number_of_read_bytes);
	for (size_t i = 0; i < number_of_read_bytes; i++) {
		ArrayItemSet(buffer[i], i, array_id);
	}

	delete[] buffer;
	buffer = NULL;

	return array_id;
}

extern "C" float FileReadAll(float file_id) {
	std::fstream* file = FileGetById(file_id);
	char* buffer = new char[FILE_READ_ALL_BUFFER_SIZE];
	float array_id = ArrayCreate(0);

	while (true) {
		size_t number_of_read_bytes = file->readsome(buffer,
			FILE_READ_ALL_BUFFER_SIZE);
		for (size_t i = 0; i < number_of_read_bytes; i++) {
			ArrayItemAppend(buffer[i], array_id);
		}

		if (number_of_read_bytes < FILE_READ_ALL_BUFFER_SIZE) {
			break;
		}
	}

	delete[] buffer;
	buffer = NULL;

	return array_id;
}

extern "C" void FileWrite(float array_id, float file_id) {
	std::fstream* file = FileGetById(file_id);
	Array* array = ArrayGetById(array_id);

	size_t array_size = array->size();
	char* buffer = new char[array_size];
	for (size_t i = 0; i < array_size; i++) {
		buffer[i] = (*array)[i];
	}

	file->write(buffer, array_size);
	file->flush();

	delete[] buffer;
	buffer = NULL;
}

extern "C" std::fstream* FileGetById(float file_id) {
	if (file_id >= 0 && file_id < files.size()) {
		FileList::const_iterator index = files.begin();
		std::advance(index, file_id);

		std::fstream* file = *index;
		if (file != NULL) {
			return file;
		}
	}

	ProcessError("Invalid file ID " + ConvertToString(file_id) + ".");
	return NULL;
}
