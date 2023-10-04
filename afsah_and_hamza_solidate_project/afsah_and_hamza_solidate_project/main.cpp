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
#include"Playspace.h"


using namespace std;



// the global objects 
vector<FieldStack> ALLStacks = {};
vector<Card> original_deck = {};
Playspace* MyPlayspace = new Playspace(55, 55);
vector<move_composition> FullMoveList = {};

//handling controls
bool DrawCard(void)
{
    if (ALLStacks[draw_stock_index].cardsInStack.size() == 0)
    {
        if (ALLStacks[talon_pile_index].cardsInStack.size() == 0)
        {
            return false;
        }
        else
        {
            int staticInt = ALLStacks[talon_pile_index].cardsInStack.size();
            for (int i = 0; i < staticInt; i++)
            {

                ALLStacks[draw_stock_index].cardsInStack.push_back(ALLStacks[talon_pile_index].cardsInStack.back());
                ALLStacks[draw_stock_index].cardsInStack.back().FlipCard();
                ALLStacks[talon_pile_index].cardsInStack.pop_back();
            }
            return true;
        }
    }
    else
    {
        ALLStacks[talon_pile_index].cardsInStack.push_back(ALLStacks[draw_stock_index].cardsInStack.back());
        ALLStacks[talon_pile_index].cardsInStack.back().FlipCard();
        ALLStacks[draw_stock_index].cardsInStack.pop_back();
        return true;
    }
}



void MoveCard(int searchCardValue, stackindex StackFromIndex, stackindex StackToIndex)
{
    /*cout << "searchCardValue: " << searchCardValue << endl;
    cout << "StackFromIndex: " << StackFromIndex << endl;
    cout << "StackToIndex: " << StackToIndex << endl;*/

    move_composition this_move;
    this_move.card_value = searchCardValue;
    this_move.from_index = StackFromIndex;
    this_move.to_index = StackToIndex;
    this_move.move_type = 'U';
    this_move.do_a_flip = 'f';

    int LCValue_In_FromStack = ALLStacks[StackFromIndex].cardsInStack.back().getValue();
    int LCValue_In_ToStack = ALLStacks[StackToIndex].cardsInStack.back().getValue();
    char LCSuit_In_FromStack = static_cast<char> (ALLStacks[StackFromIndex].cardsInStack.back().getSuit());
    char LCSuit_In_ToStack = static_cast<char> (ALLStacks[StackToIndex].getID());

    /*cout << "LCValue_In_FromStack: " << LCValue_In_FromStack << endl;
    cout << "LCValue_In_ToStack: " << LCValue_In_ToStack << endl;
    cout << "LCSuit_In_FromStack: " << LCSuit_In_FromStack << endl;
    cout << "LCSuit_In_ToStack: " << LCSuit_In_ToStack << endl;*/


    if (searchCardValue == LCValue_In_FromStack && LCSuit_In_FromStack == LCSuit_In_ToStack && StackToIndex >= spade_foundation_index)
    {
        if (searchCardValue == 1 && LCValue_In_FromStack == LCValue_In_ToStack + 1) // ace wala case 
        {
            ALLStacks[StackToIndex].cardsInStack.push_back(ALLStacks[StackFromIndex].cardsInStack.back());
            ALLStacks[StackFromIndex].cardsInStack.pop_back();
            this_move.move_type = 'n';
        }
        else if (LCValue_In_FromStack == LCValue_In_ToStack + 1)
        {
            ALLStacks[StackToIndex].cardsInStack.push_back(ALLStacks[StackFromIndex].cardsInStack.back());
            ALLStacks[StackFromIndex].cardsInStack.pop_back();
            this_move.move_type = 'n';
        }
    }
    else if ((StackFromIndex >= spade_foundation_index || StackFromIndex == 7 && searchCardValue) && searchCardValue == LCValue_In_FromStack)
    {
        if (LCValue_In_ToStack - 1 == LCValue_In_FromStack && ALLStacks[StackFromIndex].cardsInStack.back().getMovableColor() == ALLStacks[StackToIndex].cardsInStack.back().getThisColor())
        {
            ALLStacks[StackToIndex].cardsInStack.push_back(ALLStacks[StackFromIndex].cardsInStack.back());
            ALLStacks[StackFromIndex].cardsInStack.pop_back();
            this_move.move_type = 'n';
        }
        else if (LCValue_In_ToStack - 1 == LCValue_In_FromStack && ALLStacks[StackToIndex].cardsInStack.back().getThisColor() == 4)
        {
            ALLStacks[StackToIndex].cardsInStack.push_back(ALLStacks[StackFromIndex].cardsInStack.back());
            ALLStacks[StackFromIndex].cardsInStack.pop_back();
            this_move.move_type = 'n';
        }
    }
    else //!Moving Cards between piles in Tableau (meaning multiple cards can be moved at once).
    {
        if (StackFromIndex == 7)
            goto end_of_function; //You're not supposed to be in here!
        if (LCValue_In_ToStack - 1 == searchCardValue && ALLStacks[StackFromIndex].searchForCardInStack(searchCardValue).getMovableColor() == ALLStacks[StackToIndex].cardsInStack.back().getThisColor())
        {
            vector<Card> cardsInPlayerHand = ALLStacks[StackFromIndex].getCardsAfter(searchCardValue);
            for (int i = cardsInPlayerHand.size(); i > 0; i--)
            {
                ALLStacks[StackToIndex].cardsInStack.push_back(cardsInPlayerHand.back());
                cardsInPlayerHand.pop_back();
                this_move.move_type = 'i'; //'i' for Interstack
            }
        }
        else if (LCValue_In_ToStack - 1 == searchCardValue && ALLStacks[StackToIndex].cardsInStack.back().getThisColor() == 4)
        {
            vector<Card> cardsInPlayerHand = ALLStacks[StackFromIndex].getCardsAfter(searchCardValue);
            for (int i = cardsInPlayerHand.size(); i > 0; i--)
            {
                ALLStacks[StackToIndex].cardsInStack.push_back(cardsInPlayerHand.back());
                cardsInPlayerHand.pop_back();
                this_move.move_type = 'i';
            }
        }
    }

    if (ALLStacks[StackFromIndex].cardsInStack.back().getFlip() == 0)
    {
        ALLStacks[StackFromIndex].cardsInStack.back().FlipCard();
        this_move.do_a_flip = 't';
    }
    //getchar();


end_of_function:

    FullMoveList.push_back(this_move);
}


