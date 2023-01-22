#pragma once

#include <iostream> 
#include <vector>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "dirent.h"
#include "ManipulatorImg.h"
using namespace std;



//************************** IMG ********************************

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


/*************THRESHOLDING************/

processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageThresholding(int min, int max, int type) {
	//aplicação Thresholding na imagem.
	//min= valor mínimo de thresholding max = valor máximo. 
	//type = tipo de thresholding
	/*Ex:
	cv.THRESH_BINARY
	cv.THRESH_BINARY_INV
	cv.THRESH_TRUNC
	cv.THRESH_TOZERO
	cv.THRESH_TOZERO_INV
	*/

	if (min == 0) {
		cv::Mat img[3];

		cv::split(this->img.getImg(), img);
		this->img.setImg(img[0]);
	}


	cv::threshold(this->img.getImg(), this->img.getImg(), min, max, type);

	return *this;
}


processingImg::ManipulatorImg processingImg::ManipulatorImg::applySplit(int channel) {

	/*Realiza a separação dos canais encontrados na imagem
	channel = {0,1,2}
	return imagem do canal especificado
	*/
	cv::Mat img[3];

	cv::split(this->img.getImg(), img);
	this->img.setImg(img[channel]);


	return *this;

}


processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageAdaptativeTheresholding(int maxValue, int adaptiveMethods, int thresholdType, int blockSize, int C) {
	/*
	Aplicação thresholding adapitative.
	maxValue: valor máximo do threshold == 255
	adaptiveMethods = método adaptativo
		cv.ADAPTIVE_THRESH_MEAN_C
		cv.ADAPTIVE_THRESH_GAUSSIAN_C

	thresholdType = tipo de thresholding
		THRESH_BINARY // THRESH_BINARY_IN
	blockSize = tamanho do bloco utilizado para calcular o threshold
		3,5,7
	C = constante normalmente positiva
	*/

	cv::adaptiveThreshold(this->img.getImg(), this->img.getImg(), maxValue, adaptiveMethods, thresholdType, blockSize, C);


	return *this;
}



processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageGaussianBlur(cv::Size ksize, double sigmaX, double sigmaY) {
	/*
	Aplicação do método GaussianBlur na imagem desejada
	Ksize valor do kernel que irá percorrer a imagem
	sigmaX valor de desvio em X
	sigmaY valor de desvio em Y


	retorna a imagem com o blur aplicado
	*/
	cv::GaussianBlur(this->img.getImg(), this->img.getImg(), ksize, sigmaX, sigmaY);

	return *this;
}



processingImg::ManipulatorImg processingImg::ManipulatorImg::applyGammaCorrection(float gamma) {

	cv::Mat img = this->img.getImg();

	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();

	for (int i = 0; i < 256; ++i)
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

	cv::Mat res = img.clone();
	LUT(img, lookUpTable, res);
	//! [changing-contrast-brightness-gamma-correction]

	this->img.setImg(res);

	return *this;
}



processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageMorphology(int elementStruc, int morph_size, int type, int iterations) {
	/*
	Método para aplicação de operações morfológicas na imagem

	elementStruc =
		- MORPH_RECT
		- MORPH_CROSS
		- MORPH_ELIPSE

	morph_size = tamanho dos elementos estruturantes

	type
		- MORPH_ERODE
		- MORPH_DILATE
		- MORPH_OPEN
		- MORPH_CLOSE

	iterations = quantidade de iterações desejadas para serem realizadas na imagem

	retorna a imagem com a operação morfológica aplicada
	*/



	cv::Mat element = cv::getStructuringElement(elementStruc,
		cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
		cv::Point(morph_size, morph_size));

	cv::morphologyEx(this->img.getImg(), this->img.getImg(), type, element, cv::Point(-1, -1), iterations);

	return *this;
}




processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageContrast(double alpha, double beta) {
	/*
	 Aplica contraste na imagem

	 alpha
	 beta

	 g(i,j) = alpha * f(i,j) + beta

		exemplos de valores
			alpha 2.2
			beta 50

	retorna a imagem com o contraste e brilho modificados
	*/



	this->img.getImg().convertTo(this->img.getImg(), -1, alpha, beta);
	return *this;
}



