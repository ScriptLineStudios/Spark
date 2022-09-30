
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


static PyObject* createRect(PyObject* self, PyObject* args){
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

static PyObject* renderRect(PyObject* self, PyObject* args){
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

static PyMethodDef myMethods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Inits"},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns version"},
    {"not_window_close", (PyCFunction)windowClose, METH_NOARGS, "Close"},
    {"clear", (PyCFunction)clearScreen, METH_VARARGS, "Clears Screen"},
    {"render_rect", (PyCFunction)renderRect, METH_VARARGS, "Loads a new rect into memory"},
    {"create_rect", (PyCFunction)createRect, METH_VARARGS, "Loads a new rect into memory"},
    {"get_fps", (PyCFunction)getFps, METH_NOARGS, "Loads a new rect into memory"},
    {"update", (PyCFunction)render, METH_NOARGS, "Render"},
    {"set_title", (PyCFunction)setTitle, METH_VARARGS, "Loads a new rect into memory"},
    {"key_is_pressed", (PyCFunction)keyIsPressed, METH_VARARGS, "Checks key pressed"},
    //{"load_texture", (PyCFunction)load_texture, METH_VARARGS, "Checks key pressed"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef spark = {
    PyModuleDef_HEAD_INIT,
    "Spark",
    "Base",
    -1,
    myMethods
};

PyMODINIT_FUNC PyInit_spark(void){
    return PyModule_Create(&spark);
}