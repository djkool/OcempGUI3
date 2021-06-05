/* $Id: papi_atkstateset.c,v 1.9.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static PyObject* _atkstateset_new (PyTypeObject *type, PyObject *args,
                                   PyObject *kwds);
static int _atkstateset_init (PyAtkStateSet *self, PyObject *args,
                              PyObject *kwds);
static void _atkstateset_dealloc (PyAtkStateSet *self);
static PyObject* _atkstateset_is_empty (PyAtkStateSet *self);
static PyObject* _atkstateset_add_state (PyAtkStateSet *self, PyObject *args);
static PyObject* _atkstateset_add_states (PyAtkStateSet *self, PyObject *args);
static PyObject* _atkstateset_clear_states (PyAtkStateSet *self);
static PyObject* _atkstateset_contains_state (PyAtkStateSet *self,
                                              PyObject *args);
static PyObject* _atkstateset_contains_states (PyAtkStateSet *self,
                                               PyObject *args);
static PyObject* _atkstateset_remove_state (PyAtkStateSet *self,
                                            PyObject *args);
static PyObject* _atkstateset_and_sets (PyAtkStateSet *self, PyObject *args);
static PyObject* _atkstateset_or_sets (PyAtkStateSet *self, PyObject *args);
static PyObject* _atkstateset_xor_sets (PyAtkStateSet *self, PyObject *args);

/**
 * Methods, which are bound to the PyAtkStateSet type.
 */
