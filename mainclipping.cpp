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
	redPixelMatrix[x1][y1] = 255;
	greenPixelMatrix[x1][y1] = 255;
	bluePixelMatrix[x1][y1] = 255;
}

void drawBlackPoint(int x1,int y1){
	redPixelMatrix[x1][y1] = 0;
	greenPixelMatrix[x1][y1] = 0;
	bluePixelMatrix[x1][y1] = 0;
}

void floodFill(int x,int y,int redBatas,int greenBatas,int blueBatas,int redColor,int greenColor,int blueColor){
	if((x>=0 && x<WIDTH) && (y>=0 && y<HEIGHT)){
		if(!((redPixelMatrix[x][y]==redBatas && greenPixelMatrix[x][y]==greenBatas && bluePixelMatrix[x][y]==blueBatas) ||
			(redPixelMatrix[x][y]==redColor && greenPixelMatrix[x][y]==greenColor && bluePixelMatrix[x][y]==blueColor))){
			redPixelMatrix[x][y] = redColor;
		greenPixelMatrix[x][y] = greenColor;
		bluePixelMatrix[x][y] = blueColor;
		floodFill(x,y+1,redBatas,greenBatas,blueBatas,redColor,greenColor,blueColor);
		floodFill(x+1,y,redBatas,greenBatas,blueBatas,redColor,greenColor,blueColor);
		floodFill(x,y-1,redBatas,greenBatas,blueBatas,redColor,greenColor,blueColor);
		floodFill(x-1,y,redBatas,greenBatas,blueBatas,redColor,greenColor,blueColor);
		}
	}
}

void drawCircle(int x0, int y0, int radius)
{
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		drawWhitePoint(x0 - x, y0 + y);
		drawWhitePoint(x0 - y, y0 + x);
		drawWhitePoint(x0 - y, y0 - x);
		drawWhitePoint(x0 - x, y0 - y);
		drawWhitePoint(x0 + x, y0 + y);
		drawWhitePoint(x0 + y, y0 + x);
		drawWhitePoint(x0 + y, y0 - x);
		drawWhitePoint(x0 + x, y0 - y);

		if (err <= 0)
		{
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2*x + 1;
		}
	}
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

bool isCollision(int x1garis, int y1garis,int x2garis,int y2garis,int x1clip,int y1clip,int x2clip,int y2clip){
		if(x1clip==x2clip){
			int ykecil,ybesar;
			if(y1clip < y2clip){
				ykecil = y1clip;
				ybesar = y2clip;
			}else{
				ykecil = y2clip;
				ybesar = y1clip;
			}
			int delta = (x1clip - x1garis1)*(y2garis-y1garis);
			if(delta>=(ykecil-y1garis)*(x2garis-x1garis) && delta<(ybesar-y1garis)*(x2garis-x1garis)){
				return true;
			}else return false
		}else{ //y sama
			int xkecil,xbesar;
			if(y1clip < y2clip){
				xkecil = x1clip;
				xbesar = x2clip;
			}else{
				xkecil = x2clip;
				xbesar = x1clip;
			}
			int delta = (y1clip - y1garis1)*(x2garis-x1garis);
			if(delta>=(xkecil-x1garis)*(y2garis-y1garis) && delta<(xkecil-x1garis)*(y2garis-y1garis)){
					return true;
			}else return false;
		}
}

void getTitikPotong(int x1garis, int y1garis,int x2garis,int y2garis,int x1clip,int y1clip,int x2clip,int y2clip,
	int &xpot, int &ypot){
		if(x1clip==x2clip){
			int delta = (x1clip - x1garis1)*(y2garis-y1garis);
			xpot = x1clip;
			ypot = round(delta / (x2garis-x1garis)) + ygaris1;
		}else{ //y sama
			int delta = (y1clip - y1garis1)*(x2garis-x1garis);
			ypot = y1clip;
			xpot = round(delta / (y2garis-y1garis)) + xgaris1;
		}
}

bool isTitikDalam(int x,int y){
	int xclip1 = xClipper - pClipper;
	int yclip1 = yClipper - lClipper;
	int xclip2 = xClipper - pClipper;
	int yclip2 = yClipper + lClipper;
	int xclip3 = xClipper + pClipper;
	int yclip3 = yClipper + lClipper;
	int xclip4 = xClipper + pClipper;
	int yclip4 = yClipper + lClipper;
	return ((x>=0) && (x<xclip4)) && ((y>=0) && (y<yclip2));
}

