//============================================================================
// Name        : OCR.cpp
// Author      : Lee Glendenning, Minesh Varu
// Description : OCR
//============================================================================


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <iostream>     // std::cout
#include <sstream>
#include "bitmap_image.hpp"

using namespace std;
typedef struct Rectangle R;
int numPixels, width, height; // these global variables are initialized in the readBMP function
char* filename[] = { "0.txt", "1.txt", "2.txt", "3.txt", "4.txt", "5.txt", "6.txt", "7.txt", "8.txt", "9.txt" };
unsigned char* convertToGrey(bitmap_image image) {
	unsigned char red, green, blue;

	unsigned char* greyData = new unsigned char[numPixels]; //only allocate 1 byte per pixel
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image.get_pixel(j, i, red, green, blue);
			greyData[i*width + j] = 255 - ((red + green + blue) / 3);
		}
	}
	return greyData;
}

// converts greyscale array to binary
unsigned char* greyToBinary(unsigned char* data) {
	unsigned char* binaryData = new unsigned char[numPixels]; //only allocate 1 byte per pixel

	for (int i = 0; i < numPixels; i++) {
		if (data[i] < 127) {
			binaryData[i] = 0;
		}
		else {
			binaryData[i] = 1;
		}
	}
	return binaryData;
}

// prints grey level pixel values
void printGreyImg(unsigned char* greyData) {
	for (int i = 0; i < numPixels; i++) {
		printf("%d ", greyData[i]);
		if (i != 0 && (i + 1) % width == 0) {
			printf("\n");
		}
	}
}

// returns array of greyscale pixels
unsigned char* readBMP(char* filename) {
	bitmap_image image(filename);
	// set global variables
	width = image.width();
	height = image.height();
	numPixels = width*height;
	return convertToGrey(image);
}

// returns array representing 3x3 matrix (already flipped horizontally and vertically)
// this is vertical sobel kernel
double* getHighPassFilter() {
	double filter[9] = { -1,-1,-1,-1,8.1,-1,-1,-1,-1 };
	/*double* filter = new double[9];
	filter[0] = 0;
	filter[1] = (double)-1 / 4;
	filter[2] = 0;
	filter[3] = (double)-1 / 4;
	filter[4] = 2;
	filter[5] = (double)-1 / 4;
	filter[6] = 0;
	filter[7] = (double)-1 / 4;
	filter[8] = 0;*/
	return filter;
}

// returns array representing 3x3 matrix (already flipped horizontally and vertically)
// this is a 3x3 matrix filled with 1/9's
double* getLowPassFilter() {
	double* filter = new double[9];
	for (int i = 0; i < 9; i++) {
		filter[i] = (double)1 / 9;
	}
	return filter;
}

// prints grey level image as either 0 (white) or 1 (black)
void printImgBinary(unsigned char* greyData) {
	for (int i = 0; i < numPixels; i++) {
		if (greyData[i] < 127) {
			printf("0 ");
		}
		else {
			printf("1 ");
		}
		if (i != 0 && (i + 1) % width == 0) {
			printf("\n");
		}
	}
}

// used for testing 3x3ImgMat function
// prints a 3x3 matrix as binary given a 3x3 matrix in grey level
void printMatBinary(int* greyData) {
	for (int i = 0; i < 9; i++) {
		if (greyData[i] < 127) {
			printf("0 ");
		}
		else {
			printf("1 ");
		}
		if (i != 0 && (i + 1) % 3 == 0) {
			printf("\n");
		}
	}
	//printf("\n");
}

void printMat(int* mat) {
	for (int i = 1; i <= 9; i++) {
		printf("%d ", mat[i - 1]);
		if (i % 3 == 0) {
			printf("\n");
		}
	}
}

