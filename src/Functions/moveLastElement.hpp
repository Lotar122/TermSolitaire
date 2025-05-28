#pragma once

#include <vector>

#include "Concepts/StdVector.hpp"

template<StdVector Vec>
static void moveLastElement(Vec& leftHand, Vec& rightHand)
{
	if (leftHand.empty()) throw std::runtime_error("The leftHand vector was empty, no element to move.");
    rightHand.push_back(leftHand.back());
	leftHand.erase(std::prev(leftHand.end()));
}

template<StdVector Vec>
static void moveLastElement(Vec* leftHand, Vec* rightHand)
{
	if (leftHand->empty()) throw std::runtime_error("The leftHand vector was empty, no element to move.");
    rightHand->push_back(leftHand->back());
	leftHand->erase(std::prev(leftHand->end()));
}