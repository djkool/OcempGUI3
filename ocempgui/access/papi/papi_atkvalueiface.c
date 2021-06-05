/* $Id: papi_atkvalueiface.c,v 1.12.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static PyObject* _not_implemented_args (PyObject *self, PyObject *args);
static PyObject* _not_implemented_noargs (PyObject *self);
static void _value_get_current_value (AtkValue *val, GValue *value);
static void _value_get_maximum_value (AtkValue *val, GValue *value);
static void _value_get_minimum_value (AtkValue *val, GValue *value);
static gboolean _value_set_current_value (AtkValue *val, const GValue *value);
static void _value_get_minimum_increment (AtkValue *val, GValue *value);

/* Converter for converting python types to gvalues. */
static void _convert_python_value (PyObject *result, GValue *value);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkvalueiface_methods[] =
{
    { "value_get_current_value", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.value_get_current_value () -> object\n\n"
      "Gets the value of this object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "value_get_maximum_value", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.value_get_maximum_value () -> object\n\n"
      "Gets the maximum value of this object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "value_get_minimum_value", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.value_get_minimum_value () -> object\n\n"
      "Gets the minimum value of this object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "value_set_current_value", _not_implemented_args, METH_VARARGS,
      "O.value_set_current_value (value) -> bool\n\n"
      "Sets the value of this object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "value_get_minimum_increment", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.value_get_minimum_increment () -> object\n\n"
      "Gets the minimum increment value.\n\n"
      "Gets the minimum increment by which the value of this object may be\n"
      "changed. If zero, the minimum increment is undefined, which may mean\n"
      "that it is limited only by the floating point precision of the\n"
      "platform.\n\n"
      "This method has to be implemented by inherited classes." },
    { NULL, NULL, 0, NULL }
};

/**
 * Default implementation for the interfaces.
 */
static PyObject*
_not_implemented_args (PyObject *self, PyObject *args)
{
    PyErr_SetString (PyExc_NotImplementedError, "method not implemented");
    return NULL;
}

/**
 * Default implementation for the interfaces.
 */
static PyObject*
_not_implemented_noargs (PyObject *self)
{
    PyErr_SetString (PyExc_NotImplementedError, "method not implemented");
    return NULL;
}

/**
 * Wrapper for atk_value_get_current_value().
 */
static void
_value_get_current_value (AtkValue *val, GValue *value)
{
    PyObject *obj;
    PyObject *result;

    debug ("_value_get_current_value\n");
    obj = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "value_get_current_value", NULL);
    if (!result)
        return;

    _convert_python_value (result, value);
    Py_DECREF (result);
    return;
}

/**
 * Wrapper for atk_value_get_maximum_value().
 */
static void
_value_get_maximum_value (AtkValue *val, GValue *value)
{
    PyObject *obj;
    PyObject *result;

    debug ("_value_get_maximum_value\n");
    obj = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "value_get_maximum_value", NULL);
    if (!result)
        return;

    _convert_python_value (result, value);
    Py_DECREF (result);
    return;
}

/**
 * Wrapper for atk_value_get_minimum_value().
 */
static void
_value_get_minimum_value (AtkValue *val, GValue *value)
{
    PyObject *obj;
    PyObject *result;

    debug ("_value_get_minimum_value\n");
    obj = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "value_get_minimum_value", NULL);
    if (!result)
        return;

    _convert_python_value (result, value);
    Py_DECREF (result);
    return;
}

/**
 * Wrapper for atk_value_set_current_value().
 */
static gboolean
_value_set_current_value (AtkValue *val, const GValue *value)
{
    PyObject *obj;
    PyObject *result;
    PyObject *conv;
    gboolean retval = FALSE;

    debug ("_value_set_current_value\n");

    conv = pyobject_from_gvalue (value);

    obj = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "value_set_current_value", "O", conv);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_value_get_minimum_increment().
 */
static void
_value_get_minimum_increment (AtkValue *val, GValue *value)
{
    PyObject *obj;
    PyObject *result;

    debug ("_value_get_minimum_increment\n");
    obj = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "value_get_minimum_increment", NULL);
    if (!result)
        return;

    _convert_python_value (result, value);
    Py_DECREF (result);
    return;
}

static void
_convert_python_value (PyObject *result, GValue *value)
{
    /* TODO: Add support for specific type ranges, e.g. float
     * vs. double, int64, ulong, lists, tuples, dicts, etc.pp. */

    /* Basic type checks and conversions. */
    if (PyInt_Check (result))
    {
        g_value_set_int (value, PyInt_AsLong (result));
    }
    else if (PyBool_Check (result))
    {
        g_value_set_boolean (value, (result == Py_True) ? TRUE : FALSE);
    }
    else if (PyLong_Check (result)) 
    {
        g_value_set_long (value, PyLong_AsLong (result));
    }
    else if (PyFloat_Check (result))
    {
        g_value_set_double (value, PyFloat_AsDouble (result));
    }
    else if (PyString_Check (result))
    {
        g_value_set_string (value, PyString_AsString (result));
    }
    /* Do we have to implement specific high level object types here,
     * like e.g. lists and tuples?
     */
}

/**
 * Bind the different AtkValueIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkvalueiface_init (AtkValueIface *iface)
{
    iface->get_current_value = _value_get_current_value;
    iface->get_maximum_value = _value_get_maximum_value;
    iface->get_minimum_value = _value_get_minimum_value;
    iface->set_current_value = _value_set_current_value;
    iface->get_minimum_increment = _value_get_minimum_increment;
}

/**
 * Creates PyObject instances fom GValue types.
 */
PyObject*
pyobject_from_gvalue (GValue *value)
{
    /* TODO: Add support for specific type ranges, e.g. float
     * vs. double, int64, ulong, lists, tuples, dicts, etc.pp. */

    if (G_VALUE_HOLDS_INT (value))
    {
        return PyInt_FromLong (g_value_get_int (value));
    }
    else if (G_VALUE_HOLDS_BOOLEAN (value))
    {
        if (g_value_get_boolean (value))
            Py_RETURN_TRUE;
        Py_RETURN_FALSE;
    }
    else if (G_VALUE_HOLDS_LONG (value))
    {
        return PyLong_FromLong (g_value_get_long (value));
    }
    else if (G_VALUE_HOLDS_DOUBLE (value))
    {
        return PyFloat_FromDouble (g_value_get_double (value));
    }
    else if (G_VALUE_HOLDS_STRING (value))
    {
        return PyString_FromString (g_value_get_string (value));
    }

    Py_RETURN_NONE;
}

/**
 * Gets a modified interface for the AtkValueIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkvalueiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkvalueiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkValue methods to the passed object instance.
 */
void
atkvalueiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkvalueiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkvalueiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkvalueiface_methods[i].ml_name, func);
        i++;
    }
}

