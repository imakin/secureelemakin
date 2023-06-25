#include "securedataflash.h"

/**
 * read chiper data inside (const char* filename)
 * at this->pos with this->length
 * @param uint8_t*: target_buffer to write into. 
 *      example 
 *      uint8_t buff = malloc(100);
 *      sdf->read(buff+30); //to write into buff at offset 30
 * @return int: length of data (this->length) or real length written
 */
int SecureDataFlash::read(uint8_t* target_buffer){
    File securefile = LittleFS.open("/securedata","r");
    securefile.seek(this->pos);
    int len = securefile.readBytes((char*)target_buffer, this->length);
    securefile.close();
    return len;
}
