#pragma once

#include <ranges>
#include <algorithm>
#include <string>
#include <vector>
#include <charconv>
#include <system_error>

#include "Classes/Card/Card.hpp"

class Logic
{
	static std::string_view trim(std::string_view stringView);
	static std::pair<std::string_view, std::string_view> parseInput(std::string_view input);
	static std::pair<std::string_view, std::string_view> parseCardCount(std::string_view range);

	static inline bool cardsDifferentColors(Card* leftHand, Card* rightHand)
	{
		return leftHand->isRed() != rightHand->isRed();
	}

	static inline bool rankSmallerByOne(Card* leftHand, Card* rightHand)
	{
		return static_cast<int>(leftHand->_rank()) == static_cast<int>(rightHand->_rank()) - 2;
	}

	static inline bool rankBiggerByOne(Card* leftHand, Card* rightHand)
	{
		return static_cast<int>(leftHand->_rank()) - 2 == static_cast<int>(rightHand->_rank());
	}
public:
    static void handle(
		std::string& input,
        std::vector<std::vector<Card*>>& mainPiles,
        std::vector<std::vector<Card*>>& sidePiles
    );
};