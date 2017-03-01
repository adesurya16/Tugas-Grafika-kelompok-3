#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <termios.h>
#include <math.h>
#include <thread>
#include <vector>
#include "Object.h"


using namespace std;

#define HEIGHT 650 //HEIGHT jangan panjang panjang kan ada 2 layar satu untuk mini satu zoom in/out
#define WIDTH 650

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *fbp = 0;

//matriks mini map
int redPixelMatrix[WIDTH][HEIGHT];
int greenPixelMatrix[WIDTH][HEIGHT];
int bluePixelMatrix[WIDTH][HEIGHT];

//matriks zoom map
int redPixelMatrixZoomMap[WIDTH][HEIGHT];
int greenPixelMatrixZoomMap[WIDTH][HEIGHT];
int bluePixelMatrixZoomMap[WIDTH][HEIGHT];


int posX;
int posY;
int lastCorrectState = 's';
bool exploded = false;

void clearMatrix() {
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			redPixelMatrix[i][j] = 0;
			greenPixelMatrix[i][j] = 0;
			bluePixelMatrix[i][j] = 0;
		}
	}

	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			redPixelMatrixZoomMap[i][j] = 0;
			greenPixelMatrixZoomMap[i][j] = 0;
			bluePixelMatrixZoomMap[i][j] = 0;
		}
	}
}

void drawWhitePoint2(int x1, int y1) {
	redPixelMatrixZoomMap[x1][y1] = 255;
	greenPixelMatrixZoomMap[x1][y1] = 255;
	bluePixelMatrixZoomMap[x1][y1] = 255;
}

void drawWhitePoint(int x1, int y1) {
    if (x1 < WIDTH && x1 >= 0 && y1 >=0 && y1 < HEIGHT) {
        redPixelMatrix[x1][y1] = 255;
        greenPixelMatrix[x1][y1] = 255;
        bluePixelMatrix[x1][y1] = 255;
    }
}

void drawRedPoint(int x1,int y1){
	redPixelMatrix[x1][y1] = 255;
	greenPixelMatrix[x1][y1] = 0;
	bluePixelMatrix[x1][y1] = 0;
}

void drawBlackPoint(int x1,int y1){
	redPixelMatrix[x1][y1] = 0;
	greenPixelMatrix[x1][y1] = 0;
	bluePixelMatrix[x1][y1] = 0;
}

bool drawWhiteLine(int x1, int y1, int x2, int y2) {
	//Than kode lu gua benerin dikit di sini, harusnya ngk usah pake absolut
	bool ret = false;

	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int ix = deltaX > 0 ? 1 : -1;
	int iy = deltaY > 0 ? 1 : -1;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);

	int x = x1;
	int y = y1;

	drawWhitePoint(x,y);

	if (deltaX >= deltaY) {
		int error = 2 * deltaY - deltaX;

		while (x != x2) {
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= deltaX;
				y += iy;
			}

			error += deltaY;
			x += ix;


			if (redPixelMatrix[x][y] == 255 && greenPixelMatrix[x][y] == 255 && bluePixelMatrix[x][y] == 255) {
				ret = true;
			}
			drawWhitePoint(x, y);
		}
	} else {
		int error = 2 * deltaX - deltaY;

		while (y != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= deltaY;
				x += ix;
			}

			error += deltaX;
			y += iy;


			if (redPixelMatrix[x][y] == 255 && greenPixelMatrix[x][y] == 255 && bluePixelMatrix[x][y] == 255) {
				ret = true;
			}
			drawWhitePoint(x, y);
		}
	}
	return ret;
}

bool drawWhiteLine2(int x1, int y1, int x2, int y2) {
	//Than kode lu gua benerin dikit di sini, harusnya ngk usah pake absolut
	bool ret = false;

	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int ix = deltaX > 0 ? 1 : -1;
	int iy = deltaY > 0 ? 1 : -1;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);

	int x = x1;
	int y = y1;

	drawWhitePoint2(x,y);

	if (deltaX >= deltaY) {
		int error = 2 * deltaY - deltaX;

		while (x != x2) {
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= deltaX;
				y += iy;
			}

			error += deltaY;
			x += ix;


			if (redPixelMatrixZoomMap[x][y] == 255 && greenPixelMatrixZoomMap[x][y] == 255 && bluePixelMatrixZoomMap[x][y] == 255) {
				ret = true;
			}
			drawWhitePoint2(x, y);
		}
	} else {
		int error = 2 * deltaX - deltaY;

		while (y != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= deltaY;
				x += ix;
			}

			error += deltaX;
			y += iy;


			if (redPixelMatrixZoomMap[x][y] == 255 && greenPixelMatrixZoomMap[x][y] == 255 && bluePixelMatrixZoomMap[x][y] == 255) {
				ret = true;
			}
			drawWhitePoint2(x, y);
		}
	}
	return ret;
}

