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
	Gosu::Image Startbildschirm;
	GameWindow()
		: Window(800, 600)
		, Spielfigur("Spielfigur_1.png")
		, Hintergrund("Hintergrund.jpg")
		, Startbildschirm("Startbildschirm.png")

	{
		set_caption("Adriano Game");


	}


	// variablen

	double jump = 430;									//Startpunkt der Figur
	double high = 60;									//Sprungh�he	
	double down = 5;									//Falltiefe
	bool start = false;
	bool Flankemerker = false;
	double x_koordinate_Figur = 210;									//x_Korrdinate Mitte Spielfigur
	double y_koordinate_Figur = jump-10;								//y_Korrdinate Mitte Spielfigur
	bool Tod = false;
	double v = 0.4;															//Durchlaufgeschwindigkeit
	double spielfeld = 450;													// y_Position des Spielfeldes
	double g��e_hindernisse = 40;											// Abst�nde der einzelnen koordinaten der Hindernisse
	bool crash = false;
	double run = 0;
	
	

	
	vector<double>x_crash;												//x_werte von der Mitte der Dreicke
	vector<double>y_crash;	

	//Mapvektor
	vector<string> map;
	





	
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
				case '>':

					graphics().draw_triangle(								//Bildung von Dreiecken fals > in Textdatei
						(x - run)*g��e_hindernisse, y*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						(x - run)*g��e_hindernisse + 20, (y - 1)*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						((x - run) + 1)*g��e_hindernisse, y*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						0.0);


					x_crash.push_back(x * g��e_hindernisse+20);				//Beschreiben x-vector mit werten
					y_crash.push_back((y - 1) * 20 + spielfeld);			//Beschreiben y_vector mit werten


					break;


				case '|':

					graphics().draw_quad(									//Bildung von Quadraten falls | in Textdatei
						(x - run)*g��e_hindernisse, y*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						((x - run) + 1)*g��e_hindernisse, (y)*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						((x - run) + 1)*g��e_hindernisse, (y - 1)*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						(x - run)*g��e_hindernisse, (y - 1)*g��e_hindernisse + spielfeld, Gosu::Color::BLACK,
						0.0
					);
					break;




				}
			}
			
		}


			
		
			
		

		Spielfigur.draw_rot(200, jump, 0.0, 0, 0.5, 0.5, 1, 1);						//Spielfigur
		Hintergrund.draw(0, 0, -1);															//Hintergrund



	}





	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//Einlesen der Levels
		//ifstream f("C:\\Users\\sofia\\source\\repos\\dhbw-objektorientierung\\Beispielprojekt\\Level1.txt");
		ifstream f("C:\\Users\\adria\\Documents\\Studium\\3. Semester\\Informatik 3\\Spiel\\Eigenes Spiel\\Beispielprojekt\\Level1.txt");
		string zeile;
		while (getline(f, zeile))
		{
			map.push_back(zeile);

		}


		if (start)
		{
			
			run = run +v;
			
		}
		if(start==false)
		{
			run = 0;
		}

	

		//Einlesen der Springtaste ->Leertaste
		double Space = input().down(Gosu::ButtonName::KB_SPACE);							
		
		
		if (Space)																			
		{
			
			Flankemerker = true;
		}
		else
		{
			Flankemerker = false;
		}
		
		//Flankenmerker zum Springen
		if (SteigendeFlanke(Flankemerker))
		{
			jump = jump - high;
		}
		if(jump !=430)
		{
			jump = jump + down;
		}

		
		
		
		


		double diffx=400;
		double diffy=400;

		for (auto i = 0; i < x_crash.size(); i++)							//Durchgehen des x-vectors und nach Diffdernz schauen
		{
			if (x_crash[i] <= 800)
			{
				diffx = x_crash[i] - x_koordinate_Figur;
				cout << "x_differenz: " << diffx << endl; ;

			}
			
		}

		for (auto i = 0; i < y_crash.size(); i++)							//Durchgehen des y-Vectors und nach differenz schauen
		{
			if (x_crash[i] <= 800)
			{
				diffy = y_crash[i] - y_koordinate_Figur;
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




			bool Start = input().down(Gosu::ButtonName::KB_A);							//Einlesen der Entertaste ->Start des Bilddurchlaufes
			bool Stop = input().down(Gosu::ButtonName::KB_B);						//Einlesen der Entertaste ->Start des Bilddurchlaufes

			if (Start)
			{
				start = true;
			}

			if (Stop || crash)																			//Stopmerker
			{
				start = false;
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