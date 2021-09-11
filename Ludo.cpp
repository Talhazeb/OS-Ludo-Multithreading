//===========================================================================================================
//=============== Muhammad Talha Zeb Khan ---------CS-A--------- Khajista Zainab ============================
//===================== 19I-0641 ================================== 19I-XXXX ================================
//===========================================================================================================

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <string>

#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

pthread_t tid[9];

sem_t lock1;
sem_t lock2;

void *bring_token_toField(void *arg);

using namespace std;

char token_names[4][4] = {{'a', 'b', 'c', 'd'}, {'e', 'f', 'g', 'h'}, {'i', 'j', 'k', 'l'}, {'m', 'n', 'o', 'p'}}; //token names
int token_home_pos[4][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}};                         //storing indeces of tokens
int start_ind[4] = {17, 30, 56, 43};                                                                               //the point token comes out after dice value 6
bool check_kill_1 = false;                                                                                         //killing token score
bool check_kill_2 = false;                                                                                         //check_kill_1 of adding in score twice when token is reached
bool player_token_inFieldCheck = false;                                                                            //coming token out
bool win = false;                                                                                                  //check which player has won
char player_token_inFieldchoice;
int player_selection[4] = {0, 0, 0, 0}; //Array for selecting random player for play.

struct LUDO_GAME
{
    int player_num;                  //num of players
    char tokens[4][4];               //all tokens
    char player_name[4][20];         //player names
    char playable_tokens[4][4];      //home tokens
    char tokens_won[4][4];           //tokens entered home
    char ludo_board[88];             //game ludo_board
    int scores[4][4];                //score of each token
    char player_token_inField[4][4]; //tokens in play
    int token_num;                   //token's number at input
    int select_player;               //player selection during iteration
};

void token_pos_handler(int player, int ind, int dice, LUDO_GAME &l);        //handles tokens poss
int cal_token_inField(int player);                                          //token in board
void tokens_won_fun(int player, int ind);                                   //token at home
void token_kill(int playerind, int player, int pos, LUDO_GAME &l, int dice); //token kill!
bool available_token(int player, char ch);                                 //token availability for bringing out
void token_movement(int player, LUDO_GAME &l, int dice);                              //token movement
void token_dice_conditions(int player, int dice);                           //from where the functions are called
int arrived_home(int player);                                               //Token reached home
void update_display(int player);                                            //Update display for token movement
int playable_tokens(int player);                                            //token that are playable with

//--------
LUDO_GAME l; //Global object
//---------

