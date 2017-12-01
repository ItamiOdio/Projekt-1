// Projekt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <algorithm>
#include <cmath>

int arrayHSV[3]; //Tablica do przechowywania zmiennych HSV
int arrayYUV[3]; //Tablica do przechowywania zmiennych YUV
int arrayYUVRGB[3]; //Tablica do zmiennych przekonwertowanych z YUV do RGB
int arrayHSVRGB[3]; //Tablica do zmiennych przekonwertowanych z HSV do RGB

// Obliczanie wartosci HSV na podstawie RGB
int *RGBtoHSV(int R, int G, int B) // jako parametry skladowe RGB
{
	int Hue;
	int Saturation;
	int Value;
	int X;
	int Val;

	Val = std::max(std::max(R, G), B);
	X = std::min(std::min(R, G), B);

	if (Val == X)
	{
		Hue = 0;
	}
	else
	{
		if (R == Val)
		{
			Hue = ((G - B) * 60) / (Val - X);
		}
		else
		{
			if (G == Val)
			{
				Hue = 120 + (((B - R) * 60) / (Val - X));
			}
			else
			{
				Hue = 240 + (((R - G) * 60) / (Val - X));
			}
		}
	}
	if (Hue < 0)
	{
		Hue = Hue + 360;
	}

	if (Val == X)
	{
		Saturation = 0;
	}
	else
	{
		Saturation = ((Val - X) * 100) / Val;
	}

	Value = (100 * Val) / 255;

	arrayHSV[0] = Hue;
	arrayHSV[1] = Saturation;
	arrayHSV[2] = Value;

	return arrayHSV;
}

// Obliczanie wartosci YUV na podstawie RGB
int *RGBtoYUV(int R, int G, int B) // jako parametry skladowe RGB
{
	int Y;
	int U;
	int V;

	Y = R *  .299000 + G *  .587000 + B *  .114000;
	U = R * -.168736 + G * -.331264 + B *  .500000 + 128;
	V = R *  .500000 + G * -.418688 + B * -.081312 + 128;

	arrayYUV[0] = Y;
	arrayYUV[1] = U;
	arrayYUV[2] = V;

	return arrayYUV;
}

//Obliczanie wartosci RGB na podstawie HSV
int *HSVtoRGB(float H, float S, float V) //jako parametry skladowe HSV
{
	float R;
	float G;
	float B;

	float i;
	float f;
	float p;
	float q;
	float t;

	S /= 100;
	V /= 100;
	H /= 60;
	i = floor(H);
	f = H - i;
	p = V*(1 - S);
	q = V*(1 - (S*f));
	t = V*(1 - (S*(1 - f)));

	if (i == 0)
	{
		R = V;
		G = t;
		B = p;
	}
	else if (i == 1)
	{
		R = q;
		G = V;
		B = p;
	}
	else if (i == 2)
	{
		R = p;
		G = V;
		B = t;
	}
	else if (i == 3) { R = p; G = q; B = V; }
	else if (i == 4) { R = t; G = p; B = V; }
	else if (i == 5) { R = V; G = p; B = q; }

	arrayHSVRGB[0] = R * 255;
	arrayHSVRGB[1] = G * 255;
	arrayHSVRGB[2] = B * 255;
	return arrayHSVRGB;
}

