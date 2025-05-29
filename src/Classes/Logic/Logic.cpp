#include "Logic.hpp"

#include "Classes/Game/Game.hpp"
#include "Functions/moveLastElement.hpp"

std::string_view Logic::trim(std::string_view stringView)
{
	auto isNotSpace = [](char c) { return !std::isspace(static_cast<unsigned char>(c)); };

	auto begin = std::find_if(stringView.begin(), stringView.end(), isNotSpace);
	auto end = std::find_if(stringView.rbegin(), stringView.rend(), isNotSpace).base();

	if (begin >= end) return {};

	size_t startOffset = static_cast<size_t>(begin - stringView.begin());
	size_t length = static_cast<size_t>(end - begin);

	return std::string_view(stringView.data() + startOffset, length);
}

std::pair<std::string_view, std::string_view> Logic::parseInput(std::string_view input)
{
	constexpr std::string_view arrow = ">";

	size_t arrowPosition = input.find(arrow);
	if (arrowPosition == std::string_view::npos) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: missing '>'.");

	if (input.find(arrow, arrowPosition + arrow.size()) != std::string_view::npos) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: multiple '>' found.");

	auto lhs = trim(input.substr(0, arrowPosition));
	auto rhs = trim(input.substr(arrowPosition + arrow.size()));

	if (lhs.empty()) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: left operand is empty.");
	if (rhs.empty()) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: right operand is empty.");

	return std::make_pair(lhs, rhs);
}

std::pair<std::string_view, std::string_view> Logic::parseCardCount(std::string_view leftHand)
{
	auto openParanthesis = leftHand.find('(');
	auto closeParanthesis = leftHand.find(')', openParanthesis);

	if (
		openParanthesis == std::string_view::npos ||
		closeParanthesis == std::string_view::npos ||
		openParanthesis >= closeParanthesis
		)
	{
		throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: invalid format, expected X(Y-Z)");
	}

	std::string_view x = leftHand.substr(0, openParanthesis);
	std::string_view y = leftHand.substr(openParanthesis + 1, closeParanthesis - openParanthesis - 1);

	return std::make_pair(x, y);
}

void Logic::handle(
	std::string& input,
	std::vector<std::vector<Card*>>& mainPiles,
	std::vector<std::vector<Card*>>& sidePiles
)
{
	std::pair<std::string_view, std::string_view> parsedInput = parseInput(input);

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
			case 'a':
				rightHand = &sidePiles[(int)SidePileEnum::A];
				break;
			case 'b':
				rightHand = &sidePiles[(int)SidePileEnum::B];
				break;
			case 'c':
				rightHand = &sidePiles[(int)SidePileEnum::C];
				break;
			case 'd':
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
				case 'e':
					leftHand = &sidePiles[(int)SidePileEnum::E];
					break;
				case 'a':
					leftHand = &sidePiles[(int)SidePileEnum::A];
					break;
				case 'b':
					leftHand = &sidePiles[(int)SidePileEnum::B];
					break;
				case 'c':
					leftHand = &sidePiles[(int)SidePileEnum::C];
					break;
				case 'd':
					leftHand = &sidePiles[(int)SidePileEnum::D];
					break;
			}
		}

		if(leftHand && rightHand)
		{
			if(
				!leftHand->empty() && !rightHand->empty() && 
				std::ranges::find_if(sidePiles, [&rightHand](const std::vector<Card*>& v){ return rightHand == &v; }) == sidePiles.end() &&
				cardsDifferentColors(leftHand->back(), rightHand->back()) &&
				rankSmallerByOne(leftHand->back(), rightHand->back())
			)
			{
				moveLastElement(leftHand, rightHand);
				if(!leftHand->empty()) leftHand->back()->up = true;
			}
			else if(
				!leftHand->empty() &&
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
					rankBiggerByOne(leftHand->back(), rightHand->back()) &&
					leftHand->back()->_color() == rightHand->back()->_color()
				)
				{
					moveLastElement(leftHand, rightHand);
					if(!leftHand->empty()) leftHand->back()->up = true;
				}
			}
			else if(
				!leftHand->empty() && rightHand->empty() &&
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
		std::pair<std::string_view, std::string_view> parsedCardCount = parseCardCount(parsedInput.first);
		int64_t leftHandIndex = -1, cardCount = -1;

		int value;
		auto result = std::from_chars(parsedCardCount.first.data(), parsedCardCount.first.data() + parsedCardCount.first.size(), value);
		if (result.ec == std::errc())
		{
			leftHandIndex = value;
		}

		result = std::from_chars(parsedCardCount.second.data(), parsedCardCount.second.data() + parsedCardCount.second.size(), value);
		if (result.ec == std::errc())
		{
			cardCount = value;
		}

		if (leftHandIndex != -1 && cardCount != -1) leftHand = &mainPiles[leftHandIndex - 1];
		else return;

		if (leftHand && rightHand && leftHand->size() >= cardCount && leftHand->back()->up)
		{
			if (
				!rightHand->empty() && !leftHand->empty() &&
				cardsDifferentColors(leftHand->at(leftHand->size() - cardCount), rightHand->back()) &&
				rankSmallerByOne(leftHand->at(leftHand->size() - cardCount), rightHand->back())
			)
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + leftHand->size() - cardCount, leftHand->begin() + leftHand->size());
				leftHand->erase(leftHand->begin() + leftHand->size() - cardCount, leftHand->begin() + leftHand->size());

				if (!leftHand->empty()) leftHand->back()->up = true;
			}
			else if (leftHand->at(leftHand->size() - cardCount)->_rank() == CardRank::King && rightHand->empty())
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + leftHand->size() - cardCount, leftHand->begin() + leftHand->size());
				leftHand->erase(leftHand->begin() + leftHand->size() - cardCount, leftHand->begin() + leftHand->size());

				if (!leftHand->empty()) leftHand->back()->up = true;
			}
		}
	}
}