void *ludo_board_display(void *arg)
{
    sem_wait(&lock1);
    LUDO_GAME *l = (LUDO_GAME *)arg;

    cout << " _________________________________________________________________\n";

    cout << "|                       |__" << l->ludo_board[27] << "__|__" << l->ludo_board[28] << "__|__" << l->ludo_board[29] << "__|                       |\n";

    cout << "|     " << l->ludo_board[0] << "           " << l->ludo_board[1] << "     |__" << l->ludo_board[26] << "__|__" << l->ludo_board[73] << "__|__" << l->ludo_board[30] << "__|     " << l->ludo_board[4] << "           " << l->ludo_board[5] << "     |\n";

    cout << "|                       |__" << l->ludo_board[25] << "__|__" << l->ludo_board[74] << "__|__" << l->ludo_board[31] << "__|                       |\n";

    cout << "|                       |__" << l->ludo_board[24] << "__|__" << l->ludo_board[75] << "__|__" << l->ludo_board[32] << "__|                       |\n";

    cout << "|     " << l->ludo_board[2] << "           " << l->ludo_board[3] << "     |__" << l->ludo_board[23] << "__|__" << l->ludo_board[76] << "__|__" << l->ludo_board[33] << "__|     " << l->ludo_board[6] << "           " << l->ludo_board[7] << "     |\n";

    cout << "|_______________________|__" << l->ludo_board[22] << "__|__" << l->ludo_board[77] << "__|__" << l->ludo_board[34] << "__|_______________________|\n";

    cout << "|   |   |   |   |   |   |                 |   |   |   |   |   |   |\n";

    cout << "| " << l->ludo_board[16] << " | " << l->ludo_board[17] << " | " << l->ludo_board[18] << " | " << l->ludo_board[19] << " | " << l->ludo_board[20] << " | " << l->ludo_board[21] << " |   OS  PROJECT   | " << l->ludo_board[35] << " | " << l->ludo_board[36] << " | " << l->ludo_board[37] << " | " << l->ludo_board[38] << " | " << l->ludo_board[39] << " | " << l->ludo_board[40] << " |\n";

    cout << "|___|___|___|___|___|___|                 |___|___|___|___|___|___|\n";

    cout << "|   |   |   |   |   |   |                 |   |   |   |   |   |   |\n";

    cout << "| " << l->ludo_board[67] << " | " << l->ludo_board[68] << " | " << l->ludo_board[69] << " | " << l->ludo_board[70] << " | " << l->ludo_board[71] << " | " << l->ludo_board[72] << " |    Talha zeb    | " << l->ludo_board[82] << " | " << l->ludo_board[81] << " | " << l->ludo_board[80] << " | " << l->ludo_board[79] << " | " << l->ludo_board[78] << " | " << l->ludo_board[41] << " |\n";

    cout << "|___|___|___|___|___|___|                 |___|___|___|___|___|___|\n";

    cout << "|   |   |   |   |   |   |                 |   |   |   |   |   |   |\n";

    cout << "| " << l->ludo_board[66] << " | " << l->ludo_board[65] << " | " << l->ludo_board[64] << " | " << l->ludo_board[63] << " | " << l->ludo_board[62] << " | " << l->ludo_board[61] << " | Khajista Zainab | " << l->ludo_board[47] << " | " << l->ludo_board[46] << " | " << l->ludo_board[45] << " | " << l->ludo_board[44] << " | " << l->ludo_board[43] << " | " << l->ludo_board[42] << " |\n";

    cout << "|___|___|___|___|___|___|_________________|___|___|___|___|___|___|\n";

    cout << "|                       |__" << l->ludo_board[60] << "__|__" << l->ludo_board[87] << "__|__" << l->ludo_board[48] << "__|                       |\n";

    cout << "|     " << l->ludo_board[8] << "           " << l->ludo_board[9] << "     |__" << l->ludo_board[59] << "__|__" << l->ludo_board[86] << "__|__" << l->ludo_board[49] << "__|     " << l->ludo_board[12] << "           " << l->ludo_board[13] << "     |\n";

    cout << "|                       |__" << l->ludo_board[58] << "__|__" << l->ludo_board[85] << "__|__" << l->ludo_board[50] << "__|                       |\n";

    cout << "|                       |__" << l->ludo_board[57] << "__|__" << l->ludo_board[84] << "__|__" << l->ludo_board[51] << "__|                       |\n";

    cout << "|     " << l->ludo_board[10] << "           " << l->ludo_board[11] << "     |__" << l->ludo_board[56] << "__|__" << l->ludo_board[83] << "__|__" << l->ludo_board[52] << "__|     " << l->ludo_board[14] << "           " << l->ludo_board[15] << "     |\n";

    cout << "|_______________________|__" << l->ludo_board[55] << "__|__" << l->ludo_board[54] << "__|__" << l->ludo_board[53] << "__|_______________________|\n";

    sem_post(&lock1);
    pthread_exit(NULL);
}

void tokens_won_fun(int player, int ind)
{
    int temp_pos = start_ind[player] + l.scores[player][ind];

    if (player != 0)
    {
        if (temp_pos > 67) //because smallest value on borad out of homes is 16
        {
            temp_pos %= 52; //will make all values >=16
        }
        if (l.scores[player][ind] >= 51) //51 is the path length of all tokens of players
        {
            if (player == 1)
            {
                temp_pos = l.scores[player][ind] + 22; //This will move the token inside house of 1
            }
            else if (player == 2)
            {
                temp_pos = l.scores[player][ind] + 32; //It will move token inside house of player 2
            }
            else if (player == 3)
            {
                temp_pos = l.scores[player][ind] + 27; //It will move token inside house of player 3
            }
        }
    }


    l.player_token_inField[player][ind] = ' ';         //the token that is out of home
    l.playable_tokens[player][ind] = ' ';              //As it has completed path so now player can't choose it to move on ludo_board
    l.ludo_board[temp_pos] = ' ';                      //now the token is won so there should be space on ludo_board
    l.tokens_won[player][ind] = l.tokens[player][ind]; //removing won token from player's token array


    if (arrived_home(player) == l.token_num)           //it means all tokens of a player have reached home and the player has won
    {
        cout << l.player_name[player] << " won."; //displaying name of player that has won
        win = true;
        return;
    }

    int dice = rand() % 6 + 1;

    update_display(player);

    cout << "Another turn for a home arrived token!\n"; //In case one player has won one token so now it will be given bonus turn
    cout << "Die rolled.... " << dice << endl;

    if (dice != 6 and !cal_token_inField(player)) //how many tokens are out of home
    {
        return;
    }

    else if (dice == 6 and !cal_token_inField(player)) //now take out the remaining token
    {
        pthread_create(&tid[1], NULL, bring_token_toField, NULL);
        pthread_join(tid[1], NULL);

    }

    else if (cal_token_inField(player)) //OR move the token already out of home
    {
        token_movement(player, l, dice);
    }
}