//given a pixel number, returns the 3x3 matrix of pixels surrounding it
//if pixel given is an edge or corner of the image, this function will return a matrix with correct 0 pading
int* get3x3ImgMat(int i, unsigned char* data) {
	int* pixelMat = new int[9];
	int WHITE = 0;

	// initialize pixelMat with -1 values
	for (int j = 0; j < 9; j++) {
		pixelMat[j] = -1;
	}

	if (i < width) { // set top row to white
					 //printf("top\n");
		pixelMat[0] = WHITE;
		pixelMat[1] = WHITE;
		pixelMat[2] = WHITE;
	}
	if (i > width*(height - 1) - 1) { // set bottom row to white
									  //printf("bot\n");
		pixelMat[6] = WHITE;
		pixelMat[7] = WHITE;
		pixelMat[8] = WHITE;
	}
	if (i % width == 0) { // set left column to white
						  //printf("left\n");
		pixelMat[0] = WHITE;
		pixelMat[3] = WHITE;
		pixelMat[6] = WHITE;
	}
	if ((i + 1) % width == 0) { // set right column to white
								//printf("right\n");
		pixelMat[2] = WHITE;
		pixelMat[5] = WHITE;
		pixelMat[8] = WHITE;
	}

	// set iterior pixels to their value
	if (pixelMat[0] == -1) {
		pixelMat[0] = data[i - width - 1];
	}
	if (pixelMat[1] == -1) {
		pixelMat[1] = data[i - width];
	}
	if (pixelMat[2] == -1) {
		pixelMat[2] = data[i - width + 1];
	}
	if (pixelMat[3] == -1) {
		pixelMat[3] = data[i - 1];
	}
	if (pixelMat[4] == -1) {
		pixelMat[4] = data[i];
	}
	if (pixelMat[5] == -1) {
		pixelMat[5] = data[i + 1];
	}
	if (pixelMat[6] == -1) {
		pixelMat[6] = data[i + width - 1];
	}
	if (pixelMat[7] == -1) {
		pixelMat[7] = data[i + width];
	}
	if (pixelMat[8] == -1) {
		pixelMat[8] = data[i + width + 1];
	}

	return pixelMat;
}

// filter: 0 - lpFilter, 1 - hpFilter
// does convolution multiplication of filter and pxMat as if they are overlapped completely. ie: returns a single value that belongs at the middle of pxMat
int filterMult(int filter, int* pxMat) {
	double* lp = getLowPassFilter();
	double* hp = getHighPassFilter();
	double result = 0;

	if (filter == 0) {
		for (int i = 0; i < 9; i++) {
			result += lp[i] * pxMat[i];
			//printf("lp[%d] * pxMat[%d] = %f * %d = %f\n",i,i,lp[i],pxMat[i],lp[i]*pxMat[i]);
		}
		//result = lp[0] * pxMat[0] + lp[1] * pxMat[1] + lp[2] * pxMat[2] + lp[3] * pxMat[3] + lp[4] * pxMat[4] + lp[5] * pxMat[5] + lp[6] * pxMat[6] + lp[7] * pxMat[7] + lp[8] * pxMat[8];
	}
	else if (filter == 1) {
		for (int i = 0; i < 9; i++) {
			result += hp[i] * pxMat[i];
			//printf("hp[%d] * pxMat[%d] = %f * %d = %f\n",i,i,hp[i],pxMat[i],hp[i]*pxMat[i]);
		}
		//result = hp[0] * pxMat[0] + hp[1] * pxMat[1] + hp[2] * pxMat[2] + hp[3] * pxMat[3] + hp[4] * pxMat[4] + hp[5] * pxMat[5] + hp[6] * pxMat[6] + hp[7] * pxMat[7] + hp[8] * pxMat[8];
	}
	else { // this should never been reached. Just put here for testing purposes
		printf("Bad filter value...");
		exit(1);
	}
	if (result < 0) {
		result = 0;
	}
	else if (result > 255) {
		result = 255;
	}
	//printf("result: %f\n",result);
	return result;
}


// filter: 0 - lpFilter, 1 - hpFilter
// For every pixel in img, create 3x3 matrix (pixel in question at center) and apply filter.
unsigned char* applyFilter(int filter, unsigned char* data) {

	unsigned char* filteredPxls = new unsigned char[numPixels];

	for (int i = 0; i < numPixels; i++) {
		//printf("filtering for %d\n",i);
		filteredPxls[i] = filterMult(filter, get3x3ImgMat(i, data));
	}
	return filteredPxls;
}

