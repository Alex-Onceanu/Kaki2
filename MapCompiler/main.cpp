#include <iostream>
#include <fstream>
#include <sstream>

#include "..\Tools\Map.h"

using namespace std;

void write_map_header(ofstream& f)
{
	const char * pass{ "KAKI2" };
	f.write(pass, strlen(pass));
	int key = 2;
	f.write((char*)&key, sizeof(key));
}

bool IsEmpty(ifstream& file)
{
	return file.peek() == ifstream::traits_type::eof();
}

int main(int argc, char** argv)
{	
	Map map;

	string inputPath, outputPath;

	if (argc == 1)
	{
		cout << "Input path: " << endl;
		getline(cin, inputPath);
		cin.clear();
		cout << "Output path: " << endl;
		getline(cin, outputPath);
		cin.clear();
	}
	else if (argc == 3)
	{
		inputPath = string (argv[1]);
		outputPath = string(argv[2]);
	}
	else
	{
		cout << "Arguments doivent etre de la forme <input path> <output path> et pas " << argv[0]
			<< "\n\nAppuyez sur entree pour quitter" << endl;
		cin.get();
		cin.clear();
		return 2;
	}

	MapFile input(inputPath.c_str());
	
	if (not input.is_open())
	{
		cout << "Fichier \"" << inputPath << "\" n'a pas pu etre ouvert\n\nAppuyez sur entree pour quitter" << endl;
		cin.get();
		cin.clear();
		return 1;
	}

	if (IsEmpty(input))
	{
		cout << "Fichier \"" << inputPath << "\" introuvable ou vide\n\nAppuyez sur entree pour quitter" << endl;
		cin.get();
		cin.clear();
		return 1;
	}

	cout << "\nCompilation en cours..." << endl;

	input >> map;

	input.close();

	ofstream output(outputPath.c_str(), ios::binary);

	if (not output.is_open())
	{
		cout << "Fichier \"" << inputPath << "\" n'a pas pu etre ouvert\n\nAppuyez sur entree pour quitter" << endl;
		cin.get();
		cin.clear();
		return 1;
	}

	write_map_header(output);
	output << map;

	output.close();

	ifstream test(outputPath.c_str(), ios::binary);

	if (IsEmpty(test))
	{
		cout << "La compilation a echoue.\n\nAppuyez sur entree pour quitter" << endl;
		cin.get();
		cin.clear();
		return 1;
	}

	cout << "Compilation terminee.\n\nAppuyez sur entree pour quitter" << endl;
	cin.get();
	cin.clear();

	return 0;
}