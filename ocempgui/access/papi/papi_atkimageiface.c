/* $Id: papi_atkimageiface.c,v 1.10.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static void _image_get_image_position (AtkImage *image, gint *x, gint *y,
                                       AtkCoordType coord_type);
static const gchar* _image_get_image_description (AtkImage *image);
static gboolean _image_set_image_description (AtkImage *image,
                                              const gchar *desc);
static void _image_get_image_size (AtkImage *image, gint *width, gint *height);
static const gchar* _image_get_locale (AtkImage *image);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkimageiface_methods[] =
{
    { "image_get_image_position", _not_implemented_args, METH_VARARGS,
      "O.image_get_image_position (coord_type) -> int, int\n\n"
      "Gets the top-left position of the image.\n\n"
      "Gets the position of the image in the form of a point specifying the\n"
      "images top-left corner.\n\n"
      "This method has to be implemented by inherited classes." },
    { "image_get_image_description", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.image_get_image_description () -> string\n\n"
      "Get a textual description of this image.\n\n"
      "This method has to be implemented by inherited classes." },
    { "image_set_image_description", _not_implemented_args, METH_VARARGS,
      "O.image_set_image_description (text) -> bool\n\n"
      "Sets the textual description for this image.\n\n"
      "This method has to be implemented by inherited classes." },
    { "image_get_image_size", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.image_get_image_size () -> int, int\n\n"
      "Gets the width and height of the image.\n\n"
      "Get the width and height in pixels for the specified image. The\n"
      "values of width and height are returned as -1 if the values cannot be\n"
      "obtained.\n\n"
      "This method has to be implemented by inherited classes." },
    { "image_get_locale", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.image_get_locale () -> string\n\n"
      "Gets the locae of the image.\n\n"
      "Gets a UTF-8 string indicating the POSIX-style LC_MESSAGES locale of\n"
      "this image." },
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
 * Wrapper for atk_image_get_image_position().
 */
static void
_image_get_image_position (AtkImage *image, gint *x, gint *y,
                           AtkCoordType coord_type)
{
    PyObject *result;
    PyObject *obj;

    debug ("_image_get_image_position\n");

    obj = g_object_get_data (G_OBJECT (image), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "image_get_image_position", "i",
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
 * Wrapper for atk_image_get_image_description().
 */
static const gchar*
_image_get_image_description (AtkImage *image)
{
    PyObject *result;
    PyObject *obj;
    const gchar* retval = NULL;

    debug ("_image_get_image_description\n");

    obj = g_object_get_data (G_OBJECT (image), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "image_get_image_description", NULL);
    if (!result)
        return 0;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_image_set_image_description().
 */
static gboolean
_image_set_image_description (AtkImage *image, const gchar *desc)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_image_set_image_description\n");

    obj = g_object_get_data (G_OBJECT (image), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "image_set_image_description", "s",
                                  desc);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_image_get_image_size().
 */
static void
_image_get_image_size (AtkImage *image, gint *width, gint *height)
{
    PyObject *result;
    PyObject *obj;

    debug ("_image_get_image_size\n");

    obj = g_object_get_data (G_OBJECT (image), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "image_get_image_size", NULL);
    
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
 * Wrapper for atk_image_get_locale().
 */
static const gchar*
_image_get_locale (AtkImage *image)
{
    PyObject *obj;
    PyObject *result;
    const gchar *retval;

    debug ("_image_get_locale\n");

    obj = g_object_get_data (G_OBJECT (image), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "image_get_locale", NULL);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkImageIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkimageiface_init (AtkImageIface *iface)
{
    iface->get_image_position = _image_get_image_position;
    iface->get_image_description = _image_get_image_description;
    iface->set_image_description = _image_set_image_description;
    iface->get_image_size = _image_get_image_size;
    iface->get_image_locale = _image_get_locale;
}

/**
 * Gets a modified interface for the AtkImageIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkimageiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkimageiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkImage methods to the passed object instance.
 */
void
atkimageiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkimageiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkimageiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkimageiface_methods[i].ml_name, func);
        i++;
    }
}
