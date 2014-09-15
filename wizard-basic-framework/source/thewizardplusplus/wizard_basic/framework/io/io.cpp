#include "io.h"
#include "../array/array.h"
#include "../utils/utils.h"
#include <iostream>

using namespace thewizardplusplus::wizard_basic::framework::io;
using namespace thewizardplusplus::wizard_basic::framework::array;
using namespace thewizardplusplus::wizard_basic::framework::utils;

static const size_t FILE_READ_ALL_BUFFER_SIZE = 1024;

static FileList files;

extern "C" void Show(float string_id) {
	Array* string = ArrayGetById(string_id);
	for (size_t i = 0; i < string->size(); i++) {
		std::cout << static_cast<char>((*string)[i]);
	}
	std::cout << std::flush;
}

extern "C" float FileOpen(float mode, float filename_id) {
	const char* filename = ArrayConvertToString(filename_id);
	std::fstream* file = new std::fstream();
	if (mode == FILE_MODE_READ) {
		file->open(filename, std::ios_base::binary | std::ios_base::in);
	} else if (mode == FILE_MODE_APPEND) {
		file->open(
			filename,
			std::ios_base::binary | std::ios_base::out | std::ios_base::app
		);
	} else if (mode == FILE_MODE_REWRITE) {
		file->open(
			filename,
			std::ios_base::binary | std::ios_base::out | std::ios_base::trunc
		);
	}

	ArrayClearMemoryAfterConvertsToStrings();
	if (!file->is_open()) {
		delete file;
		file = NULL;

		return -1;
	}

	files.push_back(file);
	return files.size() - 1;
}

extern "C" void FileClose(float file_id) {
	if (file_id < 0 || file_id >= files.size()) {
		ProcessError("Invalid file ID " + ConvertToString(file_id) + ".");
	}

	if (files[file_id] != NULL) {
		files[file_id]->close();

		delete files[file_id];
		files[file_id] = NULL;
	}
}

extern "C" void FileCloseAll(void) {
	for (size_t i = 0; i < files.size(); i++) {
		if (files[i] != NULL) {
			files[i]->close();

			delete files[i];
			files[i] = NULL;
		}
	}
}

extern "C" void FileRemove(float filename_id) {
	std::remove(ArrayConvertToString(filename_id));
	ArrayClearMemoryAfterConvertsToStrings();
}

extern "C" float FileRead(float number_of_bytes, float file_id) {
	std::fstream* file = FileGetById(file_id);

	char* buffer = new char[static_cast<size_t>(number_of_bytes)];
	size_t number_of_read_bytes = file->readsome(buffer, number_of_bytes);

	float array_id = ArrayCreate(number_of_read_bytes);
	Array* array = ArrayGetById(array_id);
	for (size_t i = 0; i < number_of_read_bytes; i++) {
		(*array)[i] = buffer[i];
	}

	delete[] buffer;
	buffer = NULL;

	return array_id;
}

extern "C" float FileReadAll(float file_id) {
	std::fstream* file = FileGetById(file_id);
	float array_id = ArrayCreate(0);
	Array* array = ArrayGetById(array_id);

	char* buffer = new char[FILE_READ_ALL_BUFFER_SIZE];
	std::streampos position = file->tellg();
	file->seekg(0);
	while (true) {
		size_t number_of_read_bytes = file->readsome(
			buffer,
			FILE_READ_ALL_BUFFER_SIZE
		);
		for (size_t i = 0; i < number_of_read_bytes; i++) {
			array->push_back(buffer[i]);
		}

		if (number_of_read_bytes < FILE_READ_ALL_BUFFER_SIZE) {
			break;
		}
	}

	file->seekg(position);
	delete[] buffer;
	buffer = NULL;

	return array_id;
}

extern "C" void FileWrite(float array_id, float file_id) {
	std::fstream* file = FileGetById(file_id);
	Array* array = ArrayGetById(array_id);

	char* buffer = new char[array->size()];
	for (size_t i = 0; i < array->size(); i++) {
		buffer[i] = (*array)[i];
	}

	file->write(buffer, array->size());
	file->flush();

	delete[] buffer;
	buffer = NULL;
}

extern "C" std::fstream* FileGetById(float file_id) {
	std::fstream* file = NULL;
	if (file_id >= 0 && file_id < files.size()) {
		file = files[file_id];
	}

	if (file == NULL) {
		ProcessError("Invalid file ID " + ConvertToString(file_id) + ".");
	}

	return file;
}