void ForceUndo(void)
{
    move_composition moveComp = FullMoveList.back();

    if (moveComp.move_type == 'U')
        goto end_of_function;

    if (moveComp.move_type == 'd')
    {
        if (ALLStacks[talon_pile_index].cardsInStack.size() == 0 && ALLStacks[draw_stock_index].cardsInStack.size() != 0)
        {
            int staticInt = ALLStacks[draw_stock_index].cardsInStack.size();
            for (int i = 0; i < staticInt; i++)
            {
                ALLStacks[talon_pile_index].cardsInStack.push_back(ALLStacks[draw_stock_index].cardsInStack.back());//!draw >> talon
                ALLStacks[talon_pile_index].cardsInStack.back().FlipCard();//!
                ALLStacks[draw_stock_index].cardsInStack.pop_back();
            }
        }
        else
        {
            ALLStacks[draw_stock_index].cardsInStack.push_back(ALLStacks[talon_pile_index].cardsInStack.back());
            ALLStacks[draw_stock_index].cardsInStack.back().FlipCard();
            ALLStacks[talon_pile_index].cardsInStack.pop_back();//!
        }
    }

    if (moveComp.do_a_flip == 't')
        ALLStacks[moveComp.from_index].cardsInStack.back().FlipCard();

    if (moveComp.move_type == 'n')
    {
        ALLStacks[moveComp.from_index].cardsInStack.push_back(ALLStacks[moveComp.to_index].cardsInStack.back());
        ALLStacks[moveComp.to_index].cardsInStack.pop_back();
    }
    if (moveComp.move_type == 'i')
    {
        vector<Card> cardsInPlayerHand = ALLStacks[moveComp.to_index].getCardsAfter(moveComp.card_value);
        for (int i = cardsInPlayerHand.size(); i > 0; i--)
        {
            ALLStacks[moveComp.from_index].cardsInStack.push_back(cardsInPlayerHand.back());
            cardsInPlayerHand.pop_back();
        }
    }

end_of_function:

    FullMoveList.pop_back();
}

