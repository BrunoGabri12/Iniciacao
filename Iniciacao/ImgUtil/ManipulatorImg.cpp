#pragma once

#include <iostream> 
#include <vector>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "dirent.h"
#include "ManipulatorImg.h"
using namespace std;








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
	cv::Mat img = this->img.getImg();
	cv::adaptiveThreshold(this->img.getImg(),img, maxValue, adaptiveMethods, thresholdType, blockSize, C);

	this->img.setImg(img);
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

	cv::Mat img = this->img.getImg();

	this->img.imShow();

	this->img.getImg().convertTo(img, -1, alpha, beta);
	return *this;
}


processingImg::ManipulatorImg processingImg::ManipulatorImg::applySuperPixelsSegmentation(int algorithm, int region_size= 10, float ruler= 0.075f, int iterate= 10) {
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


		//if (applyClassificationRefinedTree(BGR)) {
			//imgClass.setTo(cv::Scalar(0, 0, 255), superpixel_mask);  //caso o método de classificação tenha retornado verdadeiro, então é um biofilme 
		//}
		//caso contrário é um pedaço da prótese 


	}



		this->setImg(imgClass);

		return *this;
}


/*recebe o pixel e classifica entre biofilme e prótese com base na arvore treinada com as 10 instâncias das imagens,
escolhendo 10 pixels de biofilme e 10 de prótese(200 pixels)
*/
bool processingImg::ManipulatorImg::applyClassificationOldTree(cv::Scalar BGR ) {

	if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
		return false;
	}

	if (BGR[0] > 184) {
		if (BGR[0] > 210) {
			return false;
		}
		else {
			if (BGR[1] > 132) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		if (BGR[1] > 100) {
			return true; 
		}
		else {
			if (BGR[0] > 122) {
				return false;
			}
			else {
				return true; 
			}
		}
	}


}


/*recebe o pixel e classifica entre biofilme e prótese com base na arvore treinada com todas as instancias, 
escolhendo 10 pixels de biofilme e 10 de prótese de modo aleatório

*/
bool processingImg::ManipulatorImg::applyClassificationNewTree(cv::Scalar BGR) {
	
	if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
		return false;
	}

	if (BGR[0] > 193) {
		if (BGR[0] > 246) {
			return false;
		}
		else {
			if (BGR[1] > 172) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		if (BGR[1] > 88) {
			return true;
		}
		else {
			if (BGR[0] > 140) {
				if (BGR[2] > 15) {
					return false;
				}
				else {
					return true;
				}
			}
			else {
				if (BGR[1] > 28) {
					return true;
				}
				else {
					if (BGR[0] > 58) {
						return false;
					}
					else {
						return true;
					}
				}
			}
		}
	}

}


/*recebe o pixel e classifica entre biofilme e prótese com base na arvore treinada com todas as instancias,
escolhendo 10 pixels de biofilme e 10 de prótese de modo aleatório

*/
bool processingImg::ManipulatorImg::applyClassificationRefinedTree(cv::Scalar BGR) {

	if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
		return false;
	}

	if (BGR[0] > 206) {
		if (BGR[0] > 246) {
			return false;
		}
		else {
			if (BGR[1] > 88) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		if (BGR[1] > 36) {
			if (BGR[0] > 91) {
				if (BGR[1] > 67) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return true; 
			}
		}
		else {
			if (BGR[0] > 66) {
				return false;
			}
			else {
				return true; 
			}
		
		}
	}


}

