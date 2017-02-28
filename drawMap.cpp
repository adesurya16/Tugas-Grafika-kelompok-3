void drawPoly (int[] x, int[] y, int N) {
	int i;	
	for (i = 0; i < N - 1; i++) {
		drawWhiteLine(x[i], y[i], x[i + 1], y[i + 1];
	}
	drawWhiteLine(x[i], y[i], x[0], y[0]);
}

void drawMap() {
	/** KELOMPOK 1 **/
	int xKubus[8], yKubus[8];
	xKubus[0] = 156; yKubus[0] = 414;
	xKubus[1] = 159; yKubus[1] = 406;
	xKubus[2] = 166; yKubus[2] = 400;
	xKubus[3] = 167; yKubus[3] = 382;
	xKubus[4] = 194; yKubus[4] = 382;
	xKubus[5] = 195; yKubus[5] = 298;
	xKubus[6] = 201; yKubus[6] = 407;
	xKubus[7] = 204; yKubus[7] = 414;
	drawPoly(xKubus, yKubus, 8);

	int xKotakKiri[4], yKotakKiri[4];
	xKotakKiri[0] = 75; yKotakKiri[0] = 415;
	xKotakKiri[1] = 71; yKotakKiri[1] = 406;
	xKotakKiri[2] = 100; yKotakKiri[2] = 396;
	xKotakKiri[3] = 103; yKotakKiri[3] = 405;
	drawPoly(xKotakKiri, yKotakKiri, 4);
 
	int xKotakKanan[5], yKotakKanan[5];
	xKotakKanan[0] = 221; yKotakKanan = 415;
	xKotakKanan[1] = 219; yKotakKanan = 412;
	xKotakKanan[2] = 229; yKotakKanan = 394;
	xKotakKanan[3] = 248; yKotakKanan = 404;
	xKotakKanan[4] = 243; yKotakKanan = 415;
	drawPoly(xKotakKanan, yKotakKanan, 5);

	int xLapSil[5], yLapSil[5];
	xLapSil[0] = 61; yLapSil[0] = 395;
	xLapSil[1] = 57; yLapSil[1] = 385;
	xLapSil[2] = 95; yLapSil[2] = 371;
	xLapSil[3] = 141; yLapSil[3] = 362;
	xLapSil[4] = 141; yLapSil[4] = 355;
	drawPoly(xLapSil, yLapSil, 5);
	drawWhiteLine(141, 355, 144, 372);

	int xLapSRKiri[4], yLapSRKiri[4];
	xLapSRKiri[0] = 210; yLapSRKiri[0] = 374;
	xLapSRKiri[1] = 211; yLapSRKiri[1] = 364;
	xLapSRKiri[2] = 216; yLapSRKiri[2] = 364;
	xLapSRKiri[3] = 215; yLapSRKiri[3] = 354;
	drawPoly(xLapSRKiri, yLapSRKiri, 4);

	int xLapSRAtas[4], yLapSRAtas[4];
	xLapSRAtas[0] = 217; yLapSRAtas[0] = 355;
	xLapSRAtas[1] = 217; yLapSRAtas[1] = 364;
	xLapSRAtas[2] = 255; yLapSRAtas[2] = 372;
	xLapSRAtas[3] = 287; yLapSRAtas[3] = 384;
	drawPoly(xLapSRAtas, yLapSRAtas, 4);

	/** KELOMPOK 2 **/
	int xCoklat[5], yCoklat[5];
	xCoklat[0] = 231; yCoklat[0] = 394;
	xCoklat[1] = 250; yCoklat[1] = 405;
	xCoklat[2] = 244; yCoklat[2] = 415;
	xCoklat[3] = 223; yCoklat[3] = 415;
	xCoklat[4] = 220; yCoklat[4] = 413;
	drawPoly(xCoklat, yCoklat, 5);

	int xUngu[4], yUngu[4];
	xUngu[0] = 173; yUngu[0] = 343;
	xUngu[1] = 172; yUngu[1] = 349;
	xUngu[2] = 153; yUngu[2] = 349;
	xUngu[3] = 153; yUngu[3] = 343;
	drawPoly(xUngu, yUngu, 4);

	int xCoklat1[4], yCoklat[4];
	xCoklat1[0] = 210; yCoklat1[0] = 343;
	xCoklat1[1] = 210; yCoklat1[1] = 349;
	xCoklat1[2] = 190; yCoklat1[2] = 349;
	xCoklat1[3] = 190; yCoklat1[3] = 343;
	drawPoly(xCoklat1, yCoklat1, 4);

	int xAlbar[16], yAlbar[16];
	xAlbar[0] = 149; yAlbar[0] = 311;
	xAlbar[1] = 149; yAlbar[1] = 315;
	xAlbar[2] = 155; yAlbar[2] = 315;
	xAlbar[3] = 155; yAlbar[3] = 317;
	xAlbar[4] = 158; yAlbar[4] = 317;
	xAlbar[5] = 158; yAlbar[5] = 326;
	xAlbar[6] = 151; yAlbar[6] = 326;
	xAlbar[7] = 151; yAlbar[7] = 335;
	xAlbar[8] = 122; yAlbar[8] = 335;
	xAlbar[9] = 122; yAlbar[9] = 328;
	xAlbar[10] = 115; yAlbar[10] = 328;
	xAlbar[11] = 115; yAlbar[11] = 317;
	xAlbar[12] = 119; yAlbar[12] = 317;
	xAlbar[13] = 119; yAlbar[13] = 314;
	xAlbar[14] = 125; yAlbar[14] = 315;
	xAlbar[15] = 125; yAlbar[15] = 311;
	drawPoly(xAlbar, yAlbar, 16);

	int xAltim[16], yAltim[16];
	xAltim[0] = 237; yAltim[0] = 311;
	xAltim[1] = 237; yAltim[1] = 315;
	xAltim[2] = 242; yAltim[2] = 315;
	xAltim[3] = 242; yAltim[3] = 317;
	xAltim[4] = 247; yAltim[4] = 317;
	xAltim[5] = 247; yAltim[5] = 327;
	xAltim[6] = 240; yAltim[6] = 327;
	xAltim[7] = 240; yAltim[7] = 335;
	xAltim[8] = 211; yAltim[8] = 335;
	xAltim[9] = 211; yAltim[9] = 327;
	xAltim[10] = 204; yAltim[10] = 327;
	xAltim[11] = 204; yAltim[11] = 318;
	xAltim[12] = 208; yAltim[12] = 318;
	xAltim[13] = 208; yAltim[13] = 315;
	xAltim[14] = 213; yAltim[14] = 315;
	xAltim[15] = 213; yAltim[15] = 311;
	drawPoly(xAltim, yAltim, 16);

	/** KELOMPOK 3 **/
	int xHMTL[12], yHMTL[12];
	xHMTL[0] = 234; yHMTL[0] = 235;
	xHMTL[1] = 244; yHMTL[1] = 235;
	xHMTL[2] = 244; yHMTL[2] = 238;
	xHMTL[3] = 302; yHMTL[3] = 238;
	xHMTL[4] = 302; yHMTL[4] = 250;
	xHMTL[5] = 286; yHMTL[5] = 250;
	xHMTL[6] = 286; yHMTL[6] = 253;
	xHMTL[7] = 280; yHMTL[7] = 253;
	xHMTL[8] = 280; yHMTL[8] = 250;
	xHMTL[9] = 244; yHMTL[9] = 250;
	xHMTL[10] = 244; yHMTL[10] = 253;
	xHMTL[11] = 234; yHMTL[9] = 253;
	drawPoly(xHMTL, yHMTL, 12);

	int xLIXA[6], yLIXA[6];
	xLIXA[0] = 234; yLIXA[0] = 259;
	xLIXA[1] = 269; yLIXA[1] = 259;
	xLIXA[2] = 269; yLIXA[2] = 273;
	xLIXA[3] = 250; yLIXA[3] = 273;
	xLIXA[4] = 250; yLIXA[4] = 282;
	xLIXA[5] = 234; yLIXA[5] = 282;
	drawPoly(xLIXA, yLIXA, 6);

	int xLIXC[4], yLIXC[4];
	xLIXC[0] = 272; yLIXC[0] = 259;
	xLIXC[1] = 304; yLIXC[1] = 259;
	xLIXC[2] = 304; yLIXC[2] = 273;
	xLIXC[3] = 272; yLIXC[3] = 273;
	drawPoly(xLIXC, yLIXC, 4);

	int xKotaKecil[4], yKotakKecil[4];
	xKotakKecil[0] = 267; yKotakKecil[0] = 274;
	xKotakKecil[1] = 275; yKotakKecil[1] = 274;
	xKotakKecil[2] = 275; yKotakKecil[2] = 281;
	xKotakKecil[3] = 267; yKotakKecil[3] = 281;
	drawPoly(xKotakKecil, yKotakKecil, 4);

	int xLIXB[4], yLIXB[4];
	xLIXB[0] = 272; yLIXB[0] = 281;
	xLIXB[1] = 304; yLIXB[1] = 281;
	xLIXB[2] = 304; yLIXB[2] = 295;
	xLIXB[3] = 272; yLIXB[3] = 295;
	drawPoly(xLIXB, yLIXB, 4);

	int xMerahB[4], yMerahB[4];
	xMerahB[0] = 243; yMerahB[0] = 282;
	xMerahB[1] = 269; yMerahB[1] = 282;
	xMerahB[2] = 269; yMerahB[2] = 295;
	xMerahB[3] = 242; yMerahB[3] = 295;
	drawPoly(xMerahB, yMerahB, 4);

	int xMerahK1[4], yMerahK1[4];
	xMerahK1[0] = 247; yMerahK1[0] = 298;
	xMerahK1[1] = 256; yMerahK1[1] = 298;
	xMerahK1[2] = 256; yMerahK1[2] = 311;
	xMerahK1[3] = 247; yMerahK1[3] = 311;
	drawPoly(xMerahK1, yMerahK1, 4);

	int xMerahK2[4], yMerahK2[4];
	xMerahK2[0] = 258; yMerah[0] = 298;
	xMerahK2[1] = 268; yMerah[1] = 298;
	xMerahK2[2] = 268; yMerah[2] = 311;
	xMerahK2[3] = 258; yMerah[3] = 311;
	drawPoly(xMerahK2, yMerahK2, 4);

	int xMerahK3[4], yMerahK3[4];
	xMerahK3[0] = 256; yMerahK3[0] = 312;
	xMerahK3[1] = 268; yMerahK3[1] = 312;
	xMerahK3[2] = 268; yMerahK3[2] = 320;
	xMerahK3[3] = 256; yMerahK3[3] = 320;
	drawPoly(xMerahK3, yMerahK3, 4);

	int xMerah[7], yMerah[7];
	xMerah[0] = 270; yMerah[0] = 300;
	xMerah[1] = 280; yMerah[1] = 300;
	xMerah[2] = 280; yMerah[2] = 306;
	xMerah[3] = 282; yMerah[3] = 306;
	xMerah[4] = 282; yMerah[4] = 314;
	xMerah[5] = 284; yMerah[5] = 314;
	xMerah[6] = 270; yMerah[6] = 325;
	drawPoly(xMerah, yMerah, 7);

	/** KELOMPOK 4 **/
	int xUMH[4], yUMH[4];
	xUMH[0] = 272; yUMH[0] = 177;
	xUMH[1] = 318; yUMH[1] = 177;
	xUMH[2] = 318; yUMH[2] = 164;
	xUMH[3] = 272; yUMH[3] = 164;
	drawPoly(xUMH, yUMH, 4);

	int xGKUTim[8], yGKUTim[8];
	xGKUTim[0] = 284; yGKUTim[0] = 180;
	xGKUTim[1] = 318; yGKUTim[1] = 180;
	xGKUTim[2] = 318; yGKUTim[2] = 195;
	xGKUTim[3] = 312; yGKUTim[3] = 196;
	xGKUTim[4] = 312; yGKUTim[4] = 192;
	xGKUTim[5] = 298; yGKUTim[5] = 192;
	xGKUTim[6] = 298; yGKUTim[6] = 196;
	xGKUTim[7] = 284; yGKUTim[7] = 196;
	drawPoly(xGKUTim, yGKUTim, 8);

	int xTGeo[4], yTGeo[4];
	xTGeo[0] = 308; yTGeo[0] = 197;
	xTGeo[1] = 321; yTGeo[1] = 197;
	xTGeo[2] = 321; yTGeo[2] = 212;
	xTGeo[3] = 308; yTGeo[3] = 212;
	drawPoly(xTGeo, yTGeo, 4);

	int xM[4], yM[4];
	xM[0] = 308; yM[0] = 212;
	xM[1] = 316; yM[1] = 212;
	xM[2] = 316; yM[2] = 225;
	xM[3] = 308; yM[3] = 225;
	drawPoly(xM, yM, 4);

	int xM2[12], yM2[12];
	xM2[0] = 310; yM2[0] = 228;
	xM2[1] = 316; yM2[1] = 228;
	xM2[2] = 316; yM2[2] = 225;
	xM2[3] = 321; yM2[3] = 225;
	xM2[4] = 321; yM2[4] = 230;
	xM2[5] = 318; yM2[5] = 230;
	xM2[6] = 318; yM2[6] = 233;
	xM2[7] = 321; yM2[7] = 233;
	xM2[8] = 321; yM2[8] = 236;
	xM2[9] = 316; yM2[9] = 236;
	xM2[10] = 316; yM2[10] = 234;
	xM2[11] = 310; yM2[11] = 233;
	drawPoly(xM2, yM2, 12);

	int xAP[4], yAP[4];
	xAP[0] = 268; yAP[0] = 200;
	xAP[1] = 297; yAP[1] = 200;
	xAP[2] = 297; yAP[2] = 225;
	xAP[3] = 268; yAP[3] = 225;
	drawPoly(xAP, yAP, 4);

	int xAP1[8], yAP1[8];
	xAP1[0] = 268; yAP1[0] = 183;
	xAP1[1] = 277; yAP1[1] = 183;
	xAP1[2] = 277; yAP1[2] = 177;
	xAP1[3] = 281; yAP1[3] = 177;
	xAP1[4] = 281; yAP1[4] = 182;
	xAP1[5] = 284; yAP1[5] = 183;
	xAP1[6] = 284; yAP1[6] = 195;
	xAP1[7] = 267; yAP1[7] = 196;
	drawPoly(xAP1, yAP1, 8);

	/** KELOMPOK 5 **/

	/** KELOMPOK 6 **/
	int xCCBar[4], yCCBar[4];
	xCCBar[0] = 143; yCCBar[0] = 240;
	xCCBar[1] = 174; yCCBar[1] = 240;
	xCCBar[2] = 143; yCCBar[2] = 251;
	xCCBar[3] = 174; yCCBar[3] = 251;
	drawPoly(xCCBar, yCCBar, 4);

	int xLapBas[4], yLapBas[4];
	xLapBas[0] = 146; yLapBas[0] = 256;
	xLapBas[1] = 170; yLapBas[1] = 256;
	xLapBas[2] = 146; yLapBas[2] = 287;
	xLapBas[3] = 170; yLapBas[3] = 287;
	drawPoly(xLapBas, yLapBas, 4);

	int xGedung[4], yGedung[4];
	xGedung[0] = 147; yGedung[0] = 288;
	xGedung[1] = 147; yGedung[1] = 304;
	xGedung[2] = 127; yGedung[2] = 304;
	xGedung[3] = 127; yGedung[3] = 288;
	drawPoly(xGedung, yGedung, 4);

	/** KELOMPOK 7 **/
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
	drawPoly(xTekSipil, yTekSipil, 14);

	int xHimafi[8], yHimafi[8];
	xHimafi[0] = 60; yHimafi[0] 233;
	xHimafi[1] = 125; yHimafi[1] 233;
	xHimafi[2] = 125; yHimafi[2] 277;
	xHimafi[3] = 89; yHimafi[3] 277;
	xHimafi[4] = 89; yHimafi[4] 269;
	xHimafi[5] = 118; yHimafi[5] 269;
	xHimafi[6] = 118; yHimafi[6] 266;
	xHimafi[7] = 60; yHimafi[7] 266;
	drawPoly(xHimafi, yHimafi, 8);

	/** KELOMPOK 8 **/

	/** KELOMPOK 9 **/

	/** KELOMPOK 10 **/

	/** KELOMPOK 11 **/

	/** KELOMPOK 12 **/

	/** KELOMPOK 13 **/
	int xA[4], yA[4];
	xA[0] = 30; yA[0] = 111;
	xA[1] = 78; yA[1] = 111;
	xA[2] = 78; yA[2] = 138;
	xA[3] = 30; yA[3] = 138;
	drawPoly(xA, yA, 4);

	int xB[4], yB[4];
	xB[0] = 30; yB[0] = 140;
	xB[1] = 55; yB[1] = 140;
	xB[2] = 55; yB[2] = 153;
	xB[3] = 30; yB[3] = 153;
	drawPoly(xB, yB, 4);

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
	drawPoly(xC, yC, 12);

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
	drawPoly(xD, yD, 11);

	/** KELOMPOK 14 **/

}