void drawBlackLine(int x1, int y1, int x2, int y2) {
    //Than kode lu gua benerin dikit di sini, harusnya ngk usah pake absolut
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int ix = deltaX > 0 ? 1 : -1;
	int iy = deltaY > 0 ? 1 : -1;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);

	int x = x1;
	int y = y1;

	drawBlackPoint(x,y);

	if (deltaX >= deltaY) {
		int error = 2 * deltaY - deltaX;

		while (x != x2) {
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= deltaX;
				y += iy;
			}

			error += deltaY;
			x += ix;

			drawBlackPoint(x, y);
		}
	} else {
		int error = 2 * deltaX - deltaY;

		while (y != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= deltaY;
				x += ix;
			}

			error += deltaX;
			y += iy;

			drawBlackPoint(x, y);
		}
	}
}

void drawRedLine(int x1, int y1, int x2, int y2) {
    //Than kode lu gua benerin dikit di sini, harusnya ngk usah pake absolut
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int ix = deltaX > 0 ? 1 : -1;
	int iy = deltaY > 0 ? 1 : -1;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);

	int x = x1;
	int y = y1;

	drawRedPoint(x,y);

	if (deltaX >= deltaY) {
		int error = 2 * deltaY - deltaX;

		while (x != x2) {
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= deltaX;
				y += iy;
			}

			error += deltaY;
			x += ix;

			drawRedPoint(x, y);
		}
	} else {
		int error = 2 * deltaX - deltaY;

		while (y != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= deltaY;
				x += ix;
			}

			error += deltaX;
			y += iy;

			drawRedPoint(x, y);
		}
	}
}

void drawWhitePointZoom(int x1, int y1) {
    if (x1 < WIDTH && x1 > 0 && y1 > 0 && y1 < HEIGHT) {
        redPixelMatrixZoomMap[x1][y1] = 255;
        greenPixelMatrixZoomMap[x1][y1] = 255;
        bluePixelMatrixZoomMap[x1][y1] = 255;
    }
}

