#include "Display.hpp"

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

		// renderInteger<uint16_t>(U'O', moves, x, y, 3);
		// renderInteger<uint16_t>(U'P', score, x, y, 3);
		// renderInteger<uint8_t>(U'Q', minutes, x, y, 2);
		// renderInteger<uint8_t>(U'R', seconds, x, y, 2);
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
				for(int yi = mainPileCounters[pileID] == 18 ? y : y + cardHeight - 2; yi < y + cardHeight; yi++)
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
				screen[yi][xi] = cardBackSprite[(yi - y) * cardWidth + (xi - x)];
			}
		}
	}
}

template<Integer T>
void Display::renderInteger(char32_t symbol, T number, size_t x, size_t y, size_t lenght)
{
	if(screenMap[y * screenMapWidth + x] == symbol)
	{
		//finish the rendering code
		//the max lenght is 3 only that much is handled
			
		//extracts the c in abc
		//0x30 is the unicode for 0
		screen[y][x + lenght - 1] = (char32_t)(0x30 + number - ((number / 10) * 10));
		if((number - ((number / 100) * 100)) / 10)
		{
			//extracts the b in abc
			//0x30 is unicode for 0
			screen[y][x + lenght - 2] = (char32_t)(0x30 + ((number - ((number / 100) * 100)) / 10));
		}
		else
		{
			screen[y][x + lenght - 2] = U' ';
		}
		if(static_cast<T>((number / 100)))
		{
			//extracts the a in abc
			//0x30 is unicode for 0x30
			screen[y][x] = (char32_t)(0x30 + (number / 100));
		}
		else
		{
			screen[y][x] = U' ';
		}
	}
}