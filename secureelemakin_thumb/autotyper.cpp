#include "autotyper.h"
//~ #define AUTOTYPER_DEBUG

Autotyper::Autotyper(uint8_t *sequence_index, uint8_t *sequence_action, uint8_t sequence_length, uint8_t *name){
    this->name = name;
    this->sequence_index = sequence_index;
    this->sequence_action = sequence_action;
    this->sequence_length = sequence_length;
}

//workaround (void*)() to be compatible with charmenu, also it will save ram if the function can be made one instead of one for each object
void autotyper_execute(){
    charmenu.lcdclear();
    charmenu.lcdgotoxy(0,0);
    charmenu.lcddisplay("input master key");
    set_password_with_button_sequence(10,2,false);
    
    //the selected autotyper in charmenu has "mycurrentpos" which represents the index in autotypers array
    uint8_t i = charmenu.current_menu->myposition;
    autotyper_execute(i);
    charmenu.show_current();//back to charmenu (make lcd to show current menu)
}

//execute autotyper[autotyper_index]
void autotyper_execute(uint8_t autotyper_index){
    
    Autotyper current_autotyper = autotypers[autotyper_index];//current_autotyper found

    uint8_t pos=0;
    for (pos=0;pos<current_autotyper.sequence_length;pos++){
        //autotyper contains sequence of securedata
        SecureData current_securedata = securedatas[
            current_autotyper.sequence_index[pos]
        ];
        //autotyper contains sequence of action
        uint8_t current_sequence_action = current_autotyper.sequence_action[pos];
        enc.clear_block();
        for (uint8_t j=0;j<current_securedata.length;j++){
            enc.block[j] = SecureData::getDataAt(current_securedata,j);//current_securedata.getDataAt(j);
        }
        enc.decrypt_gcm(99,current_securedata.length);
        #ifdef AUTOTYPER_DEBUG
        Serial.println("suppose to write:");
        #endif
        for (uint8_t j=0;j<current_securedata.length;j++){
            if (enc.block[j]!=0 && enc.block[j]!=EncClass::block_default_item){
                #ifdef AUTOTYPER_DEBUG
                Serial.write((char)enc.block[j]);
                #endif
                Keyboard.print((char)enc.block[j]);
            }
        }
        delay(1000);
        if (current_sequence_action==AUTOTYPER_ENTER) {
            #ifdef AUTOTYPER_DEBUG
            Serial.println("[ENTER]");
            #endif
            Keyboard.press(KEY_RETURN);
        }
        else if (current_sequence_action==AUTOTYPER_TAB){
            #ifdef AUTOTYPER_DEBUG
            Serial.println("[TAB]");
            #endif
            Keyboard.press(KEY_TAB);
        }
        delay(100);
        Keyboard.releaseAll();
        delay(4000);
    }
    enc.clear();
}

//type one secure data, instead of Autotyper (alias sequence of secure data)
void autotyper_execute_single(SecureData current_securedata)
{
    enc.clear_block();
    for (uint8_t j=0;j<current_securedata.length;j++){
        enc.block[j] = SecureData::getDataAt(current_securedata,j);//current_securedata.getDataAt(j);
    }
    enc.decrypt_gcm(99,current_securedata.length);
    #ifdef AUTOTYPER_DEBUG
    Serial.println("suppose to write:");
    #endif
    for (uint8_t j=0;j<current_securedata.length;j++){
        if (enc.block[j]!=0 && false==EncClass::is_block_default_item(enc.block[j])){
            #ifdef AUTOTYPER_DEBUG
            Serial.write((char)enc.block[j]);
            #endif
            Keyboard.print((char)enc.block[j]);
        }
    }
    enc.clear();
    delay(100);
    Keyboard.releaseAll();
    delay(500);
}

void autotyper_key(char key){
    Keyboard.press(key);
    delay(100);
    Keyboard.releaseAll();
}
void autotyper_return(){
    autotyper_key(KEY_RETURN);
}
void autotyper_tab(){
    autotyper_key(KEY_TAB);
}

/**
 * register the array autotypers to charmenu under *parent
 * @param Menu *parent: register the array autotypers to charmenu under *parent
 */
void autotypers_register_charmenu(Menu *parent) {
    for (uint8_t i=0;i<autotypers_size;i++){
        Menu *_menu = new Menu(autotypers[i].name, parent);
        _menu->action = &autotyper_execute;
    }
}
