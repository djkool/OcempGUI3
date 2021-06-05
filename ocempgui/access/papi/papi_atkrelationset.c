/* $Id: papi_atkrelationset.c,v 1.5.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static PyObject* _atkrelationset_new (PyTypeObject *type, PyObject *args,
                                      PyObject *kwds);
static int _atkrelationset_init (PyAtkRelationSet *self, PyObject *args,
                                 PyObject *kwds);
static void _atkrelationset_dealloc (PyAtkRelationSet *self);
static PyObject* _atkrelationset_contains (PyAtkRelationSet *self,
                                           PyObject *args);
static PyObject* _atkrelationset_remove (PyAtkRelationSet *self,
                                         PyObject *args);
static PyObject* _atkrelationset_add (PyAtkRelationSet *self, PyObject *args);
static PyObject* _atkrelationset_get_n_relations (PyAtkRelationSet *self);
static PyObject* _atkrelationset_get_relation (PyAtkRelationSet *self,
                                               PyObject *args);
static PyObject* _atkrelationset_get_relation_by_type (PyAtkRelationSet *self,
                                                       PyObject *args);
static PyObject* _atkrelationset_add_relation_by_type (PyAtkRelationSet *self,
                                                       PyObject *args);

/**
 * Methods, which are bound to the PyAtkRelationSet type.
 */
