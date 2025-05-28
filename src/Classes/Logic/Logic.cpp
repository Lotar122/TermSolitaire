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
	constexpr std::string_view arrow = "->";

	size_t arrowPosition = input.find(arrow);
	if (arrowPosition == std::string_view::npos) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: missing '->'.");

	if (input.find(arrow, arrowPosition + arrow.size()) != std::string_view::npos) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: multiple '->' found.");

	auto lhs = trim(input.substr(0, arrowPosition));
	auto rhs = trim(input.substr(arrowPosition + arrow.size()));

	if (lhs.empty()) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: left operand is empty.");
	if (rhs.empty()) throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: right operand is empty.");

	return std::make_pair(lhs, rhs);
}

std::tuple<std::string_view, std::string_view, std::string_view> Logic::parseCardRange(std::string_view range)
{
	auto openParanthesis = range.find('(');
	auto dash = range.find('-', openParanthesis);
	auto closeParanthesis = range.find(')', dash);

	if (
		openParanthesis == std::string_view::npos ||
		dash == std::string_view::npos ||
		closeParanthesis == std::string_view::npos ||
		openParanthesis >= dash || dash >= closeParanthesis
		)
	{
		throw std::system_error(std::error_code(1, std::generic_category()), "Parse error: invalid format, expected X(Y-Z)");
	}

	std::string_view x = range.substr(0, openParanthesis);
	std::string_view y = range.substr(openParanthesis + 1, dash - closeParanthesis - 1);
	std::string_view z = range.substr(dash + 1, closeParanthesis - dash - 1);

	return std::make_tuple(x, y, z);
}

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
				!rightHand->empty() &&
				cardsDifferentColors(leftHand->at(rangeStart - 1), rightHand->back())  &&
				rankSmallerByOne(leftHand->at(rangeStart - 1), rightHand->back())
			)
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);
				leftHand->erase(leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);

				if(!leftHand->empty()) leftHand->back()->up = true;
			}
			else if(leftHand->at(rangeStart - 1)->_rank() == CardRank::King && rightHand->empty())
			{
				rightHand->insert(rightHand->end(), leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);
				leftHand->erase(leftHand->begin() + rangeStart - 1, leftHand->begin() + rangeEnd);

				if(!leftHand->empty()) leftHand->back()->up = true;
			}
		}
	}
}