#include <Python.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb.h>
#include <stdbool.h>

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
int textureIndex = 0;

GLuint textureShader;

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

    window = glfwCreateWindow(800, 800, title, NULL, NULL);
    if (window == NULL)
	{
		glfwTerminate();
	}
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 800, 800);

    if (unlockFPS){
        glfwSwapInterval(0);
    }

    //Create a default shader that will be used for drawing textures
    const char* vertexShaderSource = GLSL(
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec3 aColor;
                layout (location = 2) in vec2 aTex;

                out vec3 color;
                out vec2 texCoord;

                uniform float scale;

                void main()
                {
                    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
                    color = aColor;
                    texCoord = aTex;
                }
            );
    const char* fragmentShaderSource = GLSL(
            out vec4 FragColor;
            in vec3 color;
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

    textureShader = shaderProgram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //End Defualt shader


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

static PyObject* get_fps(PyObject* self){
    return PyLong_FromLong(FPS);
}

static PyObject* set_title(PyObject* self, PyObject* args){
    const char * title;

    if (!PyArg_ParseTuple(args, "s", &title)){
        return NULL;
    }

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

static PyObject* clear_screen(PyObject* self, PyObject* args){
    float r, g, b;

    if (!PyArg_ParseTuple(args, "(fff)", &r, &g, &b)){
        return NULL;
    }

    //TODO: Add color argument
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject* create_rect(PyObject* self, PyObject* args){
    float r;
    float g;
    float b;


    if (!PyArg_ParseTuple(args, "fff", &r, &g, &b)){
        return NULL;
    }

    const char* vertexShaderSource = GLSL(
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 outColor;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                outColor = aColor;
            }
        );
    const char* fragmentShaderSource = GLSL(
            out vec4 FragColor;
            in vec3 outColor;
            
            void main() 
            {
                FragColor = vec4(outColor, 1.0);
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

    return PyLong_FromLong(shader_index-1);
}

static PyObject* render_rect(PyObject* self, PyObject* args){
    float x, y, size;
    int index;
    int r;
    int g;
    int b;

    if (!PyArg_ParseTuple(args, "i(ff)(iii)f", &index, &x, &y, &r,&g,&b, &size)){
        return NULL;
    }

    float colorR = r/255;
    float colorG = g/255;
    float colorB = b/255;

    //RENDER TRIANGLE
    GLfloat verticies[] =
    {
        x,      y+size, 0.0f,  colorR, colorG, colorB,
        x,      y,      0.0f,  colorR, colorG, colorB,
        x+size, y,      0.0f,  colorR, colorG, colorB,
        x+size, y+size, 0.0f,  colorR, colorG, colorB,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(shaders[index]);
    glBindVertexArray(VAO);  //Render Triangle
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);
    
    Py_INCREF(Py_None);
    return Py_None; 
}

static PyMethodDef myMethods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Inits"},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns version"},
    {"not_window_close", (PyCFunction)should_window_close, METH_NOARGS, "Close"},
    {"clear", (PyCFunction)clear_screen, METH_VARARGS, "Clears Screen"},
    {"render_rect", (PyCFunction)render_rect, METH_VARARGS, "Loads a new rect into memory"},
    {"create_rect", (PyCFunction)create_rect, METH_VARARGS, "Loads a new rect into memory"},
    {"get_fps", (PyCFunction)get_fps, METH_NOARGS, "Loads a new rect into memory"},
    {"update", (PyCFunction)render, METH_NOARGS, "Render"},
    {"set_title", (PyCFunction)set_title, METH_VARARGS, "Loads a new rect into memory"},


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