bool processingImg::ManipulatorImg::applyClassificationV3Tree(cv::Scalar BGR) {
	if (BGR[0] < 20 && BGR[1] < 20 && BGR[2] < 20) {
		return false;
	}

	if (BGR[0] > 163) {
		if (BGR[1] > 86) {
			if (BGR[0] > 246) {
				return false;
			}
			else {
				if (BGR[1] > 137) {
					return true; 
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}
	else {
		if (BGR[1] > 13) {
			if (BGR[0] > 90) {
				if (BGR[1] > 52) {
					if (BGR[1] > 122) {
						return true;
					}
					else {
						if (BGR[0] > 133) {
							if (BGR[0] > 143 ) {
								return true;
							}
							else {
								return false; 
							}
						}
						else {
							return true; 
						}
					}
				}
				else {
					return false;
				}
			}
			else {
				if (BGR[1] > 33) {
					return true;
				}
				else {
					if (BGR[0] > 43) {
						return false;
					}
					else {
						return true; 
					}
				}
			}
		}
		else {
			return false;
		}
	}
}




//subtrai uma imageClass da outra resultando apenas no biofilme 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applyClassificationBiofilmeAndProtese() {

	cv::Mat img = this->img.getImg();

	for (int i = 0; i < img.rows; i++) {

		for (int j = 0; j < img.cols; j++) {
			if (!applyClassificationV3Tree(img.at<cv::Vec3b>(i, j))) {
				continue;
			}
			else {
					img.at<cv::Vec3b>(i, j)[0] = 0;
					img.at<cv::Vec3b>(i, j)[1] = 0;
					img.at<cv::Vec3b>(i, j)[2] = 255;
			}

		}

	}

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

				//Vermelho é verdadeiro positivo
				img.at<cv::Vec3b>(i, j)[0] = 0;
				img.at<cv::Vec3b>(i, j)[1] = 0;
				img.at<cv::Vec3b>(i, j)[2] = 255;
			}

			//Se o padrão é !vermelho (protese) e a imagem é bioflme, temos então um falso positvo
			else if (!redPattern and redImg) {
				falsopositivos += 1;

				//Rosa Escuro é falso positivo
				img.at<cv::Vec3b>(i, j)[0] = 128;
				img.at<cv::Vec3b>(i, j)[1] = 0;	
				img.at<cv::Vec3b>(i, j)[2] = 128;
			}

			//Se o padrão é !vermelho  (protese) e a imagem é !vermelho (protese) , temos então um verdadeiro negativo
			else if (!redPattern and !redImg) {
				vedadeiroNegativo += 1;

				//Verde é verdadeiro negativp
				img.at<cv::Vec3b>(i, j)[0] = 0;
				img.at<cv::Vec3b>(i, j)[1] = 255;
				img.at<cv::Vec3b>(i, j)[2] = 0;
			}

			//Caso padrão seja vermelho e imagem é !vermelho (região de protese), temos um falso negativo 
			else if (redPattern and !redImg) {
				falsonegativo += 1;

				//Amarelho é um falso negativo 
				img.at<cv::Vec3b>(i, j)[0] = 0;
				img.at<cv::Vec3b>(i, j)[1] = 255;
				img.at<cv::Vec3b>(i, j)[2] = 255;
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



void  processingImg::ManipulatorImg::applyRandomShort(int size) {

	//Método para escolher pixels aleatórios dentro da imagem 
	// valor de size para especificar a quantidade de pixels aleatórios deseja-se

	cv::Mat img = this->img.getImg();

	int imgCols = img.cols;
	int imgRows = img.rows;

	cv::Vec3b RGB;
	std::vector <cv::Vec3b> arrayRGB; 
	int count = 0;


	//colocar os valores em um arrayList

	for (int i = 0; i < imgCols; i++) {
		for (int j = 0; j < imgRows; j++) {


			if (img.at<cv::Vec3b>(j, i)[0] <= 20 and img.at<cv::Vec3b>(j,i)[1] <= 20 and img.at<cv::Vec3b>(j, i)[2] <= 20)
				continue; //pula os valores de RGB preto

			RGB = img.at<cv::Vec3b>(j, i);
			arrayRGB.push_back(RGB); //Adiciona o RGB no array 
		}
	}

	

	srand((unsigned)time(NULL));

	int lineRandom = 0;  //seleciona a linha aleatóriamente de 0 a array.size()

	while (count < size) {


		if (arrayRGB.size() == 0) { // para o caso onde não há biofilme na imagem
			count += 1; 
			continue; 
		}
		else {
			lineRandom = 0 + (rand() % arrayRGB.size());
		}



		int B = arrayRGB[lineRandom][0];
		int G = arrayRGB[lineRandom][1];
		int R = arrayRGB[lineRandom][2];


		std::cout<< R << "\t " << G << "\t" << B << "\t" << "protese" << endl; //mostra os valores expressos de R,G e B 

		count += 1;
	

	}

	


}

//Subtrai uma imagem da outra 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageSubtract(imageClass img) {

	cv::Mat base =  img.getImg();
	cv::Mat mask =  this->img.getImg();
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



//subtrai uma imageClass da outra resultando apenas no biofilme 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applySubtractBiofilme(imageClass img) {

	cv::Mat imgj = img.getImg();
	cv::Mat imgp = this->img.getImg();

	for (int i = 0; i < imgp.rows; i++) {

		for (int j = 0; j < imgp.cols; j++) {

			if (imgj.at<cv::Vec3b>(i, j)[0] == 0 and imgj.at<cv::Vec3b>(i, j)[1] == 0 and imgj.at<cv::Vec3b>(i, j)[2] == 255) {
				continue;
			}
			else {
				
					imgp.at<cv::Vec3b>(i, j) = 0; 
				 }

		}

	}

	return *this;
}




//subtrai a protese da imagem, resultando somente em biofilme 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applySubtractProtese(imageClass img) {

	cv::Mat imgj = img.getImg();
	cv::Mat imgp = this->img.getImg();

	for (int i = 0; i < imgp.rows; i++) {

		for (int j = 0; j < imgp.cols; j++) {

			if (imgj.at<cv::Vec3b>(i, j)[0] == 0 and imgj.at<cv::Vec3b>(i, j)[1] == 255 and imgj.at<cv::Vec3b>(i, j)[2] == 0) {
				continue;
			}
			else {
				imgp.at<cv::Vec3b>(i, j) = 0;
			}

		}

	}

	return *this;
}


//recorta a área da regua na imagem 
processingImg::ManipulatorImg processingImg::ManipulatorImg::applyImageCut() {



	bool flag = false;
	int i, j;

	cv::Mat img = this->img.getImg();

	for (i = img.rows / 2; i < img.rows; ++i) {


		for (j = 0; j < img.cols ; ++j) { //percorre uma linha inteira para 
											//verificar se existe uma linha somente com zeros 

			if (!img.at<uchar>(i, j) == 0) {
				flag = false;	//caso tenha achado uma linha vazia ativa flag 
			}

		}

		if (flag) { //se flag== false, então linha em preto foi encontrada

			for (; i < img.rows; i++) {	//
				for (int t = 0; t < img.cols; t++) {
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

//calculo da área da protese 
int processingImg::ManipulatorImg::calcAreaProtese() {

	cv::Mat img = this->img.getImg();
	int size = 0;


	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			
			if (img.at<cv::Vec3b>(i, j)[0] == 0 and img.at<cv::Vec3b>(i, j)[1] == 0 and img.at<cv::Vec3b>(i, j)[2] != 255) {
				continue;
			}
			else {
				size++;
			}

		}
	}

	return size;
}


int processingImg::ManipulatorImg::calcAreaBiofilme() {

	cv::Mat img = this->img.getImg();
	int size = 0;


	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img.at<cv::Vec3b>(i, j)[0] == 0 and img.at<cv::Vec3b>(i, j)[1] == 0 and img.at<cv::Vec3b>(i, j)[2] == 255) {
				size++;
			}

		}
	}

	return size;
}




