/* $Id: papi_atkattribute.c,v 1.6.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static PyObject* _atkattribute_new (PyTypeObject *type, PyObject *args,
                                    PyObject *kwds);
static int _atkattribute_init (PyAtkAttribute *self, PyObject *args,
                               PyObject *kwds);
static void _atkattribute_dealloc (PyAtkAttribute *self);
static PyObject* _atkattribute_get_name (PyAtkAttribute *self, void *closure);
static int _atkattribute_set_name (PyAtkAttribute *self, PyObject *value,
                                   void *closure);
static PyObject* _atkattribute_get_value (PyAtkAttribute *self, void *closure);
static int _atkattribute_set_value (PyAtkAttribute *self, PyObject *value,
                                    void *closure);
/**
 * Getters and setters for the PyAtkAttribute.
 */
static PyGetSetDef _atkattribute_getsets[] = {
    { "name", (getter) _atkattribute_get_name,
      (setter) _atkattribute_set_name, "The attribute name.", NULL },
    { "value",(getter) _atkattribute_get_value,
      (setter) _atkattribute_set_value,
      "The value of the attribute, represented as a string. For values\n"
      "which are numbers, the string representation of the number is in\n"
      "value.", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkAttribute wrapper type, that encapsulates the AtkAttribute.
 */
PyTypeObject PyAtkAttribute_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkAttribute",       /* tp_name */
    sizeof (PyAtkAttribute),    /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkattribute_dealloc, /* tp_dealloc */
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
    "AtkAttribute (name=\"\", value=\"\") -> AtkAttribute\n\n"
    "ATK text attribute name-value pair.\n\n",
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    0,                          /* tp_methods */
    0,                          /* tp_members */
    _atkattribute_getsets,      /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atkattribute_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkattribute_new,          /* tp_new */
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
 * Creates a new PyAtkAttribute.
 */
static PyObject*
_atkattribute_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkAttribute *self;

    debug ("_atkattribute_new\n");

    self = (PyAtkAttribute *) type->tp_alloc (type, 0);
    self->obj = PyMem_New (AtkAttribute, 1);
    if (!self->obj)
    {
        PyErr_SetNone (PyExc_MemoryError);
        return NULL;
    }
    self->obj->name = "";
    self->obj->value = "";

    return (PyObject *) self;
}

/**
 * Initializes the PyAtkAttribute members.
 */
static int
_atkattribute_init (PyAtkAttribute *self, PyObject *args, PyObject *kwds)
{
    char *name;
    char *value;
    static char *kwlist[] = { "name", "value", NULL };

    debug ("_atkattribute_init\n");

    /* Optional keywords - name and value. */
    if (!PyArg_ParseTupleAndKeywords (args, kwds, "|ss", kwlist, &name, &value))
        return -1;
    
    /* Set the name. */
    if (name)
        self->obj->name = name;
    
    /* Set the value. */
    if (value)
        self->obj->value = value;
    return 0;
}

/**
 * Deallocates the PyAtkAttribute and its members.
 */
static void
_atkattribute_dealloc (PyAtkAttribute *self)
{
    debug ("_atkattribute_dealloc\n");

    if (self->obj)
        PyMem_Free (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Getter for AtkAttribute.name.
 */
static PyObject*
_atkattribute_get_name (PyAtkAttribute *self, void *closure)
{
    debug ("_atkattribute_get_name\n");
    return PyString_FromString (self->obj->name);
}

/**
 * Setter for AtkAttribute.name.
 */
static int
_atkattribute_set_name (PyAtkAttribute *self, PyObject *value, void *closure)
{
    char *val = NULL;
    debug ("_atkattribute_set_name\n");

    val = PyString_AsString (value);
    if (!val)
        return -1;
    self->obj->name = val;
    return 0;
}

/**
 * Getter for AtkAttribute.value.
 */
static PyObject*
_atkattribute_get_value (PyAtkAttribute *self, void *closure)
{
    debug ("_atkattribute_get_value\n");

    return PyString_FromString (self->obj->name);
}

/**
 * Setter for AtkAttribute.value.
 */
static int
_atkattribute_set_value (PyAtkAttribute *self, PyObject *value, void *closure)
{
    char *val = NULL;
    debug ("_atkattribute_set_value\n");

    val = PyString_AsString (value);
    if (!val)
        return -1;
    self->obj->name = val;
    return 0;
}
