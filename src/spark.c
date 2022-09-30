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

static PyObject* createTexture(PyObject* self, PyObject* args){
    const char * textureLocation = NULL;

    if (!PyArg_ParseTuple(args, "s", &textureLocation)){
        return NULL;
    }

    const char* vertexShaderSource = GLSL(
        layout (location = 0) in vec3 aPos; 
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTex;

        out vec3 outColor;

        out vec2 texCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            outColor = aColor;
            texCoord = aTex;
        }
    );
    const char* fragmentShaderSource = GLSL(
        out vec4 FragColor;
        in vec3 outColor;

        in vec2 texCoord;

        uniform sampler2D tex0;
        
        void main() 
        {
            FragColor = texture(tex0, texCoord);
        }
    );

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
    glUseProgram(shaders[shader_index-1]);
    glUniform1i(tex0Uni, 0);

    textures[shader_index-1] = texture;

    return PyLong_FromLong(shader_index-1);
}

static PyObject* sendShader(PyObject* self, PyObject* args){
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



static PyObject* renderTexture(PyObject* self, PyObject* args){
    float x, y;
    float size;
    int index;
    int r;
    int g;
    int b;

    if (!PyArg_ParseTuple(args, "i(ff)(iii)f", &index, &x, &y, &r,&g,&b, &size))
        return NULL;

    float colorR = r/255;
    float colorG = g/255;
    float colorB = b/255;

    float renderX = x;
    float renderY = y;

    GLfloat verticies[] =
    {
        renderX,      renderY+(size/windowY),           0.0f,   colorR, colorG, colorB, 0.0f, 0.0f, 
        renderX,      renderY,                          0.0f,   colorR, colorG, colorB, 0.0f, 1.0f,
        renderX+(size/windowX), renderY,                0.0f,   colorR, colorG, colorB, 1.0f, 1.0f,
        renderX+(size/windowX), renderY+(size/windowY), 0.0f,   colorR, colorG, colorB, 1.0f, 0.0f
    };

    GLuint indicies[] =
    {
        0, 2, 1,
        0, 3, 2,
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

    glUseProgram(shaders[index]);
    glBindTexture(GL_TEXTURE_2D, textures[index]);

    glBindVertexArray(VAO);  //Render Triangle
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

    Py_INCREF(Py_None);                       
    return Py_None; 
}

static PyMethodDef base_methods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Inits"},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns version"},
    {"not_window_close", (PyCFunction)windowClose, METH_NOARGS, "Close"},
    {"clear", (PyCFunction)clearScreen, METH_VARARGS, "Clears Screen"},
    {"render_image", (PyCFunction)renderTexture, METH_VARARGS, "Loads a new rect into memory"},
    {"create_image", (PyCFunction)createTexture, METH_VARARGS, "Loads a new rect into memory"},
    {"get_fps", (PyCFunction)getFps, METH_NOARGS, "Loads a new rect into memory"},
    {"update", (PyCFunction)render, METH_NOARGS, "Render"},
    {"set_title", (PyCFunction)setTitle, METH_VARARGS, "Loads a new rect into memory"},
    {"load_shader", (PyCFunction)loadShader, METH_VARARGS, "Loads a new rect into memory"},
    {"send_shader", (PyCFunction)sendShader, METH_VARARGS, "Loads a new rect into memory"},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef shape_methods[] = {
    {"render_rect", (PyCFunction)renderRect, METH_VARARGS, "Loads a new rect into memory"},
};

static PyMethodDef key_methods[] = {
    {"key_is_pressed", (PyCFunction)keyIsPressed, METH_VARARGS, "Checks key pressed"},
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
    
    return base;
}