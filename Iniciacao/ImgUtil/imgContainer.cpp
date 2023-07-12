#include "imageClass.h"
#include "imgContainer.h"
#include "dirent.h"

using namespace std;


//retorna processingImg::imageClass presente no intervalo do index 
processingImg::imageClass processingImg::imgContainer::getImgInVector(int number) {

	if (number <= imageVector.size()) {
		processingImg::imageClass img = imageVector.at(number);
		return img;
	}
	else {
		std::cout << "index out of range";

	}

}


//retorna o nome das imagens presentes no vector 
std::vector <std::string> processingImg::imgContainer::returnNamesImages() {

	//busca o nome de cada arquivo de imagem contido no caminho especificado
	//retorna um vetor de strings com o nome de cada imagem do diretorio


	std::vector <std::string> names_imgs; //name vector
	std::string pathElement = getPath();


	struct dirent* dir;

	DIR* d;

	try {	//abertura do diret�rio 

		d = opendir(pathElement.c_str());


		while ((dir = readdir(d)) != NULL) {
			names_imgs.push_back(dir->d_name);
		}


		closedir(d);

		return names_imgs;

	}
	catch (char* ex) {

		std::cout << ex << std::endl;
	}

}



// obtem um vector com o nome das imagens presentes no diretório 
void processingImg::imgContainer::addListImgs() {

	//Adiciona cada imagem em um vetor de imagens
	//Retorna um vetor com todas as imagens do diret�rio


	processingImg::imageClass imgElement;

	std::vector <std::string> list_imgs = returnNamesImages();
	cv::Mat test;

	for (int i = 2; i < list_imgs.size(); i++) {

		string pt = path + list_imgs[i];

		imgElement.setImg(pt.c_str());


		if (!imgElement.emptyImg()) {
			this->imageVector.push_back(imgElement);

		}

	}

}

void processingImg::imgContainer::addImgs(processingImg::imageClass img) {
	if (!img.emptyImg()) {
		this->imageVector.push_back(img);
	}

}