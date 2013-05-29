#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <list>
#include <fstream>

namespace thewizardplusplus {
namespace nicole_framework {

typedef std::list<std::fstream*> FileList;
class FileOpenMode {
public:
	enum Types {
		READ,
		APPEND,
		REWRITE
	};
};

extern "C" {

void Show(float string_id);
float FileOpen(float mode, float filename_id);
void FileClose(float file_id);
void FileCloseAll(void);
void FileDelete(float filename_id);
float FileRead(float number_of_bytes, float file_id);
float FileReadAll(float file_id);
void FileWrite(float array_id, float file_id);
std::fstream* FileGetById(float file_id);

}

}
}
#endif
