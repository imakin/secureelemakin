#ifndef CHARMENU_H
#define CHARMENU_H

#include <Arduino.h>
#include <inttypes.h>

class Menu {
    public:
        Menu(char* menutext, Menu* menuparent);
        char* text;
        void (*action)(void);
        void (*hoveraction)(void);
        Menu* next;
        Menu* prev;
        Menu* firstchild;//leftmost direct children in menu cursor
        Menu* lastchild;//rightmost direct children in menu cursor
        Menu* parent;
        
        uint8_t myposition; //number position of this menu among its sibling
        uint8_t childrencount;//total amount of direct children of this menu
};


typedef void (*typefunction_lcdclear)(void);
typedef void (*typefunction_lcdgotoxy)(uint8_t,uint8_t);
typedef void (*typefunction_lcddisplay)(char*);

class CharMenuClass {
    public: 
        CharMenuClass(
            typefunction_lcddisplay,
            typefunction_lcdgotoxy,
            typefunction_lcdclear
        );
        typefunction_lcddisplay lcddisplay;
        typefunction_lcdgotoxy  lcdgotoxy;
        typefunction_lcdclear   lcdclear;
        
        void begin();

        void action_next();
        void action_prev();
        void action_back();
        void action_enter();
        void show_current();//refresh display. display current_menu text and execute the current_menu->hoveraction() if any
        Menu* root;
        
        Menu* current_menu;
    protected:
        void show_scroller();//display scroller in 2nd row of lcd
        char scroller_text[16];
};

#endif

