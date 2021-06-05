/* $Id: papi.c,v 1.13.2.5 2008/05/10 08:03:46 marcusva Exp $
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
 * Called upon import. Initializes the underlying GType system and sets
 * up the Atk*Class objects to use the correct interfaces.
 */
static void
_init_module (void)
{
    gpointer data;

    g_type_init ();

    /* Bind the AtkUtilClass interfaces. */
    data = g_type_class_ref (ATK_TYPE_UTIL);
    atkutilclass_init (data);
    g_type_class_unref (data);
    
    /* Bind the AtkObjectClass interfaces. */
    data = g_type_class_ref (ATK_TYPE_OBJECT);
    atkobjectclass_init (data);
    g_type_class_unref (data);
}

/**
 * Python module initialization function. This will be called upon the
 * import <module> statement and sets up the papi module.
 */
PyMODINIT_FUNC
initpapi (void)
{
    PyObject *mod;

    _init_module ();

    /* Complete types. */
    if (PyType_Ready (&PyAtkObject_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkStateSet_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkRelation_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkRelationSet_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkHyperlink_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkAttribute_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkTextRectangle_Type) < 0)
        return;
    if (PyType_Ready (&PyAtkTextRange_Type) < 0)
        return;

    mod = Py_InitModule3 ("papi", NULL,
                          "ATK accessibility module for Python objects.");

    Py_INCREF (&PyAtkObject_Type);
    Py_INCREF (&PyAtkStateSet_Type);
    Py_INCREF (&PyAtkRelation_Type);
    Py_INCREF (&PyAtkRelationSet_Type);
    Py_INCREF (&PyAtkHyperlink_Type);
    Py_INCREF (&PyAtkAttribute_Type);
    Py_INCREF (&PyAtkTextRectangle_Type);
    Py_INCREF (&PyAtkTextRange_Type);

    PyModule_AddObject (mod, "AtkObject", (PyObject *) &PyAtkObject_Type);
    PyModule_AddObject (mod, "AtkStateSet", (PyObject *) &PyAtkStateSet_Type);
    PyModule_AddObject (mod, "AtkRelation", (PyObject *) &PyAtkRelation_Type);
    PyModule_AddObject (mod, "AtkRelationSet",
                        (PyObject *) &PyAtkRelationSet_Type);
    PyModule_AddObject (mod, "AtkHyperlink",
                        (PyObject *) &PyAtkHyperlink_Type);
    PyModule_AddObject (mod, "AtkAttribute",
                        (PyObject *) &PyAtkAttribute_Type);
    PyModule_AddObject (mod, "AtkTextRectangle",
                        (PyObject *) &PyAtkTextRectangle_Type);
    PyModule_AddObject (mod, "AtkTextRange",
                        (PyObject *) &PyAtkTextRange_Type);
    PyModule_AddStringConstant (mod, "__version__", "0.0.5");

    /* Export the 'global' functions. */
    atkbridge_export_funcs (mod);
    atkrole_export_funcs (mod);
    atkrelationtype_export_funcs (mod);
    atkstatetype_export_funcs (mod);
    atktextattribute_export_funcs (mod);
    atkutil_export_funcs (mod);

    /* Export the Atk constants. */
    constants_export (mod);
}
