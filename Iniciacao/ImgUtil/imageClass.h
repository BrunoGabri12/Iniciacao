#pragma once
#include<opencv2/opencv.hpp>

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

}