char PlayerControlHandeler(string full_player_control_string, Playspace* what_playspace)
{
    string external_from_stack;
    string external_moving_card;
    string external_to_stack;

    istringstream control_interpreter(full_player_control_string);
    control_interpreter >> external_moving_card;
    control_interpreter >> external_from_stack;
    control_interpreter >> external_to_stack;

    stackindex local_from_stack;
    stackindex local_to_stack;
    int local_moving_card_value;
    char returnChar = 'T'; //Default return is T for "True", meaning the function executed successfully

    /*cout << "external from: '" << external_from_stack << "'" << endl;
    cout << "external card: '" << external_moving_card << "'" << endl;
    cout << "external to  : '" << external_to_stack << "'" << endl;*/

    if (external_moving_card == "quit") //Translates external_from
    {
        returnChar = 'q';
        goto end_of_function;
    }
    else if (external_moving_card == "help")
    {
        returnChar = 'c';
        goto end_of_function;
    }
    else if (external_moving_card == "refresh")
    {
        returnChar = 'r';
        goto end_of_function;
    }
    else if (external_moving_card == "d")
    {
        returnChar = 'd';
        goto end_of_function;
    }
    else if (external_moving_card == "u")
    {
        returnChar = 'u';
        goto end_of_function;
    }
    else if (external_moving_card == "a")
        local_moving_card_value = 1;
    else if (external_moving_card == "2")
        local_moving_card_value = 2;
    else if (external_moving_card == "3")
        local_moving_card_value = 3;
    else if (external_moving_card == "4")
        local_moving_card_value = 4;
    else if (external_moving_card == "5")
        local_moving_card_value = 5;
    else if (external_moving_card == "6")
        local_moving_card_value = 6;
    else if (external_moving_card == "7")
        local_moving_card_value = 7;
    else if (external_moving_card == "8")
        local_moving_card_value = 8;
    else if (external_moving_card == "9")
        local_moving_card_value = 9;
    else if (external_moving_card == "10")
        local_moving_card_value = 10;
    else if (external_moving_card == "j")
        local_moving_card_value = 11;
    else if (external_moving_card == "q")
        local_moving_card_value = 12;
    else if (external_moving_card == "k")
        local_moving_card_value = 13;
    else
    {
        returnChar = 'e';
        goto end_of_function;
    }

    if (external_from_stack == "t")
        local_from_stack = talon_pile_index;
    else if (external_from_stack == "sf")
        local_from_stack = spade_foundation_index;
    else if (external_from_stack == "hf")
        local_from_stack = heart_foundation_index;
    else if (external_from_stack == "cf")
        local_from_stack = club_foundation_index;
    else if (external_from_stack == "df")
        local_from_stack = diamond_foundation_index;
    else if (external_from_stack == "1")
        local_from_stack = pile_1_index;
    else if (external_from_stack == "2")
        local_from_stack = pile_2_index;
    else if (external_from_stack == "3")
        local_from_stack = pile_3_index;
    else if (external_from_stack == "4")
        local_from_stack = pile_4_index;
    else if (external_from_stack == "5")
        local_from_stack = pile_5_index;
    else if (external_from_stack == "6")
        local_from_stack = pile_6_index;
    else if (external_from_stack == "7")
        local_from_stack = pile_7_index;
    else
    {
        returnChar = 'e';
        goto end_of_function;
    }

    if (external_to_stack == "sf") //Translates external_to
        local_to_stack = spade_foundation_index;
    else if (external_to_stack == "hf")
        local_to_stack = heart_foundation_index;
    else if (external_to_stack == "cf")
        local_to_stack = club_foundation_index;
    else if (external_to_stack == "df")
        local_to_stack = diamond_foundation_index;
    else if (external_to_stack == "1")
        local_to_stack = pile_1_index;
    else if (external_to_stack == "2")
        local_to_stack = pile_2_index;
    else if (external_to_stack == "3")
        local_to_stack = pile_3_index;
    else if (external_to_stack == "4")
        local_to_stack = pile_4_index;
    else if (external_to_stack == "5")
        local_to_stack = pile_5_index;
    else if (external_to_stack == "6")
        local_to_stack = pile_6_index;
    else if (external_to_stack == "7")
        local_to_stack = pile_7_index;
    else
    {
        returnChar = 'e';
        goto end_of_function;
    }


    MoveCard(local_moving_card_value, local_from_stack, local_to_stack);

end_of_function:
    return returnChar;
}

void Complete_Game(int moves)
{
    system("CLS");
    cout << "CONGRATULATIONS!!" << endl << "You won in " << moves << " moves.";
}

//==================================================================================//

// defining Game class here cuz it uses global varaibles and i don't want to mess up with global objects 
// actually i am fed up :(
class Game
{
private:
    int number_of_moves;
    int won;
public:
    Game();
    void game_setup(void);
    void start_game(void);
};

