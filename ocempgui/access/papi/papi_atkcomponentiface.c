/* $Id: papi_atkcomponentiface.c,v 1.11.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static guint _component_add_focus_handler (AtkComponent *component,
                                           AtkFocusHandler handler);
static void _component_remove_focus_handler (AtkComponent *component,
                                             guint handler_id);
static gboolean _component_contains (AtkComponent *component, gint x, gint y,
                                     AtkCoordType coord_type);
static AtkObject* _component_ref_accessible_at_point (AtkComponent *component,
                                                      gint x, gint y,
                                                      AtkCoordType coord_type);
static void _component_get_extents (AtkComponent *component, gint *x, gint *y,
                                    gint *width, gint *height,
                                    AtkCoordType coord_type);
static void _component_get_position (AtkComponent *component, gint *x, gint *y,
                                     AtkCoordType coord_type);
static AtkLayer _component_get_layer (AtkComponent *component);
static void _component_get_size (AtkComponent *component, gint *width,
                                 gint *height);
static gboolean _component_grab_focus (AtkComponent *component);
static gboolean _component_set_extents (AtkComponent *component, gint x,
                                        gint y, gint width, gint height,
                                        AtkCoordType coord_type);
static gboolean _component_set_position (AtkComponent *component, gint x,
                                         gint y, AtkCoordType coord_type);
static gboolean _component_set_size (AtkComponent *component, gint width,
                                     gint height);
static gint _component_get_mdi_zorder (AtkComponent *component);
static gdouble _component_get_alpha (AtkComponent *component);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkcomponentiface_methods[] = 
{
    { "component_add_focus_handler", _not_implemented_args, METH_VARARGS,
      "O.component_add_focus_handler (handler) -> int\n\n"
      "Adds a specific handler to be executed on focusing the object.\n\n"
      "Add the specified handler to the set of functions to be called when\n"
      "this object receives focus events (in or out). If the handler is\n"
      "already added it is not added again.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_remove_focus_handler", _not_implemented_args, METH_VARARGS,
      "O.component_remove_focus_handler (handler_id) -> None\n\n"
      "Removes a handler from the execution list.\n\n"
      "Remove the handler specified by handler_id from the list of functions\n"
      "to be executed when this object receives focus events (in or out).\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_contains", _not_implemented_args, METH_VARARGS,
      "O.component_contains (x, y, coord_type) -> bool\n\n"
      "Checks whether a specific point is within the extent of the component."
      "\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_extents", _not_implemented_args, METH_VARARGS,
      "O.component_get_extents (coord_type) -> (x, y, width, height)\n\n"
      "Gets the rectangle which gives the extent of the component.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_set_extents", _not_implemented_args, METH_VARARGS,
      "O.component_set_extents (x, y, width, height, coord_type) -> bool\n\n"
      "Sets the extents of component.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_layer", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.component_get_layer () -> int\n\n"
      "Gets the layer of the component.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_mdi_zorder", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.component_get_mdi_zorder () -> int\n\n"
      "Gets the zorder of the component.\n\n"
      "The value None will be returned if the layer of the component is not\n"
      "ATK_LAYER_MDI or ATK_LAYER_WINDOW.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_position", _not_implemented_args, METH_VARARGS,
      "O.component_get_position (coord_type) -> int, int\n\n"
      "Gets the position of the top-left corner of the component.\n\n"
      "Gets the position of component in the form of a point specifying\n"
      "component's top-left corner.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_set_position", _not_implemented_args, METH_VARARGS,
      "O.component_set_position (x, y, coord_type) -> bool\n\n"
      "Sets the position of component.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_size", (PyCFunction) _not_implemented_args, METH_NOARGS,
      "O.component_get_size () -> int, int\n\n"
      "Gets the size of the component in terms of width and height.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_set_size", _not_implemented_args, METH_VARARGS,
      "O.component_set_size (width, height) -> bool\n\n"
      "Sets the size of the component in terms of width and height.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_grab_focus", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.component_grab_focus () -> bool\n\n"
      "Grabs the focus for the component.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_ref_accessible_at_point", _not_implemented_args, METH_VARARGS,
      "O.component_ref_accessible_at_point (x, y, coord_type) -> AtkObject\n\n"
      "Gets a reference to the child at the specified point\n\n"
      "Gets a reference to the accessible child, if one exists, at the "
      "coordinate point specified by x and y.\n\n"
      "This method has to be implemented by inherited classes." },
    { "component_get_alpha", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.component_get_alpha () -> float\n\n"
      "Returns the alpha value for this component.\n\n"
      "Returns the alpha value (i.e. the opacity) for this component, on a\n"
      "scale from 0 (fully transparent) to 1.0 (fully opaque)." },
    
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
 * Wrapper for atk_component_add_focus_handler().
 */
