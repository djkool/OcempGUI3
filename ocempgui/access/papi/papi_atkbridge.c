/* $Id: papi_atkbridge.c,v 1.8.2.4 2007/09/23 07:37:48 marcusva Exp $
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

#ifndef BRIDGE_MODULE_PATH
#define BRIDGE_MODULE_PATH "/usr/local/lib/gtk-2.0/modules/"
#endif

/**
 * atk-bridge interfaces, which take care of initializing the bridging
 * between Atk and AT-SPI.
 */
#ifndef BRIDGE_INIT
#define BRIDGE_INIT "gnome_accessibility_module_init"
#endif 
#ifndef BRIDGE_STOP
#define BRIDGE_STOP "gnome_accessibility_module_shutdown"
#endif

/** 
 * Static name of the atk-bridge module to use for the atk<->at-spi
 * interaction.
 */
#ifndef ATK_MODULE_NAME
#define ATK_MODULE_NAME "atk-bridge"
#endif

/**
 * Avoid multiple initializations of the bridge.
 */
static int _bridge_initialized = 0;

/**
 * User defined module path.
 */
static char *_bridge_module_path = NULL;

/**
 * Module interfaces from the bridge, so we can initialize and stop it.
 */
static void (*_atk_init) (void);
static void (*_atk_stop) (void);

/**
 * Sets the module path to where the atk-bridging module is located.
 */
static PyObject*
_atkbridge_set_module_path (PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple (args, "s:set_module_path", &_bridge_module_path))
        return NULL;
    Py_RETURN_NONE;
}

/**
 * Loads the atk-bridge system, that hopefully exists as module using
 * the GModule system and prepares the initialization/shutdown hooks.
 */
static PyObject*
_atkbridge_init (void)
{
    GModule *module;
    gchar *path = NULL;

    if (_bridge_initialized)
        Py_RETURN_NONE;

    /* Check, if all pointers are satisfied. */
    if (!atkutil_root_satisfied ())
    {
        PyErr_SetString
            (PyExc_NameError, "Interface get_root not satisfied.");
        return NULL;
    }

    /* Try to build the correct module path and load it. */
    if (_bridge_module_path)
        path = g_module_build_path (_bridge_module_path, ATK_MODULE_NAME);
    else
        path = g_module_build_path (BRIDGE_MODULE_PATH, ATK_MODULE_NAME);

    module = g_module_open (path, G_MODULE_BIND_LAZY);
    g_free (path);
    if (!module)
    {
        PyErr_SetString (PyExc_IOError,
                         "Failed to load the atk-bridge module.");
        return NULL;
    }
    
    if (!g_module_symbol (module, BRIDGE_INIT, (gpointer *) &_atk_init) ||
        !g_module_symbol (module, BRIDGE_STOP, (gpointer *) &_atk_stop))
    {
        PyErr_SetString (PyExc_IOError, "Needed functions " BRIDGE_INIT
                         " or " BRIDGE_STOP " not found in module.");
        return NULL;
    }

    _atk_init ();

    _bridge_initialized = 1;
    Py_RETURN_NONE;
}

/**
 * Stops the atk bridge system.
 */
static PyObject*
_atkbridge_stop (void)
{
    if (_bridge_initialized)
    {
        _bridge_initialized = 0;
        _atk_stop ();
    }
    Py_RETURN_NONE;
}

/**
 * Iterates the main context in a non-blocking way, so that the AT-SPI bridge
 * can interact with the ATK bindings.
 */
static PyObject*
_atkbridge_iterate (void)
{
    g_main_context_iteration (g_main_context_default (), FALSE);
    Py_RETURN_NONE;
}

void
atkbridge_export_funcs (PyObject *module)
{
    int i = 0;
    PyObject *func;

    static PyMethodDef methods[] = { 
        { "set_module_path", _atkbridge_set_module_path, METH_VARARGS,
          "Sets the path of the atk-bridge module." },
        { "init", (PyCFunction) _atkbridge_init, METH_NOARGS,
          "Initializes and starts the atk-bridge." },
        { "shutdown", (PyCFunction) _atkbridge_stop, METH_NOARGS,
          "Stops the atk-bridge." },
        { "iterate", (PyCFunction) _atkbridge_iterate, METH_NOARGS,
          "Runs a single iteration on the ATK backends." },
        { NULL, NULL, 0, NULL }
    };

    while (methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&methods[i], NULL);
        PyObject_SetAttrString (module, methods[i].ml_name, func);
        i++;
    }
}