// calculates number of transitions from 0 to 1 or vice versa around the edges of a 3x3 matrix of pixels
// used for thinning
int numberOfTransitions(int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9) {

	int count = 0;

	if (p2 == 0 && p3 == 1) {
		count++;
	}
	if (p3 == 0 && p4 == 1) {
		count++;
	}
	if (p4 == 0 && p5 == 1) {
		count++;
	}
	if (p5 == 0 && p6 == 1) {
		count++;
	}
	if (p6 == 0 && p7 == 1) {
		count++;
	}
	if (p7 == 0 && p8 == 1) {
		count++;
	}
	if (p8 == 0 && p9 == 1) {
		count++;
	}
	if (p9 == 0 && p2 == 1) {
		count++;
	}
	return count;

}

// Applies ZS algorithm to thin the original image
unsigned char* doThinning(unsigned char* data) {
unsigned char* thinnedPxls = greyToBinary(data);
int p1, p2, p3, p4, p5, p6, p7, p8, p9;

for (int i = 0; i < numPixels; i++) {
	int *matrix = get3x3ImgMat(i, thinnedPxls);
	p2 = matrix[1];
	p3 = matrix[2];
	p4 = matrix[5];
	p5 = matrix[8];
	p6 = matrix[7];
	p7 = matrix[6];
	p8 = matrix[3];
	p9 = matrix[0];
	int bP1 = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;

	//printf("%d\n", bP1);
	int aP1 = numberOfTransitions(p2, p3, p4, p5, p6, p7, p8, p9);

	if ((bP1 >= 2 && bP1 <= 6) && (aP1 == 1) && (p2 * p4 *p6 == 0) && (p4 * p6 *p8 == 0)) {
	thinnedPxls[i] = 0;

	}
	if ((bP1 >= 2 && bP1 <= 6) && (aP1 == 1) && (p2 * p4 *p8 == 0) && (p2 * p6 *p8 == 0)) {
	thinnedPxls[i] = 0;

	}

}
return thinnedPxls;
}

/*bool isValidThin(int px, unsigned char* data){
int* mat = get3x3ImgMat(px, data);
for (int i = 0; i < 9; i ++){
if (mat[i]== )
}
}*/

/*typedef struct{
int pxThinned;
unsigned char* thinnedPxls;
}Thin;*/



int getZSIterationResult(int* matrix, int iteration) {
	//printMat(matrix);
	int p9 = matrix[0], p2 = matrix[1], p3 = matrix[2];
	int p8 = matrix[3], p4 = matrix[5];
	int p7 = matrix[6], p6 = matrix[7], p5 = matrix[8];

	int bP1 = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
	int aP1 = numberOfTransitions(p2, p3, p4, p5, p6, p7, p8, p9);

	if (iteration == 1 && (bP1 >= 2 && bP1 <= 6) && (aP1 == 1) && (p2 * p4 * p6 == 0) && (p4 * p6 * p8 == 0)) {
		//printf("0\n\n");
		return 0;
	}
	if (iteration == 2 && (bP1 >= 2 && bP1 <= 6) && (aP1 == 1) && (p2 * p4 *p8 == 0) && (p2 * p6 *p8 == 0)) {
		//printf("0\n\n");
		return 0;
	}
	//printf("1\n\n");
	return 1;
}

unsigned char* arrCopy(unsigned char* a) {
	unsigned char* a2 = new unsigned char[numPixels]();
	for (int i = 0; i < numPixels; i++) {
		a2[i] = a[i];
	}
	return a2;
}

unsigned char* doZSThinning(unsigned char* greyData) {
	unsigned char* thin2Data = greyToBinary(greyData); // binary representation of greyData argument. Will be updated every iteration
	unsigned char* thin1Data;
	int pxThinned;

	do {
		pxThinned = 0;
		thin1Data = arrCopy(thin2Data); // make copy of updated values. Will now change thin1Data to reflect next iteration 1 application
										//printf("\nStarting iteration 1:\n");
		for (int i = 0; i < numPixels; i++) { // loop through every pixel of binData and get result of thinning with first iteration
			if (thin2Data[i] == 1 && getZSIterationResult(get3x3ImgMat(i, thin2Data), 1) == 0) { // get result of iteration 1 on pixel i
																								 //printf("thin2Data[%d] = %d\n",i,thin2Data[i]);
				thin1Data[i] = 0;
				//printf("thin2Data[%d] = %d\n",i,thin2Data[i]);
				pxThinned++;
			}
		}
		thin2Data = arrCopy(thin1Data); // make copy of updated values. Will now change thin2Data to reflect next iteration 2 application
										//printf("\nStarting iteration 2:\n");
		for (int i = 0; i < numPixels; i++) { // loop through every pixel of binData and get result of thinning with second iteration
			if (thin1Data[i] == 1 && getZSIterationResult(get3x3ImgMat(i, thin1Data), 2) == 0) { // get result of iteration 2 on pixel i
				thin2Data[i] = 0;
				pxThinned++;
			}
		}
	} while (pxThinned > 0);

	return thin2Data;
}

