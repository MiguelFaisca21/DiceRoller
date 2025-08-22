#include "ASCIIDice.h"
#include "sleep_ms.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

ASCIIDice::ASCIIDice(const string& filePath) {
    loadFrames(filePath);
}

void ASCIIDice::loadFrames(const string& filePath) {
    ifstream file(filePath);
    if (!file) throw runtime_error("Cannot open frames file: " + filePath);
    string line, frame;
    bool inFrame = false;
    while (getline(file, line)) {
        if (line.find('`') != string::npos) {
            if (!inFrame) {
                inFrame = true;
                frame.clear();
            } else {
                inFrame = false;
                auto lines = splitLines(frame);
                size_t start = 0;
                while (start < lines.size() && 
                       lines[start].find_first_not_of(" \t") == string::npos)
                    ++start;
                vector<string> trimmed(lines.begin()+start, lines.end());

                frames.push_back(trimmed);
            }
        } else if (inFrame) {
            frame += line + "\n";
        }
    }
    if (frames.empty()) throw runtime_error("No frames loaded from file");
}

vector<string> ASCIIDice::splitLines(const string& s) {
    vector<string> lines;
    size_t start = 0;
    size_t pos;
    while ((pos = s.find('\n', start)) != string::npos) {
        lines.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    if (start < s.size()) lines.push_back(s.substr(start));
    return lines;
}

void ASCIIDice::animate(int duration_ms, int fps) {
    if (frames.empty()) return;
    int totalFrames = max(1, duration_ms * fps / 1000);
    int delay_ms = max(1, 1000 / max(1, fps));
    int frameCount = frames.size();
    srand(static_cast<unsigned int>(time(0)));
    cout << "\033[?25l";
    for (int i = 0; i < totalFrames; ++i) {
        int idx = i % frameCount;
        const auto& frame = frames[idx];
        cout << "\x1b[2J\x1b[H";
        for (const auto &line : frame)
            cout << line << "\n";
        cout.flush();
        sleep_ms(delay_ms);
    }
    cout << "\033[?25h";
}

void ASCIIDice::animateWithInverse(int duration_ms, int fps) {
    if (frames.empty()) return;
    int totalFrames = max(1, duration_ms * fps / 1000);
    int delay_ms = max(1, 1000 / max(1, fps));
    int frameCount = frames.size();
    bool forward = true;
    int idx = 0;
    for (int i = 0; i < totalFrames; ++i) {
        const auto& frame = frames[idx];
        cout << "\x1b[2J\x1b[H";
        for (const auto &line : frame)
            cout << line << "\n";
        cout.flush();
        sleep_ms(delay_ms);
        if (forward) {
            idx++;
            if (idx >= frameCount) {
                idx = frameCount - 2;  
                forward = false;
            }
        } else {
            idx--;
            if (idx < 0) {
                idx = 1; 
                forward = true;
            }
        }
    }
}

void ASCIIDice::showFirstFrame() {
    if (frames.empty()) {
        cout << "No frames loaded.\n";
        return;
    }
    cout << "\x1b[2J\x1b[H";
    const auto& frame = frames[0];
    size_t start = 0;
    while (start < frame.size() &&
           frame[start].find_first_not_of(" \t") == string::npos)
        ++start;
    for (size_t i = start; i < frame.size(); ++i)
        cout << frame[i] << "\n";
    cout.flush();
}
