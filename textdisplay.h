#ifndef __TEXTDISPLAY_H__
#define __TEXTDISLAY_H__

#include "observer.h"
#include <iostream>
#include <vector>

class TextDisplay:public Observer {
    std::vector<std::vector<char>> theDisplay; // The display

    public:
    TextDisplay(); // Default Constructor
    ~TextDisplay() = default; // Destructor

    void notify(Cell &c) override; // Update the display if it is notified

    friend std::ostream &operator<<(std::ostream &o, const TextDisplay &t); // Overloading operator<<
};

#endif
