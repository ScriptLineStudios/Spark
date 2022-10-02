#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

static PyObject *collideRect(PyObject *self, PyObject*args) {
    float _rect1_x, _rect1_y, _rect1_color_r, _rect1_color_g, _rect1_color_b, _rect1_w, _rect1_h;
    float _rect2_x, _rect2_y, _rect2_color_r, _rect2_color_g, _rect2_color_b, _rect2_w, _rect2_h;


    if (!PyArg_ParseTuple(args, "((ff)(fff)(ff))((ff)(fff)(ff))", &_rect1_x, &_rect1_y, &_rect1_color_r, &_rect1_color_g, 
                    &_rect1_color_b, &_rect1_w, &_rect1_h, 
                    &_rect2_x, &_rect2_y, &_rect2_color_r, &_rect2_color_g, &_rect2_color_b, &_rect2_w, &_rect2_h)) 
    {
        return NULL;
    }

    float rect1_x = _rect1_x;
    float rect1_y = _rect1_y;
    float rect1_w = _rect1_w;
    float rect1_h = _rect1_h;

    float rect2_x = _rect2_x;
    float rect2_y = _rect2_y;
    float rect2_w = _rect2_w;
    float rect2_h = _rect2_h;

    float __x = windowX / 2;
    float __y = windowY / 2;

    if (rect1_x * __x     < rect2_x * __x     + rect2_w &&
        rect1_x * __x     + rect1_w > rect2_x * __x     &&
        rect1_y * __y     < rect2_y * __y     + rect2_h &&
        rect1_h + rect1_y * __y     > rect2_y * __y)
    {
        Py_INCREF(Py_True);
        return Py_True;
    }    
    else{
        Py_INCREF(Py_False);
        return Py_False;
    }
}

static PyObject *renderRect(PyObject* self, PyObject* args, PyObject* kwargs){
    float x, y;
    float size_x;
    float size_y;
    int index = 1;
    float r;
    float g;
    float b;

    if (!PyArg_ParseTuple(args, "(ff)(fff)(ff)|i", &x, &y, &r,&g,&b, &size_x, &size_y, &index)) {
        return NULL;
    }

    float colorR = r;
    float colorG = g;
    float colorB = b;

    float renderX = x;
    float renderY = y;

    float _y = (size_y/windowY) * 2;
    float _x = (size_x/windowX) * 2;

    float __y = (size_y/windowY);
    float __x = (size_x/windowY);

    GLfloat verticies[] =
    {
        (renderX - _x / 2) + __x,      (renderY + _y -  _y  /   2) - __y,  0.0f,   colorR, colorG, colorB, 0.0f, 0.0f, 
        (renderX - _x / 2) + __x,      (renderY - _y /   2) - __y,         0.0f,   colorR, colorG, colorB, 0.0f, 1.0f,
        (renderX + _x - _x / 2) + __x, (renderY - _y /   2) - __y,         0.0f,   colorR, colorG, colorB, 1.0f, 1.0f,
        (renderX + _x - _x / 2) + __x, (renderY + _y -  _y  /   2) - __y,  0.0f,   colorR, colorG, colorB, 1.0f, 0.0f
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
    glBindTexture(GL_TEXTURE_2D, textures[index - 2]);

    glBindVertexArray(VAO);  //Render Triangle
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

    Py_INCREF(Py_None);                       
    return Py_None; 
}