processingImg::ManipulatorImg processingImg::ManipulatorImg::applySuperPixelsSegmentation(int algorithm, int region_size, float ruler, int iterate) {
	/*
	aplicação do método de superpixel na imagem

	algorithm
		- SLIC
		- SLICO
		- MSLIC

	region_size = tamanho da região do superpixel, valor padrão de 10

	ruler = fator de suavidade do superpixel

	iterate = quantidade de iterações realizadas na imagem

	retorna a imagem com os superpixels
	*/

	cv::Mat img = this->img.getImg();
	cv::Mat imgClass = this->img.getImg();



	cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(img, algorithm, region_size, ruler);


	slic->iterate(iterate);

	cv::Mat1i labelImg(img.rows, img.cols);

	slic->getLabels(labelImg);


	for (int i = 0; i < slic->getNumberOfSuperpixels(); ++i)
	{
		// Mask for each label
		cv::Mat1b superpixel_mask = labelImg == i;



		cv::Scalar BGR = mean(img, superpixel_mask);

		img.setTo(BGR, superpixel_mask);


		//TO-DO : Colocar isso em outro método, não faz sentido estar nesse método 
			/*

			if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
				continue;
			}

			if (BGR[0] > 193) {
				if (BGR[0] > 246) {
					continue;
				}
				else {
					if (BGR[1] > 172) {
						imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
					}
					else {
						continue;
					}
				}
			}
			else {
				if (BGR[1] > 88) {
					imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
				}
				else {
					if (BGR[0] > 140)  {
						if (BGR[2] > 15) {
							continue;
						}
						else {
							imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
						}
					}
					else {
						if (BGR[1] > 28) {
							imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
						}
						else {
							if (BGR[0] > 58) {
								continue;
							}
							else {
								imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
							}
						}
					}
				}
			}

			*/


			//	PADRÃO ****************
		if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
			continue;
		}

		if (BGR[0] > 184) {
			if (BGR[0] > 210) {
				continue;
			}
			else {
				if (BGR[1] > 132) {
					imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
				}
				else {
					continue;
				}
			}
		}
		else {
			if (BGR[1] > 100) {
				imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
			}
			else {
				if (BGR[0] > 122) {
					continue;
				}
				else {
					imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);
				}
			}
		}


	}


	this->setImg(imgClass);

	return *this;
}


processingImg::ManipulatorImg processingImg::ManipulatorImg::applyMetrics(imageClass pattern) {

	/*
	pattern = imagem segmentada manualmente

	verdadeiroPositivo = Região de Biofilme e classificada como biofilme
	verdadeiroNegativo = Região de Prótese e classificada como prótese
	falsoPositivo = Região de Prótese presente na imagem e classificada como biofilme
	falsoNegativo = Região de Biofilme na Imagem e classifica como protese

	*/

	cv::Mat img = this->img.getImg();
	cv::Mat patternImg = pattern.getImg();
	float verdadeiroPositivos = 0;
	float falsopositivos = 0;
	float falsonegativo = 0;
	float vedadeiroNegativo = 0;

	//flags para avaliar se a região é vermelha ( Biofilme detectado ) ou não
	bool redPattern = false;
	bool redImg = false;

	for (int i = 0; i < img.rows; i++) {  //Percorre as duas imagens comparando pixel a pixel 

		for (int j = 0; j < img.cols; j++) {

			if (img.at<cv::Vec3b>(i, j)[0] < 20 and img.at<cv::Vec3b>(i, j)[1] < 20 and img.at<cv::Vec3b>(i, j)[2] < 20) continue;

			if (patternImg.at<cv::Vec3b>(i, j)[0] == 0 and patternImg.at<cv::Vec3b>(i, j)[1] == 0 and patternImg.at<cv::Vec3b>(i, j)[2] == 255) {
				redPattern = true;
			}
			else {
				redPattern = false;
			}


			if (img.at<cv::Vec3b>(i, j)[0] == 0 and img.at<cv::Vec3b>(i, j)[1] == 0 and img.at<cv::Vec3b>(i, j)[2] == 255) {
				redImg = true;

			}
			else {
				redImg = false;
			}


			//Se a região do padrão é vermelho e o da imagem também, então temos uma região de biofilme
			if (redPattern and redImg) {

				verdadeiroPositivos += 1;
			}

			//Se o padrão é !vermelho (protese) e a imagem é bioflme, temos então um falso positvo
			else if (!redPattern and redImg) {
				falsopositivos += 1;

			}

			//Se o padrão é !vermelho  (protese) e a imagem é !vermelho (protese) , temos então um verdadeiro negativo
			else if (!redPattern and !redImg) {
				vedadeiroNegativo += 1;
			}

			//Caso padrão seja vermelho e imagem é !vermelho (região de protese), temos um falso negativo 
			else if (redPattern and !redImg) {
				falsonegativo += 1;
			}


		}


	}


	//realiza o cálculo das métricas 
	//Precisão, especificidade, sensibilidade e acurácia 
	float precisa = (verdadeiroPositivos / (verdadeiroPositivos + falsopositivos));
	float especificidade = ((vedadeiroNegativo) / (vedadeiroNegativo + falsopositivos));
	float sensibilidade = ((verdadeiroPositivos) / (verdadeiroPositivos + falsonegativo));
	float acuracia = ((verdadeiroPositivos + vedadeiroNegativo) / (verdadeiroPositivos + vedadeiroNegativo + falsonegativo + falsopositivos));


	std::cout << verdadeiroPositivos << "," << vedadeiroNegativo << "," << falsopositivos << "," << falsonegativo << "," << precisa << "," << especificidade << "," << sensibilidade << "," << acuracia << endl;


	return *this;
}