//Obliczanie wartosci RGB na podstawie YUV
int *YUVtoRGB(int Y, int U, int V) //jako parametry skladowe YUV
{
	int R;
	int G;
	int B;

	R = Y + 1.4075 * (V - 128);
	G = Y - 0.3455 * (U - 128) - (0.7169 * (V - 128));
	B = Y + 1.7790 * (U - 128);

	if (R<0)
	{
		R = 0;
	}

	if (G<0)
	{
		G = 0;
	}

	if (B<0)
	{
		B = 0;
	}

	if (R>255)
	{
		R = 255;
	}

	if (G>255)
	{
		G = 255;
	}

	if (B>255)
	{
		B = 255;
	}

	arrayYUVRGB[0] = R;
	arrayYUVRGB[1] = G;
	arrayYUVRGB[2] = B;

	return arrayYUVRGB;
}
int main(int argc, char * argv[])
{
	if (argc == 3)
	{
		float a, b, c; //Zmienne kolejnych skladowych w pliku
		std::string przestrzen; //Zmienna dla nazwy przestrzeni w pliku
		std::string finalfile = argv[2]; //definicja pliku wejsciowego
		std::string startfile = argv[1]; //definicja pliku wyjsciowego
		std::cout << "---------------------------------------------------------" << std::endl;
		std::cout << "Konwersja kolorow miedzy przestrzeniami RGB, HSV oraz YUV" << std::endl;
		std::cout << "---------------------------------------------------------" << std::endl << std::endl;

		std::fstream start_file; //plik wejsciowy
		std::fstream final_file; //plik wyjsciowy
		start_file.open(startfile, std::ios_base::in);
		if (!start_file.good())
		{
			return false;
		}
		while (true)
		{
			start_file >> przestrzen >> a >> b >> c;
			if (start_file.good())
			{
				if (przestrzen == "RGB") // Operacje dla przestrzeni RGB
				{
					if (a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255) //Warunek prawidlowosci wprowadzonych skladowych w przestrzeni RGB
					{
						int *HSV;
						int *YUV;
						std::cout << "Dane wejsciowe: RGB: (" << a << ", " << b << ", " << c << ")" << std::endl;
						HSV = RGBtoHSV(a, b, c);
						std::cout << "HSV: (" << *HSV << ", " << *(HSV + 1) << ", " << *(HSV + 2) << ")" << std::endl;
						YUV = RGBtoYUV(a, b, c);
						std::cout << "YUV: (" << *YUV << ", " << *(YUV + 1) << ", " << *(YUV + 2) << ")" << std::endl << std::endl;

						final_file.open(finalfile, std::ios_base::out | std::ios_base::app);

						final_file << "Dane wejsciowe: RGB: (" << a << ", " << b << ", " << c << ")" << std::endl;
						final_file << "HSV: (" << *HSV << ", " << *(HSV + 1) << ", " << *(HSV + 2) << ")" << std::endl;
						final_file << "YUV: (" << *YUV << ", " << *(YUV + 1) << ", " << *(YUV + 2) << ")" << std::endl << std::endl;

						final_file.close();
					}
					else
					{
						std::cout << "Nieprawidlowe skladowe dla przestrzeni RGB!" << std::endl << std::endl; //W przypadku bledu
					}
				}
				else if (przestrzen == "HSV") // W przypadku przestrzeni HSV - operacje
				{
					if (a >= 0 && a <= 360 && b >= 0 && b <= 100 && c >= 0 && c <= 100) //Warunek prawidlowych skaldowych dla HSV
					{
						int *HSVRGB;
						int *YUV;
						HSVRGB = HSVtoRGB(a, b, c);
						std::cout << "Dane wejsciowe: HSV: (" << a << ", " << b << ", " << c << ")" << std::endl;
						std::cout << "RGB: (" << *HSVRGB << ", " << *(HSVRGB + 1) << ", " << *(HSVRGB + 2) << ")" << std::endl;
						YUV = RGBtoYUV(*HSVRGB, *(HSVRGB + 1), *(HSVRGB + 2));
						std::cout << "YUV: (" << *YUV << ", " << *(YUV + 1) << ", " << *(YUV + 2) << ")" << std::endl << std::endl;

						final_file.open(finalfile, std::ios_base::out | std::ios_base::app);
						final_file << "Dane wejsciowe: HSV: (" << a << ", " << b << ", " << c << ")" << std::endl;
						final_file << "RGB: (" << *HSVRGB << ", " << *(HSVRGB + 1) << ", " << *(HSVRGB + 2) << ")" << std::endl;
						final_file << "YUV: (" << *YUV << ", " << *(YUV + 1) << ", " << *(YUV + 2) << ")" << std::endl << std::endl;

						final_file.close();
					}
					else
					{
						std::cout << "Nieprawidlowe skladowe dla przestrzeni HSV!" << std::endl << std::endl; //Komunikat o nieprawidlowosci danych dla przestrzeni
					}
				}
				else if (przestrzen == "YUV")
				{
					if (a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255) //Warunek prawidlowych danych dla YUV
					{
						int *YUVRGB;
						int *HSV;
						YUVRGB = YUVtoRGB(a, b, c);
						std::cout << "Dane wejsciowe: YUV: (" << a << ", " << b << ", " << c << ")" << std::endl;
						std::cout << "RGB: (" << *YUVRGB << ", " << *(YUVRGB + 1) << ", " << *(YUVRGB + 2) << ")" << std::endl;
						HSV = RGBtoHSV(*YUVRGB, *(YUVRGB + 1), *(YUVRGB + 2));
						std::cout << "HSV: (" << *HSV << ", " << *(HSV + 1) << ", " << *(HSV + 2) << ")" << std::endl << std::endl;

						final_file.open(finalfile, std::ios_base::out | std::ios_base::app);

						final_file << "Dane wejsciowe: YUV: (" << a << ", " << b << ", " << c << ")" << std::endl;
						final_file << "RGB: (" << *YUVRGB << ", " << *(YUVRGB + 1) << ", " << *(YUVRGB + 2) << ")" << std::endl;
						final_file << "HSV: (" << *HSV << ", " << *(HSV + 1) << ", " << *(HSV + 2) << ")" << std::endl << std::endl;

						final_file.close();
					}
					else
					{
						std::cout << "Nieprawidlowe skladowe dla przestrzeni YUV!" << std::endl << std::endl; //Komunikat blednych danych dla YUV
					}
				}
				else
				{
					std::cout << "Nieznana przestrzen!" << std::endl << std::endl; // Komunikat w przypadku blednej przestrzeni kolorow
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "Uruchomienie tego programu wymaga wprowadzenia pliku wejsciowego i wyjsciowego!";
	}
	return 0;
}


