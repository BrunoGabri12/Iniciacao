// Manipulator.cpp : Este arquivo cont�m a fun��o 'main'. A execu��o do programa come�a e termina ali.
//

#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/slic.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "ManipulatorImg.h"

int main()
{




     
      std::string path = "C:\\Users\\BrunoGabriel\\Desktop\\img\\total_imgs\\normal\\imagens\\superpixel\\melhoresimgs\\imagens\\arvoreNova\\";
      std::string path_segmentada = "C:\\Users\\BrunoGabriel\\Desktop\\img\\total_imgs\\normal\\imagens\\superpixel\\melhoresimgs\\segmentada_manualmente\\";
      processingImg::imgContainer vector(path);
      processingImg::imgContainer  vectorSegmentada(path_segmentada);
      processingImg::ManipulatorImg process;
      processingImg::imageClass mask;
      processingImg::imageClass segmentada;
      processingImg::imageClass ManipuledImg;
      
      

      std::vector<std::string> names = vector.returnNamesImages(); 

      mask = vector.getImgInVector(19);
      segmentada = vectorSegmentada.getImgInVector(19);
      mask.imShow();
      segmentada.imShow();
          process.setImg(mask);
         ManipuledImg = process.applyMetrics(segmentada).build();

         ManipuledImg.imWrite(path, "aval_total" + names[6+2]);
           
       
       




}