static PyMethodDef _atkrelationset_methods[] =
{
    { "contains", (PyCFunction) _atkrelationset_contains, METH_VARARGS,
      "R.contains (relation_type) -> bool\n\n"
      "Checks, whether the relation set contains a relation with the type.\n\n"
      "Determines whether the relation set contains a relation that matches\n"
      "the specified type." },
    { "remove", (PyCFunction) _atkrelationset_remove, METH_VARARGS,
      "R.remove (relation) -> None\n\n"
      "Removes a relation from the relation set." },
    { "add", (PyCFunction) _atkrelationset_add, METH_VARARGS,
      "R.add (relation) -> None\n\n"
      "Adds a new relation to the relation set.\n\n"
      "Add a new relation to the current relation set if it is not already\n"
      "present." },
    { "get_relation", (PyCFunction) _atkrelationset_get_relation, METH_VARARGS,
      "R.get_relation (index) -> AtkRelation\n\n"
      "Determines the relation at the specified position in the relation\n"
      "set."
    },
    { "get_relation_by_type",
      (PyCFunction) _atkrelationset_get_relation_by_type, METH_VARARGS,
      "R.get_relation_by_type (relation_type) -> AtkRelation\n\n"
      "Finds a relation that matches the specified type." },
    { "add_relation_by_type",
      (PyCFunction) _atkrelationset_add_relation_by_type, METH_VARARGS,
      "R.add_relation_by_type (relation_type, relation) -> None\n\n"
      "Adds a new relation to the relation set.\n\n"
      "Add a new relation of the specified type with the specified target\n"
      "to the current relation set if the relation set does not contain a\n"
      "relation of that type. If it is does contain a relation of that\n"
      "type the target is added to the relation." },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the PyAtkrelationSet.
 */
static PyGetSetDef _atkrelationset_getsets[] =
{
    { "n_relations", (getter) _atkrelationset_get_n_relations, NULL,
       "Determines the number of relations in a relation set.", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkRelationSet wrapper type, that encapsulates the AtkRelationSet.
 */
PyTypeObject PyAtkRelationSet_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkRelationSet",     /* tp_name */
    sizeof (PyAtkRelationSet),  /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkrelationset_dealloc, /* tp_dealloc */
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
    "ATK accessibility relation set", /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _atkrelationset_methods,    /* tp_methods */
    0,                          /* tp_members */
    _atkrelationset_getsets,    /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atkrelationset_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkrelationset_new,        /* tp_new */
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
 * Creates a new PyAtkRelationSet.
 */
static PyObject*
_atkrelationset_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkRelationSet *self;

    debug ("_atkrelationset_new\n");

    self = (PyAtkRelationSet *) type->tp_alloc (type, 0);
    return (PyObject *) self;
}

/**
 * Initializes the PyAtkRelationSet members, such as the AtkRelationSet.
 */
static int
_atkrelationset_init (PyAtkRelationSet *self, PyObject *args, PyObject *kwds)
{
    debug ("_atkrelationset_init\n");

    if (!self->obj)
        self->obj = atk_relation_set_new ();
    return 0;
}

/**
 * Deallocates the PyAtkRelationSet and its members.
 */
static void
_atkrelationset_dealloc (PyAtkRelationSet *self)
{
    debug ("_atkrelationset_dealloc\n");

    if (self->obj)
        g_object_unref (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Wrapper around atk_relation_set_contains().
 */
static PyObject*
_atkrelationset_contains (PyAtkRelationSet *self, PyObject *args)
{
    AtkRelationType val;

    debug ("_atkrelationset_contains\n");

    if (!PyArg_ParseTuple (args, "i:contains", &val))
        return NULL;
    
    if (atk_relation_set_contains (ATKRELATIONSET (self), val))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper around atk_relation_set_remove().
 */
static PyObject*
_atkrelationset_remove (PyAtkRelationSet *self, PyObject *args)
{
    PyObject *val;

    debug ("_atkrelationset_remove\n");

    if (!PyArg_ParseTuple (args, "O:remove", &val))
        return NULL;

    if (!PyObject_TypeCheck (val, &PyAtkRelation_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkRelation");
        return NULL;
    }

    atk_relation_set_remove (ATKRELATIONSET (self), ATKRELATION (val));
    Py_RETURN_NONE;
}

/**
 * Wrapper around atk_relation_set_add().
 */
static PyObject*
_atkrelationset_add (PyAtkRelationSet *self, PyObject *args)
{
    PyObject *val;

    debug ("_atkrelationset_add\n");

    if (!PyArg_ParseTuple (args, "O:add", &val))
        return NULL;
    
    if (!PyObject_TypeCheck (val, &PyAtkRelation_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkRelation");
        return NULL;
    }

    atk_relation_set_add (ATKRELATIONSET (self), ATKRELATION (val));
    Py_RETURN_NONE;
}

/**
 * Wrapper around atk_relation_set_get_n_relations().
 */
static PyObject*
_atkrelationset_get_n_relations (PyAtkRelationSet *self)
{
    gint amount;

    debug ("_atkrelationset_get_n_relations\n");

    amount = atk_relation_set_get_n_relations (ATKRELATIONSET (self));
    return PyInt_FromLong ((long) amount);
}

/**
 * Wrapper around atk_relation_set_get_relation().
 */
static PyObject*
_atkrelationset_get_relation (PyAtkRelationSet *self, PyObject *args)
{
    int i;
    AtkRelation *rel;
    PyAtkRelation *obj;
    
    if (!PyArg_ParseTuple (args, "i:get_relation", &i))
        return NULL;
    if (i < 0)
    {
        PyErr_SetString (PyExc_ValueError, "parameter must be >= 0");
        return NULL;
    }

    rel = atk_relation_set_get_relation (ATKRELATIONSET (self), i);
    if (!rel)
        Py_RETURN_NONE;

    obj = PyObject_New (PyAtkRelation, &PyAtkRelation_Type);
    obj->obj = rel;
    return (PyObject *) obj;
}

/**
 * Wrapper around atk_relation_set_get_relation_by_type().
 */
static PyObject*
_atkrelationset_get_relation_by_type (PyAtkRelationSet *self, PyObject *args)
{
    AtkRelationType type;
    AtkRelation *rel;
    PyAtkRelation *obj;
    
    if (!PyArg_ParseTuple (args, "i:get_relation_by_type", &type))
        return NULL;

    rel = atk_relation_set_get_relation_by_type (ATKRELATIONSET (self), type);
    if (!rel)
        Py_RETURN_NONE;

    obj = PyObject_New (PyAtkRelation, &PyAtkRelation_Type);
    obj->obj = rel;
    return (PyObject *) obj;
}

/**
 * Wrapper around atk_relation_set_add_relation_by_type().
 */
static PyObject*
_atkrelationset_add_relation_by_type (PyAtkRelationSet *self, PyObject *args)
{
    AtkRelationType type;
    PyAtkObject *obj;

    if (!PyArg_ParseTuple (args, "iO:add_relation_by_type", &type, &obj))
        return NULL;
    
    if (!PyObject_TypeCheck (obj, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "argument 2 must be an AtkObject");
        return NULL;
    }
    
    atk_relation_set_add_relation_by_type (ATKRELATIONSET (self), type,
                                           ATKOBJECT (obj));
    Py_RETURN_NONE;
}
