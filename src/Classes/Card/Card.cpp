#include "Card.hpp"

#include "Classes/Game/Game.hpp"

Card::Card(Game* game, CardColor _color, CardRank _rank, bool _up)
{
	color = _color;
	rank = _rank;
	up = _up;

	auto cit = game->cardSpriteManager.find(encodeBytePair((uint8_t)color, (uint8_t)rank));
	if(cit == game->cardSpriteManager.end())
	{
		cardSprite = new char32_t[45];
		game->cardSpriteManager.insert(std::make_pair(encodeBytePair((uint8_t)color, (uint8_t)rank), cardSprite));

		uint8_t xCounter = 0;
		for(int i = 0; i < 45; i++)
		{
			if(cardMap[i] == U'X')
			{
				cardSprite[i] = rankSymbols[(int)rank + (xCounter >= 2 ? 1 : 0)][xCounter % 2];
				xCounter++;
			}
			else if(cardMap[i] == U'Y')
			{
				cardSprite[i] = colorSymbols[(int)color];
			}
			else
			{
				cardSprite[i] = cardMap[i];
			}
		}
	}
	else
	{
		cardSprite = cit->second;
	}
}