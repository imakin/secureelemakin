//author: Muhammad Izzulmakin. copyright(c) 2022
#include "test.h"
#define ENABLE_TESTING
#ifdef ENABLE_TESTING

#include "button.h"
#include "charmenu.h"
#include "enc.h"
#include "securedata.h"

    #ifdef __arm__
    // should use uinstd.h to define sbrk but Due causes a conflict
    extern "C" char* sbrk(int incr);
    #else  // __ARM__
    extern char *__brkval;
    #endif  // __arm__
int freeMemory() {
    char top;
    #ifdef __arm__
      return &top - reinterpret_cast<char*>(sbrk(0));
    #elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
      return &top - __brkval;
    #else  // __arm__
      return __brkval ? &top - __brkval : &top - __malloc_heap_start;
    #endif  // __arm__
}


void assert_equal(int a, int b){
    if (a!=b) {
        Serial.print("test failed: expecting ");
        Serial.print(a);
        Serial.print("==");
        Serial.println(b);
    }
    else {
        Serial.print("test equal ok ");
        Serial.print(a);
        Serial.print("==");
        Serial.println(b);
    }
}

void test_securedata(){
    Serial.println("free RAM:");
    Serial.print(freeMemory());
    const uint8_t data_sample_[][16] = {
        {23, 65, 143, 230, 171, 167, 25, 212, 34, 44, 11, 190, 183, 229, 193, 69},
        {184, 64, 240, 173, 174, 36, 171, 14, 87, 209, 182, 119, 9, 42, 83, 127}
    };
    Serial.println("running automated test");
    Serial.println("testing securedata and securedata_sample.cpp");
    for (uint8_t i=0;i<2;i++) {
        for (uint8_t j=0;j<16;j++){
            uint8_t val = securedatas[i].getDataAt(j);
            Serial.print("data_sample_");Serial.print(i);
            Serial.print("[");Serial.print(j);Serial.print("]");
            Serial.print(": ");Serial.println(val);
            assert_equal(val, data_sample_[i][j]);
        }
    }
    Serial.print("free RAM:");
    Serial.print(freeMemory());
}

extern CharMenuClass charmenu;
void test_charmenu_checkhover(){
    
}
void test_charmenu(){
    Serial.println("testing charmenu and securedata.h");
    Serial.println("testing if current menu is root->testscript");
    assert_equal(&dotest, charmenu.current_menu->action);
    Serial.println("adding submenu for root -> testscript-new");
    String menutext = "new";
    menutext += millis();
    menutext += "       ";
    uint8_t *menutext_arr = malloc(sizeof(uint8_t)*16);
    menutext.toCharArray(menutext_arr, 16);
    Menu *menu_testnew = new Menu(menutext_arr, charmenu.root);
    Serial.println("adding sub submenu");
    Menu* menu_test_datasample0 = new Menu(securedatas[0].name, menu_testnew);
    Menu* menu_test_datasample1 = new Menu(securedatas[1].name, menu_testnew);
    
    Serial.println("charmenu trigger next button clicked");
    charmenu.action_next();
    Serial.println("testing if menu is correctly created");
    assert_equal(charmenu.current_menu, menu_testnew);
    
    Serial.println("charmenu trigger enter button clicked");
    charmenu.action_enter();
    Serial.println("testing if menu has correct 1st children ");
    assert_equal(charmenu.current_menu, menu_test_datasample0);
    
    Serial.println("charmenu trigger next button clicked");
    charmenu.action_next();
    Serial.println("testing if menu has correct 2nd children ");
    assert_equal(charmenu.current_menu, menu_test_datasample1);
    
    
    Serial.println("charmenu trigger back button clicked");
    charmenu.action_back();
    Serial.println("testing if back to parent ");
    assert_equal(charmenu.current_menu, menu_testnew);
    
    Serial.println("charmenu trigger back button clicked");
    charmenu.action_back();
    Serial.println("testing if back to root ");
    assert_equal(charmenu.current_menu, charmenu.root);
    
    
    
}

void dotest(){
    test_securedata();
    test_charmenu();
}

#endif
