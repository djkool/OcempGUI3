/* $Id: papi_atktextattribute.c,v 1.3.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static PyObject* _text_attribute_register (PyObject *self, PyObject *args);
static PyObject* _text_attribute_get_name (PyObject *self, PyObject *args);
static PyObject* _text_attribute_for_name (PyObject *self, PyObject *args);
static PyObject* _text_attribute_get_value (PyObject *self, PyObject *args);

/**
 * Wrapper for atk_text_attribute_register().
 */
static PyObject*
_text_attribute_register (PyObject *self, PyObject *args)
{
    char *val;

    debug ("_text_attribute_register\n");

    if (!PyArg_ParseTuple (args, "s:atk_text_attribute_register", &val))
        return NULL;

    return PyInt_FromLong ((long) atk_text_attribute_register (val));
}

/**
 * Wrapper for atk_text_attribute_get_name().
 */
static PyObject*
_text_attribute_get_name (PyObject *self, PyObject *args)
{
    AtkTextAttribute val;

    debug ("_text_attribute_get_name\n");

    if (!PyArg_ParseTuple (args, "i:atk_text_attribute_get_name", &val))
        return NULL;

    return PyString_FromString (atk_text_attribute_get_name (val));
}

/**
 * Wrapper for atk_text_attribute_for_name().
 */
static PyObject*
_text_attribute_for_name (PyObject *self, PyObject *args)
{
    char *val;

    debug ("_text_attribute_for_name\n");

    if (!PyArg_ParseTuple (args, "s:atk_text_attribute_for_name", &val))
        return NULL;
    
    return PyInt_FromLong ((long) atk_text_attribute_for_name (val));
}

/**
 * Wrapper for atk_text_attribute_get_value().
 */
static PyObject*
_text_attribute_get_value (PyObject *self, PyObject *args)
{
    AtkTextAttribute val;
    gint index_;
    const gchar *retval;

    debug ("_text_attribute_get_value\n");

    if (!PyArg_ParseTuple (args, "ii:atk_text_attribute_get_value", &val,
                           &index_))
        return NULL;
    retval = atk_text_attribute_get_value (val, index_);
    if (!retval)
        Py_RETURN_NONE;

    return PyString_FromString (retval);
}

/**
 * Exports the different atk_attribute* functions into the module.
 */
void
atktextattribute_export_funcs (PyObject *module)
{
    int i = 0;
    PyObject *func;

    static PyMethodDef methods[] = { 
        { "atk_text_attribute_get_name", _text_attribute_get_name,
          METH_VARARGS,
          "atk_text_attribute_get_name (attribute) -> string\n\n"
          "Gets the name corresponding to the AtkTextAttribute." },
        { "atk_text_attribute_for_name", _text_attribute_for_name,
          METH_VARARGS,
          "atk_text_attribute_for_name (name) -> int\n\n"
          "Get the AtkTextAttribute type corresponding to a attribute name." },
        { "atk_text_attribute_get_value", _text_attribute_get_value,
          METH_VARARGS,
          "atk_text_attribute_get_value (attribute, index) -> string\n\n"
          "Gets the value for the index of the AtkTextAttribute." },
        { "atk_text_attribute_register", _text_attribute_register,
          METH_VARARGS,
          "atk_text_attribute_register (name) -> int\n\n"
          "Associate name with a new AtkTextAttribute." },
        { NULL, NULL, 0, NULL }
    };

    while (methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&methods[i], NULL);
        PyObject_SetAttrString (module, methods[i].ml_name, func);
        i++;
    }
}
