#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageDuplicator.h"

//#include "itksys/SystemTools.hxx"
#include <sstream>

typedef short      PixelType;
typedef unsigned int maximum,minimum;
const   unsigned int        Dimension = 3;
typedef itk::Image< PixelType, Dimension >    ImageType;
typedef itk::Image<float, 3>          FloatImageType;
typedef itk::ImageFileReader< ImageType >  ReaderType;
typedef itk::ImageFileReader< FloatImageType >  FloatReaderType;
typedef itk::ImageFileWriter< FloatImageType >  WriterType;
typedef itk::ImageFileWriter< ImageType  >  WriterTypeshort;
typedef  itk::SignedMaurerDistanceMapImageFilter< ImageType, FloatImageType  > SignedMaurerDistanceMapImageFilterType;

int main(int argc, char **argv)
{
float eps = 0.0001;

ReaderType::Pointer reader = ReaderType::New();
// FloatReaderType::Pointer reader1 = FloatReaderType::New();
const char * inputFilename  = argv[1];
const char * outputFilename = argv[2];

if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputFileImage " << std::endl;

//    return EXIT_FAILURE;
    inputFilename = "/autofs/cluster/con_003/users/mert/lukeliu/zhaolong/data/rawdata/lable01.nii.gz";
    outputFilename = "/autofs/cluster/con_003/users/mert/lukeliu/zhaolong/data/results/distancemapoutput.nii.gz";
    std::cout<<"if not, we will use data" <<inputFilename<<" as input data"<<std::endl;
    std::cout<<"and data " <<outputFilename<<" as output data"<<std::endl;

    }




reader->SetFileName( inputFilename  );

// reader1->SetFileName( inputFilename  );
reader->Update();
// reader1->Update();
ImageType::Pointer image = ImageType::New();
  
  image = reader->GetOutput();
  typedef itk::ImageDuplicator< ImageType > DuplicatorType;
  DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(image);
  duplicator->Update();
  ImageType::Pointer image_label = duplicator->GetOutput();


  FloatImageType::Pointer image_output = FloatImageType::New();
  ImageType::Pointer image2 = duplicator->GetOutput();
  

//image_label = image;

   typedef itk::CastImageFilter< ImageType, FloatImageType > CastFilterType;
   CastFilterType::Pointer castFilter = CastFilterType::New();
   castFilter->SetInput(image2);
   castFilter->Update();
   image_output = castFilter->GetOutput();

   ImageType::RegionType region = image->GetLargestPossibleRegion();
   ImageType::SizeType size = region.GetSize();

for (unsigned int x = 0; x < size[0]; x++)
  {
    for(unsigned int y = 0; y < size[1]; y++)
    {   
      for(unsigned int z = 0; z < size[2]; z++)
      {   
        ImageType::IndexType pixelIndex;
        pixelIndex[0] = x;
        pixelIndex[1] = y;
        pixelIndex[2] = z;
        image_output->SetPixel(pixelIndex,0);
       }
      }
  }

   
// image_output =reader1->GetOutput();

//get the array of labels
unsigned int array_label[100]={0};
unsigned int i=0,j,pixelValue_label;
for (unsigned int x = 0; x < size[0]; x++)
  {
    for(unsigned int y = 0; y < size[1]; y++)
    {
      for(unsigned int z = 0; z < size[2]; z++)
      {
        ImageType::IndexType pixelIndex;
        pixelIndex[0] = x;
        pixelIndex[1] = y;
        pixelIndex[2] = z;
        pixelValue_label = image->GetPixel(pixelIndex);
	unsigned int k=0;
	  if (pixelValue_label !=0)
  	    {
    		for(j=1;j<100;j++)
     		   {
       	              if( pixelValue_label == array_label[j-1])
         	         {
               		    k=k+1;
         		 }
     		    }
   		if(k==0 )
        	 {
                    array_label[i]=pixelValue_label;
                   // std::cout << array_label[i]<< std::endl;
                    i=i+1;
         	  }

            }

      }
    }
  }