void token_kill(int playerind, int player, int pos, LUDO_GAME &l, int dice)
{

    if (l.ludo_board[pos] != ' ') //means the pos on which token is going to be placed is not empty
    {
        int i, j, player_to_be_killed, ind; //player_to_be_killed is the one who is going to b killed

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < l.token_num; j++)
            {
                if (l.tokens[i][j] == l.ludo_board[pos])
                {
                    player_to_be_killed = i; //detecting the player
                    ind = j;                 //detecting player ind
                    break;
                }
            }
        }

        if (pos == start_ind[player_to_be_killed]) //you can't kill if the player is at safe place
        {
            if(player_to_be_killed == player)
            {
                cout<<"Token already present at that postion!\n";
            }
            else
            {
                cout<<"Not possible, player at start point!\n";
            }

            if (cal_token_inField(player) >= 2) //you can't place two tokens at one place if you have another token out move that
            {
                cout << "Select some other token!\n";

                token_movement(player, l, dice);

                check_kill_1 = true; //variable that will tell that we have checked killing condition
            }
            else
            {
                check_kill_1 = true; //in case user has only one token out of the home
                return;
            }
        }

        else if (player_to_be_killed == player) //if they belong to same grp no they can't kill each other
        {
            cout << "Group token not to kill!\n";

            if (cal_token_inField(player) >= 2) //just choose another to move
            {
                cout << "Select some other token!\n";

                token_movement(player, l, dice);

                check_kill_1 = true;
            }
            else
                return;
        }

        else //not at safe place and the prev tokken got killed
        {
            int ludo_board_pos = token_home_pos[player_to_be_killed][ind]; //we have stored the pos of killed token in indeces
            
            l.ludo_board[pos] = ' ';
            l.player_token_inField[player_to_be_killed][ind] = ' ';

            l.scores[player_to_be_killed][ind] = 0;      //score of this token is 0

            l.ludo_board[ludo_board_pos] = l.ludo_board[pos];
            l.playable_tokens[player_to_be_killed][ind] = l.tokens[player_to_be_killed][ind]; //now the token which is killed can be reused

            if (pos == start_ind[player]) //if the pos of a player is starting pos
            {
                player_token_inFieldCheck = true; //tell that it is availabe

                pthread_create(&tid[1], NULL, bring_token_toField, NULL); //create thread bringing token out
                pthread_join(tid[1], NULL);

                check_kill_1 = true; //signal that we have checked token
            }
        }
    }
}

void *bring_token_toField(void *arg)
{ 
    //Lock here so no other thread interfare
    sem_wait(&lock2);

    if (arrived_home(l.select_player) == 4 or cal_token_inField(l.select_player) == l.token_num) //4 is max and token can choose two
    {
        return NULL;                                                                             //if all tokens are out of home you cant bring it out
    }

    int ind;

    if (!player_token_inFieldCheck) //player has no token out of home
    {
        cout << "Token to bring out: ";

        for (int i = 0; i < l.token_num; i++)
        {
            if (l.playable_tokens[l.select_player][i] != ' ')         //the token ind which is not empty in home
            {
                cout << l.playable_tokens[l.select_player][i] << ' '; //now bring that out
            }
        }

        cout << ": ";

        do
        { //making the right choice
            cin >> player_token_inFieldchoice;

            if ((!available_token(l.select_player, player_token_inFieldchoice))) //if the token he is entring is not in our range
            {
                cout << "Invalid Choice! Choose Only Available Token : ";
            }

        } while (!available_token(l.select_player, player_token_inFieldchoice)); //until the user decides to play :)

        player_token_inFieldCheck = true;
    }

    for (int i = 0; i < l.token_num; i++)
    {
        if (l.tokens[l.select_player][i] == player_token_inFieldchoice)
        {
            ind = i;                              //finding ind of token choosen by player
        }
    }
    
    int start = start_ind[l.select_player]; //home out token ind

    if (player_token_inFieldCheck) // the token that is already out of home
    {
        l.ludo_board[start] = l.ludo_board[token_home_pos[l.select_player][ind]]; //put it on starting ind
        l.ludo_board[token_home_pos[l.select_player][ind]] = ' ';                 //putting space on home ind of token

        l.playable_tokens[l.select_player][ind] = ' ';                            //storing space in array so that we know token is out
        l.player_token_inField[l.select_player][ind] = player_token_inFieldchoice;

        player_token_inFieldCheck = false;
        int dice = rand() % 6 + 1;

        update_display(l.select_player);

        cout << "Another Turn on bringing the Token Out\nDie Rolled : " << dice << endl;

        token_movement(l.select_player, l, dice);
    }
    sem_post(&lock2);
}

