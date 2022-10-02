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

GLuint indicies[] =
{
    0, 2, 1,
    0, 3, 2,
};

GLuint VAO, VBO, EBO;

/*
Shader layout
-------------

0 = texture shader (default)

*/

void _loadShader(const char *vertexShaderSource, const char *fragmentShaderSource) {
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
}

void setupDefaultShaders() {
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
    _loadShader(vertexShaderSource, fragmentShaderSource);

    const char* _vertexShaderSource = GLSL(
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
    const char* _fragmentShaderSource = GLSL(
        out vec4 FragColor;
        in vec3 outColor;

        in vec2 texCoord;

        uniform sampler2D tex0;
        
        void main() 
        {
            FragColor = vec4(outColor, 1.0);
        }
    );

    _loadShader(_vertexShaderSource, _fragmentShaderSource);
}

GLuint textures[256];
int texture_index = 0;

int windowX;
int windowY;

GLuint using_shader = 0;
GLuint using_texture = 0;


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
    glfwSetMouseButtonCallback(window, mouseCallback);

    gladLoadGL();
    gluOrtho2D(0, x, y, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // glViewport(0, 0, x, y);
    // float aspect = (float)x / (float)y;
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, x, y, 0, 0, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    windowX = x;
    windowY = y;

    if (unlockFPS){
        glfwSwapInterval(0);
    }    
    setupDefaultShaders();

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