void drawWhiteLineClipping(int x1garis, int y1garis, int x2garis, int y2garis){
	/* Asumsi Clipping hanya 1 dan sudah diektahui titiknya dimana */
		int xclip1 = xClipper - pClipper;
		int yclip1 = yClipper - lClipper;
		int xclip2 = xClipper - pClipper;
		int yclip2 = yClipper + lClipper;
		int xclip3 = xClipper + pClipper;
		int yclip3 = yClipper + lClipper;
		int xclip4 = xClipper + pClipper;
		int yclip4 = yClipper + lClipper;

		if(isTitikDalam(x1garis,y1garis) && isTitikDalam(x2garis,y2garis)){
			//kalo didalem semua
			//gambar biasa
			drawWhiteLine(x1garis,y1garis,x2garis,y2garis);
		// }else if(isTitikDalam(x1garis,x1garis) || isTitikDalam(x2garis,y2garis)){
		// //Kasus 2 kalo 2 titik salah satu ada yang didalem
		// 		//dapetin titik potong
		//
		// 		if(isTitikDalam(x1garis,y1garis)){
		// 				//gambar biasa ke garis x1 y1
		//
		// 		}else{
		// 				//gambar biasa ke garis x2 y2
		//
		// 		}
		// }
	}else if(!isTitikDalam(x1garis,y1garis) && !isTitikDalam(x2garis,y2garis)){
		//kalo dia dititik clip gimana perpotongannya?
		int x[2],y[2];
		int i = 0;
		//kalo gak perpotongan biarkan;
		if(isCollision(xclip1,yclip1,xclip2,yclip2,x1garis,y1garis,x2garis,y2garis) && i<2){
			getTitikPotong(xclip1,yclip1,xclip2,yclip2,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);
			i++;
		}
		if(isCollision(xclip3,yclip3,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis) && i<2){
			getTitikPotong(xclip3,yclip3,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);
			i++;
		}
		if(isCollision(xclip2,yclip2,xclip3,yclip3,x1garis,y1garis,x2garis,y2garis) && i<2){
			getTitikPotong(xclip2,yclip2,xclip3,yclip3,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);
			i++;
		}
		if(isCollision(xclip1,yclip1,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis) && i<2){
			getTitikPotong(xclip1,yclip1,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);
			i++;
		}
		//drawWhiteLine
		drawWhiteLine(x[1],y[1],x[2],y[2]);
	}else{
		int x,y;
		if(isCollision(xclip1,yclip1,xclip2,yclip2,x1garis,y1garis,x2garis,y2garis)){
				getTitikPotong(xclip1,yclip1,xclip2,yclip2,x1garis,y1garis,x2garis,y2garis,x,y);

		}else
		if(isCollision(xclip3,yclip3,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis)){
				getTitikPotong(xclip3,yclip3,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis,x,y);

		}else
		if(isCollision(xclip2,yclip2,xclip3,yclip3,x1garis,y1garis,x2garis,y2garis)){
			  getTitikPotong(xclip2,yclip2,xclip3,yclip3,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);

		}else
		if(isCollision(xclip1,yclip1,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis)){
				getTitikPotong(xclip1,yclip1,xclip4,yclip4,x1garis,y1garis,x2garis,y2garis,x[i],y[i]);
		}

		if(isTitikDalam(x1garis,y1garis)){
			drawWhiteLine(x1garis,y1garis,x,y);
		}else{
			drawWhiteLine(x2garis,y2garis,x,y);
		}
	}
	//kasus 4 kalo 2 titik diluar tapi ngelewatin garis clipping abaikan
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

void drawSemiCircle(int x0, int y0, int radius)
{
	drawWhiteLine(x0, y0-radius, x0, y0+radius);
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		drawWhitePoint(x0 - x, y0 + y);
		drawWhitePoint(x0 - y, y0 + x);
		drawWhitePoint(x0 - y, y0 - x);
		drawWhitePoint(x0 - x, y0 - y);

		if (err <= 0)
		{
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2*x + 1;
		}
	}

    //warnain
	//floodFill(x0-5,y0,255,255,255,255,255,0);
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

    void drawExplosion(int x,int y){
    //x = 70
    // bentuk bintang ada 8 garis sesuai dengan parameter titik pusat (x,y)
    	int pointx1 = x-20, pointy1 =y+20;
    	int pointx3 = x+20, pointy3 =y+20;
    	int pointx5 = x+20, pointy5 =y-20;
    	int pointx7 = x-20, pointy7 =y-20;

    	int pointx2 = x, pointy2 = y+10;
    	int pointx4 = x+10, pointy4 = y;
    	int pointx6 = x, pointy6 = y-10;
    	int pointx8 = x-10, pointy8 = y;

    //gambar ledakan
    	drawWhiteLine(pointx1,pointy1,pointx2,pointy2);
    	drawWhiteLine(pointx2,pointy2,pointx3,pointy3);
    	drawWhiteLine(pointx3,pointy3,pointx4,pointy4);
    	drawWhiteLine(pointx4,pointy4,pointx5,pointy5);
    	drawWhiteLine(pointx5,pointy5,pointx6,pointy6);
    	drawWhiteLine(pointx6,pointy6,pointx7,pointy7);
    	drawWhiteLine(pointx7,pointy7,pointx8,pointy8);
    	drawWhiteLine(pointx8,pointy8,pointx1,pointy1);

    //warnain
    	//floodFill(x,y,255,0,0,255,255,0);
    }


    void drawStars() {
			drawExplosion(100,100);
			drawExplosion(100,200);
			drawExplosion(100,300);
			drawExplosion(100,400);
			drawExplosion(100,500);
			drawExplosion(200,100);
			drawExplosion(200,200);
			drawExplosion(200,300);
			drawExplosion(200,400);
			drawExplosion(200,500);
    	drawExplosion(400, 100);
    	drawExplosion(300, 200);
    	drawExplosion(500, 200);
    	drawExplosion(400, 300);
    	drawExplosion(300, 400);
    	drawExplosion(500, 400);
    	drawExplosion(400, 500);
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
    			if (KeyPressed=='j' || KeyPressed=='l' || KeyPressed=='i' || KeyPressed=='k' || KeyPressed=='z') { //Listener Clipper
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
	clip(xClipper, yClipper, pClipper, lClipper);
	do{
		clearMatrix();
		clip(xClipper, yClipper, pClipper, lClipper);
		drawFrame();
		drawStars();
	  DrawMapAndMiniMapToScreen();
		usleep(40000);
	} while (KeyPressed!='C' && !exploded);
	thread1.detach();
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
