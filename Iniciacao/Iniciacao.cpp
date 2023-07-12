
// Manipulator.cpp : Este arquivo cont�m a fun��o 'main'. A execu��o do programa come�a e termina ali.
//

#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/slic.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "ImgUtil/imgContainer.h"
#include "ImgUtil/ManipulatorImg.h"



int main()
{


     std::string pathPattern = "C:\\Users\\BrunoGabriel\\Desktop\\IC\\img\\total_imgs\\normal\\todas_imagens\\imagens segmentadas\\";
     //std::string pathImgs = "C:\\Users\\BrunoGabriel\\Desktop\\img\\total_imgs\\normal\\todas_imagens\\superpixel\\melhoresimgs\\imagens\\v3\\imgs\\";
     std::string pathImagesWithoutSegmentation = "C:\\Users\\BrunoGabriel\\Desktop\\IC\\img\\total_imgs\\normal\\exp7\\imgs\\fotos\\";
     
     
     processingImg::imgContainer vectorImgs(pathImagesWithoutSegmentation);
     int BioFilme = 0;
     int Protese = 0;


      processingImg::ManipulatorImg processor; 


      std::vector<std::string> imagens = vectorImgs.returnNamesImages();
      processingImg::imageClass resultado;

      for (int i = 0; i < vectorImgs.sizeVector(); i++) {

          processor.setImg(vectorImgs.getImgInVector(i));
            

          //pipeline of operations for background segmentation and superpixel tecnique 
          resultado = processor.applyImageGaussianBlur(cv::Size(5, 5), 1, 1)
                .applySplit(2)
                .applyImageAdaptativeTheresholding(255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 1051, 0)
                .applyImageMorphology(cv::MORPH_ELLIPSE, 3, cv::MORPH_ERODE, 1)
                .applyImageMorphology(cv::MORPH_ELLIPSE, 2, cv::MORPH_CLOSE, 2)
                .applyImageCut()
                .applyImageSubtract(vectorImgs.getImgInVector(i))
                .applyGammaCorrection(2.75)
                .applySuperPixelsSegmentation(102, 10, 10, 5)
                .build();
          
          //tenho que retornar a imagem com o fundo segmentado
          //a imagem do biofilme pintado de vermelho 
          //a porcentagem do biofilme e da protese 


   
         resultado.imShow();

           
      }


       
       
      

}
