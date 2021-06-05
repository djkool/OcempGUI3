/* $Id: papi_atkdocumentiface.c,v 1.10.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static PyObject* _not_implemented_noargs (PyObject *self);
static PyObject* _not_implemented_args (PyObject *self, PyObject *args);
static const gchar* _document_get_document_type (AtkDocument *document);
static gpointer _document_get_document (AtkDocument *document);
static AtkAttributeSet* _document_get_attributes (AtkDocument *document);
static const gchar* _document_get_attribute_value (AtkDocument *document,
                                                   const gchar *name);
static gboolean _document_set_attribute_value (AtkDocument *document,
                                               const gchar *name,
                                               const gchar *value);
static const gchar* _document_get_locale (AtkDocument *document);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkdocumentiface_methods[] =
{
    { "document_get_document_type", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.document_get_document_type () -> string\n\n"
      "Gets a string indicating the document type.\n\n"
      "This method has to be implemented by inherited classes." },
    { "document_get_document", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.document_get_document () -> object\n\n"
      "Gets a reference that points to an instance of the DOM.\n\n"
      "It is up to the caller to check get_document_type to determine how to "
      "cast this reference.\n\n"
      "This method has to be implemented by inherited classes." },
    { "document_get_locale", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.document_get_locale () -> string\n\n"
      "Gets the locale of the document.\n\n"
      "Gets a UTF-8 string indicating the POSIX-style LC_MESSAGES locale of\n"
      "the content of this document instance. Individual text substrings or\n"
      "images within this document may have a different locale, see\n"
      "the text_get_attributes and image_get_image_locale interfaces." },
    { "document_get_attribute_value", _not_implemented_args, METH_VARARGS,
      "O.document_get_attribute_value (name) -> string\n\n"
      "Gets the attribute value for the specified name.\n\n"
      "Gets the attribute value associated with the specified name for the\n"
      "document or None, if a value for name has not be specified"},
    { "document_set_attribute_value", _not_implemented_args, METH_VARARGS,
      "O.document_set_attribute_value (name, value) -> bool\n\n"
      "Sets a specific attribute value for the document.\n\n"
      "Sets a specific attribute value for name and returns True on\n"
      "success, False otherwise (e.g. the attribute does not allow to be\n"
      "modified)."},
    { "document_get_attributes", _not_implemented_args, METH_VARARGS,
      "O.document_get_attributes (name) -> list\n\n"
      "Gets a list of the document-wide attributes.\n\n"
      "Gets a list which describes document-wide attributes as name-value\n"
      "pairs." },

    { NULL, NULL, 0, NULL }
};

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
 * Default implementation for the interfaces.
 */
static PyObject*
_not_implemented_args (PyObject *self, PyObject *args)
{
    PyErr_SetString (PyExc_NotImplementedError, "method not implemented");
    return NULL;
}

/**
 * Wrapper for atk_document_get_document_type().
 */
static const gchar*
_document_get_document_type (AtkDocument *document)
{
    PyObject *result;
    PyObject *obj;
    const gchar *retval;

    debug ("_document_get_document_type\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_get_document_type", NULL);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_document_get_document().
 */
static gpointer
_document_get_document (AtkDocument *document)
{
    PyObject *result;
    PyObject *obj;
    gpointer retval;

    debug ("_document_get_document\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_get_document", NULL);
    if (!result)
        return NULL;

    retval = PyCObject_AsVoidPtr (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_document_get_attributes().
 */
static AtkAttributeSet*
_document_get_attributes (AtkDocument *document)
{
    PyObject *obj;
    PyObject *result;
    PyObject *attr;
    int amount = 0;
    AtkAttribute *at;
    AtkAttributeSet *retval = NULL;

    debug ("_document_get_attributes\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_get_attributes", NULL);

    if (result)
    {
        if (PySequence_Check (result))
        {
            amount = PySequence_Size (result);

            /* Walk through the list. */
            for (int i = 0; i < amount; i++)
            {
                attr = PySequence_GetItem (result, i);
                if (!PyObject_TypeCheck (attr, &PyAtkAttribute_Type))
                {
                    atk_attribute_set_free (retval);
                    Py_DECREF (attr);
                    return NULL;
                }

                /* Clone AtkAttribute. */
                at = g_new (AtkAttribute, 1);
                at->name = g_strdup (((PyAtkAttribute *) attr)->obj->name);
                at->value = g_strdup (((PyAtkAttribute *) attr)->obj->value);
                retval = g_slist_append (retval, at);
                Py_DECREF (attr);
            }
        }
        Py_DECREF (result);
    }
    return retval;
}

/**
 * Wrapper for atk_document_get_attribute_value().
 */
static const gchar*
_document_get_attribute_value (AtkDocument *document, const gchar *name)
{
    PyObject *obj;
    PyObject *result;
    const gchar *retval;

    debug ("_document_get_attribute_value\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_get_attribute_value", "s",
                                  name);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_document_set_attribute_value().
 */
static gboolean
_document_set_attribute_value (AtkDocument *document, const gchar *name,
                               const gchar *value)
{
    PyObject *obj;
    PyObject *result;
    gboolean retval = FALSE;

    debug ("_document_set_attribute_value\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_set_attribute_value", "ss",
                                  name, value);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_document_get_locale().
 */
static const gchar*
_document_get_locale (AtkDocument *document)
{
    PyObject *obj;
    PyObject *result;
    const gchar *retval;

    debug ("_document_get_locale\n");

    obj = g_object_get_data (G_OBJECT (document), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "document_get_locale", NULL);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkDocumentIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkdocumentiface_init (AtkDocumentIface *iface)
{
    iface->get_document_type = _document_get_document_type;
    iface->get_document = _document_get_document;
    iface->get_document_attributes = _document_get_attributes;
    iface->set_document_attribute = _document_set_attribute_value;
    iface->get_document_attribute_value = _document_get_attribute_value;
    iface->get_document_locale = _document_get_locale;
}

/**
 * Gets a modified interface for the AtkDocumentIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkdocumentiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkdocumentiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkDocument methods to the passed object instance.
 */
void
atkdocumentiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkdocumentiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkdocumentiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkdocumentiface_methods[i].ml_name,
                                func);
        i++;
    }
}
