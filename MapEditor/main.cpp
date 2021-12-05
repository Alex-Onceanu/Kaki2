#define SDL_MAIN_HANDLED

#include "MapEditor.h"
#include "..\Renderer.h"


#include <iostream>
#include <string>
#include <memory>
#include <tuple>

using namespace std;

void FlushCin()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

unsigned int GetMapWidth()
{
	int resultat;
	cout << "Quelle taille horizontale (en pixels) de map ? (nombre entier compris entre 100 et 10000)\nx = ";
	cin >> resultat;
	FlushCin();

	if (cin.fail() or resultat <= 100 or resultat > 10000)
	{
		cout << "Merci d'entrer un nombre entier compris entre 100 et 10000.\n" << endl;
		return GetMapWidth();
	}

	return unsigned(std::abs(resultat));
}

unsigned int GetMapHeight()
{
	int resultat;
	cout << "\nQuelle taille verticale (en pixels) de map ? (nombre entier compris entre 100 et 10000)\ny = ";
	cin >> resultat;
	FlushCin();

	if (cin.fail() or resultat <= 100 or resultat > 10000)
	{
		cout << "Merci d'entrer un nombre entier compris entre 100 et 10000." << endl;
		return GetMapHeight();
	}

	return unsigned(std::abs(resultat));
}

unsigned int GetMapTileSize()
{
	int resultat;
	cout << "\nQuelle longueur pour un carreau de sol (en pixels)? Generalement 128 ou 64 (nombre entier compris entre 10 et 300)\ntile_size = ";
	cin >> resultat;
	FlushCin();

	if (cin.fail() or resultat <= 10 or resultat > 300)
	{
		cout << "Merci d'entrer un nombre entier compris entre 10 et 300.\n" << endl;
		return GetMapTileSize();
	}

	return unsigned(std::abs(resultat));
}

void Frame(MapEditor* m)
{
	m->ProcessInput();
	m->Update();
	m->Draw();
	Renderer::Flip();
}


int main()
{
	const unsigned int map_w = GetMapWidth();
	const unsigned int map_h = GetMapHeight();
	const unsigned int tile_size = GetMapTileSize();
	
	try
	{
		unique_ptr<MapEditor> app = make_unique<MapEditor>(map_w, map_h, tile_size);

		while (app->running)
		{
			Frame(app.get());
		}
	}
	catch (const exception e)
	{
		ofstream log("log.txt", ios_base::app);
		log << e.what() << endl;
		return -1;
	}

	return 0;
}