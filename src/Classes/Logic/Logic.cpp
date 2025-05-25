#include "Logic.hpp"

#include "Classes/Game/Game.hpp"
#include "Functions/moveLastElement.hpp"

void Logic::handle(
	std::string& input,
	std::vector<std::vector<Card*>>& mainPiles,
	std::vector<std::vector<Card*>>& sidePiles
)
{
	auto parsedInput = parseInput(input);

	//if the second operand contains any of these characters its invalid syntax therefore we should not do anyhing;
	if(
		std::ranges::find(parsedInput.second, '(') != parsedInput.second.end() || 
		std::ranges::find(parsedInput.second, ')') != parsedInput.second.end() ||
		std::ranges::find(parsedInput.second, '-') != parsedInput.second.end()
	)
	{ return; }

	std::vector<Card*>* leftHand = nullptr;
	std::vector<Card*>* rightHand = nullptr;

	int value;
	auto [ptr, ec] = std::from_chars(parsedInput.second.data(), parsedInput.second.data() + parsedInput.second.size(), value);
	if(ec == std::errc())
	{
		if(value <= mainPiles.size()) rightHand = &mainPiles[value - 1];
	}
	else 
	{
		switch(parsedInput.second[0])
		{
			// you cant PUT cards onto the E stack
			// case 'E':
			// 	rightHand = &sidePiles[(int)SidePileEnum::E];
			// 	break;
			case 'A':
				rightHand = &sidePiles[(int)SidePileEnum::A];
				break;
			case 'B':
				rightHand = &sidePiles[(int)SidePileEnum::B];
				break;
			case 'C':
				rightHand = &sidePiles[(int)SidePileEnum::C];
				break;
			case 'D':
				rightHand = &sidePiles[(int)SidePileEnum::D];
				break;
		}
	}

	if(
		std::ranges::find(parsedInput.first, '(') == parsedInput.first.end() &&
		std::ranges::find(parsedInput.first, ')') == parsedInput.first.end() &&
		std::ranges::find(parsedInput.first, '-') == parsedInput.first.end()
	)
	{
		int value;
		auto [ptr, ec] = std::from_chars(parsedInput.first.data(), parsedInput.first.data() + parsedInput.first.size(), value);
		if(ec == std::errc())
		{
			if(value <= mainPiles.size()) leftHand = &mainPiles[value - 1];
		}
		else 
		{
			switch(parsedInput.first[0])
			{
				case 'E':
					leftHand = &sidePiles[(int)SidePileEnum::E];
					break;
				case 'A':
					leftHand = &sidePiles[(int)SidePileEnum::A];
					break;
				case 'B':
					leftHand = &sidePiles[(int)SidePileEnum::B];
					break;
				case 'C':
					leftHand = &sidePiles[(int)SidePileEnum::C];
					break;
				case 'D':
					leftHand = &sidePiles[(int)SidePileEnum::D];
					break;
			}
		}

		if(leftHand && rightHand)
		{
			if(
				!leftHand->empty() && !rightHand->empty() && 
				std::ranges::find_if(sidePiles, [&rightHand](const std::vector<Card*>& v){ return rightHand == &v; }) == sidePiles.end() &&
				leftHand->back()->isRed() != rightHand->back()->isRed() &&
				(int)leftHand->back()->_rank() == ((int)rightHand->back()->_rank() - 2)
			)
			{
				moveLastElement(leftHand, rightHand);
				if(!leftHand->empty()) leftHand->back()->up = true;
			}
			else if(
				leftHand->size() > 0 &&
				std::ranges::find_if(sidePiles, [&rightHand](const std::vector<Card*>& v){ return rightHand == &v; }) != sidePiles.end()
			)
			{
				if(rightHand->empty() && leftHand->back()->_rank() == CardRank::Ace)
				{
					moveLastElement(leftHand, rightHand);
					if(!leftHand->empty()) leftHand->back()->up = true;
				}
				else if(
					!rightHand->empty() &&
					((int)leftHand->back()->_rank() - 2) == (int)rightHand->back()->_rank() &&
					leftHand->back()->_color() == rightHand->back()->_color()
				)
				{
					moveLastElement(leftHand, rightHand);
					if(!leftHand->empty()) leftHand->back()->up = true;
				}
			}
			else if(
				leftHand->size() > 0 && rightHand->size() == 0 &&
				leftHand->back()->_rank() == CardRank::King
			)
			{
				moveLastElement(leftHand, rightHand);
				if(!leftHand->empty()) leftHand->back()->up = true;
			}
		}
	}
	else
	{
		auto parsedRange = parseCardRange(parsedInput.first);
		size_t leftHandIndex = -1, rangeStart = -1, rangeEnd = -1;

		int value;
		auto result = std::from_chars(std::get<0>(parsedRange).data(), std::get<0>(parsedRange).data() + std::get<0>(parsedRange).size(), value);
		if(result.ec == std::errc())
		{
			leftHandIndex = value;
		}

		result = std::from_chars(std::get<1>(parsedRange).data(), std::get<1>(parsedRange).data() + std::get<1>(parsedRange).size(), value);
		if(result.ec == std::errc())
		{
			rangeStart = value;
		}

		result = std::from_chars(std::get<2>(parsedRange).data(), std::get<2>(parsedRange).data() + std::get<2>(parsedRange).size(), value);
		if(result.ec == std::errc())
		{
			rangeEnd = value;
		}

		if(leftHandIndex != -1 && rangeStart != -1 && rangeEnd != -1) leftHand = &mainPiles[leftHandIndex - 1];
		else return;

		if(rangeEnd != leftHand->size()) return;

		if(leftHand && rightHand && rangeEnd <= leftHand->size() && rangeStart >= 1 && leftHand->back()->up)
		{
			if(
				rightHand->size() > 0 &&
				leftHand->at(rangeStart - 1)->isRed() != rightHand->back()->isRed() &&
				(int)leftHand->at(rangeStart - 1)->_rank() == (int)rightHand->back()->_rank() - 2
			)
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);
				leftHand->erase(leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);

				if(leftHand->size() > 0) leftHand->back()->up = true;
			}
			else if(leftHand->at(rangeStart - 1)->_rank() == CardRank::King && rightHand->size() == 0)
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);
				leftHand->erase(leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);

				if(leftHand->size() > 0) leftHand->back()->up = true;
			}
		}
	}
}