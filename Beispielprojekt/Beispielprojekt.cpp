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
	Gosu::Image Spielfigur, Hintergrund, Startbildschirm;
	Gosu::Song Spielsong;
	GameWindow()
		: Window(800, 600)
		, Spielfigur("Spielfigur_1.png")
		, Hintergrund("Hintergrund.jpg")
		, Startbildschirm("Startbildschirm.png")
		, Spielsong("The Caribbean Theme Song.mp3")
	{
		set_caption("square Game");
	}


	//Koordinaten der Figur
	double x_koordinate_Figur = 210;									//x_Korrdinate Mitte Spielfigur
	double y_koordinate_Figur = jump - 10;								//y_Korrdinate Mitte Spielfigur
	double jump = 430;													//Startpunkt der Figur
	double high = 60;													//Sprunghöhe	
	double down = 5;													//Falltiefe
	
	//Start und Stop
	bool start = false;
	bool springen = false;
	bool Tod = false;
	bool crash = false;

	//Durchlaufgeschwindigkeit
	double v = 0.1;															
	double run = 0;
	int zaehler=0;
	int zaehler_v = 0;

	//Spielfeld und Hindernissgröße
	double spielfeld = 450;												// y_Position des Spielfeldes
	double göße_hindernisse = 40;										// Abstände der einzelnen koordinaten der Hindernisse
	

	
	
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

									dreieck.push_back(Dreieck(x*göße_hindernisse, (x + 1)*göße_hindernisse, x*göße_hindernisse + 20, y*göße_hindernisse + spielfeld, (y - 1)*göße_hindernisse + spielfeld));
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
								viereck.at(i ).xl, viereck.at(i).yu, Gosu::Color::BLACK,
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

				/*	for (auto i = 0; i < xd.size(); i++)
					{

					}
				*/


				//Spielfigur
				Spielfigur.draw_rot(200, jump, 0.0, 0, 0.5, 0.5, 1, 1);						
				//Hintergrund
				Hintergrund.draw(0, 0, -1);	
				//Startbildschirm
				if (startbildschirm == false)
				{
					Startbildschirm.draw(0, 0, 0, 1, 1);
				}


	}


	


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		if (lesen) // wird genau ein mal ausgelesen
		{
			//Einlesen der Levels
			//ifstream f("C:\\Users\\sofia\\source\\repos\\dhbw-objektorientierung\\Beispielprojekt\\Level1.txt");
			ifstream f("C:\\Users\\adria\\Documents\\Studium\\3. Semester\\Informatik 3\\Spiel\\Eigenes Spiel\\Beispielprojekt\\Level1.txt");
			
			string zeile;
			while (getline(f, zeile))
			{
				map.push_back(zeile);
			}
			lesen = false;
		}


		
		
		//Einlesen der Springtaste ->Leertaste
		bool Springen = input().down(Gosu::ButtonName::KB_SPACE);							
	
		if (Springen)																			
		{
			springen = true;
		}
		else
		{
			springen = false;
		}
		
		//Flankenmerker zum Springen
		if (SteigendeFlanke(springen))
		{
			jump = jump - high;
		}

		if(jump !=430)
		{
			jump = jump + down;
		}

		
		
		
		
		

		double diffx=400;
		double diffy=400;

		for (auto i = 0; i < dreieck.size(); i++)							//Durchgehen des x-vectors und nach Diffdernz schauen
		{
			 
			if (dreieck.at(i).xo<= 230) // 230 durch ausprobieren!
			{
				diffx = dreieck.at(i).xo - x_koordinate_Figur;
				cout << "x_differenz: " << diffx << endl; ;
			
			}
			
		}

		
		for (auto i = 0; i < dreieck.size(); i++)							//Durchgehen des y-Vectors und nach differenz schauen
		{
			if (dreieck.at(i).xo <= 230)
			{
				diffy = 430 - y_koordinate_Figur;							// Y Koordinate ändert sich nicht 
				cout << "y_differenz: " << diffy << endl; ;
				
			}
		}

		if ((diffx < 40) && (diffy < 40))
		{
			crash = true;
		}


		if (crash)
		{
			cout << "tod" << endl;
		}



		
			bool Start = input().down(Gosu::ButtonName::KB_S);						//Einlesen der "S" Taste ->Start des Bilddurchlaufes
			bool Stop = input().down(Gosu::ButtonName::KB_B);						//Einlesen der "B" ->Stop des Bilddurchlaufes

			if (Start)
			{
				start = true;
			}

			if (start == false || crash == true)
			{
				run = 0;
			}
			if (start == true)
			{
				
					run = run + v; // Run wird immer größer, somit wird geschwindigkeit höher.. Wollen wir das ? wenn nicht minus v 
					for (auto i = dreieck.begin(); i != dreieck.end(); i++)
					{
						(i->xl) = (i->xl) - run; // wird im run nach links verschoben und in Vektor gespeichert
						(i->xr) = (i->xr) - run;
						(i->xo) = (i->xo) - run;

					}

					for (auto i = viereck.begin(); i != viereck.end(); i++)
					{
						(i->xl) = (i->xl) - run;
						(i->xr) = (i->xr) - run;

					}
					Sleep(10);
				
			}

			

			if (Stop || crash)																			//Stopmerker
			{
				start = false;
			}
			
			//Startbildschirm aufplotten lassen
			if (Start)
			{
				startbildschirm = true;
			}
			
			if (start)
			{
				punkte ++;
				Spielsong.play();
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