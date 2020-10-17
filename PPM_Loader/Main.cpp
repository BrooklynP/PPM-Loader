
#include<iostream>
#include<fstream>
#include<cstring>

#include "PPM_pixel.h"


int main(int argv, char* argc[]) {
	//get an fstring context for loading the ppm file
	std::fstream file;

	//attempt to open the file
	file.open("mario.ppm", std::ios_base::in | std::ios_base::binary);
	if (file.is_open()) 
	{
		//ppm file contains a header
		//Header consists of 
		//a magic number 'P6'
		//comment or whitespace
		//width ascii character
		//whitespace
		//height ascii character
		//whitespace
		//Max colours per channel 0-255, 256->65536
		//	if max colours is less than 256m then 1 byte per channel otherwise 2 bites per channel


		char ppmBuffer[512];
		//read in the magic number
		file.getline(ppmBuffer, 512);
		if (ppmBuffer[0] != 'p' && ppmBuffer[1] != '6') {
			std::cout << "This is not a valid ppm file (Incorrect file identifier)" << std::endl;
			file.close();
			return 1;
		}

		//test to see if theres a comment in the file.
		memset(ppmBuffer, 0, 512);
		//read in the comment
		file.getline(ppmBuffer, 512);
		if (ppmBuffer[0] == '#') {
			std::cout << "PPM file contains a comment: " << ppmBuffer[1] << std::endl;
		}

		unsigned int width = 0; unsigned int height = 0;
		file >> width;
		file >> height;

		unsigned int maxColourValue = 0;
		file >> maxColourValue;
		//work out number of pytes to read per pixel per channel
		unsigned int bytesPerChannel = (maxColourValue >= 256) ? 2 : 1;
		//there is still one whitespace character to read before the image data
		file.ignore(1);


		//now we can process the image data
		void* pixelData = nullptr;
		unsigned char pixelSize = 0;
		if (bytesPerChannel == 1) {
			pixelData = new PPMPixel<unsigned char>[width*height];
			pixelSize = sizeof(PPMPixel<unsigned char>);
		}
		else {
			pixelData = new PPMPixel<unsigned short>[width*height];
			pixelSize = sizeof(PPMPixel<unsigned short>);
		}

		file.read((char*)pixelData, width*height*pixelSize);

		//editing file
		for (unsigned int i = 0; i < width * height; i++) {
			if (bytesPerChannel == 1) {
				((PPMPixel<unsigned char>*)pixelData)[i].green = 0;
			}
			else {
				((PPMPixel<unsigned short>*)pixelData)[i].green = 0;
			}
		}
		file.close();

		file.open("noGreen.ppm", std::ios_base::out | std::ios_base::binary);
		if (file.is_open()) {
			//write out header data
			file << "P6" << std::endl;
			file << ppmBuffer << std::endl;
			file << width << " " << height << std::endl;
			file << maxColourValue << std::endl;
			file.write((const char*)pixelData, width * height * pixelSize);

			file.close();
		}
	}

	return 1;
}