Game::Game()
{
    won = 0;
    system("Color F0");

    vector<Card> tempDeck = {};
    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card tempCard(i, ALL_SUITS[j]);
            tempDeck.push_back(tempCard);
        }
    }
    random_shuffle(tempDeck.begin(), tempDeck.end());

    int k = 0;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            MyPlayspace->place_card(tempDeck[k], 4 * i + 1, 7 + (12 * j));
            k++;
        }
    }
    for (int j = 0; j < 4; j++)
    {
        MyPlayspace->place_card(tempDeck[k], 4 * 12, 7 + (12 * j));
        k++;
    }


    char settings_input = 'n';
    cout << black_c << "Please adjust this window to your preference before continuing. (Press the \"Enter\" key on your keyboard to continue)" << endl;
    getchar();
    cout << "Please enter your refresh type ('c' for clear [default], 's' for scroll, 't' to test the refresh, or 'x' to continue)." << endl << "Make sure you are able to see the entire field each time the screen is refreshed." << endl;
    while (settings_input != 'x')
    {
        cin >> settings_input;
        if (settings_input == 'x' || settings_input == 'c' || settings_input == 's' || settings_input == 't')
        {
            switch (settings_input)
            {
            case 'c': MyPlayspace->set_scroll_type(settings_input);
                cout << "Your refresh setting has been set to " << settings_input << ". " << endl << "You can choose to change your refresh type again or continue." << endl << "Please enter 'c' for clear, 's' for scroll, 't' to test the refresh, or 'x' to continue." << endl;
                break;
            case 's': MyPlayspace->set_scroll_type(settings_input);
                cout << "Your refresh setting has been set to " << settings_input << ". " << endl << "You can choose to change your refresh type again or continue." << endl << "Please enter 'c' for clear, 's' for scroll, 't' to test the refresh, or 'x' to continue." << endl;
                break;
            case 't': MyPlayspace->display_space();
                cout << "You can choose to change your refresh type again or continue." << endl << "Please enter 'c' for clear, 's' for scroll, 't' to test the refresh, or 'x' to continue." << endl;
                break;
            }
        }
        else
        {
            cout << "That is an invalid refresh type." << endl << "You can choose to change your refresh type again or continue." << endl << "Please enter 'c' for clear, 's' for scroll, 't' to test the refresh, or 'x' to continue." << endl;
        }
    }

    system("CLS");

    cout << endl << "Would you like to know the rules of the game? (enter 'y' for yes or 'n' for no): ";
    char explain_rules;
    cin >> explain_rules;
    if (explain_rules == 'y')
        explainRulesFunction();

    cout << endl << endl << "Press the \"Enter\" key on your keyboard to start the game. At any point during the game, you can enter \"help\" to see the controls." << endl << "Have fun :D!" << endl;
    number_of_moves = 0;
    getchar();
    getchar();
}

void Game::game_setup(void)
{
    number_of_moves = 0;

    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card tempCard(i, ALL_SUITS[j]);
            original_deck.push_back(tempCard);
        }
    }
    random_shuffle(original_deck.begin(), original_deck.end()); //Deck is shuffled.

    original_deck = flipDeck(original_deck); //!All cards in the deck are flipped.

    for (int i = 0; i < 13; i++)
    {
        FieldStack tempStack(ALL_STACKTYPES[i]);
        ALLStacks.push_back(tempStack);
    }

    for (int i = 0; i < 7; i++)
    {
        for (int j = 1; j < i + 2; j++)
        {
            ALLStacks[i].pushCard(original_deck.back());
            original_deck.pop_back();
        }
        ALLStacks[i].cardsInStack.back().FlipCard();
    }

    int cards_left = original_deck.size();
    for (int i = 0; i < cards_left; i++)
    {
        ALLStacks[draw_stock_index].pushCard(original_deck.back());
        original_deck.pop_back();
    }

}

void Game::start_game(void)
{
    string player_control = "initialize";
    char move_status;

    while (player_control != "quit")
    {
        MyPlayspace->full_refresh_space(ALLStacks);

    new_control_without_refresh:
        cout << "Enter move: ";
        getline(cin, player_control);

        move_status = PlayerControlHandeler(player_control, MyPlayspace);
        if (move_status == 'c')
        {
            explainControlsFunction();
            goto new_control_without_refresh;
        }
        else if (move_status == 'e')
        {
            cout << "That is an invalid control statement." << endl;
            goto new_control_without_refresh;
        }
        else if (move_status == 'd')
        {
            if (DrawCard())
            {
                number_of_moves++;
                move_composition this_move;
                this_move.move_type = 'd';
                FullMoveList.push_back(this_move);
            }
        }
        else if (move_status == 'u' && FullMoveList.size() == 0)
        {
            cout << "No moves to undo" << endl;
            goto new_control_without_refresh;
        }
        else if (move_status == 'u' && FullMoveList.size() != 0)
        {
            ForceUndo();
            number_of_moves++;
        }
        else if (move_status == 'T')
        {
            number_of_moves++;
        }


        if (ALLStacks[spade_foundation_index].cardsInStack.size() == 14 && ALLStacks[heart_foundation_index].cardsInStack.size() == 14 && ALLStacks[club_foundation_index].cardsInStack.size() == 14 && ALLStacks[diamond_foundation_index].cardsInStack.size() == 14)
        {
            Complete_Game(number_of_moves);
            goto end_of_function;
        }
    }
end_of_function: {}
}


int main()
{
    system("mode 200");

    cout << "This program allows the user to play a game of solitaire." << endl;

    srand(time(0));
    system("Color F0"); // black text & white bg best combo(baki fazool)

    Game Solitaire;
    Solitaire.game_setup();
    Solitaire.start_game();

    /*
    _getchar();
    _getchar();
    */
    return 0;
}