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
	Gosu::Image Spielfigur, Hintergrund, Startbildschirm, Level_geschafft,Ende, Level;
	Gosu::Song Spielsong;
	
	GameWindow()
		: Window(800, 600)
		, Spielfigur("Spielfigur_1.png")
		, Hintergrund("Hintergrund.jpg")
		, Startbildschirm("Startbildschirm.png")
		, Ende("Ende.png")
		, Spielsong("The Caribbean Theme Song.mp3")
		, Level_geschafft("Level_geschafft.png")
		, Level ("Levelauswahl.png")
		
	{
		set_caption("square Game");
	}


	//Koordinaten der Figur
	double jump = 430;	
	double x_koordinate_Figur = 210;									//x_Korrdinate Mitte Spielfigur
	double y_koordinate_Figur = 0;						//y_Korrdinate Mitte Spielfigur											//Startpunkt der Figur
	double high = 5;													//Sprunghöhe	
	bool nach_unten = false;

	//Start und Stop
	bool start = false;
	bool reset=false;

	bool springen = false;
	bool crash = false;
	
	bool level1 = false; // true wenn Level 1 ausgewählt 
	bool level2 = false; // true wenn Level 2 ausgewählt

	//Zum anzeigen der Levelauswahl
	bool levelauswahl = true;

	//Durchlaufgeschwindigkeit
	double v = 5;	

	int zaehler=0; // Zaehler der Dreiecke in Map
	int zaehler_v = 0; // Zaehler der Vierecke in Map
	int zähler_springen_hoch=0;
	int zähler_springen_runter = 0;

	//Spielfeld und Hindernissgröße
	double spielfeld = 450;												// y_Position des Spielfeldes
	double göße_hindernisse = 40;										// Abstände der einzelnen koordinaten der Hindernisse
	bool hm_viereck=false;

	
	
	bool lesen = true; // Karte wird ausgelesen wenn true
	
	

	//Zum (nicht) Anzeigen des Hauptbildschirmes
	bool startbildschirm = true; // true wenn Bild nicht angezeigt!

	//Score anzeige
	int punkte=0;

	//Mapvektor
	vector<string> map;
	
	//Vectoren der Koordinaten
	vector <Dreieck> dreieck;
	vector <Viereck> viereck;




	bool schleife = true; // Vektoren werden bestückt solange true
	bool merker = false; // Schleife darf nur false gesetzt werden, wenn bereits die Map ausgelesen wurde, der Merker signalisiert das wenn true
	
						 
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
					

				
					
					if (start == false )

					{
						if (schleife == true) // wird genau so oft aufgerufen wie Dreiecke in der Map sind
						{	

									dreieck.push_back(Dreieck(x*göße_hindernisse, (x + 1)*göße_hindernisse, x*göße_hindernisse + 20, y*göße_hindernisse + spielfeld, (y - 1)*göße_hindernisse + spielfeld));
									zaehler++;
									cout << "Test" << endl;
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

						viereck.push_back(Viereck( x*göße_hindernisse, (x + 1)*göße_hindernisse, y*göße_hindernisse + spielfeld, (y - 1)*göße_hindernisse + spielfeld));
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
		
		if (merker) // nur wenn Map bereits ausgelesen wurde
		{
			schleife = false;
		}



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
				
				if (levelauswahl)
				{
					Level.draw(0, 0, 0, 1, 1);
				}

				/*for (auto i = dreieck.begin(); i != dreieck.end(); i++)
					if (dreieck.at(i + 1)xr)
				{
					Level_geschafft.draw(0, 0, 0, 1, 1);
				}*/

	}


	


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		bool Start = input().down(Gosu::ButtonName::KB_S);						//Einlesen der "S" Taste ->Start des Bilddurchlaufes
		bool Stop = input().down(Gosu::ButtonName::KB_B);						//Einlesen der "B" ->Stop des Bilddurchlaufes
		bool Return = input().down(Gosu::ButtonName::KB_R);						// Einlesen der Taste "R" für Zurücksetzen
		bool L1 = input().down(Gosu::ButtonName::KB_1);							// Wenn Taste 1, dann Level 1
		bool L2 = input().down(Gosu::ButtonName::KB_2);							// Wenn Taste 2, dann Level 2

		if (L1)
		{
			level1 = true; 
		}

		if (L2)
		{
			level2 = true;
		}

		if (Start)
		{
			
			startbildschirm = true;//Spiel beginnt
			Sleep(100);
			start = true;
			
		}

			//Einlesen der Levels
		
			if (lesen) // wird genau ein mal ausgelesen	
			{
				if (level1)
				{
					
					ifstream f(".//Level1.txt");

					string zeile;
					while (getline(f, zeile))
					{
						map.push_back(zeile);
					}
					

					levelauswahl = false;
					startbildschirm = false;
					lesen = false;
					merker = true; // Check, ob Karte ausgelesen wurde
				}

				if (level2)
				{
			
					ifstream f(".//Level2.txt");
					
					string zeile;
					while (getline(f, zeile))
					{
						map.push_back(zeile);
					}
					

					levelauswahl = false;
					startbildschirm = false;
					lesen = false;
					merker = true; // Check, ob Karte ausgelesen wurde
					
				}
			
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
				zähler_springen_hoch = zähler_springen_hoch + high;
				y_koordinate_Figur = jump + 10;
			}

			if (zähler_springen_hoch == 90)
			{
				springen = false;
				nach_unten = true;
				zähler_springen_hoch = 0;
			}

			if (nach_unten)
			{
				jump = jump + high;
				zähler_springen_runter = zähler_springen_runter + high;
				y_koordinate_Figur = jump + 10;
			}

			if (jump == 430)
			{
				nach_unten = false;
				zähler_springen_runter = 0;
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
					diffy = 430 - y_koordinate_Figur;							// Y Koordinate ändert sich nicht 

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
				
				//punkte ++;
				//cout << punkte << endl;
				Spielsong.play();
				//Sleep(1);
			}


			if (Return)
			{	
				reset = true;
			}

			if (reset)
			{

				// Leeren der Vektoren, sonst Performanceprobleme
				map.clear();
				dreieck.clear();
				viereck.clear();


				zaehler = 0;
				zaehler_v = 0;

				
				schleife = true;
				lesen = true;
				merker = false;

				// Es wird die Levelauswahl angezeigt, wenn Return
				startbildschirm = true;
				levelauswahl = true;

				level1 = false;
				level2 = false;

				diffx = 400;
				diffy = 400;
				crash = false;
				springen = false;
				nach_unten = false;
				zähler_springen_hoch = 0;
				zähler_springen_runter = 0;
				hm_viereck = false;
				jump = 430;

				// wird so nur ein Mal ausgeführt
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