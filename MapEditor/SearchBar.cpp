#include "SearchBar.h"

#include "..\Resolution.h"
#include "..\Renderer.h"


SearchBar::SearchBar(Font* font)
	: usedFont(font)
{
	barRect = { RES_X - 300,RES_Y - 110,270,80 };
}

void SearchBar::AddLetter(char what)
{
	if (barContent.length() >= 13) return;
	barContent += what;
	Update();
}

void SearchBar::BackSpace()
{
	if (barContent.empty()) return;
	barContent.pop_back();
	Update();
}

void SearchBar::Erase()
{
	barContent.clear();
	Update();
}

bool SearchBar::IsEmpty()
{
	return barContent.empty();
}

std::string SearchBar::GetContent()
{
	return barContent;
}

void SearchBar::Update()
{
	if (this->IsEmpty())
	{
		textImage.reset();
		return;
	}
	textImage.reset();
	textImage = Renderer::LoadTextTexture(barContent, usedFont, 255, 255, 255, 64 * barContent.length());
	textRect = Renderer::GetRect(textImage.get());
	textRect.x = RES_X - 280;
	textRect.y = RES_Y - 90;
}

void SearchBar::Draw()
{
	Renderer::DrawFillRect(barRect, 24, 154, 211);
	if (textImage.get() != nullptr)
	{
		Renderer::FullBlit(textImage.get(), textRect);
	}
}