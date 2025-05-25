#pragma once

#include <vector>

#include "Concepts/StdVector.hpp"

template<StdVector Vec>
static void moveLastElement(Vec& leftHand, Vec& rightHand)
{
    rightHand.push_back(leftHand.back());
	leftHand.erase(std::prev(leftHand.end()));
}

template<StdVector Vec>
static void moveLastElement(Vec* leftHand, Vec* rightHand)
{
    rightHand->push_back(leftHand->back());
	leftHand->erase(std::prev(leftHand->end()));
}