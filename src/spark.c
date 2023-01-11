#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

#include "key.c"
#include "base.c"
#include "shapes.c"
#include "shaders.c"
#include "rectangle.c"

static PyObject *mouseGetPos(PyObject *self) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    PyObject * res = PyTuple_New(2);
    PyTuple_SetItem(res, 0, PyFloat_FromDouble(((xpos - windowX / 2) / windowX)*2));
    PyTuple_SetItem(res, 1, PyFloat_FromDouble((-((ypos - windowY / 2) / windowY)*2)));
    return res;
}

static PyObject* createTexture(PyObject* self, PyObject* args){
    const char * textureLocation = NULL;

    if (!PyArg_ParseTuple(args, "s", &textureLocation)){
        return NULL;
    }
    texture_index += 1;

    int imgWidth, imgHeight, colorChannels;
    unsigned char* bytes = stbi_load(textureLocation, &imgWidth, &imgHeight, &colorChannels, 0);

    if (!bytes) {
        PyErr_SetString(PyExc_FileNotFoundError, "The texture was not found");
        return NULL;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
    

    GLuint tex0Uni = glGetUniformLocation(shaders[shader_index-1], "tex0");
    glUseProgram(shaders[0]);

    
    
    glUniform1i(tex0Uni, 0);

    textures[texture_index - 2] = texture;

    return PyLong_FromLong(texture_index - 2);

}

#define VERTEX_ONE(x, y) (x - _x / 2) + __x, \
            (y + _y -  _y / 2) + __y,  0.0f,   0, 0, 0, 0.0f, 0.0f
#define VERTEX_TWO(x, y) (x - _x / 2) + __x, \
            (y - _y / 2) + __y,  0.0f,   0, 0, 0, 0.0f, 1.0f
#define VERTEX_THREE(x, y) (x + _x - _x / 2) + __x, \
            (y - _y / 2) + __y,  0.0f,   0, 0, 0, 1.0f, 1.0f
#define VERTEX_FOUR(x, y) (x + _x - _x / 2) + __x, \
            (y + _y - _y / 2) + __y,  0.0f,   0, 0, 0, 1.0f, 0.0f,

#define SQUARE(x, y) VERTEX_ONE(x, y), VERTEX_TWO(x, y), VERTEX_THREE(x, y), VERTEX_FOUR(x, y)

static PyObject* renderTexture(PyObject* self, PyObject* args){
    float x, y;
    float size_x;
    float size_y;
    int shader_index = 0;

    int index;

    if (!PyArg_ParseTuple(args, "i(ff)(ff)|i", &index, &x, &y, &size_x, &size_y, &shader_index))
        return NULL;

    float renderX = x / (windowX / 2);
    float renderY = y / (windowY / 2);

    float _y = (size_y/windowY) * 2;
    float _x = (size_x/windowX) * 2;

    float __y = (size_y/windowY);
    float __x = (size_x/windowX);

    GLfloat verticies[] = 
    {
        SQUARE(renderX, renderY)
    };

    GLuint indicies[] =
    {
        0, 2, 1,
        0, 3, 2,

        4, 6, 5,
        4, 7, 6,
    };

    GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glUseProgram(shaders[shader_index]);

    if (using_texture != textures[index])
        glBindTexture(GL_TEXTURE_2D, textures[index]);
    using_texture = textures[index];

    glBindVertexArray(VAO);  //Render Triangle
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    Py_INCREF(Py_None);                       
    return Py_None; 
}

static PyMethodDef base_methods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Initializes the display with a specified width, height and title."},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns current Spark version"},
    {"not_window_close", (PyCFunction)windowClose, METH_NOARGS, "Signals weather or not the window should close. eg when the exit button is pressed. Typically used for creating main loops."},
    {"clear", (PyCFunction)clearScreen, METH_VARARGS, "Clears the display"},
    {"render_image", (PyCFunction)renderTexture, METH_VARARGS, "Renders an image at a specified location and with a specified size"},
    {"create_image", (PyCFunction)createTexture, METH_VARARGS, "Loads an image into memory"},
    {"get_fps", (PyCFunction)getFps, METH_NOARGS, "Returns a float, representing the fps as per the last rendered frame"},
    {"update", (PyCFunction)render, METH_NOARGS, "Renders all objects to the display"},
    {"set_title", (PyCFunction)setTitle, METH_VARARGS, "Sets the window title"},
    {"load_shader", (PyCFunction)loadShader, METH_VARARGS, "Loads a new shader into memory"},
    {"send_float", (PyCFunction)sendFloat, METH_VARARGS, "Sends a float value to the shader at a specified uniform"},
    {"send_image", (PyCFunction)sendTexture, METH_VARARGS, "Sends an image to the shader at a specified uniform"},
    {"mouse_pos", (PyCFunction)mouseGetPos, METH_NOARGS, "Returns the x, y of the current mouse position"},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef shape_methods[] = {
    {"render_rect", (PyCFunction)renderRect, METH_VARARGS, "Renders a rectangle with a specified position, color and size"},
    {"collide_rect", (PyCFunction)collideRect, METH_VARARGS, "Returns a boolean specifying weather or not two rectangles are colliding with each other or not"},
};

