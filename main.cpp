#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include "Dice.h"
#include "ASCIIDice.h"

using namespace std;

int safeReadInt(int defaultValue) {
    int value;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return defaultValue;
    }
    return value;
}

void waitForEnter() {
    cout << "\nPress Enter to Go Back to Main Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    while (true) { 
        cout << R"(====== Dice Roller (Main Menu) ======

Choose mode:
  1) Roll a single d6 (w/ cube animation)
  2) Custom roll 
  3) Custom roll (w/ statistics)
  0) Exit
Enter choice: )";

        int choice = safeReadInt(-1);
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        try {
            if (choice == 0) {
                cout << "Exiting...\n";
                break;
            }
            else if (choice == 1) {
                Dice dice(1, 6, 0);
                ASCIIDice anim("data//frames.txt");
                anim.animateWithInverse(2500, 15); 
                dice.rolld6();
                waitForEnter();
            }
            else if (choice == 2 || choice == 3) {
                string input;
                cout << R"(=====> Roll Dice in Standard Notation: NdM +- K

Where: 
N = number of dice (default 1 if omitted) 
M = number of sides on each die 
K = optional modifier (can be + or -) 

Examples: 
3d6 -> roll 3 six-sided dice 
1d20+5 -> roll 1 twenty-sided die and add 5 
4d8-2 -> roll 4 eight-sided dice and subtract 2

Please Enter Your Roll: )";

                cin >> input;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Dice dice = Dice::parseToDice(input);
                if (choice == 2) {
                    if (dice.getNumDice() == 1 && dice.getSize() == 6 && dice.getModifier() == 0) {
                        ASCIIDice anim("data//frames.txt");
                        anim.animateWithInverse(2500, 15);
                    }
                    dice.roll();
                } else if (choice == 3) {
                    cout << "Enter number of simulations: ";
                    int sims = safeReadInt(1000);
                    dice.statistics(sims);
                }
                waitForEnter();
            } 
            else {
                cout << "Invalid choice. Try again.\n";
                waitForEnter();
            }
        } 
        catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
            waitForEnter();
        }
        cout << "\x1b[2J\x1b[H";
    }
    return 0;
}