/*unsigned char* doZSThinning(unsigned char* data) {
//unsigned char* thinnedPxls = greyToBinary(data);
Thin t;
t.pxThinned = 0;
t.thinnedPxls = greyToBinary(data);
//int pxThinned;
//int count = 0;
int dwidth = 0, dheight = 0;
while (dwidth*2 < width && dheight*2 < height){ // t.pxThinned > 0 &&
//count ++;
t.pxThinned = 0;

// pixels for the current layer we are trying to shave off
int x1 = dheight*width + dwidth;
int x2 = (dheight+1)*width - dwidth - 1;
int x3 = (height*width-1) - (dheight+1)*width + dwidth + 1;
int x4 = (height*width-1) - dheight*width - dwidth;

cout << "x1 = " << x1 << endl;
cout << "x2 = " << x2 << endl;
cout << "x3 = " << x3 << endl;
cout << "x4 = " << x4 << endl;

//ITERATION 1
for (int i = x1; i <= x2; i ++){ // top pxs
t = doZSIteration(i, t, 1);
}
for (int i = x3; i <= x4; i ++){ // bottom pxs
t = doZSIteration(i, t, 1);
}
for (int i = x1; i <= x3; i += width){ // left pxs
t = doZSIteration(i, t, 1);
}
for (int i = x2; i <= x4; i += width){ // right pxs
t = doZSIteration(i, t, 1);
}

//ITERATION 2
for (int i = x1; i <= x2; i ++){ // top pxs
t = doZSIteration(i, t, 2);
}
for (int i = x3; i <= x4; i ++){ // bottom pxs
t = doZSIteration(i, t, 2);
}
for (int i = x1; i <= x3; i += width){ // left pxs
t = doZSIteration(i, t, 2);
}
for (int i = x2; i <= x4; i += width){ // right pxs
t = doZSIteration(i, t, 2);
}

dwidth ++;
dheight ++;
}
return t.thinnedPxls;
}*/

struct Rectangle
{
	int minX;
	int minY;
	int maxX;
	int maxY;
};

R getMinRectangle(unsigned char* greyData) {
	// initialize minRect values as worst possible case
	R minRect = {
		width,
		height,
		0,
		0
	};

	// update minRect values to actual
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (greyData[i*width + j] != 0) { // black pixel
				if (j < minRect.minX) { // update minX value with current j value
					minRect.minX = j;
				}
				if (i < minRect.minY) { // update minY value with current i value
					minRect.minY = i;
				}
				if (j > minRect.maxX) { // update maxX value with current j value
					minRect.maxX = j;
				}
				if (i > minRect.maxY) { // update maxY value with current i value
					minRect.maxY = i;
				}
			}
		}
	}
	return minRect;
}

void printFeatureVector(double featVec[]) {
	printf("[");
	for (int i = 0; i < 16; i++) {
		printf("%f", featVec[i]); // zone i
		if (i != 15) {
			printf(", ");
		}
	}
	printf("]\n");
}

