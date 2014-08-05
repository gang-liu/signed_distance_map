#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkImageFileWriter.h"
#include "itksys/SystemTools.hxx"
#include <sstream>
#include <boost/timer.hpp>
//#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
typedef itk::Image<float, 2>          FloatImageType;
typedef itk::Image<unsigned char, 2>  ImageType; 
static void CreateImage1(UnsignedCharImageType::Pointer image);
static void CreateImage2(UnsignedCharImageType::Pointer image);


int main(int argc, char * argv[])
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  if (argc < 2)
    {
   boost::timer timer;

    CreateImage1(image);
   std::cout<<"the iteration elapsed time is: "<< timer.elapsed()<<std::endl;

   
   timer.restart();

    CreateImage2(image);
    std::cout<<"the getpixel elapsed time is: "<< timer.elapsed()<<std::endl;


    }
 
}
void CreateImage2(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(200);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();
  for (unsigned int x = 0; x < 200; x++)
  {
    for(unsigned int y = 0; y < 200; y++)
    {
   //   for(unsigned int z = 0; z < 255; z++)
     // {

        ImageType::IndexType pixelIndex;
        pixelIndex[0] = x;
        pixelIndex[1] = y;
 //       pixelIndex[2] = z;
          for(unsigned int i = 1; i<1000; i++)
            {
          image->SetPixel(pixelIndex,255);
             }
     }
   }

}
void CreateImage1(ImageType::Pointer image)
{
ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(200);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());


   while(!imageIterator.IsAtEnd())
   {
//   if((imageIterator.GetIndex()[0] > 5 && imageIterator.GetIndex()[0] < 10) &&
//   (imageIterator.GetIndex()[1] > 5 && imageIterator.GetIndex()[1] < 10) )
  //     {
          for(unsigned int i = 1; i<1000; i++)
            {

           imageIterator.Set(255);
}           //imageIterator.Get()
   //       unsigned char val = imageIterator.Get();
   //        std::cout << (int)val << std::endl;

    //   }
//   else
  //    {
     //     imageIterator.Set(0);
    //  }
      ++imageIterator;
   }
}






