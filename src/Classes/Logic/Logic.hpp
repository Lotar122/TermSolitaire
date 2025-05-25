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
    static std::string_view trim(std::string_view stringView) {
		auto isNotSpace = [](char c) { return !std::isspace(static_cast<unsigned char>(c)); };

		auto begin = std::find_if(stringView.begin(), stringView.end(), isNotSpace);
		auto end = std::find_if(stringView.rbegin(), stringView.rend(), isNotSpace).base();

		if (begin >= end) return {};

		size_t startOffset = static_cast<size_t>(begin - stringView.begin());
		size_t length = static_cast<size_t>(end - begin);

		return std::string_view(stringView.data() + startOffset, length);
	}

	static std::pair<std::string_view, std::string_view> parseInput(std::string_view input) 
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

	static std::tuple<std::string_view, std::string_view, std::string_view> parseCardRange(std::string_view range)
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
public:
    static void handle(
		std::string& input,
        std::vector<std::vector<Card*>>& mainPiles,
        std::vector<std::vector<Card*>>& sidePiles
    );
};