/**
 * @author: makin
 * @date: 2023
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "enc.h"
#define array(tipe,n) (tipe *)malloc((n)*sizeof(tipe))

void free_block(){
    if (enc.block!=NULL){
        free(enc.block);
        enc.block = NULL;
    }
}

/**
uint8_t *encrypt(
    char* password
    ,int passwordlength
    ,char* message
    ,int messagelength
);
*/
static PyObject* py_encrypt(PyObject* self, PyObject* args) {
    const char* password;
    int password_length;
    const char* message;
    int unpad_message_length;

    if (!PyArg_ParseTuple(args, "s#s#", &password, &password_length, &message, &unpad_message_length)) {
        return NULL;
    }
    int message_length = 16;
    for (;message_length<16*15;message_length+=16){
        if (message_length>(unpad_message_length+8)){
            break;
        }
    }
    
    //~ free_block();
    //~ enc.block = array(uint8_t,16*15);
    enc.clear();
    enc.set_password((uint8_t*)password, (uint8_t)password_length);
    
    for (int i = 0; i < unpad_message_length; i++) {
        enc.block[i] = (uint8_t)message[i];
    }
    enc.encrypt_gcm((uint8_t)message_length);

    PyObject* result = PyBytes_FromStringAndSize(reinterpret_cast<const char*>(enc.block), message_length);
    
    //~ free_block();
    return result;
}

static PyObject* py_decrypt(PyObject* self, PyObject* args) {
    const char* password;
    int password_length;
    const char* chiper;
    int chiper_length;

    //~ if (!PyArg_ParseTuple(args, "sy#", &password, &chiper, &chiper_length)) {
    if (!PyArg_ParseTuple(args, "s#y#", &password, &password_length, &chiper, &chiper_length)) {
        return NULL;
    }
    enc.clear();
    enc.set_password((uint8_t*)password, (uint8_t)password_length);
    
    for (int i = 0; i < chiper_length; i++) {
        enc.block[i] = (uint8_t)chiper[i];
    }
    enc.decrypt_gcm(0,(uint8_t)chiper_length);
    
    int string_length = 0;
    for (int i = 0; i < chiper_length; i++) {
        if (enc.block[i]==0) {
            string_length = i;
            break;
        }
    }
    

    PyObject* result = PyBytes_FromStringAndSize(reinterpret_cast<const char*>(enc.block), string_length);
    
    //~ free_block();
    return result;
}
static PyMethodDef module_methods[] = {
    {"encrypt", py_encrypt, METH_VARARGS, "Encrypt data for securelemakin-dual secure storage"},
    {"decrypt", py_decrypt, METH_VARARGS, "Decrypt data for securelemakin-dual secure storage"},
    {NULL, NULL, 0, NULL}  // Sentinel element
};

static struct PyModuleDef libsecurelemakin = {
    PyModuleDef_HEAD_INIT,
    "libsecurelemakin",
    "python linux tool for securelemakin",
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_libsecurelemakin(void) {
    return PyModule_Create(&libsecurelemakin);
}
