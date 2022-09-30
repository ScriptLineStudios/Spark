#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"


GLFWwindow* window;

static PyObject* version(PyObject* self){
    return Py_BuildValue("s", "Version 1.0");
}
double prevTime = 0.0;
double crntTime = 0.0;
double timeDiff;
unsigned int counter = 0;
int FPS;

#define GLSL(src) "#version 330 core\n" #src

GLuint shaders[256];
int shader_index = 0;

GLuint textures[256];

int windowX;
int windowY;

static PyObject* init(PyObject* self, PyObject* args){
    const char *title;
    int x;
    int y;
    bool unlockFPS = false;

    if (!PyArg_ParseTuple(args, "siib", &title, &x, &y, &unlockFPS)){
        return NULL;
    }

    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(x, y, title, NULL, NULL);
    if (window == NULL)
	{
		glfwTerminate();
	}
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    gladLoadGL();
    glViewport(0, 0, x, y);
    glMatrixMode(GL_PROJECTION);
    float aspect = (float)x / (float)y;
    glOrtho(-aspect, aspect, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    windowX = x;
    windowY = y;

    if (unlockFPS){
        glfwSwapInterval(0);
    }    

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject* windowClose(PyObject* self){
    if (!glfwWindowShouldClose(window))
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* getFps(PyObject* self){
    return PyLong_FromLong(FPS);
}

static PyObject* setTitle(PyObject* self, PyObject* args){
    const char * title;

    if (!PyArg_ParseTuple(args, "s", &title))
        return NULL;

    glfwSetWindowTitle(window, title);
    
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject* render(PyObject* self){
    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    counter++;

    if (timeDiff >= 1.0 / 30.0)
    {
        FPS = ((1.0 / timeDiff) * counter);
        prevTime = crntTime;
        counter = 0;
    }
    
    glfwSwapBuffers(window); //Update 
    glfwPollEvents();
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* clearScreen(PyObject* self, PyObject* args){
    float r, g, b;

    if (!PyArg_ParseTuple(args, "(fff)", &r, &g, &b)){
        return NULL;
    }

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Py_INCREF(Py_None);
    return Py_None;
}