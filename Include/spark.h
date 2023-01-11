#ifndef SPARK
#define SPARK
#include <Python.h>

typedef struct {
    double x, y, w, h;
} Rectangle;


typedef struct {
    PyObject_HEAD Rectangle rect;
    PyObject *weakreflist;
} rectangleObject;
#endif