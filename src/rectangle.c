#include <Python.h>
#include <stdio.h>
#include <stb/stb.h>
#include <stdbool.h>

#include "../Include/glfw/include/GLFW/glfw3.h"
#include "../Include/stb/stb_image.h"
#include "../Include/glfw/deps/glad/gles2.h"

typedef struct {
    double x, y, w, h;
} Rectangle;


typedef struct {
    PyObject_HEAD Rectangle rect;
    PyObject *weakreflist;
} rectangleObject;


static int __init__(rectangleObject *self, PyObject *args, PyObject *kwds) {
    if (!PyTuple_Check(args)) {
        return -1;
    }
    PyObject **farray = PySequence_Fast_ITEMS(args);
    self->rect.x = PyFloat_AsDouble(farray[0]);
    self->rect.y = PyFloat_AsDouble(farray[1]); 
    self->rect.w = PyFloat_AsDouble(farray[2]); 
    self->rect.h = PyFloat_AsDouble(farray[3]); 

    return 0;
}

static PyObject *
__new__(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    rectangleObject *self = (rectangleObject *)type->tp_alloc(type, 0);

    if (self != NULL) {
        self->rect.x = self->rect.y = 0;
        self->rect.w = self->rect.h = 0;
        self->weakreflist = NULL;
    }
    return (PyObject *)self;
}

static int
set_x(rectangleObject *self, PyObject *args)  {
    if (PyFloat_Check(args)) {
        self->rect.x = PyFloat_AS_DOUBLE(args);
        return 0;
    }
    else if (PyLong_Check(args)) {
        self->rect.x = PyLong_AsDouble(args);
        return 0;
    }
    return -1;
}

static int
set_y(rectangleObject *self, PyObject *args)  {
    if (PyFloat_Check(args)) {
        self->rect.y = PyFloat_AS_DOUBLE(args);
        return 0;
    }
    else if (PyLong_Check(args)) {
        self->rect.y = PyLong_AsDouble(args);
        return 0;
    }
    return -1;
}

static int
set_w(rectangleObject *self, PyObject *args)  {
    if (PyFloat_Check(args)) {
        self->rect.w = PyFloat_AS_DOUBLE(args);
        return 0;
    }
    else if (PyLong_Check(args)) {
        self->rect.w = PyLong_AsDouble(args);
        return 0;
    }
    return -1;
}

static int
set_h(rectangleObject *self, PyObject *args)  {
    if (PyFloat_Check(args)) {
        self->rect.h = PyFloat_AS_DOUBLE(args);
        return 0;
    }
    else if (PyLong_Check(args)) {
        self->rect.h = PyLong_AsDouble(args);
        return 0;
    }
    return -1;
}

static PyObject *
get_x(rectangleObject *self)  {
    return PyFloat_FromDouble(self->rect.x);
}

static PyObject *
get_y(rectangleObject *self)  {
    return PyFloat_FromDouble(self->rect.y);
}

static PyObject *
get_w(rectangleObject *self)  {
    return PyFloat_FromDouble(self->rect.w);
}

static PyObject *
get_h(rectangleObject *self)  {
    return PyFloat_FromDouble(self->rect.h);
}

static PyGetSetDef getsets[] = {
    {"x", (getter)get_x, (setter)set_x, NULL, NULL},
    {"y", (getter)get_y, (setter)set_y, NULL, NULL},
    {"w", (getter)get_w, (setter)set_w, NULL, NULL},
    {"h", (getter)get_h, (setter)set_h, NULL, NULL},
    {NULL, 0, NULL, NULL, NULL} /* Sentinel */
};


static PyTypeObject rectangleType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "spark.Rectangle",
    .tp_init = (initproc)__init__,
    .tp_new = __new__,
    .tp_getset = getsets,

};
