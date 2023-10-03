#pragma once
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>


#include"Assets.h"
#include"Cards.h"


class FieldStack
{
private:
    Card* tempCard;
    stacktype stack_identifyer;
public:
    vector<Card> cardsInStack = {};
    FieldStack(stacktype stackId);
    stacktype getID(void) { return stack_identifyer; };
    Card GetTempCard(void) { return *tempCard; };
    void pushCard(Card cardToPush) { cardsInStack.push_back(cardToPush); };
    void flip_stack(void);
    void MoveCard(int searchCardValue, vector<Card> whereToMove);
    Card searchForCardInStack(int whatCardValue);
    vector<Card> getCardsAfter(int whatCardValue);
};

FieldStack::FieldStack(stacktype stackId)
{
    stack_identifyer = stackId;
    Card* tempCard = new Card;
    if (stackId == 't' || stackId == 'r')
    {
        //Do nothing.
    }
    else if (stackId == '1' || stackId == '2' || stackId == '3' || stackId == '4' || stackId == '5' || stackId == '6' || stackId == '7')
    {
        cardsInStack.push_back(emptySpaceBack);
    }
    else
    {
        cardsInStack.push_back(emptySpaceFront);
    }
}

void FieldStack::flip_stack(void)
{
    cardsInStack = flipDeck(cardsInStack);
}

Card FieldStack::searchForCardInStack(int whatCardValue)
{
    for (int i = 0; i < cardsInStack.size(); i++)
    {
        if (cardsInStack[i].getValue() == whatCardValue && cardsInStack[i].getMovable() == 1)
        {
            return cardsInStack[i];
        }
    }
    return cardsInStack.back();
}

vector<Card> FieldStack::getCardsAfter(int whatCardValue)
{
    vector<Card> tempPlayerHand = {};

    int foundDepth = 0;
    for (int i = 0; i < cardsInStack.size(); i++)
    {
        if (cardsInStack[i].getValue() == whatCardValue && cardsInStack[i].getMovable() == 1)
        {
            foundDepth = cardsInStack.size() - i;
        }
    }

    if (foundDepth == 0 && !(cardsInStack.back().getValue() == whatCardValue))
        goto end_of_function;

    for (int i = 0; i < foundDepth; i++)
    {
        tempPlayerHand.push_back(cardsInStack.back());
        cardsInStack.pop_back();
    }

end_of_function:
    return tempPlayerHand;
}