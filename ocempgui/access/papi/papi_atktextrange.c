/* $Id: papi_atktextrange.c,v 1.3.2.5 2007/09/23 07:37:49 marcusva Exp $
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
static PyObject* _atktextrange_new (PyTypeObject *type, PyObject *args,
                                    PyObject *kwds);
static int _atktextrange_init (PyAtkTextRange *self, PyObject *args,
                               PyObject *kwds);
static void _atktextrange_dealloc (PyAtkTextRange *self);
static PyObject* _atktextrange_get_bounds (PyAtkTextRange *self,
                                           void *closure);
static int _atktextrange_set_bounds (PyAtkTextRange *self, PyObject *value,
                                     void *closure);
static PyObject* _atktextrange_get_start_offset (PyAtkTextRange *self,
                                                 void *closure);
static int _atktextrange_set_start_offset (PyAtkTextRange *self,
                                           PyObject *value, void *closure);
static PyObject* _atktextrange_get_end_offset (PyAtkTextRange *self,
                                               void *closure);
static int _atktextrange_set_end_offset (PyAtkTextRange *self, PyObject *value,
                                         void *closure);
static PyObject* _atktextrange_get_content (PyAtkTextRange *self,
                                            void *closure);
static int _atktextrange_set_content (PyAtkTextRange *self, PyObject *value,
                                      void *closure);

/**
 * Getters and setters for the PyAtkTextRange.
 */
