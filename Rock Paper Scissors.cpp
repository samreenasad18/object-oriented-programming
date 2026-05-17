#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <windows.h>
using namespace std;

// ?? ANSI Color Codes ????
#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define CYAN      "\033[36m"
#define MAGENTA   "\033[35m"
#define WHITE     "\033[97m"
#define BG_RED    "\033[41m"
#define BG_GREEN  "\033[42m"
#define BG_YELLOW "\033[43m"

// ?? Unicode Emojis ????
#define GAME_CONTROLLER "\U0001F3AE"
#define STAR            "\U0001F31F"
#define ROCK            "\U0001FAA8"
#define PAPER           "\U0001F4C4"
#define SCISSORS        "\u2702"
#define CROSS           "\u274C"
#define TROPHY          "\U0001F3C6"
#define CELEBRATE       "\U0001F389"
#define HANDSHAKE       "\U0001F91D"
#define COMPUTER        "\U0001F4BB"
#define BRAIN           "\U0001F9E0"
#define DICE            "\U0001F3B2"
#define WAVE            "\U0001F44B"
#define SWORDS          "\u2694"
#define FIRE            "\U0001F525"
#define LIGHTNING       "\u26A1"
#define GIFT            "\U0001F381"
#define SHIELD          "\U0001F6E1"

// ?? Unicode Box-Drawing ????
#define DBL_HORIZ  "\u2550"
#define BOLD_HORIZ "\u2501"
#define THIN_HORIZ "\u2500"
#define VERT       "\u2502"
#define TOP_LEFT   "\u250C"
#define TOP_RIGHT  "\u2510"
#define BOT_LEFT   "\u2514"
#define BOT_RIGHT  "\u2518"


//  CLASS 1: Player  (Abstract Base Class)
//  Holds name and score. Every player must implement makeChoice().

class Player 
{
private:
    string name;
    int    score;

public:
    //constructor 
    Player(string n) : name(n), score(0) {}

    //virtual destructor
    virtual ~Player() {}

    //pure virtual function
    virtual char makeChoice() = 0;

    //getters
    string getName()  const { return name; }
    int    getScore() const { return score; }

    //function to add score
    void addScore(int amount) { score += amount; }

    //function to reset score
    void resetScore() { score = 0; }
};


//  CLASS 2: HumanPlayer  (extends Player)
//  Reads R / P / S from keyboard.
//  Tracks winStreak for the even-number bonus system.

class HumanPlayer : public Player
{
private:
    char lastMove;   // used by HardComputer to counter the human
    int  winStreak;  // consecutive rounds won; resets on loss or tie

public:
    //default constructor
    HumanPlayer(string n) : Player(n), lastMove(' '), winStreak(0) {}

    //virtual function usage
    char makeChoice() override 
    {
        char move;
        while (true) {
            cout << "\n  Your move (R = Rock / P = Paper / S = Scissors): ";
            cin >> move;
            move = toupper(move);
            if (move == 'R' || move == 'P' || move == 'S') {
                lastMove = move;
                return move;
            }
            cout << RED << "  " << CROSS << " Invalid input! Enter R, P, or S.\n" << RESET;
        }
    }

    //getters
    char getLastMove()  const { return lastMove; }
    int  getWinStreak() const { return winStreak; }

    //function to increment streak
    void incrementStreak() { winStreak++; }

    //function to reset streak
    void resetStreak() { winStreak = 0; }
};

//  CLASS 3: ComputerPlayer  (Abstract, extends Player)

class ComputerPlayer : public Player 
{
public:
    //destructor
    ComputerPlayer(string n) : Player(n) {}

    //virtual function implementation
    virtual char makeChoice() = 0;

    //virtual destructor
    virtual ~ComputerPlayer() {}
};


//  CLASS 4: EasyComputer  (Level 1 — always Rock)

class EasyComputer : public ComputerPlayer 
{
public:
    //destructor
    EasyComputer() : ComputerPlayer("Easy Computer") {}

    //virtual function implementation
    char makeChoice() override { return 'R'; }
};


//  CLASS 5: MediumComputer  (Level 2 — random move)

class MediumComputer : public ComputerPlayer {
public:

    //constructor
    MediumComputer() : ComputerPlayer("Medium Computer") {}

    ////virtual function implementation
    char makeChoice() override {
        int r = rand() % 3;
        if (r == 0) return 'R';
        if (r == 1) return 'P';
        return 'S';
    }
};


//  CLASS 6: HardComputer  (Level 3 — counters human's last move)

class HardComputer : public ComputerPlayer {
private:
    HumanPlayer* humanRef;

public:
    //constructor
    HardComputer(HumanPlayer* h) : ComputerPlayer("Hard Computer"), humanRef(h) {}

    ////virtual function implementation
    char makeChoice() override {
        char last = humanRef->getLastMove();
        if (last == ' ') {           // first round — no history yet
            int r = rand() % 3;
            if (r == 0) return 'R';
            if (r == 1) return 'P';
            return 'S';
        }
        if (last == 'R') return 'P'; // Paper beats Rock
        if (last == 'P') return 'S'; // Scissors beats Paper
        return 'R';                  // Rock beats Scissors
    }
};