std::vector<R> getZones(unsigned char* greyData) {
	R minRect = getMinRectangle(greyData);
	std::vector<R> zones;
	int minX = minRect.minX;
	int minY = minRect.minY;
	int maxX = minRect.maxX;
	int maxY = minRect.maxY;
	int zoneWidth = (minRect.maxX - minRect.minX) / 4;
	int zoneHeight = (minRect.maxY - minRect.minY) / 4;

	// row 1
	zones.push_back({ minX, minY, minX + zoneWidth, minY + zoneHeight }); // zone 1
	zones.push_back({ minX + zoneWidth, minY, minX + 2 * zoneWidth, minY + zoneHeight }); // zone 2
	zones.push_back({ minX + 2 * zoneWidth, minY, minX + 3 * zoneWidth, minY + zoneHeight }); // zone 3
	zones.push_back({ minX + 3 * zoneWidth, minY, maxX, minY + zoneHeight }); // zone 4
																			  // row 2
	zones.push_back({ minX, minY + zoneHeight, minX + zoneWidth, minY + 2 * zoneHeight }); // zone 5
	zones.push_back({ minX + zoneWidth, minY + zoneHeight, minX + 2 * zoneWidth, minY + 2 * zoneHeight }); // zone 6
	zones.push_back({ minX + 2 * zoneWidth, minY + zoneHeight, minX + 3 * zoneWidth, minY + 2 * zoneHeight }); // zone 7
	zones.push_back({ minX + 3 * zoneWidth, minY + zoneHeight, maxX, minY + 2 * zoneHeight }); // zone 8
																							   // row 3
	zones.push_back({ minX, minY + 2 * zoneHeight, minX + zoneWidth, minY + 3 * zoneHeight }); // zone 9
	zones.push_back({ minX + zoneWidth, minY + 2 * zoneHeight, minX + 2 * zoneWidth, minY + 3 * zoneHeight }); // zone 10
	zones.push_back({ minX + 2 * zoneWidth, minY + 2 * zoneHeight, minX + 3 * zoneWidth, minY + 3 * zoneHeight }); // zone 11
	zones.push_back({ minX + 3 * zoneWidth, minY + 2 * zoneHeight, maxX, minY + 3 * zoneHeight }); // zone 12
																								   // row 4
	zones.push_back({ minX, minY + 3 * zoneHeight, minX + zoneWidth, maxY }); // zone 13
	zones.push_back({ minX + zoneWidth, minY + 3 * zoneHeight, minX + 2 * zoneWidth, maxY }); // zone 14
	zones.push_back({ minX + 2 * zoneWidth, minY + 3 * zoneHeight, minX + 3 * zoneWidth, maxY }); // zone 15
	zones.push_back({ minX + 3 * zoneWidth, minY + 3 * zoneHeight, maxX, maxY }); // zone 16

	return zones;
}

// gets percentage of black pixels in each of 16 zones (of smallest containing rectangle of digit)
double* getZoningFeatures(unsigned char* greyData) {
	std::vector<R> zones = getZones(greyData);
	unsigned char* binData = greyToBinary(greyData);
	double* zoning = new double[16];

	for (int i = 0; i < 16; i++) {
		int whiteCount = 0;
		int blackCount = 0;
		for (int j = zones.at(i).minY; j < zones.at(i).maxY; j++) {
			for (int k = zones.at(i).minX; k < zones.at(i).maxX; k++) {
				if (binData[j*width + k] == 0) { // white pixel
					whiteCount++;
				}
				else { // black pixel
					blackCount++;
				}
			}
		}
		zoning[i] = (double)blackCount / (blackCount + whiteCount); // percentage of black pixels in zone i
	}
	//printFeatureVector(zoning);
	return zoning;
}

// returns vector of length 32. first 16 are x zones, last 16 are y
std::vector<R> getHistogramZones(unsigned char* greyData) {
	R minRect = getMinRectangle(greyData);
	std::vector<R> zones;
	int minX = minRect.minX;
	int minY = minRect.minY;
	int maxX = minRect.maxX;
	int maxY = minRect.maxY;
	int zoneWidth = (minRect.maxX - minRect.minX) / 16;
	int zoneHeight = (minRect.maxY - minRect.minY) / 16;

	// x
	for (int i = 0; i < 16; i++) {
		if (i != 15) {
			zones.push_back({ minX + i*zoneWidth, minY, minX + (i + 1)*zoneWidth, maxY });
		}
		else {
			zones.push_back({ minX + i*zoneWidth, minY, maxX, maxY });
		}
	}

	// y
	for (int i = 0; i < 16; i++) {
		if (i != 15) {
			zones.push_back({ minX, minY + i*zoneHeight, maxX, minY + (i + 1)*zoneHeight });
		}
		else {
			zones.push_back({ minX, minY + i*zoneHeight, maxX, maxY });
		}
	}

	return zones;
}

