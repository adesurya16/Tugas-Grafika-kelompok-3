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

struct bullet
{
	int xStart;
	int yStart;
	int xEnd;
	int yEnd;
	float m;
	float c;
	int partisi;
	int iteration;
	int x1;
	int x2;
	int n;
};

vector<bullet> bullets;

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
int xClipper = 150;
int yClipper = 150;
int pClipper = 100;
int lClipper = 100;

//zoom constant
int zoomConst = 3;

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

void drawShooter(int xp, int yp, char mode) {
    //gambar tembakan dengan titik pusat lingkaran tembakan
    //(yp,xp)
	switch (mode) {
		case'd':
		case 'D': {
			posX = xp+50;
			posY = yp-50;
			drawCircle(yp,xp,25);
			//floodFill(yp, xp, 255, 255, 255, 255, 0, 0);
			drawWhiteLine(yp,xp+25,yp-50,xp+50);
            //drawWhiteLine(yp-25,xp,yp-50,xp+25);
			drawWhiteLine(yp-25,xp,yp-50,xp+50);
			//floodFill(yp-15, xp+20, 255, 255, 255, 0, 0, 255);
			break;
		}

		case 's':
		case 'S': {
			posX = xp;
			posY = 500;
			drawCircle(yp,xp,25);
			//floodFill(yp, xp, 255, 255, 255, 0, 255, 0);
			drawWhiteLine(yp-15,xp+20,yp-50,xp);
			drawWhiteLine(yp-50,xp,yp-15,xp-20);
            //drawWhiteLine(yp-50,xp+20,yp-50,xp-20);
			//floodFill(yp-35, xp, 255, 255, 255, 0, 0, 255);
			break;
		}


		case 'a':
		case 'A': {
			posX = xp-50;
			posY = yp-50;
			drawCircle(yp,xp,25);
			//floodFill(yp, xp, 255, 255, 255, 255, 180, 0);
			drawWhiteLine(yp,xp-25,yp-50,xp-50);
			drawWhiteLine(yp-25,xp,yp-50,xp-50);
            //drawWhiteLine(yp-25,xp-50,yp-50,xp-25);
			//floodFill(yp-25, xp-25, 255, 255, 255, 0, 0, 255);
			break;
		}
		default: {}
	}
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


	void drawExplosion2(int x,int y){
    //x = 70
    // bentuk bintang ada 8 garis sesuai dengan parameter titik pusat (x,y)
		int pointx1 = x-20, pointy1 =y+20;
		int pointx3 = x+30, pointy3 =y+30;
		int pointx5 = x+20, pointy5 =y-20;
		int pointx7 = x-30, pointy7 =y-30;

		int pointx2 = x, pointy2 = y+15;
		int pointx4 = x+10, pointy4 = y;
		int pointx6 = x, pointy6 = y-10;
		int pointx8 = x-15, pointy8 = y;

    //gambar ledakan
		drawRedLine(pointx1,pointy1,pointx2,pointy2);
		drawRedLine(pointx2,pointy2,pointx3,pointy3);
		drawRedLine(pointx3,pointy3,pointx4,pointy4);
		drawRedLine(pointx4,pointy4,pointx5,pointy5);
		drawRedLine(pointx5,pointy5,pointx6,pointy6);
		drawRedLine(pointx6,pointy6,pointx7,pointy7);
		drawRedLine(pointx7,pointy7,pointx8,pointy8);
		drawRedLine(pointx8,pointy8,pointx1,pointy1);

    //warnain
    	//floodFill(x,y,255,0,0,255,0,0);
	}

	void drawUFO(int x1, int y1) {
		drawWhiteLine(x1, y1, x1+20, y1+20);
		drawWhiteLine(x1, y1, x1, y1-50);
		drawWhiteLine(x1, y1-50, x1+20, y1-70);
		drawWhiteLine(x1+20, y1-70, x1+20, y1+20);

    	//floodFill(x1+5, y1, 255, 255, 255, 0, 255, 0);

		drawSemiCircle(x1, y1-25, 25);
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

	void addBullet(int x1, int y1, int x2, int y2 , int n)
//x1,y1 titik asal peluru
//x2,y2 titik sampai peluru
//n adalah pembagian tahap gerak peluru
	{
		bullet newBullet;
    //persamaan garis
		newBullet.m = (y2-y1);
		newBullet.m /= (x2-x1);
		newBullet.c = y1 - newBullet.m * x1;

		newBullet.partisi = 0;
		for (int i=1;i<=n;i++) {
			newBullet.partisi += i;
		}

		newBullet.xStart = x1;
		newBullet.yStart = (int) floor(newBullet.m * newBullet.xStart + newBullet.c + 0.5);
		newBullet.xEnd = x1 + (x2-x1) * n / newBullet.partisi;
		newBullet.yEnd = (int) floor(newBullet.m * newBullet.xEnd + newBullet.c + 0.5);

		newBullet.x1 = x1;
		newBullet.x2 = x2;
		newBullet.iteration = n;
		newBullet.n = n;

		bullets.push_back(newBullet);
	}

	void drawKeyShooter(){
		while(!exploded){
			if(!detectKeyStroke()) {
				char KeyPressed = getchar();
				switch (KeyPressed) {
					case 'a': case 's': case 'd': lastCorrectState = KeyPressed; break;

					case ' ': {
						if (lastCorrectState == 'a')
							addBullet(posY,posX,0,0,20);
						else if (lastCorrectState == 's')
							addBullet(posY,posX,0,600,20);
						else if (lastCorrectState == 'd')
							addBullet(posY,posX,0,1200,20);	
					}

					case 'j': yClipper -= 20; break;
					case 'l': yClipper += 20; break;
					case 'i': xClipper -= 20; break;
					case 'm': xClipper += 20; break;
					case 'z': isClipping = !isClipping; break;

					case 'q': zoomConst += 1; printf("MASUK"); break;
					case 'e': zoomConst -= 1; break;
				}
				/*
				if ((KeyPressed=='A')||(KeyPressed=='a') ||(KeyPressed=='S') ||(KeyPressed=='s') ||(KeyPressed=='D') ||(KeyPressed=='d')) {
					lastCorrectState = KeyPressed;
				} else if (KeyPressed==' ') {

					if (lastCorrectState == 'a')
						addBullet(posY,posX,0,0,20);
					else if (lastCorrectState == 's')
						addBullet(posY,posX,0,600,20);
					else if (lastCorrectState == 'd')
						addBullet(posY,posX,0,1200,20);

    			} else if (KeyPressed=='j' || KeyPressed=='l' || KeyPressed=='i' || KeyPressed=='k' || KeyPressed=='z') { //Listener Clipper
    				//IsClip
    				if (KeyPressed == 'z') {
    					isClipping = !isClipping;
    				}

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
    			}*/
			}
		}

	}

	void drawKeyClipping(){
		while(true){
			if(!detectKeyStroke()) {
				char KeyPressed = getchar();
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
				if (KeyPressed == ',') {
					zoomConst += 1;
				}
				if (KeyPressed == '.') {
					zoomConst -= 1;
				}

				if (zoomConst <= 1) {
					zoomConst = 1;
				}

				//Diprediksi dulu kalo mau zoom
				int futurep = ((WIDTH) / zoomConst) / 2;
				int futurel = ((HEIGHT) / zoomConst) / 2;
				
				//Penanganan keluar kotak
				if (xClipper - futurep < 0) {
					xClipper = futurep;
				}
				if (yClipper - futurel < 0) {
					yClipper = futurel;
				}
				if (xClipper + futurep > WIDTH) {
					xClipper = WIDTH - futurep;
				}
				if (yClipper + futurel > HEIGHT) {
					yClipper = HEIGHT - futurel;
				}

				//update
				pClipper = futurep;
				lClipper = futurel;

				printf(">> zoomConst: %d\n", zoomConst);
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

	void drawBullets() {
    //persamaan garis
		for (int i = bullets.size()-1; i >=0; --i)
		{
			if (bullets[i].iteration >0) {
				if (drawWhiteLine(bullets[i].xStart,bullets[i].yStart,bullets[i].xEnd,bullets[i].yEnd)) exploded = true;
				bullets[i].xStart = bullets[i].xEnd;
				bullets[i].yStart = bullets[i].yEnd;
				bullets[i].xEnd = bullets[i].xStart + (bullets[i].x2 - bullets[i].x1) * (bullets[i].iteration - 1) / bullets[i].partisi;
				bullets[i].yEnd = (int) floor(bullets[i].m * bullets[i].xEnd + bullets[i].c + 0.5);
				bullets[i].iteration--;
			}
		}
	}

	void drawplaneright(int x1,int y1){
		drawWhiteLine(x1, y1, x1, y1-80);
		drawWhiteLine(x1,y1-80,x1,y1-100);
		drawWhiteLine(x1,y1-100,x1-20,y1-80);
		drawWhiteLine(x1-20,y1-80,x1, y1-80);
		drawWhiteLine(x1-20,y1-80,x1-20,y1-20);
		drawWhiteLine(x1-20,y1-20,x1-40,y1);
		drawWhiteLine(x1-40,y1,x1-20,y1);
		drawWhiteLine(x1-20,y1,x1-20,y1-20);
		drawWhiteLine(x1-20,y1,x1,y1);

    //gambar sayap
		drawWhiteLine(x1-15, y1-60, x1-12, y1-45);
		drawWhiteLine(x1-5, y1-65, x1-12, y1-45);
		drawWhiteLine(x1-5, y1-65, x1-15, y1-60);

    //mewarnai pesawat
    //warnain badan
    	//floodFill(x1-1,y1-1,255,255,255,255,0,127);

    //warnain ekor
    	//floodFill(x1-21, y1-1, 255, 255, 255, 208, 163, 237);

    //warnain kepala
    	//floodFill(x1-1, y1-81, 255, 255, 255, 208, 163, 237);
	}

	void drawplaneleft(int x1,int y1){

		drawWhiteLine(x1, HEIGHT - y1, x1, HEIGHT - (y1-80));
		drawWhiteLine(x1,HEIGHT - (y1-80),x1,HEIGHT - (y1-100));
		drawWhiteLine(x1,HEIGHT - (y1-100),x1-20,HEIGHT - (y1-80));
		drawWhiteLine(x1-20,HEIGHT -(y1-80),x1,HEIGHT - (y1-80));
		drawWhiteLine(x1-20,HEIGHT -(y1-80),x1-20,HEIGHT -(y1-20));
		drawWhiteLine(x1-20,HEIGHT -(y1-20),x1-40,HEIGHT - y1);
		drawWhiteLine(x1-40,HEIGHT - y1,x1-20,HEIGHT - y1);
		drawWhiteLine(x1-20,HEIGHT -y1,x1-20,HEIGHT - (y1-20));
		drawWhiteLine(x1-20,HEIGHT - y1,x1,HEIGHT - y1);

    //gambar sayap
		drawWhiteLine(x1-15, HEIGHT - (y1-60), x1-12, HEIGHT - (y1-45));
		drawWhiteLine(x1-5, HEIGHT - (y1-65), x1-12, HEIGHT - (y1-45));
		drawWhiteLine(x1-5, HEIGHT - (y1-65), x1-15,HEIGHT - (y1-60));

    //mewarnai pesawat
    	//floodFill(x1-1,HEIGHT-(y1-1),255,255,255,255,0,127);
    	//floodFill(x1-21, HEIGHT - (y1-1), 255, 255, 255, 208, 163, 237);

    	//floodFill(x1-1, HEIGHT-(y1-81), 255, 255, 255, 208, 163, 237);

	}

	void drawwheel(int x1,int y1,int r,double sudut){
    // r > 20
    //(0,)

		int x2 = r-20;
		int y2 = 0;

		int x3 = 0;
		int y3 = r-20;

		int x22 = round(x2 * cos(sudut) - y2 * sin(sudut));
		int y22 = round(x2 * sin(sudut) + y2 * cos(sudut));

		int x32 = round(x3 * cos(sudut) - y3 * sin(sudut));
		int y32 = round(x3 * sin(sudut) + y3 * cos(sudut));



		drawCircle(x1,y1,r-20);
		drawWhiteLine(x1+x22,y1+y22,x1-x22,y1-y22);
		drawWhiteLine(x1+x32,y1+y32,x1-x32,y1-y32);

    //gambar 2 garis didalam roda


	}

	void drawParachute(int x, int y) {
		const int rPara = 30;
		const int rKepala = 10;
		const int lBadan = 30;
		const int lKeBawah = rPara + lBadan + lBadan/2;
    //Parasut
		drawSemiCircle(x, y, rPara);
    //Tali Parasut
		drawWhiteLine(x, y-rPara, x+rPara, y);
		drawWhiteLine(x, y+rPara, x+rPara, y);
    //Kepala
		drawCircle(x + rPara - rKepala, y, rKepala);
    	//floodFill(x + rPara - rKepala, y, 255, 255, 255, 255, 255, 255);
    //Badan
		drawWhiteLine(x + rPara, y, x + rPara + lBadan, y);
    //Tangan kanan
		drawWhiteLine(x + rPara + lBadan/3, y, x + rPara, y + (lBadan/3)*2);
    //Tangan kiri
		drawWhiteLine(x + rPara + lBadan/3, y, x + rPara, y - (lBadan/3)*2);
    //Kaki kanan
		drawWhiteLine(x + rPara + lBadan, y, x + rPara + lBadan + lBadan/2, y - (lBadan/3)*2);
    //Kaki kiri
		drawWhiteLine(x + rPara + lBadan, y, x + rPara + lBadan + lBadan/2, y + (lBadan/3)*2);
	}

	void moveBounce(double* x, double* y, double* vx, double* vy, int yground) {

		double a = 10;

		double x2 = *x + *vx;
		double vx2 = *vx;

		if(x2 <= 0) {
			x2 *= -1;
			vx2 *= -1;
		}

		if(x2 >= 1200) {
			x2 = 2400 - x2;
			vx2 *= -1;
		}

		double y2 = *y + *vy + a/2;
		double vy2 = *vy + a;

		if(y2 > yground) {
			double t0 = (-(*vy) + sqrt((*vy)*(*vy)-2*a*(*y - yground)))/a;
			double v0 = -(*vy + a*t0);

			if(a*(v0/a)*(v0/a)/2 + v0*(-v0/a) + yground > yground-1) {
				y2 = yground;
				vy2 = 0;
			} else {
				y2 = (a*(1-t0)*(1-t0)/2 + v0*(1-t0) + yground);
				vy2 = (v0 + a*(1-t0)) * 0.8;
			}
		}

		*x = x2;
		*y = y2;
		*vx= vx2;
		*vy = vy2;
	}

	int moveParachute(int x, int y, int i) {
		const int c = 10;
		const int amp = 100;
		const double v = 0.5;
		double angleD = i * c;
		double angleRY, angleRX;
		angleRY = (angleD / 360) * 3.14159 * 2;
		angleRX = (fmod(angleD, 90) / 360) * 3.14159 * 2;
    //cout << abs(sin(angleRX) * amp) << endl;
    //cout << fmod(angleD, 90) << endl;
    int xPar = x + ((int)(angleD / 90) * (amp/4));// - abs(sin(angleRY) * amp);
    int yPar = sin(angleRY) * amp + y;
    xPar = x+(i*c);
    if (xPar > 520) {
    	xPar = 520;
    }
    if (yPar < 31) {
    	yPar = 31;
    }
    if (yPar > 1219) {
    	yPar = 1219;
    }
    drawParachute(xPar, yPar);
    return xPar;
}

int zoomR[WIDTH][HEIGHT];
int zoomG[WIDTH][HEIGHT];
int zoomB[WIDTH][HEIGHT];

void genZoomMatrix() {
	//printf("ZOOMING\n");
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			zoomR[i][j] = 0;
			zoomG[i][j] = 0;
			zoomB[i][j] = 0;
		}
	}
	
	//printf("> Gen Zoom Mat\n");
	int cx = 0;
	int cy = 0;
	//printf("> Done init\n");

	//y zoom
	for (int i = xClipper - pClipper; i < xClipper + pClipper - 1; i++) {
		cx = 0;
		cy = 0;
		for (int j = yClipper - lClipper; j < yClipper + lClipper - 1; j++) {
			zoomR[i][cy] = redPixelMatrix[i][j];
			zoomG[i][cy] = greenPixelMatrix[i][j];
			zoomB[i][cy] = bluePixelMatrix[i][j];

			//calculate op
			int opR = abs(redPixelMatrix[i][j] - redPixelMatrix[i][j+1]) / zoomConst;
			int opG = abs(greenPixelMatrix[i][j] - greenPixelMatrix[i][j+1]) / zoomConst;
			int opB = abs(bluePixelMatrix[i][j] - bluePixelMatrix[i][j+1]) / zoomConst;

			//bandingkan mana yg lebih gelap
			int a = redPixelMatrix[i][j] + greenPixelMatrix[i][j] + bluePixelMatrix[i][j];
			int b = redPixelMatrix[i][j + 1] + greenPixelMatrix[i][j + 1] + bluePixelMatrix[i][j + 1];

			int patokanR, patokanG, patokanB;
			int sign;

			//tentukan patokan
			patokanR = redPixelMatrix[i][j];
			patokanG = greenPixelMatrix[i][j];
			patokanB = bluePixelMatrix[i][j];
			if (a > b) {
				sign = -1;
			} else {
				sign = 1;
			}

			//isi yang di-stretch sesuai patokan
			for (int l = 1; l <= zoomConst - 1; l++) {
				zoomR[i][l + cy] = patokanR + (sign * (opR * l));
				if (zoomR[i][l+ cy] < 0 && zoomR[i][l+cy] != 0) {
					printf(">> %d\n", zoomR[i][l + cy]);
					printf(">> opr %d\n", opR);
					printf(">> pat %d\n", patokanR);
					printf(">> sig %d\n\n", sign);
				}
				zoomG[i][l + cy] = patokanG + (sign * (opR * l));
				zoomB[i][l + cy] = patokanB + (sign * (opR * l));
			}

			//inkremen current y
			cy += zoomConst;
		}
	}

	//x zoom
	for (int j = 0; j < HEIGHT; j++) {
		cx = 0;
		cy = 0;
		for (int i = xClipper - pClipper; i < xClipper + pClipper - 1; i++) {
			redPixelMatrixZoomMap[cx][j] = zoomR[i][j];
			greenPixelMatrixZoomMap[cx][j] = zoomG[i][j];
			bluePixelMatrixZoomMap[cx][j] = zoomB[i][j];

			//calculate op
			int opR = abs(zoomR[i][j] - zoomR[i+1][j]) / zoomConst;
			int opG = abs(zoomG[i][j] - zoomG[i+1][j]) / zoomConst;
			int opB = abs(zoomB[i][j] - zoomB[i+1][j]) / zoomConst;

			//bandingkan mana yg lebih gelap
			int a = zoomR[i][j] + zoomG[i][j] + zoomB[i][j];
			int b = zoomR[i+1][j] + zoomG[i+1][j] + zoomB[i+1][j];

			int patokanR, patokanG, patokanB;
			int sign;
			
			//tentukan patokan
			patokanR = zoomR[i][j];
			patokanG = zoomG[i][j];
			patokanB = zoomB[i][j];
			if (a > b) {
				sign = -1;
			} else {
				sign = 1;
			}

			//isi yang di-stretch sesuai patokan
			for (int l = 1; l <= zoomConst - 1; l++) {
				
				redPixelMatrixZoomMap[l + cx][j] = patokanR + (sign * (opR * l));
				greenPixelMatrixZoomMap[l + cx][j] = patokanG + (sign * (opR * l));
				bluePixelMatrixZoomMap[l + cx][j] = patokanB + (sign * (opR * l));
			}
			
			//inkremen current x
			cx += zoomConst;
		}
	}

	//donezooming
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
	pClipper = ((WIDTH) / zoomConst) / 2;
	lClipper = ((HEIGHT) / zoomConst) / 2;
	do{
		clearMatrix();
		//printf(">> p: %d | l: %d\n", pClipper, lClipper);
		clip(xClipper, yClipper, pClipper, lClipper);
		drawFrame();
		drawStars();
		genZoomMatrix();
		DrawMapAndMiniMapToScreen();
		usleep(40000);
	} while (KeyPressed!='C' && !exploded);
	thread1.detach();
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
