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
#include"FieldStack.h"

class Playspace
{
private:
    int width;
    int height;
    color_char playspace_image[55][55];
    char refresh_type;
public:
    Playspace();
    Playspace(int objWidth, int objHeight);
    int getWidth(void) { return width; };
    int getHeight(void) { return height; };
    void display_space(void);
    void set_scroll_type(char chosen_type) { refresh_type = chosen_type; };
    void set_single_char(color_char set_this, int placeY, int placeX) { playspace_image[placeY][placeX] = set_this; };
    void place_card(Card heldCard, int placeY, int placeX);
    void place_stacks(vector<FieldStack> stacks_container);
    void hard_reset_space(void);
    void full_refresh_space(vector<FieldStack> stacks_container);
};

Playspace::Playspace()
{
    width = 55;
    height = 55;
    refresh_type = 'c';
    color_char tempChar;
    tempChar.color_escape_code = black_c;
    tempChar.hold_char = ' ';
    for (int i = 0; i < 55; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            playspace_image[i][j] = tempChar;
        }
    }

    tempChar.color_escape_code = green_c;
    tempChar.hold_char = '+';
    for (int i = 0; i < 55; i++)
    {
        playspace_image[i][0] = tempChar;
        playspace_image[i][55 - 1] = tempChar;
        playspace_image[0][i] = tempChar;
        playspace_image[55 - 1][i] = tempChar;
    }
}

Playspace::Playspace(int objWidth, int objHeight)
{
    width = objWidth;
    height = objHeight;
    refresh_type = 'c';
    color_char tempChar;
    tempChar.color_escape_code = black_c;
    tempChar.hold_char = ' ';
    for (int i = 0; i < objHeight; i++)
    {
        for (int j = 0; j < objWidth; j++)
        {
            playspace_image[i][j] = tempChar;
        }
    }

    tempChar.color_escape_code = green_c;
    tempChar.hold_char = '+';
    for (int i = 0; i < objWidth; i++)
    {
        playspace_image[i][0] = tempChar;
        playspace_image[i][objWidth - 1] = tempChar;
        playspace_image[0][i] = tempChar;
        playspace_image[objHeight - 1][i] = tempChar;
    }
}

void Playspace::display_space(void)
{
    if (refresh_type == 's')
    {
        for (int i = 0; i < 25; i++)
        {
            cout << endl;
        }
    }
    else
        system("CLS");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << white_c << playspace_image[i][j].color_escape_code << playspace_image[i][j].hold_char;
        }
        cout << endl;
    }
    cout << white_c << black_c;
}

void Playspace::place_card(Card heldCard, int placeY, int placeX)
{
    color_char tempChar;
    tempChar.color_escape_code = heldCard.getColorSequence();
    if (placeX < 0 || placeX > 49 || placeY < 0 || placeY > 50)
        goto skip;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tempChar.hold_char = heldCard.getImagePiece(i, j);
            playspace_image[placeY + i][placeX + j] = tempChar;
        }
    }
skip: {}
}


void Playspace::hard_reset_space(void)
{
    color_char tempChar;
    tempChar.color_escape_code = black_c;
    tempChar.hold_char = ' ';
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            playspace_image[i][j] = tempChar;
        }
    }
    for (int i = 0; i < 7; i++)
    {
        if (i == 6)
            this->place_card(emptySpaceFront, 1, 37);
        this->place_card(emptySpaceFront, 1, 8 * i + 1);
    }
    for (int i = 0; i < 7; i++)
    {
        this->place_card(emptySpaceFront, 8, 8 * i + 1);
    }

    char stacklabels[7] = { '1', '2', '3', '4', '5', '6', '7' };
    for (int i = 0; i < 7; i++)
    {
        tempChar.hold_char = stacklabels[i];
        this->set_single_char(tempChar, 7, 8 * i + 3);
    }

}

void Playspace::place_stacks(vector<FieldStack> stacks_container)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 1; j < stacks_container[i].cardsInStack.size(); j++)
        {
            this->place_card(stacks_container[i].cardsInStack[j], 2 * (j - 1) + 8, 1 + (8 * i));
        }
    }

    if (stacks_container[draw_stock_index].cardsInStack.size() != 0)
        this->place_card(stacks_container[draw_stock_index].cardsInStack.back(), 1, 49);

    if (stacks_container[spade_foundation_index].cardsInStack.size() != 0)
        this->place_card(stacks_container[spade_foundation_index].cardsInStack.back(), 1, 1);
    if (stacks_container[heart_foundation_index].cardsInStack.size() != 0)
        this->place_card(stacks_container[heart_foundation_index].cardsInStack.back(), 1, 9);
    if (stacks_container[club_foundation_index].cardsInStack.size() != 0)
        this->place_card(stacks_container[club_foundation_index].cardsInStack.back(), 1, 17);
    if (stacks_container[diamond_foundation_index].cardsInStack.size() != 0)
        this->place_card(stacks_container[diamond_foundation_index].cardsInStack.back(), 1, 25);

    int talonSize = stacks_container[talon_pile_index].cardsInStack.size();
    switch (talonSize)
    {
    case 0: break;
    case 1: this->place_card(stacks_container[talon_pile_index].cardsInStack.back(), 1, 33);
        break;
    case 2: this->place_card(stacks_container[talon_pile_index].cardsInStack[talonSize - 2], 1, 33);
        this->place_card(stacks_container[talon_pile_index].cardsInStack.back(), 1, 37);
        break;
    default: this->place_card(stacks_container[talon_pile_index].cardsInStack[talonSize - 3], 1, 33);
        this->place_card(stacks_container[talon_pile_index].cardsInStack[talonSize - 2], 1, 37);
        this->place_card(stacks_container[talon_pile_index].cardsInStack.back(), 1, 41);
    }
}

void Playspace::full_refresh_space(vector<FieldStack> stacks_container)
{
    this->hard_reset_space();
    this->place_stacks(stacks_container);
    this->display_space();
}




