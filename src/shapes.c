#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

static PyObject* renderRect(PyObject* self, PyObject* args, PyObject* kwargs){
    float x, y;
    float size;
    int index = 1;
    float r;
    float g;
    float b;

    if (!PyArg_ParseTuple(args, "(ff)(fff)f|i", &x, &y, &r,&g,&b, &size, &index)) {
        return NULL;
    }

    float colorR = r;
    float colorG = g;
    float colorB = b;

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
    glBindTexture(GL_TEXTURE_2D, textures[index - 2]);

    glBindVertexArray(VAO);  //Render Triangle
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

    Py_INCREF(Py_None);                       
    return Py_None; 
}