bool drawWhiteLineZoom(int x1, int y1, int x2, int y2) {
	//Than kode lu gua benerin dikit di sini, harusnya ngk usah pake absolut
	bool ret = false;

	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int ix = deltaX > 0 ? 1 : -1;
	int iy = deltaY > 0 ? 1 : -1;
	deltaX = abs(deltaX);
	deltaY = abs(deltaY);

	int x = x1;
	int y = y1;

	drawWhitePointZoom(x,y);

	if (deltaX >= deltaY) {
		int error = 2 * deltaY - deltaX;

		while (x != x2) {
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= deltaX;
				y += iy;
			}

			error += deltaY;
			x += ix;


			if (redPixelMatrixZoomMap[x][y] == 255 && greenPixelMatrixZoomMap[x][y] == 255 && bluePixelMatrixZoomMap[x][y] == 255) {
				ret = true;
			}
			drawWhitePointZoom(x, y);
		}
	} else {
		int error = 2 * deltaX - deltaY;

		while (y != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= deltaY;
				x += ix;
			}

			error += deltaX;
			y += iy;


			if (redPixelMatrixZoomMap[x][y] == 255 && greenPixelMatrixZoomMap[x][y] == 255 && bluePixelMatrixZoomMap[x][y] == 255) {
				ret = true;
			}
			drawWhitePointZoom(x, y);
		}
	}
	return ret;
}
//List of Object
Object *listObject;
int nObject = 2; /////////////////////////////////////////////////////////////////////JANGAN LUPA UPDATE
void makeWorld() {

    int pointSize;
    Point *listPoint;
    listObject = (Object*) malloc(nObject*sizeof(Object));
    int idxObject = 0;

    //Diulang-ulang
    //////////////////////////////////////////////////////////////////////////////////////////
    pointSize = 8;
    listPoint =  (Point*)malloc(pointSize*sizeof(Point));
    int xKubus[8], yKubus[8];
	xKubus[0] = 156; yKubus[0] = 414;
	xKubus[1] = 159; yKubus[1] = 406;
	xKubus[2] = 166; yKubus[2] = 400;
	xKubus[3] = 167; yKubus[3] = 382;
	xKubus[4] = 194; yKubus[4] = 382;
	xKubus[5] = 195; yKubus[5] = 298;
	xKubus[6] = 201; yKubus[6] = 407;
	xKubus[7] = 204; yKubus[7] = 414;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xKubus[i];
        listPoint[i].y = yKubus[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
	//////////////////////////////////////////////////////////////////////////////////////////
    pointSize = 4;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xKotakKiri[4], yKotakKiri[4];
	xKotakKiri[0] = 75; yKotakKiri[0] = 415;
	xKotakKiri[1] = 71; yKotakKiri[1] = 406;
	xKotakKiri[2] = 100; yKotakKiri[2] = 396;
	xKotakKiri[3] = 103; yKotakKiri[3] = 405;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xKotakKiri[i];
        listPoint[i].y = yKotakKiri[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
    //////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 14;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xTekSipil[14], yTekSipil[14];
	xTekSipil[0] = 62; yTekSipil[0] = 272;
	xTekSipil[1] = 86; yTekSipil[1] = 272;
	xTekSipil[2] = 86; yTekSipil[2] = 281;
	xTekSipil[3] = 74; yTekSipil[3] = 281;
	xTekSipil[4] = 74; yTekSipil[4] = 285;
	xTekSipil[5] = 111; yTekSipil[5] = 285;
	xTekSipil[6] = 111; yTekSipil[6] = 280;
	xTekSipil[7] = 125; yTekSipil[7] = 280;
	xTekSipil[8] = 125; yTekSipil[8] = 308;
	xTekSipil[9] = 70; yTekSipil[9] = 308;
	xTekSipil[10] = 70; yTekSipil[10] = 299;
	xTekSipil[11] = 98; yTekSipil[11] = 299;
	xTekSipil[12] = 98; yTekSipil[12] = 294;
	xTekSipil[13] = 62; yTekSipil[13] = 294;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xTekSipil[i];
        listPoint[i].y = yTekSipil[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
	//////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 8;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xHimafi[8], yHimafi[8];
	xHimafi[0] = 60; yHimafi[0] 233;
	xHimafi[1] = 125; yHimafi[1] 233;
	xHimafi[2] = 125; yHimafi[2] 277;
	xHimafi[3] = 89; yHimafi[3] 277;
	xHimafi[4] = 89; yHimafi[4] 269;
	xHimafi[5] = 118; yHimafi[5] 269;
	xHimafi[6] = 118; yHimafi[6] 266;
	xHimafi[7] = 60; yHimafi[7] 266;
	
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xHimafi[i];
        listPoint[i].y = yHimafi[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
    //////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 4;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xA[4], yA[4];
	xA[0] = 30; yA[0] = 111;
	xA[1] = 78; yA[1] = 111;
	xA[2] = 78; yA[2] = 138;
	xA[3] = 30; yA[3] = 138;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xA[i];
        listPoint[i].y = yA[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
	//////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 4;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xB[4], yB[4];
	xB[0] = 30; yB[0] = 140;
	xB[1] = 55; yB[1] = 140;
	xB[2] = 55; yB[2] = 153;
	xB[3] = 30; yB[3] = 153;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xB[i];
        listPoint[i].y = yB[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
	//////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 12;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xC[12], yC[12];
	xC[0] = 30; yC[0] = 155;
	xC[1] = 55; yC[1] = 155;
	xC[2] = 55; yC[2] = 158;
	xC[3] = 65; yC[3] = 158;
	xC[4] = 65; yC[4] = 153;
	xC[5] = 75; yC[5] = 153;
	xC[6] = 75; yC[6] = 169;
	xC[7] = 65; yC[7] = 169;
	xC[8] = 65; yC[8] = 166;
	xC[9] = 55; yC[9] = 166;
	xC[10] = 55; yC[10] = 169;
	xC[11] = 30; yC[11] = 169;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xC[i];
        listPoint[i].y = yC[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
	//////////////////////////////////////////////////////////////////////////////////////////
	pointSize = 12;
    listPoint = (Point*)malloc(pointSize*sizeof(Point));
    int xD[11], yD[11];
	xD[0] = 46; yD[0] = 94;
	xD[1] = 71; yD[1] = 94;
	xD[2] = 71; yD[2] = 99;
	xD[3] = 73; yD[3] = 99;
	xD[4] = 73; yD[4] = 103;
	xD[5] = 71; yD[5] = 103;
	xD[6] = 71; yD[6] = 108;
	xD[7] = 47; yD[7] = 108;
	xD[8] = 46; yD[8] = 99;
	xD[9] = 33; yD[9] = 99;
	xD[10] = 33; yD[10] = 97;
	for(int i = 0; i < pointSize; i++) {
        listPoint[i].x = xD[i];
        listPoint[i].y = yD[i];
	}
	listObject[idxObject] = Object();
	listObject[idxObject].buildPolygon(listPoint, pointSize);
	idxObject++;
	free(listPoint);
}

void drawAllObject() {
    for(int i = 0; i < nObject; i++) {
        int x0 = listObject[i].position.x;
        int y0 = listObject[i].position.y;
        for (int j = 0; j < listObject[i].nLines; j++) {
            Point p1 = listObject[i].lines[j].p1;
            Point p2 = listObject[i].lines[j].p2;
            drawWhiteLine(x0 + p1.x, y0 + p1.y, x0 + p2.x, y0 + p2.y);
        }
    }
}



//CLipper Constants
bool isClipping = false;
int xClipper = 100;
int yClipper = 100;
int pClipper = 100;
int lClipper = 100;

void clip(int xClipper, int yClipper, int pClipper, int lClipper) {
	if (isClipping) {
		for (int i = 0; i < WIDTH; ++i)
		{
			for (int j = 0; j < HEIGHT; ++j)
			{
				if ((abs(xClipper - i) > pClipper) || (abs(yClipper - j) > lClipper)) {
					redPixelMatrix[i][j] = 0;
					greenPixelMatrix[i][j] = 0;
					bluePixelMatrix[i][j] = 0;
				}
			}
		}
		drawWhiteLine(xClipper+pClipper, yClipper+lClipper, xClipper-pClipper, yClipper+lClipper);
		drawWhiteLine(xClipper+pClipper, yClipper+lClipper, xClipper+pClipper, yClipper-lClipper);
		drawWhiteLine(xClipper+pClipper, yClipper-lClipper, xClipper-pClipper, yClipper-lClipper);
		drawWhiteLine(xClipper-pClipper, yClipper+lClipper, xClipper-pClipper, yClipper-lClipper);
	}
}

bool isClipped(Line l, Point pos) {
    double my = (double)(l.p1.y - l.p2.y)/(l.p1.x - l.p2.x);
    double cy = (double)(pos.y+l.p1.y) - my;
    double mx = (double)(l.p1.x - l.p2.x)/(l.p1.y - l.p2.y);
    double cx = (double)(pos.x+l.p1.x) - mx;
    int x1 = 0;
    int x2 = WIDTH;
    int y1 = 0;
    int y2 = HEIGHT;
    if((my*x1+cy) <= y2 && (my*x1+cy) >= y1
        || ((my*x2+cy) <= y2 && (my*x2+cy) >= y1)
        || ((mx*y1+cx) <= x2 && (mx*y1+cx) >= x1)
        || ((mx*y2+cx) <= x2 && (mx*y2+cx) >= x1)) {
        if (((pos.y+l.p1.y < y1 && pos.y+l.p2.y < y1)
            ||(pos.y+l.p1.y > y2 && pos.y+l.p2.y > y2)
            || (pos.x+l.p1.x < x1 && pos.x+l.p2.x < x1)
            || (pos.x+l.p1.x > x2 && pos.x+l.p2.x > x2))) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
void drawClipObject(Object o){
    Point pos = o.position;
    for (int i = 0; i < o.nLines; i++) {
        if (isClipped(o.lines[i], o.position)) {
            Point p1 = o.lines[i].p1;
            Point p2 = o.lines[i].p2;
            drawWhiteLineZoom(pos.x + p1.x, pos.y + p1.y, pos.x + p2.x, pos.y + p2.y);
        }
    }
}
void drawClipAllObject() {
    double ratioX = (double)WIDTH/(2*lClipper-1);
    double ratioY = (double)HEIGHT/(2*pClipper-1);
    for(int i = 0; i < nObject; i++){
        //drawClipObject(listObject[i].translate(lClipper-xClipper-1, pClipper-yClipper-1).zoom(ratioX,ratioY,Point(0, 0)));
        //drawClipObject(listObject[i].zoom(ratioX,ratioY,Point(xClipper,yClipper)).translate(round(lClipper*ratioX-xClipper), round(pClipper*ratioY-yClipper)));
        drawClipObject(listObject[i].translate(WIDTH/2-xClipper, HEIGHT/2-yClipper).zoom(ratioX,ratioY,Point(WIDTH/2,HEIGHT/2)));
    }
}

int detectKeyStroke() {
    //deteksi adanya keyboard yang ditekan.
    //0 jika tidak, >=1 jika iya
	static bool flag = false;
	static const int STDIN = 0;

	if (!flag) {
        //Memakai termios untuk mematikan line buffering
		struct termios T;

		tcgetattr(STDIN, &T);
		T.c_lflag &= ~ICANON;
		tcsetattr(STDIN, TCSANOW, &T);
		setbuf(stdin, NULL);
		flag = true;
	}

	int NByte;
    ioctl(STDIN, FIONREAD, &NByte);  // STDIN = 0

    return NByte;
}

void DrawToScreen(){
    /* prosedur yang menggambar ke layar dari matriks RGB (harusnya rata tengah)*/
	long int location = 0;
	int x , y;
	for (y = vinfo.yres/2 - WIDTH/2; y < WIDTH + vinfo.yres/2 - WIDTH/2; y++)
		for (x = vinfo.xres/2 - HEIGHT/2; x < HEIGHT + vinfo.xres/2 - HEIGHT/2; x++) {
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
			if (vinfo.bits_per_pixel == 32) {
        //4byte
        *(fbp + location) = bluePixelMatrix[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT/2];        // Some blue
        *(fbp + location + 1) = greenPixelMatrix[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT/2];     // A little green
        *(fbp + location + 2) = redPixelMatrix[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT/2];    // A lot of red
        *(fbp + location + 3) = 0;      // No transparency
        //location += 4;
      } else  { //assume 16bpp
        	int b = 0;
          int g = 100;     // A little green
          int r = 0;    // A lot of red
          unsigned short int t = r<<11 | g << 5 | b;
          *((unsigned short int*)(fbp + location)) = t;
      }
    }
}

int redTempMatriks[WIDTH][HEIGHT*2];
int greenTempMatriks[WIDTH][HEIGHT*2];
int blueTempMatriks[WIDTH][HEIGHT*2];

void DrawMapAndMiniMapToScreen(){
		/* menggambar mini map dan map yang bisa zoom in/out */
		/* mini map dikiri dan satunya dikanan */
		/* harusnya rata tengah */
	  // buat temporer matriks

		long int location = 0;
		int x , y;
		int HEIGHT2 = HEIGHT*2;
		for(int i=0;i<WIDTH;i++){
			for(int j=0;j<HEIGHT2;j++){
					if(j<HEIGHT){
						redTempMatriks[i][j] = redPixelMatrix[i][j];
						greenTempMatriks[i][j] = greenPixelMatrix[i][j];
						blueTempMatriks[i][j] = bluePixelMatrix[i][j];
					}else{

						redTempMatriks[i][j] = redPixelMatrixZoomMap[i][j - HEIGHT];
						greenTempMatriks[i][j] = greenPixelMatrixZoomMap[i][j - HEIGHT];
						blueTempMatriks[i][j] = bluePixelMatrixZoomMap[i][j - HEIGHT];
					}
			}
		}

		for (y = vinfo.yres/2 - WIDTH/2; y < WIDTH + vinfo.yres/2 - WIDTH/2; y++)
			for (x = vinfo.xres/2 - HEIGHT2/2; x < HEIGHT2 + vinfo.xres/2 - HEIGHT2/2; x++) {
				location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
				if (vinfo.bits_per_pixel == 32) {
	        //4byte
	        *(fbp + location) = blueTempMatriks[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT2/2];        // Some blue
	        *(fbp + location + 1) = greenTempMatriks[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT2/2];     // A little green
	        *(fbp + location + 2) = redTempMatriks[y - vinfo.yres/2 + WIDTH/2][x - vinfo.xres/2 + HEIGHT2/2];    // A lot of red
	        *(fbp + location + 3) = 0;      // No transparency
	        //location += 4;
	      } else  { //assume 16bpp
	        	int b = 0;
	          int g = 100;     // A little green
	          int r = 0;    // A lot of red
	          unsigned short int t = r<<11 | g << 5 | b;
	          *((unsigned short int*)(fbp + location)) = t;
	      }
	    }

}

    void drawFrame() {
    	drawWhiteLine(1, 1, 1, HEIGHT-1);
    	drawWhiteLine(1, HEIGHT-1, WIDTH-1, HEIGHT-1);
    	drawWhiteLine(WIDTH-1, HEIGHT-1,WIDTH-1, 1);
    	drawWhiteLine(WIDTH-1, 1, 1, 1);

        drawWhiteLine2(1, 1, 1, HEIGHT-1);
    	drawWhiteLine2(1, HEIGHT-1, WIDTH-1, HEIGHT-1);
    	drawWhiteLine2(WIDTH-1, HEIGHT-1,WIDTH-1, 1);
    	drawWhiteLine2(WIDTH-1, 1, 1, 1);
    }

		void drawKeyClipping(){
    	while(true){
    		if(!detectKeyStroke()) {
					char KeyPressed = getchar();
    			if (KeyPressed=='j' || KeyPressed=='l' || KeyPressed=='i' || KeyPressed=='k' || KeyPressed=='z' || KeyPressed=='s') { //Listener Clipper
    				//IsClip

    				//Clipper gerak kiri kanan atas bawah
    				if (KeyPressed == 'j') {
    					yClipper -= 20;
    				}
    				if (KeyPressed == 'l') {
    					yClipper += 20;
    				}
    				if (KeyPressed == 'i') {
    					xClipper -= 20;
    				}
    				if (KeyPressed == 'k') {
    					xClipper += 20;
    				}
    				if (KeyPressed == 'z') {
                        if (pClipper < 200) {
                            pClipper += 10;
                            lClipper += 10;
                        }
    				}
    				if (KeyPressed == 's') {
                        if (pClipper > 50) {
                            pClipper -= 10;
                            lClipper -= 10;
                        }
    				}

    				//Penanganan x diluar frame buffer
    				if (xClipper + pClipper > WIDTH) {
    					xClipper = WIDTH - pClipper - 1;
    				}
    				if (xClipper - pClipper < 0) {
    					xClipper = pClipper + 1;
    				}
    				//Penanganan y diluar frame buffer
    				if (yClipper + lClipper > HEIGHT) {
    					yClipper = HEIGHT - lClipper - 1;
    				}
    				if (yClipper - lClipper < 0) {
    					yClipper = lClipper + 1;
    				}
    			}
    		}
    	}

    }


int main() {
	clearMatrix();

	int fbfd = 0;
	long int screensize = 0;
	exploded = false;

	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
    // Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}
    // Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

    // mendapat screensize layar monitor
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	printf("%d x %d\n",vinfo.xres,vinfo.yres);
    // Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

    //display merge center
    // Menulis ke layar tengah file
    //Gambar trapesium


	thread thread1(&drawKeyClipping);
	char KeyPressed;
	isClipping = true;
	clearMatrix();
	makeWorld();
	clip(xClipper, yClipper, pClipper, lClipper);
	do{
		clearMatrix();
		clip(xClipper, yClipper, pClipper, lClipper);
		drawFrame();
		drawAllObject();
		drawClipAllObject();
        DrawMapAndMiniMapToScreen();
		usleep(40000);
	} while (KeyPressed!='C' && !exploded);
	thread1.detach();
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
