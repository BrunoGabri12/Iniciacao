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
     std::string pathPattern= "C:\\Users\\BrunoGabriel\\Desktop\\img\\total_imgs\\normal\\todas_imagens\\superpixel\\melhoresimgs\\imagens\\v2\\resultado\\";
     processingImg::imgContainer vectorImgs(pathPattern);
  



      processingImg::ManipulatorImg processor; 
      int BioFilme;
      int Protese;

      std::vector<std::string> imagens = vectorImgs.returnNamesImages();

      for (int i = 0; i < vectorImgs.sizeVector(); i++) {

          processor.setImg(vectorImgs.getImgInVector(i));


          BioFilme= processor.calcAreaBiofilme();
          Protese = processor.calcAreaProtese();

        //  segmentada = processor.applyClassificationBiofilmeAndProtese().applyMetrics(vectorn.getImgInVector(i)).build();
          
         // segmentada = processor.applyMetrics(vectorPattern.getImgInVector(i)).build();
          //segmentada.imShow();
         // vector.getImgInVector(i).imShow();
          //vectorPattern.getImgInVector(i).imShow();

         
          //vectorImgs.getImgInVector(i).imShow();

         
          std::cout  << Protese <<"," << BioFilme << std::endl;
    
      }


       
       
      //Preciso contabilizar a quantidade de pixels de biofilme e de prótese e fazer os cálculos na mão das métricas 

      

}