void home_path_handler(int player, int ind, int dice, LUDO_GAME &l, int prev)
{
    if (l.scores[player][ind] >= 51)
    { //limit of score of each token that is out of home
        if (player == 1)
        {
            prev = l.scores[player][ind] + 22; //home location is 73 of player 1
        }
        else if (player == 2)
        {
            prev = l.scores[player][ind] + 32;
        }
        else if (player == 3)
        {
            prev = l.scores[player][ind] + 27;
        }
    }

    int ludo_boardpos = l.scores[player][ind] + dice;

    if (player == 1)
    {
        ludo_boardpos += 22;
    }
    else if (player == 2)
    {
        ludo_boardpos += 32;
    }
    else if (player == 2)
    {
        ludo_boardpos += 27;
    }

    l.ludo_board[ludo_boardpos] = l.ludo_board[prev];
    l.ludo_board[prev] = ' ';
    l.scores[player][ind] += dice; //increment score of player by the value of dice
}

void token_pos_handler(int player, int ind, int dice, LUDO_GAME &l)
{
    int pos;
    int start = start_ind[player];
    int prev = start + l.scores[player][ind]; //the token has moved from starting point to current point
    int total_dice = prev + dice;
    if (player >= 1)
    {
        if (prev > 67) //just make the whole thing  greater than 16
            prev %= 52;
        
        if (l.scores[player][ind] + dice > 50)
        {
            home_path_handler(player, ind, dice, l, prev);
            return;
        }

        if (prev == 67) //in case we are at 67 player  will move to no greater than or equal to 16
        {
            pos = 15 + dice;
            token_kill(ind, player, pos, l, dice); //check if it can kill the other token

            if (check_kill_1 or check_kill_2)
            {
                check_kill_1 = false;
                check_kill_2 = false;
                return;
            }
            l.ludo_board[pos] = l.ludo_board[prev];
        }

        else if (total_dice > 67) //adding dice is making it greater than 67 so make it greater than 16
        {
            pos = (total_dice) % 52;

            token_kill(ind, player, pos, l, dice); //kill token if it is doing so
            if (check_kill_1 or check_kill_2)
            {
                check_kill_1 = false;
                check_kill_2 = false;
                return;
            }
            l.ludo_board[pos] = l.ludo_board[prev]; // pos is still the prev one
        }

        else
        {
            pos = total_dice;
            token_kill(ind, player, pos, l, dice); //is it killing another token
            if (check_kill_1 or check_kill_2)
            {
                check_kill_1 = false;
                check_kill_2 = false;
                return;
            }
            l.ludo_board[pos] = l.ludo_board[prev]; // pos is still the prev one
        }

        l.ludo_board[prev] = ' '; //empty the prev location
    }
    else //in case the token is 0
    {
        pos = total_dice;
        token_kill(ind, player, pos, l, dice);
        if (check_kill_1 or check_kill_2)
        {
            check_kill_1 = false;
            check_kill_2 = false;
            return;
        }
        l.ludo_board[pos] = l.ludo_board[prev];
        l.ludo_board[prev] = ' ';
    }
    l.scores[player][ind] += dice; //update score of player
}