// calculate sdt
unsigned int m;
for(m=0;array_label[m]>0;m++)
//for(m=0;m<=5;m++)
{
//get the single label region
  for (unsigned int x = 0; x < size[0]; x++)
  {
    for(unsigned int y = 0; y < size[1]; y++)
    {
      for(unsigned int z = 0; z < size[2]; z++)  
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = x;
      pixelIndex[1] = y;
      pixelIndex[2] = z;
     
      unsigned int pixelValue = image->GetPixel(pixelIndex);
    
      if (pixelValue == array_label[m]) 
        {
        image_label->SetPixel(pixelIndex,0);
        }
      else
        {
	image_label->SetPixel(pixelIndex,1);
	}
      }
    }
  }
 // sdt the single label region
SignedMaurerDistanceMapImageFilterType::Pointer distanceMapImageFilter = SignedMaurerDistanceMapImageFilterType::New();

distanceMapImageFilter->SetInput(image_label);
//distanceMapImageFilter->SetInsideIsPositive(true);


//  WriterType::Pointer writer = WriterType::New();
//  writer->SetFileName( "image_label.nii.gz" );
 // writer->SetInput( image_label.GetPointer() );
//  writer->Update();



//get the minimum and maximum in the single label sdm


FloatImageType::Pointer image_sdt = FloatImageType::New();
image_sdt = distanceMapImageFilter->GetOutput();// because the array_label have a lot zeros label
image_sdt->Update();



//  WriterType::Pointer writer = WriterType::New();
//  writer->SetFileName( outputFilename );
//  writer->SetInput( image_sdt.GetPointer() );
//  writer->Update();

float minimum=0, maximum, pixelValue_am;
bool flag1=true;
for (unsigned int x = 0; x < size[0]; x++)
  {
    for(unsigned int y = 0; y < size[1]; y++)
    {
      for(unsigned int z = 0; z < size[2]; z++)
      {
       
        ImageType::IndexType pixelIndex;
        pixelIndex[0] = x;
        pixelIndex[1] = y;
        pixelIndex[2] = z;
        pixelValue_am = image_sdt->GetPixel(pixelIndex);
       //  if (pixelValue_am > 0)
           if (image_label->GetPixel(pixelIndex)==0)
           {
             if(flag1)
               {
                      flag1=false;
              //        minimum = pixelValue_am;
                      maximum = pixelValue_am;
                }

             if(pixelValue_am < minimum)
                {
              //    minimum = pixelValue_am;
                }
             else if(pixelValue_am > maximum)
                {
                  maximum = pixelValue_am;
                }
           }

      }
    }
  }


// norm the sdt single region and send it to totalsignedmap
float pixelValue_sdt, pixelValue_sdt_norm,diff;
diff=(maximum-minimum)+eps;  // may be zeros, should be think again;
std::cout<< "the diff is: "<<diff<<std::endl;
std::cout<< "the minmum is: "<<minimum<<std::endl;
bool flag2; // for debug only
for (unsigned int x = 0; x < size[0]; x++)
  {
    for(unsigned int y = 0; y < size[1]; y++)
    {
      for(unsigned int z = 0; z < size[2]; z++)
      {
        ImageType::IndexType pixelIndex;
        pixelIndex[0] = x;
        pixelIndex[1] = y;
        pixelIndex[2] = z;
        pixelValue_sdt = image_sdt->GetPixel(pixelIndex);
      //  if(pixelValue_sdt >= 0 )
        if (image_label->GetPixel(pixelIndex)==0)
	{
//          if(pixelValue_sdt == 0)
//	    {
//	      image_output->SetPixel(pixelIndex,0);
//	    }
//	  else
//	    {
	      pixelValue_sdt_norm = (pixelValue_sdt-minimum)/diff;
	     image_output->SetPixel(pixelIndex,pixelValue_sdt_norm);
            //    image_output->SetPixel(pixelIndex,array_label[m]);
             //std::cout << "the value of pixelValue_Std_norm is:  "<<pixelValue_sdt_norm <<std::endl;
//	    }
	}
      }
    }
  }




}

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFilename );
  writer->SetInput( image_output.GetPointer() );
  writer->Update();





  return EXIT_SUCCESS;
}
