#pragma once
#include <string>
#include "imageClass.h"

namespace processingImg {
	class imgContainer {

	public:
		//set caminho

		imgContainer(std::string path) { this->path = path; this->addListImgs(); }

		imgContainer() {};

		bool setPath(char* path);

		std::string getPath();

		processingImg::imageClass getImgInVector(int number);

		int sizeVector() { return imageVector.size(); }

		void addImgs(processingImg::imageClass img);

		std::vector <std::string> returnNamesImages();


	private:

		std::string path = "-1";
		std::vector <processingImg::imageClass> imageVector;

		void addListImgs();

	};
}
