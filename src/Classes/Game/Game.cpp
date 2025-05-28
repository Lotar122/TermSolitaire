#include "Game.hpp"

Game::Game()
{
    randomEngine = std::mt19937(randomDevice());

    //Generate the deck, sorted and shuffled
    deck.reserve(52);
    shuffledDeck.reserve(52);

    for (size_t r = 0; r < cardRankEnumLenght; r++)
        for (size_t c = 0; c < cardColorEnumLenght; c++)
        {
            deck.push_back(Card(this, (CardSymbol)c, (CardRank)(r * 2), false));
            shuffledDeck.push_back(&deck[deck.size() - 1]);
        }

    std::ranges::shuffle(shuffledDeck, randomEngine);

    int shuffledDeckIndex = 0;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            mainPiles[i].push_back(shuffledDeck[shuffledDeckIndex]);
            if (j == i) mainPiles[i][j]->up = true;
            shuffledDeckIndex++;
        }
    }

    drawStack->resize(shuffledDeck.size() - shuffledDeckIndex);
    std::memcpy(drawStack->data(), shuffledDeck.data() + shuffledDeckIndex, drawStack->size() * sizeof(Card*));
}

int Game::update()
{
    if (std::strcmp("draw", input._inputBuffer().data()) == 0 || std::strcmp("Draw", input._inputBuffer().data()) == 0)
    {
        if (drawStack->size() > 0)
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

            for (int i = 0; i < drawStack->size(); i++)
            {
                drawStack->at(i)->up = false;
            }

            std::ranges::shuffle(*drawStack, randomEngine);
        }
    }
    else if (std::strcmp("exit", input._inputBuffer().data()) == 0 || std::strcmp("Exit", input._inputBuffer().data()) == 0)
    {
        stop = true;
        return 2;
    }
    else if (std::strcmp("restart", input._inputBuffer().data()) == 0 || std::strcmp("Restart", input._inputBuffer().data()) == 0)
    {
        return 1;
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
        catch (const std::system_error& err)
        {
            //code 1 indicates a parsing error
            if (err.code().value() != 1) throw std::system_error(err.code(), err.what());
        }
    }

    display.render(
        mainPiles,
        sidePiles,
        score, moves
    );

    if (!stop)
    {
        //goes to the beggining of the line
        fputs("\r", stdout);
        fputs("Command: ", stdout);

        input.readInput();

        //moves one line up
        fputs("\x1b[1A", stdout);
        //clears the current line
        fputs("\x1b[2K", stdout);
    }

    if (
        !sidePiles[(int)SidePileEnum::A].empty() &&
        !sidePiles[(int)SidePileEnum::B].empty() &&
        !sidePiles[(int)SidePileEnum::C].empty() &&
        !sidePiles[(int)SidePileEnum::D].empty() &&

        sidePiles[(int)SidePileEnum::A].back()->_rank() == CardRank::King &&
        sidePiles[(int)SidePileEnum::B].back()->_rank() == CardRank::King &&
        sidePiles[(int)SidePileEnum::C].back()->_rank() == CardRank::King &&
        sidePiles[(int)SidePileEnum::D].back()->_rank() == CardRank::King
        )
    {
        clearScreen();
        printUTF32(gameWin, screenMapSize);
        stop = true;
        return 2;
    }

    return 0;
}