//  CLASS 7: Game

class Game
{
private:
    HumanPlayer* human;
    ComputerPlayer* computer;
    int totalRounds;

    // Convert move letter to display name
    string moveName(char m) {
        if (m == 'R') return ROCK   " Rock";
        if (m == 'P') return PAPER  " Paper";
        return             SCISSORS "  Scissors";
    }

    // Returns 'H', 'C', or 'T'
    char evaluateRound(char h, char c) {
        if (h == c) return 'T';
        if ((h == 'R' && c == 'S') ||
            (h == 'P' && c == 'R') ||
            (h == 'S' && c == 'P')) return 'H';
        return 'C';
    }

    // Draw the result box for one round
    void displayRoundResult(int round, char hMove, char cMove,
        char result, int streak, bool bonusAwarded) {
        cout << CYAN;
        cout << "\n  " << TOP_LEFT
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << TOP_RIGHT << "\n";

        cout << "  " << VERT << RESET
            << "  " << SWORDS << " Round " << round << " Result                          "
            << CYAN << VERT << "\n";
        cout << "  " << VERT << RESET
            << "                                             " << CYAN << VERT << "\n";
        cout << "  " << VERT << RESET
            << "  You played : " << YELLOW << BOLD << moveName(hMove) << RESET
            << "                  " << CYAN << VERT << "\n";
        cout << "  " << VERT << RESET
            << "  Computer   : " << MAGENTA << BOLD << moveName(cMove) << RESET
            << "                  " << CYAN << VERT << "\n";
        cout << "  " << VERT << RESET
            << "                                             " << CYAN << VERT << "\n";

        if (result == 'H') {

            // ?? Human wins ??
            cout << "  " << VERT << GREEN << BOLD
                << "  " << CELEBRATE << " YOU WIN THIS ROUND!                      "
                << RESET << CYAN << VERT << "\n";

            // Streak line — show bonus 
            cout << "  " << VERT << RESET
                << "  " << FIRE << " Win Streak: " << GREEN << BOLD << streak << RESET;

            if (bonusAwarded) {
                cout << "   " << BG_YELLOW << WHITE << BOLD
                    << " " << LIGHTNING << " STREAK x" << streak
                    << "! BONUS +1 POINT " << GIFT << " "
                    << RESET << "     " << CYAN << VERT << "\n";
            }
            else {
                cout << "                                  " << CYAN << VERT << "\n";
            }

        }
        else if (result == 'C') {
            // ?? Computer wins ???
            cout << "  " << VERT << RED << BOLD
                << "  " << COMPUTER << " COMPUTER WINS THIS ROUND!                "
                << RESET << CYAN << VERT << "\n";
            cout << "  " << VERT << RESET
                << "  " << SHIELD << " Streak reset to 0"
                << "                        " << CYAN << VERT << "\n";
        }
        else {
            // ?? Tie ??
            cout << "  " << VERT << YELLOW << BOLD
                << "  " << HANDSHAKE << " IT'S A TIE!                              "
                << RESET << CYAN << VERT << "\n";
            cout << "  " << VERT << RESET
                << "  " << SHIELD << " Streak reset to 0"
                << "                        " << CYAN << VERT << "\n";
        }

        cout << "  " << VERT << RESET
            << "                                             " << CYAN << VERT << "\n";
        cout << "  " << VERT << RESET
            << "  Score --> You " << GREEN << human->getScore() << RESET
            << "  -  Computer " << RED << computer->getScore() << RESET
            << "               " << CYAN << VERT << "\n";

        cout << "  " << BOT_LEFT
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
            << BOT_RIGHT << "\n" << RESET;
    }

public:
    //constructor
    Game(HumanPlayer* h, ComputerPlayer* c, int rounds = 3)
        : human(h), computer(c), totalRounds(rounds) {
    }

