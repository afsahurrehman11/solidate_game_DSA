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


class Card
{
private:
    int value;
    cardsuit suit;
    int color;
    string color_sequence;
    int flip_dir; // 1 (face up) or 0 (face down)
    int movable; //1 (movable) or 0 (immovable)
    char card_image[6][5] =
    {
        {top_left,upper_lower_edge,upper_lower_edge,upper_lower_edge,top_right},
        {left_right_edge,'v',' ','s',left_right_edge},
        {left_right_edge,'p','p','p',left_right_edge},
        {left_right_edge,'p','p','p',left_right_edge},
        {left_right_edge,'p','p','p',left_right_edge},
        {bot_left,upper_lower_edge,upper_lower_edge,upper_lower_edge,bot_right}
    };
public:
    Card();
    Card(int thisValue, cardsuit thisSuit, int flip);
    int getValue(void) { return value; };
    cardsuit getSuit(void) { return suit; };
    int getThisColor(void) { return color; };
    int getMovableColor(void);
    string getColorSequence(void) { return color_sequence; };
    int getMovable(void) { return movable; };
    char getImagePiece(int y, int x) { return card_image[y][x]; };
    int getFlip(void) { return flip_dir; };
    void FlipCard(void);
    void immobilize(void) { movable = 0; };
    void makeMobile(void) { movable = 1; };
};

Card::Card()
{
    value = 0;
    suit = none;
    color = green;
    color_sequence = green_c;
    flip_dir = 1;
    movable = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            card_image[i][j] = empty_field_image[i][j];
        }
    }
}

Card::Card(int thisValue, cardsuit thisSuit, int flip = 1)
{
    value = thisValue;
    suit = thisSuit;
    if (suit == 'H' || suit == 'D')
    {
        color_sequence = red_c;
        color = red;
    }
    else if (suit == 'C' || suit == 'S')
    {
        color_sequence = black_c;
        color = black;
    }
    else
    {
        color_sequence = green_c;
        color = green;
    }
    flip_dir = flip;
    movable = flip;
    switch (thisValue)
    {
    case 0: for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            card_image[i][j] = empty_field_image[i][j];
        }
    }
          movable = 0;
          break;
    case 1: card_image[1][1] = 'A';
        break;
    case 2: card_image[1][1] = '2';
        break;
    case 3: card_image[1][1] = '3';
        break;
    case 4: card_image[1][1] = '4';
        break;
    case 5: card_image[1][1] = '5';
        break;
    case 6: card_image[1][1] = '6';
        break;
    case 7: card_image[1][1] = '7';
        break;
    case 8: card_image[1][1] = '8';
        break;
    case 9: card_image[1][1] = '9';
        break;
    case 10: card_image[1][1] = '1';
        card_image[1][2] = '0';
        break;
    case 11: card_image[1][1] = 'J';
        break;
    case 12: card_image[1][1] = 'Q';
        break;
    case 13: card_image[1][1] = 'K';
        break;
    case 14: for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            card_image[i][j] = empty_field_image[i][j];
        }
    }
           movable = 0;
           break;
    }

    card_image[1][3] = suit;

    if (flip_dir == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (suit == 'S')
                    card_image[i + 2][j + 1] = spadesIm[i][j];
                if (suit == 'H')
                    card_image[i + 2][j + 1] = heartsIm[i][j];
                if (suit == 'C')
                    card_image[i + 2][j + 1] = clubsIm[i][j];
                if (suit == 'D')
                    card_image[i + 2][j + 1] = diamondsIm[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                card_image[i + 1][j + 1] = card_back_fill;
            }
        }
        color = blue;
        color_sequence = blue_c;
    }
}

int Card::getMovableColor(void)
{
    if (color == 1)
        return 2;
    else if (color == 2)
        return 1;
    else
    {
        return 0;
    }

}

void Card::FlipCard(void)
{
    if (flip_dir == 0)
    {
        flip_dir = 1;
    }
    else
    {
        flip_dir = 0;
    }

    if (suit == 'H' || suit == 'D')
    {
        color_sequence = red_c;
        color = red;
    }
    else if (suit == 'C' || suit == 'S')
    {
        color_sequence = black_c;
        color = black;
    }
    else
    {
        color_sequence = green_c;
        color = green;
    }
    movable = flip_dir;
    switch (value)
    {
    case 0: for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            card_image[i][j] = empty_field_image[i][j];
        }
    }
          movable = 0;
          break;
    case 1: card_image[1][1] = 'A';
        card_image[1][2] = ' ';
        break;
    case 2: card_image[1][1] = '2';
        card_image[1][2] = ' ';
        break;
    case 3: card_image[1][1] = '3';
        card_image[1][2] = ' ';
        break;
    case 4: card_image[1][1] = '4';
        card_image[1][2] = ' ';
        break;
    case 5: card_image[1][1] = '5';
        card_image[1][2] = ' ';
        break;
    case 6: card_image[1][1] = '6';
        card_image[1][2] = ' ';
        break;
    case 7: card_image[1][1] = '7';
        card_image[1][2] = ' ';
        break;
    case 8: card_image[1][1] = '8';
        card_image[1][2] = ' ';
        break;
    case 9: card_image[1][1] = '9';
        card_image[1][2] = ' ';
        break;
    case 10: card_image[1][1] = '1';
        card_image[1][2] = '0';
        break;
    case 11: card_image[1][1] = 'J';
        card_image[1][2] = ' ';
        break;
    case 12: card_image[1][1] = 'Q';
        card_image[1][2] = ' ';
        break;
    case 13: card_image[1][1] = 'K';
        card_image[1][2] = ' ';
        break;
    case 14: for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            card_image[i][j] = empty_field_image[i][j];
        }
    }
           movable = 0;
           break;
    }

    card_image[1][3] = suit;

    if (flip_dir == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (suit == 'S')
                    card_image[i + 2][j + 1] = spadesIm[i][j];
                if (suit == 'H')
                    card_image[i + 2][j + 1] = heartsIm[i][j];
                if (suit == 'C')
                    card_image[i + 2][j + 1] = clubsIm[i][j];
                if (suit == 'D')
                    card_image[i + 2][j + 1] = diamondsIm[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                card_image[i + 1][j + 1] = card_back_fill;
            }
        }
        color = blue;
        color_sequence = blue_c;
    }
}

Card emptySpaceBack(14, none);
Card emptySpaceFront(0, none);

vector<Card> flipDeck(vector<Card> deck)
{
    int number_of_cards = deck.size();
    for (int i = 0; i < number_of_cards; i++)
    {
        deck[i].FlipCard();
    }
    return deck;
}
