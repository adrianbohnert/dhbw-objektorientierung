#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <forward_list>
#include <fstream>

#include "Map.h"
#include "Vektor2d.h"
using namespace std; 

// Simulationsgeschwindigkeit
const double DT = 100.0;
bool SteigendeFlanke(bool _Signal);




class GameWindow : public Gosu::Window
{
public:
	Gosu::Image Spielfigur;
	Gosu::Image Hintergrund;
	GameWindow()
		: Window(800, 600)
		, Spielfigur("Spielfigur.png")
		, Hintergrund("Hintergrund.jpg")

	{
		set_caption("Adriano Game");


	}
	

	// variablen

	double jump = 432;									//Startpunkt der Figur
	double high = 100;									//Sprungh�he	
	double down = 5;									//Falltiefe
	bool start = false;
	bool Flankemerker = false;
	double x_koordinate_Figur = 200 + 36.25;								//x_Korrdinate untere Rechte Ecke Bild
	double y_koordinate_Figur = jump + 34.25;								//y_Korrdinate untere Rechte Ecke Bild
	bool Tod = false;
	double v = 5;															//Durchlaufgeschwindigkeit
	double spielfeld = 450;													// y_Position des Spielfeldes
	double g��e_hindernisse = 40;											// Abst�nde der einzelnen koordinaten der Hindernisse
	

	forward_list<double> x_Koordinate_Dreiecke;
	forward_list<double> y_Koordinate_Dreiecke;
	vector<string> map;


	

	



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		///


		graphics().draw_quad(							// Untergrund
			0, 450, Gosu::Color::BLACK,
			0, 600, Gosu::Color::BLACK,
			800, 600, Gosu::Color::BLACK,
			800, 450, Gosu::Color::BLACK,
			0.0);
		


	

		for (auto x = 0; x < map.size(); x++)								//Die Spalten der Textdatei werden durchgegangen
		{
			
			for (auto y = 0; y <= map[x].size(); y++)						//Die Zeilen der Textdatei werden durchgegangen
			{


				switch (map[x][y])
					
				{
							case '>':			graphics().draw_triangle(								//Bildung von Dreiecken fals > in Textdatei
								x *g��e_hindernisse, y * g��e_hindernisse+spielfeld, Gosu::Color::BLACK,
								x * g��e_hindernisse  + 20, (y - 1) * 40 +spielfeld, Gosu::Color::BLACK,
								(x + 1) * g��e_hindernisse , y * g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
								0.0
							); break;

							case '|':			graphics().draw_quad(									//Bildung von Quadraten falls | in Textdatei
								x*g��e_hindernisse, y*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
								(x - 1)*g��e_hindernisse, (y )*g��e_hindernisse+spielfeld , Gosu::Color::BLACK,
								(x - 1)*g��e_hindernisse, (y-1)*g��e_hindernisse +spielfeld, Gosu::Color::BLACK,
								x*g��e_hindernisse, (y - 1)*g��e_hindernisse +spielfeld , Gosu::Color::BLACK,
								0.0
							); break;

				}
			}
				
		}
			
	