static PyMethodDef key_methods[] = {
    {"key_is_pressed", (PyCFunction)keyIsPressed, METH_VARARGS, "Checks key pressed"},
    {"mouse_is_pressed", (PyCFunction)mouseIsClicked, METH_VARARGS, "Checks if mouse pressed"},
};

static struct PyModuleDef spark = {
    PyModuleDef_HEAD_INIT,
    "Spark",
    "Base",
    -1,
    base_methods
};

static struct PyModuleDef key = {
    PyModuleDef_HEAD_INIT,
    "key",
    "key handling",
    -1,
    key_methods
};

static struct PyModuleDef shapes = {
    PyModuleDef_HEAD_INIT,
    "shapes",
    "handle shape rendering",
    -1,
    shape_methods
};

PyMODINIT_FUNC PyInit_shapes(void) {
    PyObject *mod = PyModule_Create(&shapes);
    return mod;
}

PyMODINIT_FUNC PyInit_key(void) {
    PyObject *mod = PyModule_Create(&key);

    PyModule_AddIntMacro(mod, A);
    PyModule_AddIntMacro(mod, B);
    PyModule_AddIntMacro(mod, C);
    PyModule_AddIntMacro(mod, D);
    PyModule_AddIntMacro(mod, E);
    PyModule_AddIntMacro(mod, F);
    PyModule_AddIntMacro(mod, G);
    PyModule_AddIntMacro(mod, H);
    PyModule_AddIntMacro(mod, I);
    PyModule_AddIntMacro(mod, J);
    PyModule_AddIntMacro(mod, K);
    PyModule_AddIntMacro(mod, L);
    PyModule_AddIntMacro(mod, M);
    PyModule_AddIntMacro(mod, N);
    PyModule_AddIntMacro(mod, O);
    PyModule_AddIntMacro(mod, P);
    PyModule_AddIntMacro(mod, Q);
    PyModule_AddIntMacro(mod, R);
    PyModule_AddIntMacro(mod, S);
    PyModule_AddIntMacro(mod, T);
    PyModule_AddIntMacro(mod, U);
    PyModule_AddIntMacro(mod, V);
    PyModule_AddIntMacro(mod, W);
    PyModule_AddIntMacro(mod, X);
    PyModule_AddIntMacro(mod, Y);
    PyModule_AddIntMacro(mod, Z);
    PyModule_AddIntMacro(mod, SPACE);

    PyModule_AddIntMacro(mod, MOUSE_LEFT);
    PyModule_AddIntMacro(mod, MOUSE_RIGHT);

    return mod;
}

PyMODINIT_FUNC PyInit_spark(void){
    PyObject *base = PyModule_Create(&spark);
    PyObject *key =  PyInit_key();
    PyObject *shapes =  PyInit_shapes();

    Py_INCREF(key);
    PyModule_AddObject(base, "key", key);

    Py_INCREF(shapes);
    PyModule_AddObject(base, "shapes", shapes);

    if (PyType_Ready(&rectangleType) < 0) {
        return NULL;
    }

    Py_INCREF(&rectangleType);
    if (PyModule_AddObject(base, "Rectangle", (PyObject *)&rectangleType)) {
        Py_DECREF(&rectangleType);
        Py_DECREF(&base);
        return NULL;
    }
    
    return base;
}