#ifndef ASCIIDICE_H
#define ASCIIDICE_H

#include <string>
#include <vector>

class ASCIIDice {
private:
    std::vector<std::vector<std::string>> frames;
    void loadFrames(const std::string& filePath);
    std::vector<std::string> splitLines(const std::string& s);

public:
    ASCIIDice(const std::string& filePath);
    void animate(int duration_ms = 2000, int fps = 10);
    void animateWithInverse(int duration_ms = 2000, int fps = 10);
    void showFirstFrame();
};

#endif 
