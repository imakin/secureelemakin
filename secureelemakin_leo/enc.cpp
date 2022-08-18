//secureelemakin's customized encryption 
#include "enc.h"
const uint8_t EncClass::blocksize = 16*3;
const uint8_t EncClass::keysize = 16;
const uint8_t EncClass::ivsize = 12;
const uint8_t EncClass::authsize = 4;
const uint8_t EncClass::block_default_item = 0;
const uint8_t salt[5] = {209, 189, 50, 228, 251};
#define saltsize 5

#define array(tipe,n) (tipe *)malloc((n)*sizeof(tipe))
#define array8(n) (uint8_t *)malloc((n)*sizeof(uint8_t))

uint8_t *EncClass::block = array8(EncClass::blocksize);          //(uint8_t *)malloc(EncClass::blocksize * sizeof(uint8_t));
uint8_t *EncClass::key_iv_auth = 0;
uint8_t *EncClass::key = 0;
uint8_t *EncClass::iv = 0;
uint8_t *EncClass::auth = 0;

GCM<AESTiny128> *EncClass::gcm = new GCM<AESTiny128>();
HKDF<SHA256> *EncClass::hkdf = new HKDF<SHA256>();

EncClass::EncClass(){
    uint8_t i=0;
    for(i=0;i<EncClass::blocksize;i++) {
        EncClass::block[i] = EncClass::block_default_item;
    }
}

/**
 * assign pointer of key,iv,and auth from pointer of key_iv_auth
 */
void EncClass::reattach_key_iv_auth(){
    EncClass::key = EncClass::key_iv_auth;
    EncClass::iv = EncClass::key_iv_auth + EncClass::keysize;
    EncClass::auth = EncClass::key_iv_auth + EncClass::keysize + EncClass::ivsize;
}

/**
 * hkdf the password, reiterate (length) times, will use salt
 * @param password: the input password
 * @param length: password given could be at any length, as long as the microcontroller could handle it
 */
eswkqavoid EncClass::set_password(uint8_t *password, uint8_t length){
    uint8_t keyivauthsize = EncClass::keysize + EncClass::ivsize + EncClass::authsize;
    free(EncClass::key_iv_auth);
    EncClass::key_iv_auth = array8(keyivauthsize);
    EncClass::hkdf->clear();
    
    hkdf->setKey(password, length, salt, saltsize);
    uint8_t iteration = length;//iteration is (length), keep minimal to have stronger password
    for (uint8_t i=0;i<iteration;i++){
        EncClass::hkdf->extract(EncClass::key_iv_auth, keyivauthsize);
        EncClass::hkdf->setKey(EncClass::key_iv_auth, keyivauthsize);
    }
    reattach_key_iv_auth();
}


/**
 * clear EncClass::block into array of EncClass::key_default_item and hkdf and gcm
 */
void EncClass::clear(){
    uint8_t i=0;
    //prefil pad
    for (i=0;i<EncClass::blocksize;i++){
        EncClass::block[i] = EncClass::block_default_item;
    }
    EncClass::hkdf->clear();
    EncClass::gcm->clear();
}
/**
 * clear EncClass::block into array of EncClass::key_default_item
 */
void EncClass::clear_block(){
    uint8_t i=0;
    //prefil pad
    for (i=0;i<EncClass::blocksize;i++){
        EncClass::block[i] = EncClass::block_default_item;
    }
}

/**
 * encrypt EncClass::block with ::key, ::iv, and ::auth
 * @param blocksize: limit block reading (allowed value: 16,32, or 48. override default: EncClass::blocksize);
 * @return uint8_t: one byte tag computed from the gcm of this encrypt operation
 */
uint8_t EncClass::encrypt_gcm(uint8_t blocksize){
    EncClass::gcm->setKey(EncClass::key, EncClass::keysize);
    EncClass::gcm->setIV(EncClass::iv, EncClass::ivsize);
    EncClass::gcm->addAuthData(EncClass::auth,EncClass::authsize);
    EncClass::gcm->encrypt(EncClass::block, EncClass::block, blocksize);
    uint8_t *temp_tag = array8(1);
    uint8_t tag;
    EncClass::gcm->computeTag(temp_tag,1);
    tag = temp_tag[0];
    free(temp_tag);
    return tag;
}
uint8_t EncClass::encrypt_gcm(){
    return EncClass::encrypt_gcm(EncClass::blocksize);
}
/**
 * decrypt EncClass::block with ::key, ::iv, and ::auth
 * @param uint8_t tag: one byte tag to verify
 * @param blocksize: limit block reading (allowed value: 16,32, or 48. override default: EncClass::blocksize);
 * @return bool: false if tag is wrong, true if tag is correct
 */
bool EncClass::decrypt_gcm(uint8_t tag, uint8_t blocksize){
    EncClass::gcm->setKey(EncClass::key, EncClass::keysize);
    EncClass::gcm->setIV(EncClass::iv, EncClass::ivsize);
    EncClass::gcm->addAuthData(EncClass::auth,EncClass::authsize);
    EncClass::gcm->decrypt(EncClass::block, EncClass::block, blocksize);
    return EncClass::gcm->checkTag(&tag,1);
}
bool EncClass::decrypt_gcm(uint8_t tag){
    return EncClass::decrypt_gcm(tag, EncClass::blocksize);
}


EncClass enc = EncClass();
