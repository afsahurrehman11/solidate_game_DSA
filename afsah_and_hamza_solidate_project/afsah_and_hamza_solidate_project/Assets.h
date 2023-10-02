#pragma once
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;


enum cardsuit { spade = 'S', heart = 'H', club = 'C', diamond = 'D', none = ' ' };
enum color_value { black = 1, red = 2, blue = 3, green = 4 };
enum stacktype { pile_1 = '1', pile_2 = '2', pile_3 = '3', pile_4 = '4', pile_5 = '5', pile_6 = '6', pile_7 = '7', spade_foundation = 'S', heart_foundation = 'H', club_foundation = 'C', diamond_foundation = 'D', draw_stock = 'r', talon_pile = 't' };
enum stackindex { pile_1_index = 0, pile_2_index = 1, pile_3_index = 2, pile_4_index = 3, pile_5_index = 4, pile_6_index = 5, pile_7_index = 6, talon_pile_index = 7, draw_stock_index = 8, spade_foundation_index = 9, heart_foundation_index = 10, club_foundation_index = 11, diamond_foundation_index = 12 };
stacktype ALL_STACKTYPES[13] = { pile_1, pile_2, pile_3, pile_4, pile_5, pile_6, pile_7, talon_pile, draw_stock, spade_foundation, heart_foundation, club_foundation, diamond_foundation }; //I create this array to handle the construction of all the stacks automatically.
cardsuit ALL_SUITS[4] = { spade, heart, club, diamond }; //Array holding all suits, used to help automate card creation.
const string black_c = "\033[30;10;107m";
const string red_c = "\033[31;1;107m";
const string blue_c = "\033[94;1;107m";
const string green_c = "\033[32;1;107m";
const string white_c = "\033[0m";

struct color_char // used in printing, i.e makes it easy for me ;)
{
    char hold_char;
    string color_escape_code;
};
struct move_composition
{
    int card_value;
    stackindex from_index;
    stackindex to_index;
    char do_a_flip;
    char move_type;
};

char upper_lower_edge = '\u00c3' + 65;
char left_right_edge = '\u00c3' + 48;
char top_left = '\u00c3' + 87;
char top_right = '\u00bf';
char bot_left = '\u00bf' + 1;
char bot_right = '\u00c3' + 86;
char card_back_fill = '\u00b1';
char empty_fill = '\u0020';
char empty_field_dots = '\u0000' + 248;
char two_lines = '\u0000' + 185;
char mid_sec = '\u0000' + 205;

char back_of_card[6][5] = {
    {top_left,upper_lower_edge,upper_lower_edge,upper_lower_edge,top_right},
    {left_right_edge,card_back_fill,card_back_fill,card_back_fill,left_right_edge},
    {left_right_edge,card_back_fill,card_back_fill,card_back_fill,left_right_edge},
    {left_right_edge,card_back_fill,card_back_fill,card_back_fill,left_right_edge},
    {left_right_edge,card_back_fill,card_back_fill,card_back_fill,left_right_edge},
    {bot_left,upper_lower_edge,upper_lower_edge,upper_lower_edge,bot_right} };
char empty_field_image[6][5] = {
    {empty_field_dots,empty_fill,empty_field_dots,empty_fill,empty_field_dots},
    {empty_field_dots,empty_fill,empty_fill,empty_fill,empty_field_dots},
    {empty_field_dots,empty_fill,empty_fill,empty_fill,empty_field_dots},
    {empty_field_dots,empty_fill,empty_fill,empty_fill,empty_field_dots},
    {empty_field_dots,empty_fill,empty_fill,empty_fill,empty_field_dots},
    {empty_field_dots,empty_fill,empty_field_dots,empty_fill,empty_field_dots} };
char spadesIm[3][3] = {
    {' ','^',' '},
    {'(',' ',')'},
    {' ',two_lines,' '} };
char heartsIm[3][3] = {
    {'/','V','\\'},
    {'\\',' ','/'},
    {' ','V',' '} };
char clubsIm[3][3] = {
    {' ','0',' '},
    {'0',mid_sec,'0'},
    {' ',two_lines,' '} };
char diamondsIm[3][3] = {
    {' ','^',' '},
    {'<',' ','>'},
    {' ','V',' '} };

void explainRulesFunction(void)
{
    cout << endl << "Solitaire is a one player game wherein the goal is to finish creating four \"Foundation\" piles (cards of the same suit that are stacked in ascending order)." << endl << "The game starts with seven piles which have the same number of cards as their pile number (pile 1 has 1 card, pile 2 has 2 cards, and so on) that only have" << endl << "the top card flipped over; no cards start off in the Foundation piles, and the remaining cards are put into the Stock which can be drawn from. Cards can be" << endl << "moved from the Stock to the Talon (a pile separate from the Stock containing the cards that have been drawn), from the Talon to the Tableau (the area with" << endl << "the seven piles), from the Talon to the Foundation, from the Tableau to the top of the Foundation, and from the top of the Foundation to the Tableau. Cards" << endl << "can only be moved within the Tableau if the card is being moved to a card of a different color and a higher order/value, and cards can only be moved to the" << endl << "Foundation of the same suit if the card being put into the Foundation is of a higher order/value than the card at the top of the Foundation (Aces are the" << endl << "first to be moved to the Foundation). You can also chose to move stacks of cards within the Tableau by targeting card within the middle of a pile (the target" << endl << "card and all cards below the target card will be moved)." << endl << endl << "For this program, cards will be drawn 1 at a time and all shuffles will be random - meaning some hands will be unwinnable, so be careful!";
};

void explainControlsFunction(void)
{
    cout << endl << "To move a card, enter [move card] [from stack number] [to stack number] in that order, separated by a space." << endl << ">>[move card]: enter the value displayed on the card (or the lowercase equivalent of the letter on" << endl << "\tthe card). Can be \"a\", 2-10, \"j\", \"q\", or \"k\"." << endl << ">>[from stack number]: can be 1-7 to target a specific stack in the Tableau (1 is the leftmost stack," << endl << "\t2 is the stack to the right of 1, and so on), or \"t\" to target the Talon." << endl << "\tYou can also enter \"sf\", \"hf\", \"cf\", or \"df\" to target that suit's foundation." << endl << ">>[to stack number]: same rules as the [from stack number] field." << endl << "Additionally, you can enter \"help\" to see the controls, \"quit\" to quit, or \"d\" to draw from the" << endl << "Stock, \"u\" to undo a move \"refresh\" to refresh the screen." << endl << "Make sure to enter your commands in lowercase letters." << endl;
}
