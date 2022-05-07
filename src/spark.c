#include <Python.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
#define GLSL(src) "#version 150 core\n" #src


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
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* should_window_close(PyObject* self){
    if (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS){
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

static PyObject* clear_screen(PyObject* self){
    //TODO: Add color argument
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Py_INCREF(Py_None);
    
    return Py_None;
}

static PyObject* render_triangle(PyObject* self, PyObject* args){
    float x;
    float y;
    float size;

    if (!PyArg_ParseTuple(args, "fff", &x, &y, &size)){
        return NULL;
    }

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] = {
         x,  y, // Vertex 1 (X, Y)
         x+size, y-size, // Vertex 2 (X, Y)
        x-size, y-size  // Vertex 3 (X, Y)
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    const char* vertexSource = GLSL(
        in vec2 position;
        
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    );

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    const char* fragmentSource = GLSL(
        out vec4 outColor;
        
        void main() {
            outColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
        }
    );

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    Py_INCREF(Py_None);
    return Py_None; 
}

static PyMethodDef myMethods[] = {
    {"init", (PyCFunction)init, METH_VARARGS, "Inits"},
    {"version", (PyCFunction)version, METH_NOARGS, "Returns version"},
    {"not_window_close", (PyCFunction)should_window_close, METH_NOARGS, "Close"},
    {"render_triangle", (PyCFunction)render_triangle, METH_VARARGS, "Render Triangle"},
    {"clear", (PyCFunction)clear_screen, METH_NOARGS, "Clears Screen"},
    {"update", (PyCFunction)render, METH_NOARGS, "Render"},

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