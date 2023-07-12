#include "imageClass.h"
#include "imgContainer.h"
using namespace std;

void processingImg::imageClass::setImg(const char* path) {

	try {
		this->img = cv::imread(path);
	}
	catch (char* ex) {
		std::cout << ex << std::endl;
	}

}


void processingImg::imageClass::imShow() {

	cv::namedWindow("img", cv::WINDOW_NORMAL);
	cv::imshow("img", this->img);


	cv::waitKey(0); // waits until a key is pressed
	cv::destroyAllWindows(); // destroys the window showing processingImg
}




void processingImg::imageClass::imWrite(string path, string name) {

	cv::imwrite(path + name + string(".png"), getImg());

}

std::string processingImg::imgContainer::getPath() {
	return this->path;
}

//Set path of directory
bool processingImg::imgContainer::setPath(char* path) {


	if (path == NULL) {
		throw "caminho inválido";
	}

	else {
		this->path = path;
		return true;
	}
}

