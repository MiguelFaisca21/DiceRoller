#ifndef DICE_H
#define DICE_H

#include <string>

class Dice {
private:
    int numDice;
    int size;
    int modifier;

public:
    Dice(int n = 1, int s = 6, int m = 0);
    static Dice parseToDice(const std::string& input);
    int roll() const; 
    int rolld6() const;
    int getNumDice() const { return numDice; }
    int getSize() const { return size; }
    int getModifier() const { return modifier; }
    void statistics(int simulations) const;
    int simpleAnimation(int duration_ms = 1200, int fps = 15) const;
};

#endif
