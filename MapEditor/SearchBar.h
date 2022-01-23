#pragma once

#include <memory>
#include <string>

#include "..\Rect.h"
#include "..\Texture.h"
#include "..\Tools\Font.h"

class SearchBar
{
public:
	SearchBar(Font* font);

	void AddLetter(char what);
	void BackSpace();
	void Erase();
	bool IsEmpty();
	std::string GetContent();

	void Draw();
	void Update();

private:
	Font* usedFont;

	Rect barRect;

	std::shared_ptr<Texture> textImage;
	Rect textRect = { 0 };
	std::string barContent = "";

};