processingImg::ManipulatorImg processingImg::ManipulatorImg::applyRandomShort(int size) {

	//Método para escolher pixels aleatórios dentro da imagem 
	// valor de size para especificar a quantidade de pixels aleatórios deseja-se

	cv::Mat img = this->img.getImg();

	int imgCols = img.cols;
	int imgRows = img.rows;


	int count = 0;
	int colRandom;
	int rowRandom;

	srand((unsigned)time(NULL));


	while (count < size) {

		colRandom = 0 + (rand() % imgCols);
		rowRandom = 0 + (rand() % imgRows);

		if (img.at<cv::Vec3b>(rowRandom, colRandom)[0] != 0 and img.at<cv::Vec3b>(rowRandom, colRandom)[1] != 0 and
			img.at<cv::Vec3b>(rowRandom, colRandom)[2] != 0) {

			int B = img.at<cv::Vec3b>(rowRandom, colRandom)[0];
			int G = img.at<cv::Vec3b>(rowRandom, colRandom)[1];
			int R = img.at<cv::Vec3b>(rowRandom, colRandom)[2];


			std::cout << R << "\t " << G << "\t" << B << "\t" << "biofilme" << endl; //mostra os valores expressos de R,G e B 

			count += 1;
		}


	}

	return *this;


}

//Subtrai uma imagem da outra 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageSubtract(imageClass img) {

	cv::Mat base = this->img.getImg();
	cv::Mat mask = img.getImg();
	cv::Mat result;

	base.copyTo(result, mask);


	this->setImg(result);


	return *this;
}

processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImagemAdd(imageClass img) {
	cv::add(this->img.getImg(), img.getImg(), this->img.getImg());
	return *this;
}


processingImg::ManipulatorImg processingImg::ManipulatorImg::applySuperPixelContour(int algorithm, int region_size, int ruler, int iterate, bool thick_line) {
	//Contorno do superpixel 
	cv::Mat img = this->img.getImg();
	cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(img, algorithm, region_size, ruler);

	slic->iterate(iterate);


	cv::Mat result = img.clone();

	slic->getLabelContourMask(result, TRUE);

	img.setTo(cv::Scalar(0, 0, 255), result);

	this->img.setImg(img);

	return *this;
}

//subtrai uma imageClass da outra 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applySubtractProtese(imageClass img) {

	cv::Mat imgj = img.getImg();
	cv::Mat imgp = this->img.getImg();

	for (int i = 0; i < imgp.rows; i++) {

		for (int j = 0; j < imgp.cols; j++) {

			if (imgj.at<cv::Vec3b>(i, j)[0] == 0 and imgj.at<cv::Vec3b>(i, j)[1] == 0 and imgj.at<cv::Vec3b>(i, j)[2] == 0) {
				imgp.at<cv::Vec3b>(i, j) = 0;
			}

		}

	}

	std::cout << "base medidas = " << imgp.rows << "\t" << imgp.cols << endl;
	std::cout << "mask = " << imgj.rows << "\t" << imgj.cols << endl;


	cv::waitKey(0);

	return *this;
}





//recorta a área da regua na imagem 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageCut() {



	bool flag = false;
	int i, j;

	cv::Mat img = this->img.getImg();

	for (i = img.rows / 2; i < img.rows; ++i) {


		for (j = 0; j < img.cols * 3; ++j) { //percorre uma linha inteira para 
											//verificar se existe uma linha somente com zeros 

			if (!img.at<uchar>(i, j) == 0) {
				flag = false;	//caso tenha achado uma linha vazia ativa flag 
			}

		}

		if (flag) { //se flag== false, então linha em preto foi encontrada

			for (; i < img.rows; i++) {	//
				for (int t = 0; t < img.cols * 3; t++) {
					img.at<uchar>(i, t) = 0;
				}
			}

			break;
		}

		flag = true;
	}

	return *this;
}


//calculo da área da imagem
void processingImg::ManipulatorImg::calcArea() {

	int larg = this->img.getImg().cols * 3;
	int alt = this->img.getImg().rows;

	std::cout << larg * alt << std::endl;



}

//calculo da área da protese Tenho experiência em pesquisa ciêntifica 
processingImg::ManipulatorImg processingImg::ManipulatorImg::calcAreaProtese() {

	cv::Mat img = this->img.getImg();
	int size = 0;

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img.at<cv::Vec3b>(i, j)[0] > 20 and img.at<cv::Vec3b>(i, j)[1] > 20 and img.at<cv::Vec3b>(i, j)[2] > 20) {
				//img.at<cv::Vec3b>(i, j)[0] = 0;
					//img.at<cv::Vec3b>(i, j)[1] = 255;
					//img.at<cv::Vec3b>(i, j)[2] = 0;
				size++;

			}

		}
	}




	std::cout << "Area da protese" << size << std::endl;
	return *this;
}


