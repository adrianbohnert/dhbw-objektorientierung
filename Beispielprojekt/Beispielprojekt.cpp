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

class Dreieck
{
	public:
	double xl;
	double xr;
	double xo;
	double yu;
	double yo;
	

	Dreieck(double xlinks, double xrechts, double xoben, double yunten, double yoben) : xl(xlinks), xr(xrechts), xo(xoben), yu(yunten), yo(yoben) {}
};

class Viereck
{
public:
	double xl;
	double xr;
	double yu;
	double yo;

	Viereck(double xlinks, double xrechts, double yunten, double yoben) :xl(xlinks), xr(xrechts),  yu(yunten), yo(yoben) {}
 };


class GameWindow : public Gosu::Window
{
public:
	Gosu::Image Spielfigur, Hintergrund, Startbildschirm, Ende;
	Gosu::Song Spielsong;
	GameWindow()
		: Window(800, 600)
		, Spielfigur("Spielfigur_1.png")
		, Hintergrund("Hintergrund.jpg")
		, Startbildschirm("Startbildschirm.png")
		, Ende("Ende.png")
		, Spielsong("The Caribbean Theme Song.mp3")
	{
		set_caption("square Game");
	}


	//Koordinaten der Figur
	double jump = 430;	
	double x_koordinate_Figur = 210;									//x_Korrdinate Mitte Spielfigur
	double y_koordinate_Figur = 0;						//y_Korrdinate Mitte Spielfigur											//Startpunkt der Figur
	double high = 5;													//Sprungh�he	
	bool nach_unten = false;

	//Start und Stop
	bool start = false;
	bool reset=false;
	bool springen = false;
	bool crash = false;

	//Durchlaufgeschwindigkeit
	double v = 5;															
	int zaehler=0;
	int zaehler_v = 0;
	int z�hler_springen_hoch=0;
	int z�hler_springen_runter = 0;

	//Spielfeld und Hindernissgr��e
	double spielfeld = 450;												// y_Position des Spielfeldes
	double g��e_hindernisse = 40;										// Abst�nde der einzelnen koordinaten der Hindernisse
	bool hm_viereck=false;

	
	

	
	
	bool lesen = true;
	bool schleife = true;
	

	//Zum Anzeigen des Huptbildschirmes
	bool startbildschirm = false;

	//Score anzeige
	int punkte=0;

	//Mapvektor
	vector<string> map;
	
	//Vectoren der Koordinaten
	vector <Dreieck> dreieck;
	vector <Viereck> viereck;





	
	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{



		// Untergrund
		graphics().draw_quad(
			0, 450, Gosu::Color::BLACK,
			0, 600, Gosu::Color::BLACK,
			800, 600, Gosu::Color::BLACK,
			800, 450, Gosu::Color::BLACK,
			0.0);


		//Die Spalten der Textdatei werden durchgegangen
		for (auto x = 0; x < map.size(); x++)
		{
			
			//Die Zeilen der Textdatei werden durchgegangen
			for (auto y = 0; y < map[x].size(); y++)
			{

			
				switch (map[x][y])

				{
				case '>'://Bildung von Dreiecken fals > in Textdatei
					

				
					
					if (start == false)

					{
						if (schleife == true) // wird genau so oft aufgerufen wie Dreiecke in der Map sind
						{	

									dreieck.push_back(Dreieck(x*g��e_hindernisse, (x + 1)*g��e_hindernisse, x*g��e_hindernisse + 20, y*g��e_hindernisse + spielfeld, (y - 1)*g��e_hindernisse + spielfeld));
									zaehler++;
						}
						
						
						for (auto i = 1; i <= zaehler; i++)
						{
							graphics().draw_triangle(
								dreieck.at(i - 1).xl, dreieck.at(i - 1).yu, Gosu::Color::BLACK,
								dreieck.at(i - 1).xo, dreieck.at(i - 1).yo, Gosu::Color::BLACK,
								dreieck.at(i - 1).xr, dreieck.at(i - 1).yu, Gosu::Color::BLACK,
								0.0);
						}
						
					}

					if (start == true)
					{

						
						for (auto i = 0; i < zaehler; i++)

						{

							graphics().draw_triangle(								
								dreieck.at(i).xl, dreieck.at(i).yu, Gosu::Color::BLACK,
								dreieck.at(i).xo, dreieck.at(i).yo, Gosu::Color::BLACK,
								dreieck.at(i).xr, dreieck.at(i).yu, Gosu::Color::BLACK,
								0.0);

						}

					
					}

							break;


				case '|':
					if (start == false)

					{
						if (schleife == true) // wird genau so oft aufgerufen wie Vierecke in der Map sind
						{

						viereck.push_back(Viereck( x*g��e_hindernisse, (x + 1)*g��e_hindernisse, y*g��e_hindernisse + spielfeld, (y - 1)*g��e_hindernisse + spielfeld));
						zaehler_v++;
						}

						
						for (auto i = 1; i <= zaehler_v; i++)
						{
							graphics().draw_quad(
								viereck.at(i - 1).xl,viereck.at(i - 1).yu, Gosu::Color::BLACK,
								viereck.at(i - 1).xr, viereck.at(i - 1).yu, Gosu::Color::BLACK,
								viereck.at(i - 1).xl, viereck.at(i - 1).yo, Gosu::Color::BLACK,
								viereck.at(i - 1).xr, viereck.at(i - 1).yo, Gosu::Color::BLACK,
								0.0);
						}
					}

					if (start == true)
					{


						for (auto i = 0; i < zaehler_v; i++)

						{

							graphics().draw_quad(
								viereck.at(i).xl, viereck.at(i).yu, Gosu::Color::BLACK,
								viereck.at(i).xr, viereck.at(i).yu, Gosu::Color::BLACK,
								viereck.at(i).xl, viereck.at(i).yo, Gosu::Color::BLACK,
								viereck.at(i).xr, viereck.at(i).yo, Gosu::Color::BLACK,
								0.0);

						}


					}
								
						break;
								


				default: break;
				}
			}

			

		}

		schleife = false;



				//Spielfigur
				Spielfigur.draw_rot(200, jump, 0.0, 0, 0.5, 0.5, 1, 1);						
				//Hintergrund
				Hintergrund.draw(0, 0, -1);	
				//Startbildschirm
				if (startbildschirm == false)
				{
					Startbildschirm.draw(0, 0, 0, 1, 1);
				}

				if (crash)
				{
					Sleep(100);
					Ende.draw(0, 0, 0, 1, 1);
				}
	}


	


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		bool Start = input().down(Gosu::ButtonName::KB_S);						//Einlesen der "S" Taste ->Start des Bilddurchlaufes
		bool Stop = input().down(Gosu::ButtonName::KB_B);						//Einlesen der "B" ->Stop des Bilddurchlaufes
		bool Return = input().down(Gosu::ButtonName::KB_R);						// Einlesen der Taste "R" f�r Zur�cksetzen

