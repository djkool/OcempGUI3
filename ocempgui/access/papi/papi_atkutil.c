/* $Id: papi_atkutil.c,v 1.10.2.7 2008/05/10 08:03:46 marcusva Exp $
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
 * Getter method used as wrapper for atk_get_root() and the hook for the 
 * AtkUtilClass->get_root interface. It will be bound by
 * user code usually using something like:
 *
 * papi.get_root = my_main_widget.get_accessible
 *
 */
static PyObject *_get_root = NULL;

/**
 * Listener dictionary for focus listeners executed upon focus changes.
 * Those are used to be executed upon calling the various
 * atk_*_focus_tracker_* interfaces.
 */
static PyObject* _focus_listeners = NULL;

/** 
 * Python callback for atk_focus_tracker_init().
 */
static PyObject* _focus_tracker_init = NULL;

/**
 * Listener dictionary for mapping AtkObject events to the AT-SPI
 * event hooks. See the classbindings source for initialization and the
 * atkobject source for usage.
 */
static PyObject *_global_listeners = NULL;
static PyObject *_global_signals = NULL;
static guint _global_listener_ids = 0;

/* Forward declarations. */
static PyObject* _atkutil_set_root (PyObject *self, PyObject *args);
static PyObject* _atkutil_get_root (PyObject *self);
static PyObject* _atkutil_get_toolkit_name (PyObject *self);
static PyObject* _atkutil_get_toolkit_version (PyObject *self);
static PyObject* _atkutil_add_focus_tracker (PyObject *self, PyObject *args);
static PyObject* _atkutil_remove_focus_tracker (PyObject *self,
                                                PyObject *args);
static PyObject* _atkutil_focus_tracker_init (PyObject *self, PyObject *args);
static PyObject* _atkutil_focus_tracker_notify (PyObject *self,
                                                PyObject *args);
static PyObject* _atkutil_add_key_event_listener (PyObject *self,
                                                  PyObject *args);
static PyObject* _atkutil_remove_key_event_listener (PyObject *self,
                                                     PyObject *args);
static PyObject* _atkutil_get_focus_object (PyObject *self);

/* Class bindings. */
static AtkObject* _class_get_root (void);
static G_CONST_RETURN gchar* _class_get_toolkit_name (void);
static G_CONST_RETURN gchar* _class_get_toolkit_version (void);
static void _class_focus_tracker_init (void);
static inline PyObject* _add_listener_dict (const char *event_type);
static guint _class_add_global_event_listener (GSignalEmissionHook listener,
                                               const gchar *event_type);
static void _class_remove_global_event_listener (guint id);

/**
 * papi.get_root implementation.
 */
static PyObject*
_atkutil_set_root (PyObject *self, PyObject *args)
{
    PyObject *tmp;

    debug ("_atkutil_set_root\n");

    if (!PyArg_ParseTuple (args, "O:set_root", &tmp))
        return NULL;

    if (!PyCallable_Check (tmp)
        && !PyObject_TypeCheck (tmp, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be callable or an "
                         "AtkObject");
        return NULL;
    }
    Py_XINCREF(tmp);
    Py_XDECREF(_get_root);
    _get_root = tmp;
    Py_RETURN_NONE;
}

/**
 * Python getter for the root object.
 */
static PyObject*
_atkutil_get_root (PyObject *self)
{
    PyObject *result;

    debug ("_atkutil_get_root\n");
 
    if (!_get_root)
    {
        PyErr_SetString (PyExc_NameError, "Interface get_root not satisfied.");
        return NULL;
    }

    if (!PyCallable_Check (_get_root))
    {
        Py_INCREF (_get_root);
        return _get_root;
    }

    result = (PyObject *) PyEval_CallObject (_get_root, NULL);
    if (!result)
        return NULL;

    Py_INCREF (result);
    return result;
}

/**
 * Wrapper for atk_get_toolkit_name().
 */
static PyObject*
_atkutil_get_toolkit_name (PyObject *self)
{
    debug ("_atkutil_get_toolkit_name\n");
    return PyString_FromString (_class_get_toolkit_name ());
}

/**
 * Wrapper for atk_get_toolkit_version().
 */
static PyObject*
_atkutil_get_toolkit_version (PyObject *self)
{
    debug ("_atkutil_get_toolkit_version\n");
    return PyString_FromString (_class_get_toolkit_version ());
}

/**
 * Wrapper for atk_add_focus_tracker().
 */