		Spielfigur.draw_rot(200, jump, 0.0, 0, 0.5, 0.5, 0.25, 0.25);						//Spielfigur
		Hintergrund.draw(0, 0, -1);															//Hintergrund



	}

	
	


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		ifstream f("C:\\Users\\adria\\Documents\\Studium\\3. Semester\\Informatik 3\\Spiel\\Spiel_online\\Beispielprojekt\\Level1.txt");
		string zeile;
		while (getline(f, zeile))
		{
			map.push_back(zeile);
		}





		double Space = input().down(Gosu::ButtonName::KB_SPACE);							//Einlesen der Leertaste -> SPRINGEN

		if (Space)																			//Flankenmerker zum Springen
		{
			Flankemerker = true;
		}
		else
		{
			Flankemerker = false;
		}




		if (SteigendeFlanke(Flankemerker) && jump == 432)										//Sp�ter durch Variable ersetzten die gesetzt wird, falls rechteck auf anderem Rechteck!!!!!
		{
			jump = jump - high;


		}



		if (jump != 432)
		{
			jump = jump + down;
		}

		/*x_Koordinate_Dreiecke.push_front(xd1);												//Vektor mit zahlen bef�llen
		y_Koordinate_Dreiecke.push_front(425);




		for (auto i = x_Koordinate_Dreiecke.begin(); i != x_Koordinate_Dreiecke.end(); i++)														// Bildung des Abstandes der Punkte
		{
			double x_differenz;
								x_differenz = *i-x_koordinate_Figur;
			double y_differnz;
								y_differnz = jump+17 -425  ;

			double betrag;
			betrag = sqrt(x_differenz*x_differenz + y_differnz * y_differnz);

			cout << betrag << endl;
			if (betrag <=1.3 )
			{
				cout << "Teste" << betrag << endl;
				Tod = true;
				break;
			}
		}*/





		bool Start = input().down(Gosu::ButtonName::KB_A);							//Einlesen der Entertaste ->Start des Bilddurchlaufes
		bool Stop = input().down(Gosu::ButtonName::KB_NUMPAD_PLUS);						//Einlesen der Entertaste ->Start des Bilddurchlaufes

		if (Start)
		{
			start = true;
		}

		if (Stop || Tod)																			//Stopmerker
		{
			start = false;
		}


		if (start)
		{
			for (auto x = 0; x < map.size(); x++)
			{
				for (auto y = 0; y <= map[x].size(); y++)						//Die Zeilen der Textdatei werden durchgegangen
				{

					map[x][y] - v;
				}
			}

			/*if (  != 0)																	//letzes Objekt auf Karte !!!
			{


				xq1 = xq1 - v;
				xq2 = xq2 - v;
				xq3 = xq3 - v;
				xq4 = xq4 - v;
				xq5 = xq5 - v;
				xq1_1 = xq1_1 - v;
				xq2_1 = xq2_1 - v;
				xq3_1 = xq3_1 - v;
				xq4_1 = xq4_1 - v;
				xq5_1 = xq5_1 - v;


				xd1 = xd1 - v;
				xd2 = xd2 - v;
				xd3 = xd3 - v;
				xd4 = xd4 - v;
				xd5 = xd5 - v;
				xd6 = xd6 - v;
				xd7 = xd7 - v;
				xd8 = xd8 - v;
				xd9 = xd9 - v;
				xd10 = xd10 - v;
				xd11 = xd11 - v;
				xd12 = xd12 - v;
				xd13 = xd13 - v;
				xd14 = xd14 - v;
				xd15 = xd15 - v;
				xd16 = xd16 - v;

				xd1_1 = xd1_1 - v;
				xd2_1 = xd2_1 - v;
				xd3_1 = xd3_1 - v;
				xd4_1 = xd4_1 - v;
				xd5_1 = xd5_1 - v;
				xd6_1 = xd6_1 - v;
				xd7_1 = xd7_1 - v;
				xd8_1 = xd8_1 - v;
				xd9_1 = xd9_1 - v;
				xd10_1 = xd10_1 - v;
				xd11_1 = xd11_1 - v;
				xd12_1 = xd12_1 - v;
				xd13_1 = xd13_1 - v;
				xd14_1 = xd14_1 - v;
				xd15_1 = xd15_1 - v;
				xd16_1 = xd16_1 - v;

				xd1_2 = xd1_2 - v;
				xd2_2 = xd2_2 - v;
				xd3_2 = xd3_2 - v;
				xd4_2 = xd4_2 - v;
				xd5_2 = xd5_2 - v;
				xd6_2 = xd6_2 - v;
				xd7_2 = xd7_2 - v;
				xd8_2 = xd8_2 - v;
				xd9_2 = xd9_2 - v;
				xd10_2 = xd10_2 - v;
				xd11_2 = xd11_2 - v;
				xd12_2 = xd12_2 - v;
				xd13_2 = xd13_2 - v;
				xd14_2 = xd14_2 - v;
				xd15_2 = xd15_2 - v;
				xd16_2 = xd16_2 - v;
			}
			else
			{
				xq1 = xq1 + 5000;
				xq2 = xq2 + 5000;
				xq3 = xq3 + 5000;
				xq4 = xq4 + 5000;
				xq5 = xq5 + 5000;
				xq1_1 = xq1_1 + 5000;
				xq2_1 = xq2_1 + 5000;
				xq3_1 = xq3_1 + 5000;
				xq4_1 = xq4_1 + 5000;
				xq5_1 = xq5_1 + 5000;


				xd1 = xd1 + 5000;
				xd2 = xd2 + 5000;
				xd3 = xd3 + 5000;
				xd4 = xd4 + 5000;
				xd5 = xd5 + 5000;
				xd6 = xd6 + 5000;
				xd7 = xd7 + 5000;
				xd8 = xd8 + 5000;
				xd9 = xd9 + 5000;
				xd10 = xd10 + 5000;
				xd11 = xd11 + 5000;
				xd12 = xd12 + 5000;
				xd13 = xd13 + 5000;
				xd14 = xd14 + 5000;
				xd15 = xd15 + 5000;
				xd16 = xd16 + 5000;

				xd1_1 = xd1_1 + 5000;
				xd2_1 = xd2_1 + 5000;
				xd3_1 = xd3_1 + 5000;
				xd4_1 = xd4_1 + 5000;
				xd5_1 = xd5_1 + 5000;
				xd6_1 = xd6_1 + 5000;
				xd7_1 = xd7_1 + 5000;
				xd8_1 = xd8_1 + 5000;
				xd9_1 = xd9_1 + 5000;
				xd10_1 = xd10_1 + 5000;
				xd11_1 = xd11_1 + 5000;
				xd12_1 = xd12_1 + 5000;
				xd13_1 = xd13_1 + 5000;
				xd14_1 = xd14_1 + 5000;
				xd15_1 = xd15_1 + 5000;
				xd16_1 = xd16_1 + 5000;

				xd1_2 = xd1_2 + 5000;
				xd2_2 = xd2_2 + 5000;
				xd3_2 = xd3_2 + 5000;
				xd4_2 = xd4_2 + 5000;
				xd5_2 = xd5_2 + 5000;
				xd6_2 = xd6_2 + 5000;
				xd7_2 = xd7_2 + 5000;
				xd8_2 = xd8_2 + 5000;
				xd9_2 = xd9_2 + 5000;
				xd10_2 = xd10_2 + 5000;
				xd11_2 = xd11_2 + 5000;
				xd12_2 = xd12_2 + 5000;
				xd13_2 = xd13_2 + 5000;
				xd14_2 = xd14_2 + 5000;
				xd15_2 = xd15_2 + 5000;
				xd16_2 = xd16_2 + 5000;
			}*/

		}





	}
};




// C++ Hauptprogramm
	int main()
	{
		GameWindow window;
		window.show();


	}


	bool SteigendeFlanke(bool _Signal)
	{
		static bool Flankenmerker;
		static bool Hilfsmerker;
		Flankenmerker = _Signal && !Hilfsmerker;
		Hilfsmerker = _Signal;
		return Flankenmerker;
	}