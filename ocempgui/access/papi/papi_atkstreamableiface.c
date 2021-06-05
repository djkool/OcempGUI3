/* $Id: papi_atkstreamableiface.c,v 1.11.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static gint _streamable_get_n_mime_types (AtkStreamableContent *streamable);
static const gchar*
_streamable_get_mime_type (AtkStreamableContent *streamable, gint i);
static GIOChannel* _streamable_get_stream (AtkStreamableContent *streamable,
                                           const gchar *mime_type);
static const gchar* _streamable_get_uri  (AtkStreamableContent *streamable,
                                          const gchar *mime_type);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkstreamableiface_methods[] =
{
    { "streamable_get_n_mime_types", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.streamable_get_n_mime_types () -> int\n\n"
      "Gets the number of mime types supported by this object.\n\n"
      "This method has to be implemented by inherited classes." },
    { "streamable_get_mime_type", _not_implemented_args, METH_VARARGS,
      "O.streamable_get_mime_type (type) -> string\n\n"
      "Gets the character string of the specified mime type.\n\n"
      "The first mime type is at position 0, the second at position 1,\n"
      "and so on.\n\n"
      "This method has to be implemented by inherited classes." },
    { "streamable_get_stream", _not_implemented_args, METH_VARARGS,
      "O.streamable_get_stream (type_name) -> object\n\n"
      "Gets the content in the specified mime type.\n\n"
      "This method has to be implemented by inherited classes." },
    { "streamable_get_uri", _not_implemented_args, METH_VARARGS,
      "O.streamable_get_uri (type_name) -> string\n\n"
      "Gets the URI from which the object's content may be streamed.\n\n"
      "Get a string representing a URI in IETF standard format (see\n"
      "http://www.ietf.org/rfc/rfc2396.txt) from which the object's content\n"
      "may be streamed in the specified mime-type, if one is available. If\n"
      "mime_type is None, the URI for the default (and possibly only)\n"
      "mime-type is returned.\n\n"
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
 * Wrapper for atk_streamable_content_get_n_mime_types().
 */
static gint
_streamable_get_n_mime_types (AtkStreamableContent *streamable)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_streamable_get_n_mime_types\n");

    obj = g_object_get_data (G_OBJECT (streamable), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "streamable_get_n_mime_types", NULL);
    if (!result)
        return FALSE;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_streamable_content_get_mime_type().
 */
static const gchar*
_streamable_get_mime_type (AtkStreamableContent *streamable, gint i)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval = NULL;

    debug ("_streamable_get_mime_type\n");

    obj = g_object_get_data (G_OBJECT (streamable), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "streamable_get_mime_type", "i", i);
    if (!result)
        return FALSE;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_streamable_content_get_stream().
 */
static GIOChannel*
_streamable_get_stream (AtkStreamableContent *streamable,
                        const gchar *mime_type)
{
    PyObject *result;
    PyObject *obj;
    GIOChannel *retval = NULL;

    debug ("_streamable_get_stream\n");

    obj = g_object_get_data (G_OBJECT (streamable), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "streamable_get_stream", "s",
                                  mime_type);
    if (!result)
        return NULL;

    /* Create a GIOChannel. */
    if (PyInt_Check (result))
    {
        /* File descriptor value, try to create a channel directly from it. */
#ifdef IS_WIN32
        retval = g_io_channel_win32_new_fd (PyInt_AsLong (result));
#else
        /* Try the unix way... */
        retval = g_io_channel_unix_new (PyInt_AsLong (result));
#endif
    }
    else if (PyString_Check (result))
    {
        /* A filename or something like that. */
        const gchar *val = PyString_AsString (result);
        GError *error;
        if (val)
        {
            retval = g_io_channel_new_file (val, "r", &error);
            if (!retval)
            {
                PyErr_SetString (PyExc_IOError, error->message);
                g_error_free (error);
            }
        }
    }
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_streamable_content_get_uri(). 
 */
static const gchar*
_streamable_get_uri (AtkStreamableContent *streamable, const gchar *mime_type)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval = NULL;

    debug ("_streamable_content_get_uri\n");

    obj = g_object_get_data (G_OBJECT (streamable), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "streamable_get_uri", "s", mime_type);
    if (!result)
        return FALSE;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;

}
/**
 * Bind the different AtkStreamableContentIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkstreamableiface_init (AtkStreamableContentIface *iface)
{
    iface->get_n_mime_types = _streamable_get_n_mime_types;
    iface->get_mime_type = _streamable_get_mime_type;
    iface->get_stream = _streamable_get_stream;
    iface->get_uri = _streamable_get_uri;
}

/**
 * Gets a modified interface for the AtkStreamableContentIface, which
 * uses the python wrappers as interfaces.
 */
const GInterfaceInfo*
atkstreamableiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkstreamableiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkStreamableContent methods to the passed object instance.
 */
void
atkstreamableiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkstreamableiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkstreamableiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkstreamableiface_methods[i].ml_name,
                                func);
        i++;
    }
}
