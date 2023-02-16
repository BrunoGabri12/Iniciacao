#pragma once
#ifndef _MANIPULATION_H
#define _MANIPULATION_H

#include <iostream> 
#include <vector>
#include "direct.h"
#include <stdexcept>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

#include <cstdlib>



namespace processingImg {

	class imageClass {

	public:

		imageClass() {} //construtores 

		imageClass(cv::Mat imageInstated) { this->img = imageInstated; }

		imageClass(char* path) { setImg(path); }



		bool emptyImg() { return this->img.empty(); }

		cv::Mat getImg() { return this->img; }

		void setImg(const char* path);

		void setImg(cv::Mat imageInstated) { this->img = imageInstated; }


		void imShow();

		void imWrite(std::string path, std::string name);

	private:

		cv::Mat img;

	};


	//********************* imgManipulator *****************

	class ManipulatorImg {

	public:

		ManipulatorImg() {}; //construtores 

		ManipulatorImg(processingImg::imageClass img) {
			this->img = img;
		}

		void setImg(processingImg::imageClass img) {
			this->img = img;
		}

		//Possui utilizar 

		ManipulatorImg applyImageThresholding(int min, int max, int type);

		ManipulatorImg applySplit(int channel);

		ManipulatorImg applyImageAdaptativeTheresholding(int maxValue, int adaptiveMethods, int thresholdType, int blockSize, int C);

		ManipulatorImg applyImageGaussianBlur(cv::Size ksize, double sigmaX, double sigmaY);

		ManipulatorImg applyImageMorphology(int elementStruc, int morph_size, int type, int iterations);

		ManipulatorImg applyImageContrast(double alpha, double beta);

		ManipulatorImg applySuperPixelsSegmentation(int algorithm, int region_size, float ruler, int iterate);

		bool applyClassificationOldTree(cv::Scalar BGR);

		bool applyClassificationNewTree(cv::Scalar BGR);

		bool applyClassificationRefinedTree(cv::Scalar BGR);

		processingImg::ManipulatorImg applyClassificationBiofilmeAndProtese( );

		ManipulatorImg applyMetrics(processingImg::imageClass pattern);

		ManipulatorImg applyGammaCorrection(float y);

		void applyRandomShort(int size);

		ManipulatorImg applyImageSubtract(processingImg::imageClass img);

		ManipulatorImg applyImagemAdd(processingImg::imageClass img);

		ManipulatorImg applySuperPixelContour(int algorithm, int region_size, int ruler, int iterate, bool thick_line);

	    ManipulatorImg applySubtractBiofilme(imageClass img);

		ManipulatorImg applySubtractProtese(processingImg::imageClass img);

		ManipulatorImg applyImageCut();

		int  calcAreaProtese();

		int  calcAreaBiofilme();



		void calcArea();

		processingImg::imageClass build() {
			return this->img.getImg();
		}


	private:

		processingImg::imageClass img;

	};


	//********************** vector imgs **********************
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
#endif
