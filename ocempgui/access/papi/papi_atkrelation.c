/* $Id: papi_atkrelation.c,v 1.7.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static PyObject* _atkrelation_new (PyTypeObject *type, PyObject *args,
                                   PyObject *kwds);
static int _atkrelation_init (PyAtkRelation *self, PyObject *args,
                              PyObject *kwds);
static void _atkrelation_dealloc (PyAtkRelation *self);
static PyObject* _atkrelation_get_relation_type (PyAtkRelation *self,
                                                 void *closure);
static PyObject* _atkrelation_get_target (PyAtkRelation *self);
static PyObject* _atkrelation_add_target (PyAtkRelation *self, PyObject *args);

/**
 * Methods, which are bound to the PyAtkRelation type.
 */
static PyMethodDef _atkrelation_methods[] =
{
    { "get_target", (PyCFunction) _atkrelation_get_target, METH_NOARGS,
      "O.get_target () -> list\n\n"
      "Gets the target list of the relation." },
    { "add_target", (PyCFunction) _atkrelation_add_target, METH_VARARGS,
      "O.add_target (obj) -> None\n\n"
      "Adds an AtkObject to the target for the relation.\n\n"
      "Adds the specified AtkObject to the target for the relation, if it\n"
      "is not already present." },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the AtkRelation.
 */
static PyGetSetDef _atkrelation_getsets[] = 
{
    { "relation_type", (getter) _atkrelation_get_relation_type, NULL,
      "The type of the relation.", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkRelation wrapper type, that encapsulates the AtkRelation.
 */
PyTypeObject PyAtkRelation_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkRelation",        /* tp_name */
    sizeof (PyAtkRelation),     /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkrelation_dealloc, /* tp_dealloc */
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
    "ATK accessibility relation", /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _atkrelation_methods,       /* tp_methods */
    0,                          /* tp_members */
    _atkrelation_getsets,       /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atkrelation_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkrelation_new,           /* tp_new */
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
 * Creates a new PyAtkRelation.
 */
static PyObject*
_atkrelation_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkRelation *self;

    debug ("_atkrelation_new\n");

    self = (PyAtkRelation *) type->tp_alloc (type, 0);
    return (PyObject *) self;
}

/**
 * Initializes the PyAtkRelation members, such as the AtkRelation.
 */
static int
_atkrelation_init (PyAtkRelation *self, PyObject *args, PyObject *kwds)
{
    PyObject *list = NULL;
    gboolean istuple = FALSE;
    gboolean islist = FALSE;
    AtkObject **targets = NULL;
    int i;
    int amount;
    AtkRelationType type;
    PyObject *val;
    static char *kwlist[] = { "targets", "type", NULL };
    
    debug ("_atkrelation_init\n");

    if (!PyArg_ParseTupleAndKeywords (args, kwds, "|Oi", kwlist, &list, &type))
        return -1;

    if (list)
    {
        istuple = PyTuple_Check (list);
        if (!istuple)
            islist =  PyList_Check (list);
    }

    if (!list || (!istuple && !islist))
    {
        PyErr_SetString (PyExc_TypeError, "argument 1 must be a sequence");
        return -1;
    }

    amount = (islist) ? PyList_Size (list) : PyTuple_Size (list);
    targets = PyMem_New (AtkObject*, amount);
    if (!targets)
    {
        PyErr_SetNone (PyExc_MemoryError);
        return -1;
    }

    for (i = 0; i < amount; i++)
    {
        val =  (islist) ? PyList_GetItem (list, i) : PyTuple_GetItem (list, i);
        if (!PyObject_TypeCheck (val, &PyAtkObject_Type))
        {
            PyErr_SetString (PyExc_TypeError, "an AtkObject is required");
            PyMem_Free (targets);
            return -1;
        }
        targets[i] = ATKOBJECT (val);
    }

    if (!self->obj)
        self->obj = atk_relation_new (targets, amount, type);
    PyMem_Free (targets);
    return 0;
}

/**
 * Deallocates the PyAtkRelation and its members.
 */
static void
_atkrelation_dealloc (PyAtkRelation *self)
{
    debug ("_atkrelation_dealloc\n");

    if (self->obj)
        g_object_unref (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Wrapper for atk_relation_get_relation_type().
 */
static PyObject*
_atkrelation_get_relation_type (PyAtkRelation *self, void *closure)
{
    AtkRelationType type;
    
    debug ("_atkrelation_get_relation_type\n");

    type = atk_relation_get_relation_type (ATKRELATION (self));
    return PyInt_FromLong ((long) type);
}

/**
 * Wrapper for atk_relation_get_target().
 */
static PyObject*
_atkrelation_get_target (PyAtkRelation *self)
{
    PyObject *list = NULL;
    PyAtkObject *obj = NULL;
    int i;
    GPtrArray *array = NULL;

    debug ("_atkrelation_get_target\n");

    array = atk_relation_get_target (ATKRELATION (self));

    list = PyList_New ((int) array->len);
    if (!list)
        return NULL;

    for (i = 0; i < (int) array->len; i++)
    {
        obj = (PyAtkObject *)
            PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
        obj->obj = g_object_ref (ATK_OBJECT (array->pdata[i]));
        PyList_SetItem (list, i, (PyObject *) obj);
    };

    return list;
}

/**
 * Wrapper for atk_relation_add_target().
 */
static PyObject*
_atkrelation_add_target (PyAtkRelation *self, PyObject *args)
{
    PyObject *val;

    debug ("_atkrelation_add_target\n");

    if (!PyArg_ParseTuple (args, "O:add_target", &val))
        return NULL;
    if (!PyObject_TypeCheck (val, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkObject");
        return NULL;
    }
    
    atk_relation_add_target (ATKRELATION (self), ATKOBJECT (val));
    Py_RETURN_NONE;
}
