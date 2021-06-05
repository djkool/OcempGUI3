/* $Id: papi_atkstatetype.c,v 1.2.2.4 2007/09/23 07:37:49 marcusva Exp $
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

/**
 * Wrapper for atk_state_type_get_name().
 */
static PyObject*
_state_type_get_name (PyObject *self, PyObject *args)
{
    AtkStateType val;

    debug ("_state_type_get_name\n");

    if (!PyArg_ParseTuple (args, "i:atk_state_type_get_name", &val))
        return NULL;
    
    return PyString_FromString (atk_state_type_get_name (val));
}

/**
 * Wrapper for atk_state_type_for_name().
 */
static PyObject*
_state_type_for_name (PyObject *self, PyObject *args)
{
    char *val;

    debug ("_state_type_for_name\n");

    if (!PyArg_ParseTuple (args, "s:atk_state_type_for_name", &val))
        return NULL;
    
    return PyInt_FromLong ((long) atk_state_type_for_name (val));
}

/**
 * Wrapper for atk_state_type_register().
 */
static PyObject*
_state_type_register (PyObject *self, PyObject *args)
{
    char *val;
    
    debug ("_state_type_register\n");

    if (!PyArg_ParseTuple (args, "s:atk_state_type_register", &val))
        return NULL;

    return PyInt_FromLong ((long) atk_state_type_register (val));
}

/**
 * Exports the different atk_state* functions into the module.
 */
void
atkstatetype_export_funcs (PyObject *module)
{
    int i = 0;
    PyObject *func;

    static PyMethodDef methods[] = 
    {
        { "atk_state_type_get_name", _state_type_get_name, METH_VARARGS,
          "atk_state_type_get_name (type) -> string\n\n"
          "Gets the description string describing the AtkStateType type." },
        { "atk_state_type_for_name", _state_type_for_name, METH_VARARGS,
          "atk_state_type_for_name (name) -> int\n\n"
          "Gets the AtkStateType corresponding to the name." },
        { "atk_state_type_register", _state_type_register, METH_VARARGS,
          "atk_state_type_register (name) -> int\n\n"
          "Register a new object state." },
        { NULL, NULL, 0, NULL }
    };

    while (methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&methods[i], NULL);
        PyObject_SetAttrString (module, methods[i].ml_name, func);
        i++;
    }
}