// gets ratio of histograms: ratio of the difference between the horizontal and vertical projections [i] to their summation
// uses 256 (16x16) zones
double* getHistogramFeatures(unsigned char* greyData) {
	std::vector<R> zones = getHistogramZones(greyData);
	unsigned char* binData = greyToBinary(greyData);
	//int* histoTemp = new int[32] {0};
	int histoTemp[32] = { 0 };
	double* histo = new double[16];

	for (int i = 0; i < 32; i++) {
		for (int j = zones.at(i).minY; j < zones.at(i).maxY; j++) {
			for (int k = zones.at(i).minX; k < zones.at(i).maxX; k++) {
				if (binData[j*width + k] == 1) { // black pixel
					histoTemp[i] ++;
				}
			}
		}
	}

	for (int i = 0; i < 16; i++) {
		histo[i] = (double)(abs(histoTemp[i] - histoTemp[16 + i])) / (histoTemp[i] + histoTemp[16 + i]);
	}
	//printFeatureVector(histo);
	return histo;
}

unsigned char* binaryToGrey(unsigned char* binData) {
	unsigned char* greyData = new unsigned char[numPixels]; //only allocate 1 byte per pixel

	for (int i = 0; i < numPixels; i++) {
		if (binData[i] == 1) { // black
			greyData[i] = 255;
		}
		else {
			greyData[i] = 0;
		}
	}
	return greyData;
}

void outputImage(unsigned char* greyData, char* filename, bool isThinning) {
	//unsigned char* rgbData = greyToRGB(greyData);
	bitmap_image image(width, height);
	unsigned char px;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!isThinning) {
				px = greyData[i*width + j];
			}
			else {
				px = 255 - greyData[i*width + j];
			}
			image.set_pixel(j, i, px, px, px);
		}
	}
	image.save_image(filename);
}

typedef struct {
	int digit;
	double data[16];
}Feature;
//get data from all files and store in single struct
vector<Feature> getDataFromFile(char * name[], double *data) {
	fstream f;
	string line, word;

	double x, y, distance, min;
	Feature fileFeature;
	vector<Feature> list;
	string delimiter = ",";
	char *path= "storage";
	int count = 0;
	for (int i = 0; i < 10; i++) { // open each #.txt
		f.open(name[i], f.in);
		for (int j = 0; getline(f, line); j++) {
			stringstream iss;
			iss << line;
			for (int k = 0; getline(iss, word, ','); k++) {
				if (count == 15) {
					fileFeature.digit = i;
					fileFeature.data[count] = std::strtod(word.c_str(), NULL);
					//cout << i << ": " << "pushing back " << word << endl;
					list.push_back(fileFeature);
					count = 0;
				}
				else {
					fileFeature.data[count] = std::strtod(word.c_str(), NULL);
					count++;
				}

			}


		}

		f.close();

	}
	//cout << "list size: " << list.size() << endl;
	return list;
}
typedef struct {
	int digit;
	double distance;
}Dmin;
double getAvg(vector<Dmin> data) {

	//initialize variables
	double davg[10] = { 0 };
	int n[10] = { 0 };
	double avg = 0;
	int digit = 0;
	//loop through data vector
	for (int i = 0; i < data.size(); i++) {
		//if data i is a digit 0 then add to davg as well as increment count for number of 0's
		if (data.at(i).digit == 0) {
			davg[0] += data.at(i).distance;
			n[0]++;
		}
		//if data i is a digit 1 then add to davg as well as increment count for number of 1's
		else if (data.at(i).digit == 1) {
			davg[1] += data.at(i).distance;
			n[1]++;
		}
		//if data i is a digit 2 then add to davg as well as increment count for number of 2's
		else if (data.at(i).digit == 2) {
			davg[2] += data.at(i).distance;
			n[2]++;
		}
		//if data i is a digit 3 then add to davg as well as increment count for number of 3's
		else if (data.at(i).digit == 3) {
			davg[3] += data.at(3).distance;
			n[3]++;
		}
		//if data i is a digit 4 then add to davg as well as increment count for number of 4's
		else if (data.at(i).digit == 4) {
			davg[4] += data.at(i).distance;
			n[4]++;
		}
		//if data i is a digit 5 then add to davg as well as increment count for number of 5's
		else if (data.at(i).digit == 5) {
			davg[5] += data.at(i).distance;
			n[5]++;
		}
		//if data i is a digit 6 then add to davg as well as increment count for number of 6's
		else if (data.at(i).digit == 6) {
			davg[6] += data.at(i).distance;
			n[6]++;
		}
		//if data i is a digit 7 then add to davg as well as increment count for number of 7's
		else if (data.at(i).digit == 7) {
			davg[7] += data.at(i).distance;
			n[7]++;
		}
		//if data i is a digit 8 then add to davg as well as increment count for number of 8's
		else if (data.at(i).digit == 8) {
			davg[8] += data.at(i).distance;
			n[8]++;
		}
		//if data i is a digit 9 then add to davg as well as increment count for number of 9's
		else if (data.at(i).digit == 9) {
			davg[9] += data.at(i).distance;
			n[9]++;
		}
	}
	for (int i = 0; i < 10; i++) {
		//Take the avg of each number and divide it by its count i.e davg[0] = davg[0] / n[0]
		davg[i] = davg[i] / n[i];
	}
	avg = davg[0];
	//Find the max average and get its digit
	for (int i = 0; i < 10; i++) {
		if (avg > davg[i]) {
			avg = davg[i];
			digit = i;
		}
	}

	return digit; // return the digit with highest average
}
double getMin(vector<Dmin> data) {
	//cout << data.size();
	double min = data.at(0).distance;
	double result = 0;

	for (int i = 1; i < data.size(); i++) {
		if (data.at(i).distance < min) {
			min = data.at(i).distance;
			result = data.at(i).digit;
		}
	}
	return result;
}