    //function to start game
    void playMatch() {
        // Reset everything at the start of each match
        human->resetScore();
        human->resetStreak();
        computer->resetScore();

        cout << YELLOW << BOLD;
        cout << "\n  "
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ << "\n";
        cout << "  " << GAME_CONTROLLER
            << "  Best of " << totalRounds
            << "  vs  " << computer->getName() << "\n";
        cout << "  "
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ
            << BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ BOLD_HORIZ << "\n" << RESET;

        // ?? Round loop ??
        for (int round = 1; round <= totalRounds; round++) {
            cout << CYAN << "\n  " << SWORDS
                << "  Round " << round << " of " << totalRounds << RESET;

            char hMove = human->makeChoice();
            char cMove = computer->makeChoice();
            char result = evaluateRound(hMove, cMove);

            bool bonusAwarded = false;

            if (result == 'H') {
                // ?? Human wins this round ??

                // Step 1: normal +1 point for winning the round
                human->addScore(1);

                // Step 2: increase the consecutive win streak
                human->incrementStreak();
                int streak = human->getWinStreak();


                if (streak % 2 == 0) {
                    human->addScore(1);  // +1 bonus point
                    bonusAwarded = true;
                }

            }
            else {
                // ?? Computer wins or Tie ???
                if (result == 'C') computer->addScore(1);

                // Reset streak — bonus chain is broken
                human->resetStreak();
            }

            displayRoundResult(round, hMove, cMove,
                result, human->getWinStreak(), bonusAwarded);
        }

        // ?? Final match result ??
        cout << BOLD << "\n  "
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ << "\n";

        if (human->getScore() > computer->getScore())
            cout << BG_GREEN << WHITE
            << "  " << TROPHY << "  YOU WIN THE MATCH "
            << human->getScore() << " - " << computer->getScore()
            << "!  " << TROPHY << "  " << RESET << "\n";
        else if (computer->getScore() > human->getScore())
            cout << BG_RED << WHITE
            << "  " << COMPUTER << "  COMPUTER WINS THE MATCH "
            << computer->getScore() << " - " << human->getScore()
            << "!  " << RESET << "\n";
        else
            cout << YELLOW
            << "  " << HANDSHAKE << "  MATCH IS A TIE "
            << human->getScore() << " - " << computer->getScore()
            << "!" << RESET << "\n";

        cout << BOLD << "  "
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
            << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ << "\n" << RESET;
    }
};
//  Banner & menu helpers

void showBanner() {
    cout << CYAN << BOLD;
    cout << "\n  "
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ "\n";
    cout << "     " << GAME_CONTROLLER
        << "  ROCK  PAPER  SCISSORS  --  3 LEVEL CHALLENGE  "
        << GAME_CONTROLLER << "\n";
    cout << "  "
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ "\n" << RESET;
}

void showLevelMenu() {
    cout << YELLOW << BOLD;
    cout << "\n  " << TOP_LEFT
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ << TOP_RIGHT << "\n";
    cout << "  " << VERT << "       " << STAR
        << "  Choose Computer Difficulty  " << STAR << "           " << VERT << "\n";
    cout << "  " << VERT << "                                                     " << VERT << "\n";
    cout << "  " << VERT << "   1.  " << ROCK
        << "  Easy   --  Computer Always Plays Rock      " << VERT << "\n";
    cout << "  " << VERT << "   2.  " << DICE
        << "  Medium --  Computer Plays Randomly         " << VERT << "\n";
    cout << "  " << VERT << "   3.  " << BRAIN
        << "  Hard   --  Computer Counters Your Last     " << VERT << "\n";
    cout << "  " << VERT << "                                                     " << VERT << "\n";
    cout << "  " << BOT_LEFT
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ THIN_HORIZ
        << THIN_HORIZ THIN_HORIZ THIN_HORIZ << BOT_RIGHT << "\n" << RESET;
    cout << "  Your choice (1 / 2 / 3): ";
}

//function to get level
int getLevel()
{
    int level;
    while (true) {
        cin >> level;
        if (level == 1 || level == 2 || level == 3) return level;
        cout << RED << "  " << CROSS << " Invalid! Enter 1, 2, or 3: " << RESET;
    }
}


//  main()

int main() {
    SetConsoleOutputCP(65001);
    srand(time(0));

    cout << CYAN << BOLD << "\n  Enter your name: " << RESET;
    string playerName;
    cin >> playerName;

    HumanPlayer human(playerName);
    char playAgain = 'Y';

    while (toupper(playAgain) == 'Y') {

        showBanner();
        showLevelMenu();
        int level = getLevel();

        ComputerPlayer* computer = nullptr;

        //for level 1
        if (level == 1) {
            computer = new EasyComputer();
            cout << GREEN << BOLD << "\n  " << ROCK
                << "  Level 1: EASY -- Computer is stuck on Rock!\n" << RESET;
        }
        // for level 2
        else if (level == 2) {
            computer = new MediumComputer();
            cout << MAGENTA << BOLD << "\n  " << DICE
                << "  Level 2: MEDIUM -- Computer is unpredictable!\n" << RESET;
        }
        //for level 3
        else
        {
            computer = new HardComputer(&human);
            cout << RED << BOLD << "\n  " << BRAIN
                << "  Level 3: HARD -- Computer learns your moves!\n" << RESET;
        }


        Game game(&human, computer);
        game.playMatch();

        delete computer;   //will delete all dynamic mmemories
        computer = nullptr;

        cout << "\n  Play again? (Y / N): ";   //yes or no?
        cin >> playAgain;
    }

    cout << CYAN << BOLD << "\n  "
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ "\n";
    cout << "  " << WAVE << "  Thanks for playing, "
        << human.getName() << "! See you next time!  " << WAVE << "\n";
    cout << "  "
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ DBL_HORIZ
        << DBL_HORIZ DBL_HORIZ DBL_HORIZ "\n\n" << RESET;

    return 0;
}