static PyGetSetDef _atktextrange_getsets[] = {
    { "bounds", (getter) _atktextrange_get_bounds,
      (setter) _atktextrange_set_bounds,
      "A rectangle giving the bounds of the text range.", NULL },
    { "start_offset",(getter) _atktextrange_get_start_offset,
      (setter) _atktextrange_set_start_offset,
      "The start offset of a AtkTextRange.", NULL },
    { "end_offset",(getter) _atktextrange_get_end_offset,
      (setter) _atktextrange_set_end_offset,
      "The end offset of a AtkTextRange.", NULL },
    { "content",(getter) _atktextrange_get_content,
      (setter) _atktextrange_set_content, "The text in the text range.",
      NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkTextRange wrapper type, that encapsulates the AtkTextRange.
 */
PyTypeObject PyAtkTextRange_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkTextRange",       /* tp_name */
    sizeof (PyAtkTextRange),    /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atktextrange_dealloc, /* tp_dealloc */
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
    "A structure used to describe a text range.", /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    0,                          /* tp_methods */
    0,                          /* tp_members */
    _atktextrange_getsets,      /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atktextrange_init, /* tp_init */
    0,                          /* tp_alloc */
    _atktextrange_new,          /* tp_new */
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
 * Creates a new PyAtkTextRange.
 */
static PyObject*
_atktextrange_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkTextRange *self;

    debug ("_atktextrange_new\n");

    self = (PyAtkTextRange *) type->tp_alloc (type, 0);
    self->obj = g_new (AtkTextRange, 1);
    if (!self->obj)
    {
        PyErr_SetNone (PyExc_MemoryError);
        return NULL;
    }

    return (PyObject *) self;
}

/**
 * Initializes the PyAtkTextRange members.
 */
static int
_atktextrange_init (PyAtkTextRange *self, PyObject *args, PyObject *kwds)
{
    PyAtkTextRectangle *bounds = NULL;
    static char *kwlist[] = { "bounds", "start_offset", "end_offset",
                              "content", NULL };

    debug ("_atktextrange_init\n");

    /* Optional keywords - name and value. */
    if (!PyArg_ParseTupleAndKeywords (args, kwds, "|Oiis", kwlist, &bounds,
                                      &(self->obj->start_offset),
                                      &(self->obj->end_offset),
                                      &(self->obj->content)))
        return -1;
    
    /* Set the bounds. */
    if (bounds)
    {
        if (!PyObject_TypeCheck (bounds, &PyAtkTextRectangle_Type))
        {
            PyErr_SetString (PyExc_TypeError,
                             "bounds must be a AtkTextRectangle");
            return -1;
        }
        self->obj->bounds.x = bounds->obj->x;
        self->obj->bounds.y = bounds->obj->y;
        self->obj->bounds.width = bounds->obj->width;
        self->obj->bounds.height = bounds->obj->height;
    }
    return 0;
}

/**
 * Deallocates the PyAtkTextRange and its members.
 */
static void
_atktextrange_dealloc (PyAtkTextRange *self)
{
    debug ("_atktextrange_dealloc\n");

    if (self->obj)
        g_free (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Getter for AtkTextRange.bounds.
 */
static PyObject*
_atktextrange_get_bounds (PyAtkTextRange *self, void *closure)
{
    PyAtkTextRectangle *rect;

    debug ("_atktextrange_get_bounds\n");

    rect = (PyAtkTextRectangle *)
        PyAtkTextRectangle_Type.tp_new (&PyAtkTextRectangle_Type, NULL, NULL);
    rect->obj->x = self->obj->bounds.x;
    rect->obj->y = self->obj->bounds.y;
    rect->obj->width = self->obj->bounds.width;
    rect->obj->height = self->obj->bounds.height;

    return (PyObject *) rect;
}

/**
 * Setter for AtkTextRange.bounds.
 */
static int
_atktextrange_set_bounds (PyAtkTextRange *self, PyObject *value, void *closure)
{
    PyAtkTextRectangle *rect;

    debug ("_atktextrange_set_bounds\n");

    if (!PyObject_TypeCheck (value, &PyAtkTextRectangle_Type))
    {
        PyErr_SetString (PyExc_TypeError, "value must be a AtkTextRectangle");
        return -1;
    }
    rect = (PyAtkTextRectangle *) value;
    self->obj->bounds.x = rect->obj->x;
    self->obj->bounds.y = rect->obj->y;
    self->obj->bounds.width = rect->obj->width;
    self->obj->bounds.height = rect->obj->height;
    return 0;
}

/**
 * Getter for AtkTextRange.start_offset.
 */
static PyObject*
_atktextrange_get_start_offset (PyAtkTextRange *self, void *closure)
{
    debug ("_atktextrange_get_start_offset\n");

    return PyInt_FromLong (self->obj->start_offset);
}

/**
 * Setter for AtkTextRange.start_offset.
 */
static int
_atktextrange_set_start_offset (PyAtkTextRange *self, PyObject *value,
                                void *closure)
{
    debug ("_atktextrange_set_start_offset\n");

    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->start_offset = PyInt_AsLong (value);
    return 0;
}

/**
 * Getter for AtkTextRange.end_offset.
 */
static PyObject*
_atktextrange_get_end_offset (PyAtkTextRange *self, void *closure)
{
    debug ("_atktextrange_get_end_offset\n");

    return PyInt_FromLong (self->obj->end_offset);
}

/**
 * Setter for AtkTextRange.end_offset.
 */
static int
_atktextrange_set_end_offset (PyAtkTextRange *self, PyObject *value,
                              void *closure)
{
    debug ("_atktextrange_set_end_offset\n");

    if (!PyInt_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be an integer");
        return -1;
    }
    self->obj->end_offset = PyInt_AsLong (value);
    return 0;
}

/**
 * Getter for AtkTextRange.content.
 */
static PyObject*
_atktextrange_get_content (PyAtkTextRange *self, void *closure)
{
    debug ("_atktextrange_get_content\n");

    return PyString_FromString (self->obj->content);
}

/**
 * Setter for AtkTextRange.content.
 */
static int
_atktextrange_set_content (PyAtkTextRange *self, PyObject *value,
                           void *closure)
{
    debug ("_atktextrange_set_content\n");

    if (!PyString_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "value must be a string");
        return -1;
    }
    self->obj->content = PyString_AsString (value);
    return 0;
}