static PyMethodDef _atkstateset_methods[] =
{
    { "add_state", (PyCFunction) _atkstateset_add_state, METH_VARARGS,
      "S.add_state (state) -> bool\n\n"
      "Adds a new state to the AtkStateSet.\n\n"
      "Add a new state for the specified type to the current state set,\n"
      "if it is not already present." },
    { "add_states", (PyCFunction) _atkstateset_add_states, METH_VARARGS,
      "S.add_states (states) -> bool\n\n"
      "Add the states for the specified types to the current state set." },
    { "clear_states", (PyCFunction) _atkstateset_clear_states, METH_NOARGS,
      "S.clear_states () -> None\n\n"
      "Removes all states from the state set." },
    { "contains_state", (PyCFunction) _atkstateset_contains_state,
      METH_VARARGS,
      "S.contains_state (state) -> bool\n\n"
      "Checks whether the state for the specified type is in the set." },
    { "contains_states", (PyCFunction) _atkstateset_contains_states,
      METH_VARARGS,
      "S.contains_states (states) -> bool\n\n"
      "Checks whether the states for all the specified types are in the\n"
      "set." },
    { "remove_state", (PyCFunction) _atkstateset_remove_state, METH_VARARGS,
      "S.remove_state (state) -> None\n\n"
      "Removes the state for the specified type from the state set." },
    { "and_sets", (PyCFunction) _atkstateset_and_sets, METH_VARARGS,
      "S.and_sets (set) -> AtkStateSet\n\n"
      "Constructs the intersection of the two sets.\n\n"
      "Constructs the intersection of the two sets, returning None if the\n"
      "intersection is empty." }, 
    { "or_sets", (PyCFunction) _atkstateset_or_sets, METH_VARARGS,
      "S.or_sets (set) -> AtkStateSet\n\n"
      "Constructs the union of the two sets." },
    { "xor_sets", (PyCFunction) _atkstateset_xor_sets, METH_VARARGS,
      "S.xor_sets (set) -> AtkStateSet\n\n"
      "Constructs the exclusive-or of the two sets.\n\n"
      "The set returned by this operation contains the states in exactly\n"
      "one of the two sets or None if empty." },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters of the AtkStateSet.
 */
static PyGetSetDef _atkstateset_getsets[] =
{
    { "is_empty", (getter) _atkstateset_is_empty, NULL,
      "Indicates, whether the state set is empty, i.e. has no states set.",
      NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkStateSet wrapper type, that encapsulates the AtkStateSet.
 */
PyTypeObject PyAtkStateSet_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkStateSet",        /* tp_name */
    sizeof (PyAtkStateSet),     /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkstateset_dealloc, /* tp_dealloc */
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
    "ATK accessibility stateset", /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _atkstateset_methods,       /* tp_methods */
    0,                          /* tp_members */
    _atkstateset_getsets,       /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atkstateset_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkstateset_new,           /* tp_new */
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
 * Creates a new PyAtkStateSet.
 */
static PyObject*
_atkstateset_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkStateSet *self;

    debug ("_atkstateset_new\n");

    self = (PyAtkStateSet *) type->tp_alloc (type, 0);
    return (PyObject *) self;
}

/**
 * Initializes the PyAtkStateSet members, such as the AtkStateSet.
 */
static int
_atkstateset_init (PyAtkStateSet *self, PyObject *args, PyObject *kwds)
{
    debug ("_atkstateset_init\n");

    if (!self->obj)
        self->obj = atk_state_set_new ();
    return 0;
}

/**
 * Deallocates the PyAtkStateSet and its members.
 */
static void
_atkstateset_dealloc (PyAtkStateSet *self)
{
    debug ("_atkstateset_dealloc\n");

    if (self->obj)
        g_object_unref (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Wrapper for atk_state_set_is_empty().
 */
static PyObject*
_atkstateset_is_empty (PyAtkStateSet *self)
{
    debug ("atkstateset_is_empty\n");

    if (atk_state_set_is_empty (ATKSTATESET (self)))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_state_set_add_state().
 */
static PyObject*
_atkstateset_add_state (PyAtkStateSet *self, PyObject *args)
{
    AtkStateType val;

    debug ("_atkstateset_add_state\n");

    if (!PyArg_ParseTuple (args, "i:add_state", &val))
        return NULL;

    if (atk_state_set_add_state (ATKSTATESET (self), val))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_state_set_add_states().
 */
static PyObject*
_atkstateset_add_states (PyAtkStateSet *self, PyObject *args)
{
    int i;
    int amount;
    gboolean islist = FALSE;
    gboolean istuple = FALSE;
    AtkStateType *types = NULL;
    PyObject *val;

    debug ("_atkstateset_add_states\n");

    if (!PyArg_ParseTuple (args, "O:add_states", &val))
        return NULL;

    istuple = PyTuple_Check (val);
    if (!istuple)
        islist = PyList_Check (val);
    if (!islist && !istuple)
        return NULL;

    amount = (islist) ? PyList_Size (val) : PyTuple_Size (val);
    types = PyMem_New (AtkStateType, amount);
    if (!types)
        return PyErr_NoMemory ();

    if (islist)
        for (i = 0; i < amount; i++)
            types[i] = PyInt_AsLong (PyList_GetItem (val, i));
    else
        for (i = 0; i < amount; i++)
            types[i] = PyInt_AsLong (PyTuple_GetItem (val, i));

    atk_state_set_add_states (ATKSTATESET (self), types, amount);
    PyMem_Free (types);
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_state_set_clear_states().
 */
static PyObject*
_atkstateset_clear_states (PyAtkStateSet *self)
{
    debug ("_atkstateset_clear_states\n");
    atk_state_set_clear_states (ATKSTATESET (self));
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_state_set_contains_state().
 */
static PyObject*
_atkstateset_contains_state (PyAtkStateSet *self, PyObject *args)
{
    AtkStateType val;

    debug ("_atkstateset_contains_state\n");

    if (!PyArg_ParseTuple (args, "i:contains_state", &val))
        return NULL;

    if (atk_state_set_contains_state (ATKSTATESET (self), val))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_state_set_contains_states().
 */
static PyObject*
_atkstateset_contains_states (PyAtkStateSet *self, PyObject *args)
{
    int i;
    int amount;
    gboolean islist = FALSE;
    gboolean istuple = FALSE;
    AtkStateType *types = NULL;
    PyObject *val;
    gboolean ret;

    debug ("_atkstateset_contains_states\n");

    if (!PyArg_ParseTuple (args, "O:contains_states", &val))
        return NULL;

    istuple = PyTuple_Check (val);
    if (!istuple)
        islist = PyList_Check (val);
    if (!islist && !istuple)
        return NULL;

    amount = (islist) ? PyList_Size (val) : PyTuple_Size (val);
    types = PyMem_New (AtkStateType, amount);
    if (!types)
        return PyErr_NoMemory ();

    if (islist)
        for (i = 0; i < amount; i++)
            types[i] = PyInt_AsLong (PyList_GetItem (val, i));
    else
        for (i = 0; i < amount; i++)
            types[i] = PyInt_AsLong (PyTuple_GetItem (val, i));

    ret = atk_state_set_contains_states (ATKSTATESET (self), types, amount);
    PyMem_Free (types);

    if (ret) 
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_state_set_remove_state().
 */
static PyObject*
_atkstateset_remove_state (PyAtkStateSet *self, PyObject *args)
{
    AtkStateType val;

    debug ("_atkstateset_remove_state\n");

    if (!PyArg_ParseTuple (args, "i:remove_state", &val))
        return NULL;

    if (atk_state_set_remove_state (ATKSTATESET (self), val))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_state_set_and_sets().
 */
static PyObject*
_atkstateset_and_sets (PyAtkStateSet *self, PyObject *args)
{
    PyObject *val;
    AtkStateSet *result;
    PyAtkStateSet *retval;

    debug ("_atkstateset_and_sets\n");

    if (!PyArg_ParseTuple (args, "O:and_sets", &val))
        return NULL;
    
    if (!PyObject_TypeCheck (val, &PyAtkStateSet_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkStateSet");
        return NULL;
    }

    retval = PyObject_New (PyAtkStateSet, &PyAtkStateSet_Type);
    result = atk_state_set_and_sets (ATKSTATESET (self), ATKSTATESET (val));
    retval->obj = result;
    return (PyObject *) retval;
}

/**
 * Wrapper for atk_state_set_or_sets().
 */
static PyObject*
_atkstateset_or_sets (PyAtkStateSet *self, PyObject *args)
{
    PyObject *val;
    AtkStateSet *result;
    PyAtkStateSet *retval;

    debug ("_atkstateset_or_sets\n");

    if (!PyArg_ParseTuple (args, "O:or_sets", &val))
        return NULL;

    if (!PyObject_TypeCheck (val, &PyAtkStateSet_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkStateSet");
        return NULL;
    }

    result = atk_state_set_or_sets (ATKSTATESET (self), ATKSTATESET (val));
    if (result)
    {
        retval = PyObject_New (PyAtkStateSet, &PyAtkStateSet_Type);
        retval->obj = result;
        return (PyObject *) retval;
    }
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_state_set_xor_sets().
 */
static PyObject*
_atkstateset_xor_sets (PyAtkStateSet *self, PyObject *args)
{
    PyObject *val;
    AtkStateSet *result;
    PyAtkStateSet *retval;

    debug ("_atkstateset_xor_sets\n");

    if (!PyArg_ParseTuple (args, "O:or_sets", &val))
        return NULL;

    if (!PyObject_TypeCheck (val, &PyAtkStateSet_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkStateSet");
        return NULL;
    }

    retval = PyObject_New (PyAtkStateSet, &PyAtkStateSet_Type);
    result = atk_state_set_xor_sets (ATKSTATESET (self), ATKSTATESET (val));
    retval->obj = result;
    return (PyObject *) retval;
}
