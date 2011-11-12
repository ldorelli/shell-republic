#ifndef MyTypo_hpp
#define MyTypo_hpp

#include <iostream>


/**
 * \brief Firulas
 *
 *        Serve apenas para deixar a saida mais bonita, com diferentes
 *        cores e estilos para a fonte, no terminal
 */
class MyTypo {
    
private:
    
    int style;
    int foreground;
    int background;
    bool opened;
    
public:
    
    void set_foreground (int color);
    void set_background (int color);
    void set_style      (int color);
    
    int get_foreground (void);
    int get_background (void);
    int get_style      (void);
    bool get_opened    (void);
    
    MyTypo (int style = NORMAL, int foreground = BLACK, int background = B_TRANSPARENT);
    
    void toogleOpened (void);
    
    void setParameters (int style = NORMAL, int foreground = BLACK, int background = B_TRANSPARENT);
    
    std::ostream& openTag (std::ostream& ost);
    
    std::ostream& closeTag (std::ostream& ost);
    
    enum Style {
        NORMAL = 0,
        BOLD,
        UNDER = 4,
        BLINK,
        INVERT = 7
    };
    
    enum Foreground {
        BLACK = 30,
        RED,
        GREEN,
        BROWN,
        BLUE,
        PURPLE,
        CYAN,
        GRAY
    };
    
    enum Background {
        B_TRANSPARENT = -1,
        B_BLACK = 40,
        B_RED,
        B_GREEN,
        B_BROWN,
        B_BLUE,
        B_PURPLE,
        B_CYAN,
        B_GRAY
    };
    
};

std::ostream& operator<< (std::ostream& ost, MyTypo& mt);

#endif
