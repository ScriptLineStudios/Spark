#include <Python.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

static PyObject* version(PyObject* self){
    return Py_BuildValue("s", "Version 1.0");
}

static PyObject* init(PyObject* self, PyObject* args){
    const char *title;
    int x;
    int y;

    if (!PyArg_ParseTuple(args, "sii", &title, &x, &y)){
        return NULL;
    }

    if (glfwInit() != GL_TRUE) {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(x, y, title, NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* should_window_close(PyObject* self){
    if (!glfwWindowShouldClose(window)){
        Py_RETURN_TRUE;
    }else{
        Py_RETURN_FALSE;
    }
}

static PyObject* render(PyObject* self){
    glfwSwapBuffers(window);
    glfwPollEvents();
    Py_INCREF(Py_None);
    
    return Py_None;
}

static PyMethodDef myMethods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Inits"},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns version"},
    {"not_window_close", (PyCFunction)should_window_close, METH_NOARGS, "Close"},
    {"render", (PyCFunction)render, METH_NOARGS, "Render"},

    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef spark = {
    PyModuleDef_HEAD_INIT,
    "Spark",
    "Simple game library",
    -1,
    myMethods
};

PyMODINIT_FUNC PyInit_spark(void){
    return PyModule_Create(&spark);
}