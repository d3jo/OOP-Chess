#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include "observer.h"
#include "window.h"
#include <vector>

class GraphicsDisplay:public Observer {
    Xwindow &window; // The display window
    
    public:
    GraphicsDisplay(Xwindow &window); // Constructor
    ~GraphicsDisplay() = default; // Destructor

    void notify(Cell &c) override; // Update window if it is notified
};

#endif