string itos(int n) {
	stringstream ss;
	ss << n;
	return ss.str();
}

char* stocs(string s) {
	char *cs = new char[s.length() + 1];
	strcpy(cs, s.c_str());
	return cs;
}

void writeToFile(char* name, double *data) {
	FILE* f = fopen(name, "a");
	for (int j = 0; j < 16; j++) {

		fprintf(f, "%f", data[j]);

		if (j != 15) {
			fprintf(f, ",");
		}
		else {
			fprintf(f, "\n");
		}
	}
	fclose(f);
}

// train with n files
/*void train(int n) {
	string prefixes[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= n; j++) {
			string temp = "training set/" + prefixes[i] + itos(j) + ".bmp";
			char *filename = stocs(temp);
			//cout << "Writing " << filename;
			unsigned char* greyData = readBMP(filename);
			double *data = getZoningFeatures(greyData);

			temp = "storage/" + itos(i) + ".txt";
			char *toWrite = stocs(temp);
			//cout << " to " << toWrite << endl;
			writeToFile(toWrite, data);
		}
	}

}*/
void thinAllImages(int n) {
string prefixes[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= n; j++) {
			string temp = "training set/" + prefixes[i] + itos(j) + ".bmp";
			char *filename = stocs(temp);
			

			unsigned char* greyData = readBMP(filename);
			unsigned char* thinData = doZSThinning(greyData); // thinning

			outputImage(binaryToGrey(thinData), stocs("thin_train/" +prefixes[i] +  "thin" + itos(j) + ".bmp"), true);
		}
	}
}
void blurAllImages(int n) {
	string prefixes[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= n; j++) {
			string temp = "test/" + prefixes[i] + itos(j) + ".bmp";
			char *filename = stocs(temp);


			unsigned char* greyData = readBMP(filename);
			unsigned char* blurredData = applyFilter(0, greyData); // low pass filter


			outputImage(binaryToGrey(blurredData), stocs("blur/" + prefixes[i] + "blur" + itos(j) + ".bmp"), true);
		}
	}
}
void train(int n) {
	string prefixes[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	for (int i = 0; i < 10; i++) {
		for (int j = 1; j <= n; j++) {
			string temp = "thin_train/" + prefixes[i] + "thin" + itos(j) + ".bmp";
			char *filename = stocs(temp);
			//cout << "Writing " << filename;
			unsigned char* greyData = readBMP(filename);
			double *data = getZoningFeatures(greyData);

			temp = "storage/" + itos(i) + ".txt";
			char *toWrite = stocs(temp);
			//cout << " to " << toWrite << endl;
			writeToFile(toWrite, data);
		}
	}

}

/*double doRecognition(unsigned char* greyData) {
double* zoneData = getZoningFeatures(greyData);

vector<Feature> fileData = getDataFromFile(filename, zoneData);
Dmin dmin;
double distance = 0, result;
vector<Dmin> dminList;
cout << fileData.size() << endl;
for (int i = 0; i < fileData.size(); i++) {
for (int j = 0; j < 16 ; j++) {

distance += pow((fileData.at(i).data[j] - zoneData[j]),2);
}
dmin.digit = fileData.at(i).digit;
dmin.distance = sqrt(distance);
dminList.push_back(dmin);
distance = 0;
}
result = getMin(dminList);

return result;
}*/
double doRecognition(unsigned char* greyData) {
	double* zoneData = getZoningFeatures(greyData);
	double *histData = getHistogramFeatures(greyData);
	vector<Feature> fileData = getDataFromFile(filename, zoneData);
	Dmin dmin, hdmin;
	double distance = 0, result, histoResult, distanceH = 0;
	vector<Dmin> dminList, hdminList;
	//cout << fileData.size() << endl;
	for (int i = 0; i < fileData.size(); i++) {
		for (int j = 0; j < 16; j++) {

			distance += pow((fileData.at(i).data[j] - zoneData[j]), 2);
			//distanceH += pow(fileData.at(i).data[j] - histData[j], 2);
		}
		dmin.digit = fileData.at(i).digit;
		//hdmin.digit = fileData.at(i).digit;
		dmin.distance = sqrt(distance); // average the points
		//hdmin.distance = sqrt(distanceH);
		dminList.push_back(dmin);
		hdminList.push_back(hdmin);
		distance = 0;
		//distanceH = 0;
	}
	
	result = getMin(dminList);
	//histoResult = getMin(hdminList);
	//cout << "The histogram result: " << histoResult << endl;
	//result = getAvg(dminList);
	return result;
}

void applyLowPassFilter(unsigned char* greyData) {
	unsigned char* blurredData = applyFilter(0, greyData); // low pass filter
	outputImage(blurredData, "blur.bmp", true);
	printf("Output: blur.bmp\n");
	//printGreyImg(blurredData);
}

void applyHighPassFilter(unsigned char* greyData) {
	unsigned char* edgeData = applyFilter(1, greyData); // high pass filter
	outputImage(edgeData, "edge.bmp", true);
	printf("Output: edge.bmp\n");
	//printGreyImg(edgeData);
}

void applyThinning(unsigned char* greyData) {
	unsigned char* thinData = doZSThinning(greyData); // thinning
	outputImage(binaryToGrey(thinData), "thin_ZS.bmp", true);
	//thinData = doThinning(greyData); // thinning
	//outputImage(binaryToGrey(thinData), "thin_linear.bmp", false);
	printf("Output: thin.bmp\n");
	//printGreyImg(binaryToGrey(thinData));
}

void displayMenu() {
	char filename[200];
	cout << "Enter file name (bmp only): \n";
	cin.getline(filename, sizeof filename);
	unsigned char* greyData = readBMP(filename); // greyData is an array containing all pixels in the image in grey level. 255 is black, 0 is white

	string input = "";
	while (input != "q") {
		cout << "1 - Apply high pass filter\n";
		cout << "2 - Apply low pass filter\n";
		cout << "3 - Apply thinning\n";
		cout << "4 - Recognize image\n";
		cout << "5 - Import new image\n";
		cout << "6 - Train\n";
		cout << "q - exit program\n";
		cout << "What would you like to do: \n";
		getline(cin, input);

		int number;
		switch (atoi(input.c_str())) {
		case 1:
			applyHighPassFilter(greyData);
			break;
		case 2:
			applyLowPassFilter(greyData);
			break;
		case 3:
			applyThinning(greyData);
			break;
		case 4:
			number = doRecognition(greyData);
			cout << "The Zoning result is: " << number << endl;
			break;
		case 5:
			cout << "Enter file name (bmp only): \n";
			cin.getline(filename, sizeof filename);
			greyData = readBMP(filename);
			break;
		case 6:
			cout << "Enter number of files for each number: \n";
			getline(cin, input);
			train(atoi(input.c_str()));
			break;

		case 7:
			cout << "Enter number of files for each number: \n";
			getline(cin, input);
			thinAllImages(atoi(input.c_str()));
			break;
		
		case 8:
			cout << "Enter number of files for each number: \n";
			getline(cin, input);
			blurAllImages(atoi(input.c_str()));
			break;

		}
		printf("\n\n");
		input = "";
	}
}

int main() {
	displayMenu();
	return 0;
}