		if (Start)
		{
			
			startbildschirm = true ;//Startbildschirm aufplotten lassen
			Sleep(100);
			start = true;
			
			cout << "schleife" << endl;
		}

		
			if (lesen) // wird genau ein mal ausgelesen	
			{
				//Einlesen der Levels
				ifstream f(".//Level1.txt");
				//ifstream f("C:\\Users\\adria\\Documents\\Studium\\3. Semester\\Informatik 3\\Spiel\\Eigenes Spiel\\Beispielprojekt\\Level1.txt");
				//ifstream f("H:\\Informatik\\Informatik 3\\dhbw-objektorientierung\\Beispielprojekt\\Level1.txt");


				string zeile;
				while (getline(f, zeile))
				{
					map.push_back(zeile);
				}
				lesen = false;
			}




			if (start == true)
			{


				for (auto i = dreieck.begin(); i != dreieck.end(); i++)
				{
					(i->xl) = (i->xl) - v;
					(i->xr) = (i->xr) - v;
					(i->xo) = (i->xo) - v;

				}

				for (auto i = viereck.begin(); i != viereck.end(); i++)
				{
					(i->xl) = (i->xl) - v;
					(i->xr) = (i->xr) - v;

				}
				Sleep(10);

			}

			//Einlesen der Springtaste ->Leertaste
			bool Springen = input().down(Gosu::ButtonName::KB_SPACE);


			if (Springen)
			{

				springen = true;
			}

			y_koordinate_Figur = jump + 10;
			if (springen && !nach_unten)
			{
				jump = jump - high;
				z�hler_springen_hoch = z�hler_springen_hoch + high;
				y_koordinate_Figur = jump + 10;
			}

			if (z�hler_springen_hoch == 90)
			{
				springen = false;
				nach_unten = true;
				z�hler_springen_hoch = 0;
			}

			if (nach_unten)
			{
				jump = jump + high;
				z�hler_springen_runter = z�hler_springen_runter + high;
				y_koordinate_Figur = jump + 10;
			}

			if (jump == 430)
			{
				nach_unten = false;
				z�hler_springen_runter = 0;
			}


			for (auto i = 0; i < viereck.size(); i++)
			{

				if (viereck.at(i).xl >= 220 || viereck.at(i).xr <= 200)
				{
					hm_viereck = false;
				}
				else
				{
					hm_viereck = true;
				}

				if (hm_viereck)
				{
					if (y_koordinate_Figur - 5 < 400 && y_koordinate_Figur >= 400)
					{
						nach_unten = false;

					}

					break;
				}
				else
				{

					if (jump == 390 && springen == false)
					{
						nach_unten = true;
					}

				}



			}


			double diffx = 400;
			double diffy = 400;

			for (auto i = 0; i < dreieck.size(); i++)							//Durchgehen des x-vectors und nach Diffdernz schauen
			{

				if ((dreieck.at(i).xo <= 230) && (dreieck.at(i).xo >= 190)) // 230 durch ausprobieren!
				{
					diffx = dreieck.at(i).xo - x_koordinate_Figur;


				}

			}


			for (auto i = 0; i < dreieck.size(); i++)							//Durchgehen des y-Vectors und nach differenz schauen
			{
				if ((dreieck.at(i).xo <= 230) && (dreieck.at(i).xo >= 190))
				{
					diffy = 430 - y_koordinate_Figur;							// Y Koordinate �ndert sich nicht 

				}
			}

			if ((abs(diffx < 10)) && (abs(diffy < 10)))
			{
				crash = true;
			}



			if (Stop || crash)																			//Stopmerker
			{
				start = false;
				Spielsong.stop();
			}


			if (start)
			{
				punkte++;
				Spielsong.play();
			}


			if (Return)
			{	
				reset = true;
			}

			if (reset)
			{
			
				dreieck.clear();
				viereck.clear();


				zaehler = 0;
				zaehler_v = 0;

				
				schleife = true;
				

				startbildschirm = false;
				diffx = 400;
				diffy = 400;
				crash = false;
				springen = false;
				nach_unten = false;
				z�hler_springen_hoch = 0;
				z�hler_springen_runter = 0;
				hm_viereck = false;
				jump = 430;


				reset = false;
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