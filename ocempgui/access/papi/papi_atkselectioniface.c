/* $Id: papi_atkselectioniface.c,v 1.10.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static gboolean _selection_add_selection (AtkSelection *selection, gint i);
static gboolean _selection_clear_selection (AtkSelection *selection);
static AtkObject* _selection_ref_selection (AtkSelection *selection, gint i);
static gint _selection_get_selection_count (AtkSelection *selection);
static gboolean _selection_is_child_selected (AtkSelection *selection, gint i);
static gboolean _selection_remove_selection (AtkSelection *selection, gint i);
static gboolean _selection_select_all_selection (AtkSelection *selection);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkselectioniface_methods[] =
{
    { "selection_add_selection", _not_implemented_args, METH_VARARGS,
      "O.selecion_add_selection (index) -> bool\n\n"
      "Adds the accessible child to the selection.\n\n"
      "Adds the specified accessible child of the object to the object's\n"
      "selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_clear_selection", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.selection_clear_selection () -> bool\n\n"
      "Clears the selection.\n\n"
      "Clears the selection in the object so that no children in the object\n"
      "are selected.\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_ref_selection", _not_implemented_args, METH_VARARGS,
      "O.selection_ref_selection (index) -> AtkObject\n\n"
      "Gets a references to the AtkObject at index.\n\n"
      "Gets a reference to the accessible object representing the specified\n"
      "selected child of the object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_get_selection_count", _not_implemented_args, METH_VARARGS,
      "O.selection_get_selection_count () -> int\n\n"
      "Gets the number of accessible children currently selected.\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_is_child_selected", _not_implemented_args, METH_VARARGS,
      "O.selection_is_child_selected (index) -> bool\n\n"
      "Determines if the current child of this object is selected.\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_remove_selection", _not_implemented_args, METH_VARARGS,
      "O.selection_remove_selection (index) -> bool\n\n"
      "Removes the specified child of the object from the object's selection."
      "\n\n"
      "This method has to be implemented by inherited classes." },
    { "selection_select_all_selection", _not_implemented_args, METH_VARARGS,
      "O.selection_select_all_selection () -> bool\n\n"
      "Selects all children of the object.\n\n"
      "Causes every child of the object to be selected if the object\n"
      "supports multiple selections.\n\n"
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
 * Wrapper for atk_selection_add_selection().
 */
static gboolean
_selection_add_selection (AtkSelection *selection, gint i)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_selection_add_selection\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_add_selection", "i", i);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_clear_selection().
 */
static gboolean
_selection_clear_selection (AtkSelection *selection)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_selection_clear_selection\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_clear_selection", NULL);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_ref_selection().
 */
static AtkObject*
_selection_ref_selection (AtkSelection *selection, gint i)
{
    PyObject *result;
    PyObject *obj;
    AtkObject *retval = NULL;

    debug ("_selection_ref_selection\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_ref_selection", "i", i);
    if (!result)
        return NULL;
    
    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_get_selection_count().
 */
static gint
_selection_get_selection_count (AtkSelection *selection)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_selection_get_selection_count\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_get_selection_count", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_is_child_selected().
 */
static gboolean
_selection_is_child_selected (AtkSelection *selection, gint i)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_selection_is_child_selected\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_is_child_selected", "i", i);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_remove_selection().
 */
static gboolean
_selection_remove_selection (AtkSelection *selection, gint i)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_selection_remove_selection\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_remove_selection", "i", i);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_selection_select_all_selection().
 */
static gboolean
_selection_select_all_selection (AtkSelection *selection)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_selection_select_all_selection\n");

    obj = g_object_get_data (G_OBJECT (selection), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "selection_select_all_selection", NULL);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkSelectionIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkselectioniface_init (AtkSelectionIface *iface)
{
    iface->add_selection = _selection_add_selection;
    iface->clear_selection = _selection_clear_selection;
    iface->ref_selection = _selection_ref_selection;
    iface->get_selection_count = _selection_get_selection_count;
    iface->is_child_selected = _selection_is_child_selected;
    iface->remove_selection = _selection_remove_selection;
    iface->select_all_selection = _selection_select_all_selection;
}

/**
 * Gets a modified interface for the AtkSelectionIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkselectioniface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkselectioniface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkSelection methods to the passed object instance.
 */
void
atkselectioniface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkselectioniface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkselectioniface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkselectioniface_methods[i].ml_name,
                                func);
        i++;
    }
}
