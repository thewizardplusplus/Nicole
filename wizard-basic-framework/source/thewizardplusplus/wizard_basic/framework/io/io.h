#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <vector>
#include <fstream>

namespace thewizardplusplus {
namespace wizard_basic {
namespace framework {
namespace io {

typedef std::vector<std::fstream*> FileList;
enum FileOpenMode {
	FILE_MODE_READ,
	FILE_MODE_APPEND,
	FILE_MODE_REWRITE
};

extern "C" {

void Show(float string_id);
float FileOpen(float mode, float filename_id);
void FileClose(float file_id);
void FileCloseAll(void);
void FileRemove(float filename_id);
float FileRead(float number_of_bytes, float file_id);
float FileReadAll(float file_id);
void FileWrite(float array_id, float file_id);
std::fstream* FileGetById(float file_id);

}

}
}
}
}
#endif
