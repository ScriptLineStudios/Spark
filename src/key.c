#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

int keys[256];
int mouse_clicks[2];

#define A 65
#define B 66
#define C 67
#define D 68
#define E 69
#define F 70
#define G 71
#define H 72
#define I 73
#define J 74
#define K 75
#define L 76
#define M 77
#define N 78
#define O 79
#define P 80
#define Q 81
#define R 82
#define S 83
#define T 84
#define U 85
#define V 86
#define W 87
#define X 88
#define Y 89
#define Z 90
#define SPACE 32

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //for (int i = 0; i < 256; i++){
    if (action == GLFW_PRESS)
    {
        keys[key] = 1;
    }
    else if (action == GLFW_RELEASE){
        keys[key] = 0;
    }
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    /*if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
       double xpos, ypos;
       glfwGetCursorPos(window, &xpos, &ypos);
       printf("%f %f\n", xpos / windowX, ypos / windowY);
    }*/
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