void token_movement(int player, LUDO_GAME &l, int dice)
{
    char cToken = 'u';
    bool move = true;
    if (dice == 6 and playable_tokens(player)) //if dice value  is 6 it gives two options to player
    {
        move = false;
        cout << "Do you Want to bring another token Out? \nPress y for Yes and n for No ";
        do
        {
            cin >> cToken;
            cout << endl;

            if (cToken != 'y' and cToken != 'n')
            {
                cout << "Wrong! Select y or n : ";
            }
        } while (cToken != 'y' and cToken != 'n');
    }
    if (cToken == 'y')
    {
        pthread_create(&tid[1], NULL, bring_token_toField, NULL);
        pthread_join(tid[1], NULL);
    }
    else if (cToken == 'n' or move) //user wanna move already present tokens on ludo_board
    {
        char choice;
        int ind;
        cout << "Which Token to move : ";
        do
        {
            ind = -1;
            cin >> choice;
            for (int i = 0; i < l.token_num; i++)
                if (l.tokens[player][i] == choice)
                    ind = i; //the ind where the choosen token is present

            if (ind == -1 or l.player_token_inField[player][ind] != choice)
                cout << "Token is not available! Enter Right choice : ";

        } while (ind == -1 or l.player_token_inField[player][ind] != choice); //Don't give wrong input :(
        int total_dice2 = l.scores[player][ind] + dice;

        if (total_dice2 == 56) //token has completed home path

        {
            cout << "Token Arrived Home!" << endl;

            tokens_won_fun(player, ind); //removing that token from list of that player
            return;
        }
        else if (total_dice2 > 55)
        {
            cout << "Cannot move ahead";
            if (cal_token_inField(player) == 2 or cal_token_inField(player) == 3) //as the token is at end point you can't move forward choose another if it is out of home
            {
                cout << "Select Another Token\n";
                token_movement(player, l, dice); //choose and move token
                check_kill_2 = true;
            }
            else
            {
                return;
            }
        }

        token_pos_handler(player, ind, dice, l); //make the pos according the ludo_board and player
        if (dice == 6)                                //it is first token coming out of home
        {
            dice = rand() % 6 + 1;
            update_display(player);
            cout << "Another Turn on a 6\n";
            cout << "Die Rolled : " << dice << endl;
            token_movement(player, l, dice);
        }
    }
}

