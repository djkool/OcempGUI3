/* $Id: papi_atktextrectangle.c,v 1.4.2.4 2007/09/23 07:37:49 marcusva Exp $
 *
 * Copyright (c) 2006, Marcus von Appen
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "papi.h"
#include "papi_private.h"

/* Forward declarations. */
static PyObject* _atktextrectangle_new (PyTypeObject *type, PyObject *args,
                                        PyObject *kwds);
static int _atktextrectangle_init (PyAtkTextRectangle *self, PyObject *args,
                                   PyObject *kwds);
static void _atktextrectangle_dealloc (PyAtkTextRectangle *self);
static PyObject* _atktextrectangle_get_x (PyAtkTextRectangle *self,
                                          void *closure);
static int _atktextrectangle_set_x (PyAtkTextRectangle *self, PyObject *value,
                                    void *closure);
static PyObject* _atktextrectangle_get_y (PyAtkTextRectangle *self,
                                          void *closure);
static int _atktextrectangle_set_y (PyAtkTextRectangle *self, PyObject *value,
                                    void *closure);
static PyObject* _atktextrectangle_get_width (PyAtkTextRectangle *self,
                                              void *closure);
static int _atktextrectangle_set_width (PyAtkTextRectangle *self,
                                        PyObject *value, void *closure);
static PyObject* _atktextrectangle_get_height (PyAtkTextRectangle *self,
                                               void *closure);
static int _atktextrectangle_set_height (PyAtkTextRectangle *self,
                                         PyObject *value, void *closure);

/**
 * Getters and setters for the PyAtkTextRectangle.
 */
static PyGetSetDef _atktextrectangle_getsets[] = {
    { "x", (getter) _atktextrectangle_get_x, (setter) _atktextrectangle_set_x,
      "The horizontal coordinate of a rectangle.", NULL },
    { "y",(getter) _atktextrectangle_get_y, (setter) _atktextrectangle_set_y,
      "The vertical coordinate of a rectangle.", NULL },
    { "width",(getter) _atktextrectangle_get_width,
      (setter) _atktextrectangle_set_width, "The width of a rectangle.",
      NULL },
    { "height",(getter) _atktextrectangle_get_height,
      (setter) _atktextrectangle_set_height, "The height of a rectangle.",
      NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkTextRectangle wrapper type, that encapsulates the AtkTextRectangle.
 */
PyTypeObject PyAtkTextRectangle_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkTextRectangle",   /* tp_name */
    sizeof (PyAtkTextRectangle), /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atktextrectangle_dealloc, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "A structure used to store a rectangle used by AtkText.", /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    0,                          /* tp_methods */
    0,                          /* tp_members */
    _atktextrectangle_getsets,  /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atktextrectangle_init, /* tp_init */
    0,                          /* tp_alloc */
    _atktextrectangle_new,      /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0                           /* tp_del */
};

/**
 * Creates a new PyAtkTextRectangle.
 */
static PyObject*
_atktextrectangle_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkTextRectangle *self;

    debug ("_atktextrextangle_new\n");

    self = (PyAtkTextRectangle *) type->tp_alloc (type, 0);
    self->obj = g_new (AtkTextRectangle, 1);
    if (!self->obj)
    {
        PyErr_SetNone (PyExc_MemoryError);
        return NULL;
    }
    self->obj->x = 0;
    self->obj->y = 0;
    self->obj->width = 0;
    self->obj->height = 0;

    return (PyObject *) self;
}

/**
 * Initializes the PyAtkTextRectangle members.
 */
static int
_atktextrectangle_init (PyAtkTextRectangle *self, PyObject *args,
                        PyObject *kwds)
{
    static char *kwlist[] = { "x", "y", "width", "height", NULL };

    debug ("_atktextrectangle_init\n");

    /* Optional keywords - name and value. */
    if (!PyArg_ParseTupleAndKeywords (args, kwds, "|iiii", kwlist,
                                      &(self->obj->x), &(self->obj->y),
                                      &(self->obj->width),
                                      &(self->obj->height)))
        return -1;
    return 0;
}

/**
 * Deallocates the PyAtkTextRectangle and its members.
 */
static void
_atktextrectangle_dealloc (PyAtkTextRectangle *self)
{
    debug ("_atktextrectangle_dealloc\n");

    if (self->obj)
        g_free (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Getter for AtkTextRectangle.x.
 */
static PyObject*
_atktextrectangle_get_x (PyAtkTextRectangle *self, void *closure)
{
    return PyInt_FromLong (self->obj->x);
}

/**
 * Setter for AtkTextRectangle.x.
 */
static int
_atktextrectangle_set_x (PyAtkTextRectangle *self, PyObject *value,
                         void *closure)
{
    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->x = PyInt_AsLong (value);
    return 0;
}

/**
 * Getter for AtkTextRectangle.y.
 */
static PyObject*
_atktextrectangle_get_y (PyAtkTextRectangle *self, void *closure)
{
    return PyInt_FromLong (self->obj->y);
}

/**
 * Setter for AtkTextRectangle.y.
 */
static int
_atktextrectangle_set_y (PyAtkTextRectangle *self, PyObject *value,
                         void *closure)
{
    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->y = PyInt_AsLong (value);
    return 0;
}

/**
 * Getter for AtkTextRectangle.width.
 */
static PyObject*
_atktextrectangle_get_width (PyAtkTextRectangle *self, void *closure)
{
    return PyInt_FromLong (self->obj->width);
}

/**
 * Setter for AtkTextRectangle.width.
 */
static int
_atktextrectangle_set_width (PyAtkTextRectangle *self, PyObject *value,
                             void *closure)
{
    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->width = PyInt_AsLong (value);
    return 0;
}

/**
 * Getter for AtkTextRectangle.height.
 */
static PyObject*
_atktextrectangle_get_height (PyAtkTextRectangle *self, void *closure)
{
    return PyInt_FromLong (self->obj->height);
}

/**
 * Setter for AtkTextRectangle.height.
 */
static int
_atktextrectangle_set_height (PyAtkTextRectangle *self, PyObject *value,
                              void *closure)
{
    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->height = PyInt_AsLong (value);
    return 0;
}
