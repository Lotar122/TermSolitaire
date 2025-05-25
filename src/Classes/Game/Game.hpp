#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <random>
#include <ranges>
#include <algorithm>
#include <cstring>

#include "Classes/Display/Display.hpp"
#include "Classes/Input/Input.hpp"
#include "Classes/Card/Card.hpp"
#include "Classes/Logic/Logic.hpp"

#include "Structs/BytePair.hpp"

enum class SidePileEnum : uint8_t
{
    Draw = 0,
    
    ePile = 1,
    aPile = 2,
    bPile = 3,
    cPile = 4,
    dPile = 5,

    E = 1,
    A = 2,
    B = 3,
    C = 4,
    D = 5
};

class Game
{
    Display display;
    Input input;

    std::vector<Card> deck;
    std::vector<Card*> shuffledDeck;
    std::vector<std::vector<Card*>> mainPiles = { {}, {}, {}, {}, {}, {}, {} };
    std::vector<std::vector<Card*>> sidePiles = { {}, {}, {}, {}, {}, {} };
    std::vector<Card*>* drawStack = &sidePiles[0];

    std::random_device randomDevice;
    std::mt19937 randomEngine;

    uint16_t score = 0, moves = 0;
public:
    std::unordered_map<BytePair, char32_t*> cardSpriteManager;

    Game()
    {
        randomEngine = std::mt19937(randomDevice());

        //Generate the deck, sorted and shuffled
        deck.reserve(52);
        shuffledDeck.reserve(52);

        for(size_t r = 0; r < cardRankEnumLenght; r++)
        for(size_t c = 0; c < cardColorEnumLenght; c++)
        {
            deck.push_back(Card(this, (CardColor)c, (CardRank)(r * 2), false));  
            shuffledDeck.push_back(&deck[deck.size() - 1]); 
        }

        std::ranges::shuffle(shuffledDeck, randomEngine);

        int shuffledDeckIndex = 0;
        for(int i = 0; i < 7; i++)
        {
            for(int j = 0; j < i + 1; j++)
            {
                mainPiles[i].push_back(shuffledDeck[shuffledDeckIndex]);
                if(j == i) mainPiles[i][j]->up = true;
                shuffledDeckIndex++;
            }
        }

        drawStack->resize(shuffledDeck.size() - shuffledDeckIndex);
        std::memcpy(drawStack->data(), shuffledDeck.data() + shuffledDeckIndex, drawStack->size() * sizeof(Card*));
    }

    void update()
    {
        if(input._inputBuffer() == std::string("draw") || input._inputBuffer() == std::string("Draw"))
        {
            if(drawStack->size() > 0)
            {
                sidePiles[1].push_back(drawStack->at(drawStack->size() - 1));
                drawStack->back()->up = true;
                drawStack->erase(drawStack->begin() + drawStack->size() - 1);
            }
            else
            {
                //copy the eStack into the drawStack
                *drawStack = sidePiles[1];
                sidePiles[1].clear();

                for(int i = 0; i < drawStack->size(); i++)
                {
                    drawStack->at(i)->up = false;
                }

                std::ranges::shuffle(*drawStack, randomEngine);
            }
        }
        else
        {
            try {
                Logic::handle(
                    input._inputBuffer(),
                    mainPiles, 
                    sidePiles
                );
            }
            catch(const std::system_error& err)
            {
                //code 1 indicates a parsing error
                if(err.code().value() != 1) throw std::system_error(err.code(), err.what());
            }
        }

        display.render(
            mainPiles,
            sidePiles,
            score, moves
        );

        input.readInput();
    }
};