#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

static PyObject* sendFloat(PyObject* self, PyObject* args){
    int index;
    const char *name;
    float value;

    if (!PyArg_ParseTuple(args, "isf", &index, &name, &value)) {
        return NULL;
    }

    GLuint loc = glGetUniformLocation(shaders[index], (const GLchar*)name);
    glUseProgram(shaders[index]);
    glUniform1f(loc, value);

    Py_INCREF(Py_None);                       
    return Py_None; 
}

static PyObject* sendTexture(PyObject* self, PyObject* args){
    int index;
    const char *name;
    int texture_index;

    if (!PyArg_ParseTuple(args, "isi", &index, &name, &texture_index)) {
        return NULL;
    }
    
    GLuint loc = glGetUniformLocation(shaders[index], (const GLchar*)name);
    glUseProgram(shaders[index]);
    glUniform1i(loc, textures[texture_index - 1]);

    Py_INCREF(Py_None);                       
    return Py_None; 
}

static PyObject* loadShader(PyObject* self, PyObject* args){
    const char *vertexShaderSource;
    const char *fragmentShaderSource;

    if (!PyArg_ParseTuple(args, "ss", &vertexShaderSource, &fragmentShaderSource)) {
        return NULL;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    shaders[shader_index] = shaderProgram;
    shader_index += 1;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return PyLong_FromLong(shader_index-1);
}