/* $Id: papi_atkactioniface.c,v 1.9.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static gboolean _action_do_action (AtkAction *action, gint i);
static gint _action_get_n_actions (AtkAction *action);
static const gchar* _action_get_description (AtkAction *action, gint i);
static const gchar* _action_get_keybinding (AtkAction *action, gint i);
static const gchar* _action_get_name (AtkAction *action, gint i);
static const gchar* _action_get_localized_name (AtkAction *action, gint i);
static gboolean _action_set_description (AtkAction *action, gint i,
                                         const gchar *desc);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkactioniface_methods[] = 
{
    { "action_do_action", _not_implemented_args, METH_VARARGS,
      "O.action_do_action (index) -> bool\n\n"
      "Perform the specified action on the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_get_n_actions", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.get_n_actions () -> int\n\n"
      "Gets the number of accessible actions available on the object.\n\n"
      "If there are more than one, the first one is considered the\n"
      "\"default\" action of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_get_description", _not_implemented_args, METH_VARARGS,
      "O.action_get_description (index) -> string\n\n"
      "Returns a description of the specified action of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_set_description", _not_implemented_args, METH_VARARGS,
      "action_set_description (index, text) -> bool\n\n"
      "Sets a description of the specified action of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_get_name", _not_implemented_args, METH_VARARGS,
      "O.action_get_name (index) -> string\n\n"
      "Returns the name of the specified action of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_get_localized_name", _not_implemented_args, METH_VARARGS,
      "O.action_get_localized_name (index) -> string\n\n"
      "Returns the localized name of the specified action of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "action_get_keybinding", _not_implemented_args, METH_VARARGS,
      "O.action_get_keybinding (index) -> string\n\n"
      "Returns a keybinding associated with this action, if one exists.\n\n"
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
 * Wrapper for atk_action_do_action().
 */
static gboolean
_action_do_action (AtkAction *action, gint i)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_action_do_action\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_do_action", "i", i);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_get_n_actions().
 */
static gint
_action_get_n_actions (AtkAction *action)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_action_get_n_actions\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_get_n_actions", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_get_description().
 */
static const gchar*
_action_get_description (AtkAction *action, gint i)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval;

    debug ("_action_get_description\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_get_description", "i", i);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_get_keybinding().
 */
static const gchar*
_action_get_keybinding (AtkAction *action, gint i)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval;

    debug ("_action_get_keybinding\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_get_keybinding", "i", i);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_get_name().
 */
static const gchar*
_action_get_name (AtkAction *action, gint i)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval;

    debug ("_action_get_name\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_get_name", "i", i);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_get_localized_name().
 */
static const gchar*
_action_get_localized_name (AtkAction *action, gint i)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval = 0;

    debug ("_action_get_localized_name\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_get_localized_name", "i", i);
    if (!result)
        return NULL;
    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_action_set_description().
 */
static gboolean
_action_set_description (AtkAction *action, gint i, const gchar *desc)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_action_set_description\n");

    obj = g_object_get_data (G_OBJECT (action), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "action_set_description", "is", i, desc);
    if (!result)
        return FALSE;
    retval = (result == Py_True);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkActionIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkactioniface_init (AtkActionIface *iface)
{
    iface->do_action = _action_do_action;
    iface->get_n_actions = _action_get_n_actions;
    iface->get_description = _action_get_description;
    iface->get_keybinding = _action_get_keybinding;
    iface->get_name = _action_get_name;
    iface->get_localized_name = _action_get_localized_name;
    iface->set_description = _action_set_description;
}

/**
 * Gets a modified interface for the AtkActionIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkactioniface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkactioniface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkAction methods to the passed object instance.
 */
void
atkactioniface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkactioniface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkactioniface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkactioniface_methods[i].ml_name, func);
        i++;
    }
}
