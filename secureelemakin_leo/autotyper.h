//secureelemakin autotyper
#ifndef AUTOTYPE_H
#define AUTOTYPE_H

#include <Arduino.h>
#include "Keyboard.h"
#include "charmenu.h"
#include "securedata.h"
#include "enc.h"

#define AUTOTYPER_NONE 0//do nothing
#define AUTOTYPER_TAB 179//press tab
#define AUTOTYPER_ENTER 176//press enter
#define AUTOTYPER_RETURN 176//press enter

/**
 * autotyper is index sequence of securedatas
 */
class Autotyper {
    public:
        Autotyper(uint8_t *sequence_index, uint8_t *sequence_action, uint8_t sequence_length, uint8_t *name);
        uint8_t *sequence_index; //sequence of index of securedatas
        uint8_t *sequence_action;//sequence action to do after typing securedatas[i]
        uint8_t sequence_length;//length of sequence_index and sequence_action
        uint8_t *name;
        //~ void execute();//use autotyper_execute() instead to save SRAM and to be compatible with charmenu
};
extern const uint8_t autotypers_size;
extern Autotyper autotypers[];
extern CharMenuClass charmenu; //declared in secureelemakin_leo
extern uint8_t set_password_with_button_sequence(uint8_t first_input_timeout, uint8_t between_input_timeout, bool debug); //declared in secureelemakin_leo

extern void autotyper_execute();
/**
 * register the array autotypers to charmenu under *parent
 * @param Menu *parent: register the array autotypers to charmenu under *parent
 */
void autotypers_register_charmenu(Menu *parent);

#endif
