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
	double high = 60;									//Sprunghöhe	
	double down = 5;									//Falltiefe
	bool start = false;
	bool Flankemerker = false;
	double x_koordinate_Figur = 210;									//x_Korrdinate Mitte Spielfigur
	double y_koordinate_Figur = jump-10;								//y_Korrdinate Mitte Spielfigur
	bool Tod = false;
	double v = 0.4;															//Durchlaufgeschwindigkeit
	double spielfeld = 450;													// y_Position des Spielfeldes
	double göße_hindernisse = 40;											// Abstände der einzelnen koordinaten der Hindernisse
	bool crash = false;
	double run = 0;
	
	

	
	vector<double>x_crash;												//x_werte von der Mitte der Dreicke
	vector<double>y_crash;				
	vector<string> map;
	vector<double>xd;
	vector<double>yd;
	vector<double>xr;
	vector< double>yr;







	
	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		


		graphics().draw_quad(							// Untergrund
			0, 450, Gosu::Color::BLACK,
			0, 600, Gosu::Color::BLACK,
			800, 600, Gosu::Color::BLACK,
			800, 450, Gosu::Color::BLACK,
			0.0);

		
		

			for (auto x = 0; x < map.size(); x++)								//Die Spalten der Textdatei werden durchgegangen
			{
				

					for (auto y = 0; y < map[x].size(); y++)						//Die Zeilen der Textdatei werden durchgegangen
					{

						switch (map[x][y])

						{
						case '>':	xd.push_back(x*göße_hindernisse);
									xd.push_back((x + 1)*göße_hindernisse);
									yd.push_back(y + göße_hindernisse);
									yd.push_back((y - 1)*göße_hindernisse);
									
							/*graphics().draw_triangle(								//Bildung von Dreiecken fals > in Textdatei
								(testx1), testy1 + spielfeld, Gosu::Color::BLACK,
								(testx2)+20, testy2 + spielfeld, Gosu::Color::BLACK,
								(testx1), testy1 + spielfeld, Gosu::Color::BLACK,
								0.0);*/


							//x_crash.push_back(x * göße_hindernisse+20);				//Beschreiben x-vector mit werten
							//y_crash.push_back((y - 1) * 20 + spielfeld);			//Beschreiben y_vector mit werten


							break;


						//case '|':

							/*graphics().draw_quad(									//Bildung von Quadraten falls | in Textdatei
							(x - run)*göße_hindernisse, y*göße_hindernisse + spielfeld, Gosu::Color::BLACK,
							((x - run) + 1)*göße_hindernisse, (y)*göße_hindernisse + spielfeld, Gosu::Color::BLACK,
							((x - run) + 1)*göße_hindernisse, (y - 1)*göße_hindernisse + spielfeld, Gosu::Color::BLACK,
							(x - run)*göße_hindernisse, (y - 1)*göße_hindernisse + spielfeld, Gosu::Color::BLACK,
							0.0
						);*/

							//break;


						}
					}

			}


			for (auto i = 0; i < xd.size(); i++)
			{

			}
		
			graphics().draw_triangle(								//Bildung von Dreiecken fals > in Textdatei
				(testx1), testy1 + spielfeld, Gosu::Color::BLACK,
				(testx2)+20, testy2 + spielfeld, Gosu::Color::BLACK,
				(testx1), testy1 + spielfeld, Gosu::Color::BLACK,
				0.0);
		

		Spielfigur.draw_rot(200, jump, 0.0, 0, 0.5, 0.5, 1, 1);						//Spielfigur
		Hintergrund.draw(0, 0, -1);															//Hintergrund



	}





	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (start)
		{
			
			//run = run +v;
			testx1 = testx1 - 5;
			testx2 = testx2 - 5;
		}
		if(start==false)
		{
			run = 0;
		}


		//ifstream f("C:\\Users\\sofia\\source\\repos\\dhbw-objektorientierung\\Beispielprojekt\\Level1.txt");
		ifstream f("C:\\Users\\adria\\Documents\\Studium\\3. Semester\\Informatik 3\\Spiel\\Eigenes Spiel\\Beispielprojekt\\Level1.txt");
		string zeile;
		while (getline(f, zeile))
		{
			map.push_back(zeile);
			
		}

		





		double Space = input().down(Gosu::ButtonName::KB_SPACE);							//Einlesen der Leertaste -> SPRINGEN

		/*if (Space)																			//Flankenmerker zum Springen
		{
			
			Flankemerker = true;
		}
		else
		{
			Flankemerker = false;
		}
		
		bool top = false;
	
		if (SteigendeFlanke(Flankemerker))
		{
			jump = jump - high;
		}
		if(jump !=430)
		{
			jump = jump + down;
		}

		/*if (SteigendeFlanke(Flankemerker)&&!top)
		{
			for (auto i = 0; i < high; i++)
			{
				
				jump=jump-1;

				if (i == high-1)
				{
					top = true;
					cout << "top" << endl;
				}
				
				cout << jump << endl;
			}
			Sleep(5000);
			
		}
		
		if (top)
		{
			
			Sleep(500);
			for (auto i = 0; i < high; i++)
			{
				jump++;
				if (i == high - 1)
				{
					top = true;
					cout << "bottom" << endl;
				}
			}
			
		}*/
		
		
		/*
		if (SteigendeFlanke(Flankemerker) && !top)
		{ 
			double test;
			double val;
			for (int i = 0; i <= 2500; i++)
			{
				val = i*0.01 + 0.01;
				test = ((val - sqrt(70))*(val - sqrt(70)) +360);
				
				if (test <  450)
				{
					cout << test << endl;
					jump = test;
				}
			}
		}*/

		/*

		if (SteigendeFlanke(Flankemerker) && !top)
		{


			for (int i = 430; i > 360; i--)
			{
				jump = i;

			}

			for (int i = 360; i < 430; i++)
			{
				jump = i;

			}
		}
		*/


		double diffx=400;
		double diffy=400;

		for (auto i = 0; i < x_crash.size(); i++)							//Durchgehen des x-vectors und nach Diffdernz schauen
		{
			diffx = x_crash[i] - x_koordinate_Figur;
			//cout << "x_differenz: " << diffx << endl; ;
			
		}

		for (auto i = 0; i < y_crash.size(); i++)							//Durchgehen des y-Vectors und nach differenz schauen
		{
			diffy = y_crash[i] - y_koordinate_Figur;
			//cout << "y_differenz: " << diffy << endl; ;
						
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