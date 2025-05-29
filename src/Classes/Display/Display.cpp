#include "Display.hpp"

Display::Display()
{
	screenDataLump = new char32_t[screenMapSize];
	screen = new char32_t* [screenMapHeight];

	for (size_t i = 0; i < screenMapHeight; i++)
	{
		screen[i] = screenDataLump + (screenMapWidth * i);
	}

	for (size_t x = 0; x < screenMapWidth; x++)
		for (size_t y = 0; y < screenMapHeight; y++)
		{
			screen[y][x] = screenMap[y * screenMapWidth + x];
		}
}

void Display::render(
	std::vector<std::vector<Card*>>& mainPiles,
	std::vector<std::vector<Card*>>& sidePiles,
	uint16_t score, uint16_t moves
)
{
	for(int i = 0; i < 7; i++)
	{
		mainPileCounters[i] = 0;
	}

	for(size_t x = 0; x < screenMapWidth; x++)
	for(size_t y = 0; y < screenMapHeight; y++)
	{
		if(screenMap[y * screenMapWidth + x] == U'~') screen[y][x] = U' ';
		for(size_t i = 0; i < mainPiles.size(); i++)
		{
			renderCardFromPile(mainPiles[i], i, x, y);
		}

		for(size_t i = 0; i < sidePiles.size(); i++)
		{
			if(sidePiles[i].size() > 0) renderCard(sidePileSymbols[i], sidePiles[i][sidePiles[i].size() - 1], x, y);
			else renderCard(sidePileSymbols[i], nullptr, x, y);
		}
	}

	clearScreen();
	printUTF32(screenDataLump, screenMapSize);
}

void Display::renderCardFromPile(const std::vector<Card*>& pile, uint8_t pileID, size_t x, size_t y)
{
	if(screenMap[y * screenMapWidth + x] == mainPileScreenMapSymbols[pileID])
	{
		if(pile.size() == 0)
		{
			for(int xi = x; xi < x + cardWidth; xi++)
			{
				for(int yi = y; yi < y + cardHeight; yi++)
				{
					screen[yi][xi] = U' ';
				}
			}
		}
		else if(mainPileCounters[pileID] < pile.size())
		{
			for(int xi = x; xi < x + cardWidth; xi++)
			{
				for(int yi = y; yi < y + cardHeight; yi++)
				{
					screen[yi][xi] = pile[mainPileCounters[pileID]]->getSpriteChar(xi - x, yi - y);
				}
			}     
		}
		else
		{
			for(int xi = x; xi < x + cardWidth; xi++)
			{
				for(int yi = mainPileCounters[pileID] == 18 ? y : y + cardHeight - 2; yi < y + cardHeight && yi < screenMapHeight; yi++)
				{
					screen[yi][xi] = U' ';
				}
			}  
		}

		mainPileCounters[pileID]++;
	}
}

void Display::renderCard(char32_t symbol, Card* card, size_t x, size_t y)
{
	if(screenMap[y * screenMapWidth + x] == symbol && card)
	{
		for(int xi = x; xi < x + cardWidth; xi++)
		{
			for(int yi = y; yi < y + cardHeight; yi++)
			{
				screen[yi][xi] = card->getSpriteChar(xi - x, yi - y);
			}
		}
	}
	else if(screenMap[y * screenMapWidth + x] == symbol)
	{
		for(int xi = x; xi < x + cardWidth; xi++)
		{
			for(int yi = y; yi < y + cardHeight; yi++)
			{
				screen[yi][xi] = cardEmptySprite[(yi - y) * cardWidth + (xi - x)];
			}
		}
	}
}