#include "Dice.h"
#include "sleep_ms.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <string>
#include <map>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

Dice::Dice(int n, int s, int m) : numDice(n), size(s), modifier(m) {}

Dice Dice::parseToDice(const string& input) {
    int numDice = 1, size = 6, modifier = 0;
    size_t dPos = input.find('d');
    if (dPos == string::npos)
        throw invalid_argument("Invalid format: missing 'd'");
    string numStr = input.substr(0, dPos);
    if (!numStr.empty()) numDice = stoi(numStr);
    size_t plusPos = input.find('+', dPos);
    size_t minusPos = input.find('-', dPos);
    size_t modPos = (plusPos != string::npos) ? plusPos : minusPos;
    if (modPos != string::npos) {
        size = stoi(input.substr(dPos + 1, modPos - (dPos + 1)));
        modifier = stoi(input.substr(modPos));
    } else {
        size = stoi(input.substr(dPos + 1));
    }
    return Dice(numDice, size, modifier);
}

int Dice::roll() const {
    int total = 0;
    cout << "Rolling " << numDice << "d" << size;
    if (modifier != 0) cout << (modifier > 0 ? "+" : "") << modifier;
    cout << "...\n";

    for (int i = 0; i < numDice; i++) {
        int roll = (rand() % size) + 1;
        cout << "Die Result" << (i + 1) << ": " << roll << endl;
        total += roll;
    }
    total += modifier;
    cout << "Total: " << total << endl;
    return total;
}

int Dice::rolld6() const {
    int roll = (rand() % 6) + 1;
    cout << "Die Result: " << roll << endl;
    return roll;
}

void Dice::statistics(int simulations) const {
    map<int,int> frequency;
    long long totalSum = 0;
    for (int i = 0; i < simulations; i++) {
        int rollTotal = 0;
        for (int j = 0; j < numDice; j++) {
            rollTotal += (rand() % size) + 1;
        }
        rollTotal += modifier;
        totalSum += rollTotal;
        frequency[rollTotal]++;
    }
    double average = static_cast<double>(totalSum) / simulations;
    int maxFreq = 0;
    for (auto& pair : frequency) {
        if (pair.second > maxFreq) maxFreq = pair.second;
    }
    const int maxBarWidth = 50; 
    cout << "\n=== Statistics after " << simulations << " rolls ===\n";
    cout << "Average roll: " << average << "\n\n";
    cout << "Roll\tFreq\tProb(%)\tHistogram\n";
    for (auto& pair : frequency) {
        int roll = pair.first;
        int freq = pair.second;
        double prob = (freq * 100.0) / simulations;
        int barLength = static_cast<int>((freq * maxBarWidth) / maxFreq);
        cout << roll << "\t" 
             << freq << "\t" 
             << fixed << setprecision(2) << prob << "\t";
        for (int i = 0; i < barLength; i++) cout << "#";
        cout << "\n";
    }
}


static void clearScreenAnsi() {
    cout << "\x1b[2J\x1b[H";
}

static vector<string> renderD6Face(int face, int indent = 0, int skew = 0) {
    auto pad = [&](const string& s) { return string(indent, ' ') + s; };
    auto pip = [&](bool b){ return b ? 'o' : ' '; };
    bool tl = (face==4 || face==5 || face==6);
    bool tr = (face==2 || face==3 || face==4 || face==5 || face==6);
    bool bl = (face==2 || face==3 || face==4 || face==5 || face==6);
    bool br = (face==4 || face==5 || face==6);
    bool ml = (face==6);
    bool mr = (face==6);
    bool mm = (face==1 || face==3 || face==5);
    string skew1(skew, ' ');
    string skew2(skew*2/3, ' ');
    string skew3(skew/3, ' ');
    vector<string> box;
    box.push_back( pad(skew1 + "+-------+") );
    box.push_back( pad(skew2 + "| " + pip(tl) + "   " + pip(tr) + " |") );
    box.push_back( pad(skew3 + "| " + pip(ml) + " " + pip(mm) + " " + pip(mr) + " |") );
    box.push_back( pad(skew2 + "| " + pip(bl) + "   " + pip(br) + " |") );
    box.push_back( pad(skew1 + "+-------+") );
    return box;
}

static void renderSpinner(int value, int sides) {
    clearScreenAnsi();
    cout << "Rolling d" << sides << "...\n\n";
    cout << "   [" << value << "]\n";
    cout.flush();
}

int Dice::simpleAnimation(int duration_ms, int fps) const {
    const int frames = std::max(1, duration_ms * fps / 1000);
    const int delay_ms = std::max(1, 1000 / std::max(1, fps));
    int lastFace = 1;
    for (int i = 0; i < frames; ++i) {
        int face = (i % 6) + 1;
        lastFace = face;
        if (size == 6) {
            int indent = (i % 6 < 3) ? (i % 3) * 2 : (5 - (i % 6)) * 2; 
            int skew   = (i % 6 < 3) ? (i % 3)    : (5 - (i % 6));      
            auto art = renderD6Face(face, indent, skew);
            clearScreenAnsi();
            std::cout << "ASCII Dice Viewer (d6)\n\n";
            for (auto& line : art) std::cout << line << "\n";
            std::cout.flush();
        } else {
            int value = (rand() % size) + 1;
            renderSpinner(value, size);
        }
        sleep_ms(delay_ms); 
    }
    int finalFace = (rand() % size) + 1;
    if (size == 6) {
        auto art = renderD6Face(finalFace, 2, 1);
        clearScreenAnsi();
        std::cout << "ASCII Dice Viewer (d6)\n\n";
        for (auto& line : art) std::cout << line << "\n";
        std::cout << "\nResult: " << finalFace << "\n";
        std::cout.flush();
    } else {
        renderSpinner(finalFace, size);
        std::cout << "\nResult: " << finalFace << "\n";
        std::cout.flush();
    }
    return finalFace;
}


