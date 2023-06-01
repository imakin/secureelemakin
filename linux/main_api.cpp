/**
 * @author: makin
 * @date: 2023
 */
#include <Python.h>
#include "enc.h"
#define array(tipe,n) (tipe *)malloc((n)*sizeof(tipe))

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
    uint8_t password_length;
    const char* message;
    uint8_t message_length;

    if (!PyArg_ParseTuple(args, "ss", &password, &message)) {
        return NULL;
    }
    password_length = (uint8_t) strlen(password);
    message_length = (uint8_t) strlen(message);

    enc.clear();
    enc.set_password((uint8_t*)password, password_length);
    
    for (int i = 0; i < message_length; i++) {
        enc.block[i] = (uint8_t)message[i];
    }
    enc.encrypt_gcm(message_length);

    PyObject* result = PyBytes_FromStringAndSize(reinterpret_cast<const char*>(enc.block), message_length);
    return result;
}

static PyMethodDef module_methods[] = {
    {"encrypt", py_encrypt, METH_VARARGS, "Encrypt data for securelemakin-dual secure storage"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef libsecurelemakintool = {
    PyModuleDef_HEAD_INIT,
    "libsecurelemakintool",
    "python linux tool for securelemakin",
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_libsecurelemakintool(void) {
    return PyModule_Create(&libsecurelemakintool);
}
