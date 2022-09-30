#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

int keys[256];

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (int i = 0; i < 256; i++){
        if (key == i && action == GLFW_PRESS)
        {
            keys[i] = 1;
        }
        else if (key == i && action == GLFW_RELEASE){
            keys[i] = 0;
        }
    }
}

static PyObject* keyIsPressed(PyObject* self, PyObject* args){
    int key;

    if (!PyArg_ParseTuple(args, "i", &key)) return NULL;

    if (keys[key] == 1){
        Py_INCREF(Py_True);
        return Py_True;
    }
    else{
        Py_INCREF(Py_False);
        return Py_False;
    }
}