#include <array/array.h>
#include <curl/curl.h>
#include <fstream>
#include <iostream>

using namespace thewizardplusplus::wizard_basic::framework::array;

extern "C" float file_get_contents(float filename_id) {
	std::string filename = ArrayConvertToString(filename_id);
	std::string content;
	if (
		filename.substr(0, 7) == "http://"
		|| filename.substr(0, 8) == "https://"
	) {
		CURL* curl = curl_easy_init();
		if (curl != NULL) {
			curl_easy_setopt(curl, CURLOPT_URL, filename.c_str());
			//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			CURLcode result = curl_easy_perform(curl);
			if (result != CURLE_OK) {
				std::cerr
					<< "Error! Unable to get content of file \""
					<< filename
					<< "\" (\""
					<< curl_easy_strerror(result)
					<< "\")."
					<< std::endl;
			}

			curl_easy_cleanup(curl);
		}
	} else {
		if (filename.substr(0, 7) == "file://") {
			filename = filename.substr(7);
		}

		std::ifstream file(filename.c_str());
		content = std::string(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
		);
	}

	return ArrayCreateFromString(content.c_str());
}

int main(void) {
	float filename_id = ArrayCreateFromString("http://example.com");
	float content_id = file_get_contents(filename_id);
	std::cout << ArrayConvertToString(content_id) << std::endl;
}