static guint
_component_add_focus_handler (AtkComponent *component,
                              AtkFocusHandler handler)
{
    PyObject *result;
    PyObject *obj;
    guint retval = 0;
    
    debug ("_component_add_focus_handler\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_add_focus_handler", "O",
                                  PyCObject_FromVoidPtr (handler, NULL));
    if (!result)
        return 0;
    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_remove_focus_handler().
 */
static void
_component_remove_focus_handler (AtkComponent *component, guint handler_id)
{
    PyObject *result;
    PyObject *obj;
    
    debug ("_component_remove_focus_handler\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_remove_focus_handler", "i",
                                  handler_id);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_component_contains().
 */
static gboolean
_component_contains (AtkComponent *component, gint x, gint y,
                     AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_component_contains\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_contains", "iii", x, y,
                                  coord_type);
    if (!result)
        return FALSE;

    retval = (result == Py_True);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_ref_accessible_at_point().
 */
static AtkObject*
_component_ref_accessible_at_point (AtkComponent *component, gint x, gint y,
                                    AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;
    AtkObject *retval = NULL;

    debug ("_component_ref_accesible_at_point\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_ref_accessible_at_point",
                                  "iii", x, y, coord_type);
    if (!result)
        return NULL;

    retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_get_extents().
 */
static void
_component_get_extents (AtkComponent *component, gint *x, gint *y, gint *width,
                        gint *height, AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;

    debug ("_component_get_extents\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_extents", "i",
                                  coord_type);
    *x = 0;
    *y = 0;
    *width = 0;
    *height = 0;

    if (result)
    {
        if (PyList_Check (result))
        {
            *x = PyInt_AsLong (PyList_GetItem (result, 0));
            *y = PyInt_AsLong (PyList_GetItem (result, 1));
            *width = PyInt_AsLong (PyList_GetItem (result, 2));
            *height = PyInt_AsLong (PyList_GetItem (result, 3));
        }
        else if (PyTuple_Check (result))
        {
            *x = PyInt_AsLong (PyTuple_GetItem (result, 0));
            *y = PyInt_AsLong (PyTuple_GetItem (result, 1));
            *width = PyInt_AsLong (PyTuple_GetItem (result, 2));
            *height = PyInt_AsLong (PyTuple_GetItem (result, 3));
        }
        Py_DECREF (result);
    }
}

/**
 * Wrapper for atk_component_get_position().
 */
static void
_component_get_position (AtkComponent *component, gint *x, gint *y,
                         AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;

    debug ("_component_get_position\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_position", "i",
                                  coord_type);

    *x = 0;
    *y = 0;

    if (result)
    {
        if (PyList_Check (result))
        {
            *x = PyInt_AsLong (PyList_GetItem (result, 0));
            *y = PyInt_AsLong (PyList_GetItem (result, 1));
        }
        else if (PyTuple_Check (result))
        {
            *x = PyInt_AsLong (PyTuple_GetItem (result, 0));
            *y = PyInt_AsLong (PyTuple_GetItem (result, 1));
        }
        Py_DECREF (result);
    }
}

/**
 * Wrapper for atk_component_get_layer().
 */
static AtkLayer
_component_get_layer (AtkComponent *component)
{
    PyObject *result;
    PyObject *obj;
    AtkLayer retval;

    debug ("_component_get_layer\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_layer", NULL);
    if (!result)
        retval = ATK_LAYER_WIDGET;
    else
    {
        retval = PyInt_AsLong (result);
        Py_DECREF (result);
    }
    return retval;
}

/**
 * Wrapper for atk_component_get_size().
 */
static void
_component_get_size (AtkComponent *component, gint *width, gint *height)
{
    PyObject *result;
    PyObject *obj;

    debug ("_component_get_size\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_size", NULL);

    *width = 0;
    *height = 0;

    if (result)
    {
        if (PyList_Check (result))
        {
            *width = PyInt_AsLong (PyList_GetItem (result, 0));
            *height = PyInt_AsLong (PyList_GetItem (result, 1));
        }
        else if (PyTuple_Check (result))
        {
            *width = PyInt_AsLong (PyTuple_GetItem (result, 0));
            *height = PyInt_AsLong (PyTuple_GetItem (result, 1));
        }
        Py_DECREF (result);
    }
}

/**
 * Wrapper for atk_component_grab_focus().
 */
static gboolean
_component_grab_focus (AtkComponent *component)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_component_grab_focus\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_grab_focus", NULL);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_set_extents().
 */
static gboolean
_component_set_extents (AtkComponent *component, gint x, gint y, gint width,
                        gint height, AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_component_set_extents\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_set_extents", "iiiii",
                                  x, y, width, height, coord_type);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_set_position().
 */
static gboolean
_component_set_position (AtkComponent *component, gint x, gint y,
                         AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_component_set_position\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_set_position", "iii", x, y,
                                  coord_type);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_set_size().
 */
static gboolean
_component_set_size (AtkComponent *component, gint width, gint height)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_component_set_size\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_set_size", "ii", width,
                                  height);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_component_get_mdi_zorder().
 */
static gint
_component_get_mdi_zorder (AtkComponent *component)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_component_get_mdi_zorder\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_mdi_zorder", NULL);
    if (!result)
        return FALSE;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}   

/**
 * Wrapper for atk_component_get_alpha().
 */
static gdouble
_component_get_alpha (AtkComponent *component)
{
    PyObject *result;
    PyObject *obj;
    gdouble retval = 0;

    debug ("_component_get_alpha\n");

    obj = g_object_get_data (G_OBJECT (component), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "component_get_alpha", NULL);
    if (!result)
        return FALSE;

    retval = PyFloat_AsDouble (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkComponentIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkcomponentiface_init (AtkComponentIface *iface)
{
    iface->add_focus_handler = _component_add_focus_handler;
    iface->remove_focus_handler = _component_remove_focus_handler;
    iface->contains = _component_contains;
    iface->ref_accessible_at_point = _component_ref_accessible_at_point;
    iface->get_extents = _component_get_extents;
    iface->get_position = _component_get_position;
    iface->get_layer = _component_get_layer;
    iface->get_size = _component_get_size;
    iface->grab_focus = _component_grab_focus;
    iface->set_extents = _component_set_extents;
    iface->set_position = _component_set_position;
    iface->set_size = _component_set_size;
    iface->get_mdi_zorder = _component_get_mdi_zorder;
    iface->get_alpha = _component_get_alpha;
}

/**
 * Gets a modified interface for the AtkComponentIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkcomponentiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkcomponentiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkComponent methods to the passed object instance.
 */
void
atkcomponentiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkcomponentiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkcomponentiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkcomponentiface_methods[i].ml_name,
                                func);
        i++;
    }
}