static PyObject*
_atkutil_add_focus_tracker (PyObject *self, PyObject *args)
{
    PyObject *tmp;
    PyObject *id = NULL;
    
    debug ("_atkutil_add_focus_tracker\n");

    if (!_focus_listeners)
    {
        _focus_listeners = PyDict_New ();
        if (!_focus_listeners)
            return NULL;
    }

    if (!PyArg_ParseTuple (args, "O:add_focus_tracker", &tmp))
        return NULL;
    
    if (!PyCallable_Check (tmp))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    id = PyInt_FromLong (PyDict_Size (_focus_listeners));
    if (PyDict_SetItem (_focus_listeners, id, tmp) == -1)
    {
        Py_DECREF (id);
        return NULL;
    }
    Py_DECREF (tmp);
    Py_DECREF (id);
    return id;
}

/**
 * Wrapper for atk_remove_focus_tracker().
 */
static PyObject*
_atkutil_remove_focus_tracker (PyObject *self, PyObject *args)
{
    int id = 0;
    PyObject* key = NULL;

    debug ("_atkutil_remove_focus_tracker\n");

    if (!_focus_listeners)
    {
        PyErr_SetString (PyExc_ValueError, "no listeners available");
        return NULL;
    }

    if (!PyArg_ParseTuple (args, "i:remove_focus_tracker", &id))
        return NULL;
    
    key = PyInt_FromLong (id);
    if (!PyDict_DelItem (_focus_listeners, key))
    {
        Py_DECREF (key);
        return NULL;
    }
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_focus_tracker_init().
 */
static PyObject*
_atkutil_focus_tracker_init (PyObject *self, PyObject *args)
{
    PyObject *tmp;

    debug ("_atkutil_focus_tracker_init\n");

    if (!PyArg_ParseTuple (args, "O:focus_tracker_init", &tmp))
        return NULL;
    
    if (!PyCallable_Check (tmp))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    Py_XINCREF (tmp);
    Py_XDECREF (_focus_tracker_init);
    _focus_tracker_init = tmp;
    atk_focus_tracker_init ((AtkEventListenerInit) _class_focus_tracker_init);
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_focus_tracker_notify().
 */
static PyObject*
_atkutil_focus_tracker_notify (PyObject *self, PyObject *args)
{
    PyObject *obj;
    
    debug ("_atkutil_focus_tracker_notify\n");

    if (!PyArg_ParseTuple (args, "O:focus_tracker_notify\n", &obj))
        return NULL;

    if (!PyObject_TypeCheck (obj, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkObject");
        return NULL;
    }
    atk_focus_tracker_notify (ATKOBJECT (obj));
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_add_key_event_listener().
 */
static PyObject*
_atkutil_add_key_event_listener (PyObject *self, PyObject *args)
{
    /* TODO */
    debug ("_atkutil_add_key_event_listener\n");
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_remove_key_event_listener().
 */
static PyObject*
_atkutil_remove_key_event_listener (PyObject *self, PyObject *args)
{
    /* TODO */
    debug ("_atkutil_remove_key_event_listener\n");
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_get_focus_object().
 */
static PyObject*
_atkutil_get_focus_object (PyObject *self)
{
    PyAtkObject *atkobj;
    AtkObject *obj = NULL;

    debug ("_atkutil_get_focus_object\n");

    obj = atk_get_focus_object ();
    if (obj)
    {
        atkobj = g_object_get_data (G_OBJECT (obj), PAPI_PYOBJECT);
        if (!atkobj)
        {
            atkobj = (PyAtkObject *)
                PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
            atkobj->obj = g_object_ref (obj);
            g_object_set_data (G_OBJECT (obj), PAPI_PYOBJECT, atkobj);
        }
        return (PyObject *) atkobj;
    }
    Py_RETURN_NONE;
}

/**
 * Interface, that invokes the bound python method of papi.get_root() to
 * receive an AtkObject suitable for
 * AtkUtilClass->get_root/atk_get_root().
 */
static AtkObject*
_class_get_root (void)
{
    PyObject *result;

    debug ("_class_get_root\n");

    if (!_get_root)
        return NULL;

    if (!PyCallable_Check (_get_root))
        return ATKOBJECT (_get_root);

    result = (PyObject *) PyEval_CallObject (_get_root, NULL);
    if (!result)
        return NULL;
    Py_DECREF (result);

#ifdef DEBUG
    if (!ATKOBJECT (result))
        debug ("_class_get_root has no ATK_OBJECT\n");
#endif

    return ATKOBJECT (result);
}

/**
 * Interface for AtkUtilClass->get_toolkit_name.
 */
static G_CONST_RETURN gchar*
_class_get_toolkit_name (void)
{
    debug ("_class_get_toolkit_name\n");
    return "PAPI";
}

/**
 * Interface for AtkUtilClass->get_toolkit_version.
 */
static G_CONST_RETURN gchar*
_class_get_toolkit_version (void)
{
    debug ("_class_get_toolkit_version\n");
    return "0.0.5";
}

/**
 * C execution handler for the python method set in the
 * atk_focus_tracker_init() wrapper _atkutil_focus_tracker_init().
 */
static void
_class_focus_tracker_init (void)
{
    PyObject *result;

    debug ("_class_focus_tracker_init\n");

    result = (PyObject *) PyEval_CallObject (_focus_tracker_init, NULL);
    if (result)
    {
        Py_DECREF (result);
    }
}

/**
 * Adds a new dict for a specific event type to the listeners.
 */
static inline PyObject*
_add_listener_dict (const char *event_type)
{
    PyObject *key = NULL;
    PyObject *dict = PyDict_New ();

    if (!dict)
        return NULL;

    if (PyDict_SetItemString (_global_listeners, event_type, dict) == -1)
    {
        Py_DECREF (dict);
        return NULL;
    }
    Py_DECREF (dict);

    /* Update the signal ids. */
    key = PyInt_FromLong (PyDict_Size (_global_signals));
    if (!PyDict_GetItemString (_global_signals, event_type))
    {
        PyDict_SetItemString (_global_signals, event_type, key);
        Py_DECREF (key);
    }
    return dict;
}

/**
 * Interface for AtkUtilClass->add_global_event_listener.
 */
static guint
_class_add_global_event_listener (GSignalEmissionHook listener,
                                  const gchar *event_type)
{
    PyObject *dict = NULL;
    PyObject *cb = NULL;
    PyObject *key = NULL;
    gchar **split = g_strsplit (event_type, ":", 3);

    debug ("_class_add_global_event_listener\n");

    if (!split)
        return 0;

    if (!_global_listeners)
    {
        _global_listeners = PyDict_New ();
        if (!_global_listeners)
            return 0;
    }

    if (!_global_signals)
    {
        _global_signals = PyDict_New ();
        if (!_global_signals)
            return 0;
    }

#ifdef DEBUG
    printf ("DEBUG: Installing handler for %s\n", event_type);
#endif

    /* Create the event mappings, which are used by the AtkObject system
     * and have to be implemented manually by the user.
     */
    if (strcmp ("window", split[0]) == 0)
    {
        g_strfreev (split); /* Free string. */

        /* Lookup matching dict. */
        dict = PyDict_GetItemString (_global_listeners, event_type);
        if (!dict)
        {
            dict = _add_listener_dict (event_type);
            if (!dict)
                return 0;
        }
            
        /* Add listener. */
        cb = PyCObject_FromVoidPtr (listener, NULL);
        key = PyInt_FromLong ((long) _global_listener_ids + 1);
        if (PyDict_SetItem (dict, key, cb) == -1)
        {
            Py_DECREF (cb);
            Py_DECREF (key);
            return 0;
        }
        Py_DECREF (cb);
        Py_DECREF (key);
    }
    else
    {
        /* Lookup matching dict. */
        gchar *str = g_strconcat (split[1], ":", split[2], NULL);
        g_strfreev (split);
        
        dict = PyDict_GetItemString (_global_listeners, str);
        if (!dict)
        {
            dict = _add_listener_dict (str);
            if (!dict)
            {
                g_free (str);
                return 0;
            }
        }
        g_free (str);

        /* Add listener. */
        cb = PyCObject_FromVoidPtr (listener, NULL);
        key = PyInt_FromLong ((long) _global_listener_ids + 1);
        if (PyDict_SetItem (dict, key, cb) == -1)
        {
            Py_DECREF (cb);
            Py_DECREF (key);
            return 0;
        }
        Py_DECREF (cb);
        Py_DECREF (key);
    }
    /* Increase id count upon successful addition. */
    _global_listener_ids++;
    return _global_listener_ids;
}

/**
 * Interface for AtkUtilClass->remove_global_event_listener.
 */
static void
_class_remove_global_event_listener (guint listener_id)
{
    PyObject *dict = NULL;
    PyObject *obj = NULL;
    PyObject *key = NULL;
    int pos = 0;

    debug ("_class_remove_global_event_listener\n");

    key = PyInt_FromLong ((long) listener_id);
    while (PyDict_Next (_global_listeners, &pos, NULL, &dict))
    {
        obj = PyDict_GetItem (dict, key);
        if (obj)
        {
            PyDict_DelItem (dict, key);
            Py_DECREF (key);
            return;
        }
    }
    Py_DECREF (key);
    return;
}

/**
 * Returns the static listeners.
 */
PyObject*
atkutil_get_listeners (void)
{
    debug ("atkutil_get_listeners\n");
    return _global_listeners;
}

/**
 * Gets the id for a specific signal name.
 */
guint
atkutil_lookup_signal (const char *name)
{
    PyObject *val;

    debug ("atkutil_lookup_signal\n");

    val = PyDict_GetItemString (_global_signals, name);
    if (!val)
        return 0;
    return (guint) PyInt_AsLong (val);
}

/**
 * Checks, if all class bindings and interfaces are satisfied.
 */
int
atkutil_root_satisfied (void)
{
    /* Currently only the root getter needs to be checked. */
    if (!_get_root)
        return 0;
    return 1;
}

/**
 * Bind the different AtkUtilClass interfaces to their needed
 * implementations.
 */
void
atkutilclass_init (AtkUtilClass *class)
{
    debug ("Hooking AtkUtilClass interfaces.\n");

    class->get_toolkit_name = _class_get_toolkit_name;
    class->get_toolkit_version = _class_get_toolkit_version;
    class->get_root = _class_get_root;
    class->add_global_event_listener = _class_add_global_event_listener;
    class->remove_global_event_listener = _class_remove_global_event_listener;
}

void
atkutil_export_funcs (PyObject *module)
{
    int i = 0;
    PyObject *func;

    static PyMethodDef methods[] = { 
        { "atk_add_focus_tracker", _atkutil_add_focus_tracker, METH_VARARGS,
          "atk_add_focus_tracker (callback) -> int\n\n"
          "Adds a function to be called when an object receives focus.\n\n"
          "Adds the specified function to the list of functions to be called\n"
          "when an object receives focus." },
        { "atk_remove_focus_tracker", _atkutil_remove_focus_tracker,
          METH_VARARGS,
          "atk_remove_focus_tracker (id) -> None\n\n"
          "Removes a focus tracker using its id.\n\n"
          "Removes the specified focus tracker from the list of functions\n"
          "to be called when any object receives focus." },
        { "atk_focus_tracker_init",  _atkutil_focus_tracker_init, METH_VARARGS,
          "atk_focus_tracker_init (callback) -> None\n\n"
          "Sets the function to call upon focus tracker initialization.\n\n"
          "Specifies the function to be called for focus tracker\n"
          "initialization. This function should be called by an\n"
          "implementation of  the ATK interface if any specific work needs\n"
          "to be done to enable focus tracking." },
        { "atk_focus_tracker_notify", _atkutil_focus_tracker_notify,
          METH_VARARGS,
          "atk_focus_tracker_notify (obj) -> None\n\n"
          "Executes the focus tracker function on the passed AtkObject.\n\n"
          "Cause the focus tracker functions which have been specified to\n"
          "be executed for the object." },
        /* TODO
           { "atk_add_global_event_listener", _atkutil_add_global_event_listener,
           METH_VARARGS,
           "Adds the specified function to the list of functions to be called "
           "when an event of type event_type occurs." },
           { "atk_remove_global_event_listener",
           _atkutil_remove_global_event_listener, METH_VARARGS,
           "Removes the specified event listener." },
           { "atk_add_key_event_listener", _atkutil_add_key_event_listener,
           METH_VARARGS,
           "atk_add_key_event_listener (callback)
           "Adds the specified function to the list of functions to be called\n"
           "when a key event occurs. The data element will be passed to the\n"
           "listener as parameter on notification." },
           { "remove_key_event_listener", _atkutil_remove_key_event_listener,
           METH_VARARGS, "Removes the specified event listener." },
        */
        { "atk_get_focus_object", (PyCFunction) _atkutil_get_focus_object,
          METH_NOARGS,
          "atk_get_focus_object () -> AtkObject\n\n"
          "Gets the currently focused object." },
        { "atk_get_root", (PyCFunction) _atkutil_get_root, METH_NOARGS,
          "atk_get_root () -> AtkObject\n\n"
          "Gets the root accessible container for the current application." },
        { "atk_get_toolkit_name", (PyCFunction) _atkutil_get_toolkit_name,
          METH_NOARGS,
          "atk_get_toolkit_name () -> string\n\n"
          "Gets the toolkit name string.\n\n"
          "Gets the name string for the GUI toolkit implementing ATK for\n"
          "this application." },
        { "atk_get_toolkit_version",
          (PyCFunction) _atkutil_get_toolkit_version, METH_NOARGS,
          "atk_get_toolkit_version () -> string\n\n"
          "Gets the toolkit version string.\n\n"
          "Gets version string for the GUI toolkit implementing ATK for this\n"
          "application." },
        /* Nonstandard enhancement. */
        { "set_atk_root", _atkutil_set_root, METH_VARARGS,
          "set_atk_root (callback or object) -> None\n\n"
          "Sets the method, that retrieves the root object."},
        { NULL, NULL, 0, NULL }
    };

    while (methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&methods[i], NULL);
        PyObject_SetAttrString (module, methods[i].ml_name, func);
        i++;
    }
}