//-----------------------------------------------------------
//FUNCTION TO CHECK AVAILABLE TOKENS OF EACH PLAYER WHEN A PLAYER SELECTS TO BRING IT OUT
bool available_token(int player, char ch)
{
    int count = 0;
    for (int i = 0; i < l.token_num; i++)
    {
        if (l.playable_tokens[player][i] == ch) //a player has token inside home or not
        {
            count++;
            return count;
        }
    }
    return count;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//FUNCTION TO CALCULATE THE NUMBER OF TOKEN THAT HAVE REACHED HOME BASE
int arrived_home(int player)
{
    int temp = 0;
    for (int i = 0; i < l.token_num; i++)
    {
        if (l.tokens_won[player][i] != ' ') //check how many tokens have reached home
        {
            temp += 1;
        }
    }
    return temp;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//FUNCTION FOR CHANGING PLAYER TURN AND UPDATING ludo_board DISPLAY
void update_display(int player)
{
    string player_name = l.player_name[player];
    cout << ".......Turn of " << player_name << "......." << endl;

    //AGAIN SAME THREAD FOR DISPLAYING ludo_board AS CREATED EARLIER IN START OF GAMEPLAY
    pthread_create(&tid[0], NULL, ludo_board_display, (void *)&l);
    pthread_join(tid[0], NULL);
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//FUNCTION TO CHECK AVAILABLE TOKENS ON ludo_board
int playable_tokens(int player)
{
    int temp = 0;
    for (int i = 0; i < l.token_num; i++)
    {
        if (l.playable_tokens[player][i] != ' ') //tokens out of home
        {
            temp++;
        }
    }
    return temp;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//FUNCTION TO CHECK THE NUMBER OF TOKENS IN FIELD OF ludo_board.
int cal_token_inField(int player)
{
    int temp = 0;
    for (int i = 0; i < l.token_num; i++)
    {
        if (l.player_token_inField[player][i] != ' ')
        {
            temp++;
        }
    }
    return temp;
}
//-----------------------------------------------------------

//-----------------------------------------------------------
//FUNCTION FOR ALL MAIN TOKEN CONDITIONS WHEN DICE IS ROLLED
void token_dice_conditions(int player, int dice)
{
    //Condition for token when there is neither a SIX on dice nor any token in field of ludo_board
    if ((dice != 6 and !cal_token_inField(l.select_player)))
    {
        return;
    }

    //Condition for token if there is a SIX on dice and there is not token in field so bringing a token in field of ludo_board.
    if (dice == 6 and !cal_token_inField(l.select_player))
    {
        //--------------------------------------------------------------
        //Each player will have its own thread taking the token out
        //--------------------------------------------------------------
        pthread_create(&tid[l.select_player], NULL, bring_token_toField, NULL);
        pthread_join(tid[l.select_player], NULL);
    }

    //Condition if there is already token in field of ludo_board then only moving that token is required.
    else if (cal_token_inField(l.select_player))
    {
        token_movement(l.select_player, l, dice);
    }
}
//-----------------------------------------------------------

//---------------------------------------------------------------
//=============== GAME LOGIC SEQUENCE ===========================
//---------------------------------------------------------------
void* start_game(void *arg)
{

    char start;
    int dice = 0;
    l.select_player = 0;
    char token_name = 'a';
    int cal_token_place = 0;

    //Inilializing ludo_board with default values in start
    for (int i = 0; i < 88; i++)
        l.ludo_board[i] = ' ';

    for (int i = 0; i < l.player_num; i++)
    {
        for (int j = 0; j < l.token_num; j++)
        {
            l.player_token_inField[i][j] = ' ';         //No Token on ludo_board so Empty Values in Start
            l.tokens_won[i][j] = ' ';                   //No Tokens reached so Empty Values in Start
            l.scores[i][j] = 0;                         //Starting score with zero
            cal_token_place = (i * 4) + j;              //Calculating placements of players tokens
            l.ludo_board[cal_token_place] = token_name; //Setting ludo_board tokens on required places
            l.tokens[i][j] = token_name;                //Main token names fill
            l.playable_tokens[i][j] = token_name;       //All tokens placed in start are now available to play
            token_name++;
        }
    }

    while (true)
    {
        for (int i = 0; i < l.player_num; i++)
        {
            do
            {
                l.select_player = rand() % l.player_num;
            } while (player_selection[l.select_player] == 1);

            //Displaying ludo_board with Threads
            pthread_create(&tid[l.select_player], NULL, ludo_board_display, (void *)&l); //separate thread for each thread
            pthread_join(tid[l.select_player], NULL);

            cout << endl;

            //Setting Player name and displaying their turn
            string player_name = l.player_name[l.select_player];
            cout << ".......Turn of " << player_name << "......." << endl;

            dice = rand() % 6 + 1;

            cout << "Dice Number: " << dice << endl;

            //Sleep to show Dice number and create suspense :)
            sleep(1);

            //Token Conditions
            token_dice_conditions(l.select_player, dice);

            player_selection[l.select_player] = 1;

            if (win)
            {
                break;
            }
        }

        if (win)
        {
            win = false;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            player_selection[i] = 0;
        }
    }
    pthread_exit(NULL);
}

//-----------------------------------------------------------
//===================== M . A . I . N =======================
//-----------------------------------------------------------
int main()
{
    //Seeding Random for later use of Dice values as well as for players selection.
    srand((time(0)));

    //Initializing all locks
    sem_init(&lock1, 0, 1);
    sem_init(&lock2, 0, 1);

    //Clear the display from start
    //system("clear");

    cout << "..........:::::::::: LUDO GAME ::::::::::.........." << endl;
    cout << "---------------------------------------------------" << endl;

    //----------------------------------------------------------------------------------
    //=================Number of Players Input
    cout << "Input the number of Players playing Ludo (2 - 4): ";
    do
    {
        cin >> l.player_num;
        if (l.player_num < 2 or l.player_num > 4)
        {
            cout << "The player range should be between 2-4!" << endl;
        }

    } while (l.player_num < 2 or l.player_num > 4);
    //----------------------------------------------------------------------------------

    cout << endl;

    //----------------------------------------------------------------------------------

    //==============Number of Tokens Input for each player
    cout << "Input the number of Tokens for each player (1 - 4): ";
    do
    {
        cin >> l.token_num;
        if (l.token_num < 1 or l.token_num > 4)
        {
            cout << "The token range should be between 1-4!" << endl;
        }

    } while (l.token_num < 1 or l.token_num > 4);
    //----------------------------------------------------------------------------------

    cout << "\nName Selection: " << endl;
    cout << "----------------" << endl;
    //----------------------------------------------------------------------------------

    //===============Input the player player_name
    for (int i = 0; i < l.player_num; i++)
    {
        cout << "Input name of player " << i + 1 << ": ";
        cin >> l.player_name[i];
    }
    //----------------------------------------------------------------------------------

    //Initializing all values on ludo_board according to the numbers chosen by the players.

    //Starting game engine :)
    pthread_create(&tid[6], NULL, start_game, NULL); //separate thread for each thread
    pthread_join(tid[l.select_player], NULL);

    pthread_exit(NULL);
}
