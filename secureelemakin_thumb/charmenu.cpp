#include "charmenu.h"

Menu::Menu(char* menutext, Menu* menuparent){
    text = menutext;
    parent = menuparent;
    next = NULL;
    prev = NULL;
    firstchild = NULL;
    lastchild = NULL;
    action = NULL;
    hoveraction = NULL;
    childrencount = 0;
    if (menuparent!=NULL) {
        if (menuparent->lastchild==NULL) {
            menuparent->firstchild = this;
            menuparent->lastchild = this;
            this->myposition = 0;
        }
        else {
            this->prev = menuparent->lastchild;
            menuparent->lastchild->next = this;
            menuparent->lastchild = this;
            this->myposition = menuparent->childrencount;
        }
        menuparent->childrencount += 1;
    }
}

//bind lcd before using
CharMenuClass::CharMenuClass(
            typefunction_lcddisplay _lcddisplay,
            typefunction_lcdgotoxy _lcdgotoxy,
            typefunction_lcdclear _lcdclear
) {
    lcddisplay = _lcddisplay;
    lcdgotoxy = _lcdgotoxy;
    lcdclear = _lcdclear;
    root = new Menu("root menu       ",NULL);
}

/**using this->current_menu
 * will display to lcd the current menu and execute hoveraction if any
 */
void CharMenuClass::show_current(){
    lcdclear();
    lcdgotoxy(0,0);
    lcddisplay(this->current_menu->text);
    if (this->current_menu->hoveraction!=NULL) {
        this->current_menu->hoveraction();//blocking is allowed
    }
    this->show_scroller();
}

void CharMenuClass::show_scroller(){
    lcdgotoxy(0,1);
    if (current_menu->parent==NULL) {
        lcddisplay("0               ");
    }
    sprintf(
        this->scroller_text,
        "%d / %d",
        current_menu->myposition+1,
        current_menu->parent->childrencount
    );
    lcddisplay(this->scroller_text);
}

void CharMenuClass::begin(){
    this->current_menu = root;
    this->show_current();
}

void CharMenuClass::action_enter(){
    if (this->current_menu->action!=NULL) {
        this->current_menu->action(); //blocking is allowed, action can use lcd or button as they like until it's done
    }
    if (this->current_menu->firstchild!=NULL){
        this->current_menu = current_menu->firstchild;
        this->show_current();
    }
}
void CharMenuClass::action_back(){
    if (this->current_menu->parent!=NULL){
        this->current_menu = this->current_menu->parent;
        this->show_current();
    }
}
void CharMenuClass::action_next(){
    if (this->current_menu->next!=NULL){
        this->current_menu = this->current_menu->next;
        this->show_current();
    }
    else{//reached rightmost menu, rollover to leftmost
        Menu* target = this->current_menu->prev;
        if (target==NULL) {
            //avoid retriggering hover action / redundant displaying
            return;
        }
        while (target!=NULL) {
            this->current_menu = target;
            target = this->current_menu->prev;
        }
        this->show_current();
    }
}
void CharMenuClass::action_prev(){
    if (this->current_menu->prev!=NULL){
        this->current_menu = this->current_menu->prev;
        this->show_current();
    }
    else{//reached leftmost menu, rollover to rightmost
        Menu* target = this->current_menu->next;
        if (target==NULL) {
            //avoid retriggering hover action / redundant displaying
            return;
        }
        while (target!=NULL) {
            this->current_menu = target;
            target = this->current_menu->next;
        }
        this->show_current();
    }
}
