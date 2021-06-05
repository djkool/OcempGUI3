/* $Id: papi_atkhypertextiface.c,v 1.10.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static AtkHyperlink* _hypertext_get_link (AtkHypertext *hypertext,
                                          gint link_index);
static gint _hypertext_get_n_links (AtkHypertext *hypertext);
static gint _hypertext_get_link_index (AtkHypertext *hypertext,
                                       gint char_index);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkhypertextiface_methods[] =
{
    { "hypertext_get_link", _not_implemented_args, METH_VARARGS,
      "O.hypertext_get_link (index) -> AtkHyperlink\n\n"
      "Gets the link in this hypertext document at the specified index.\n\n"
      "This method has to be implemented by inherited classes." },
    { "hypertext_get_n_links", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.hypertext_get_n_links () -> int\n\n"
      "Gets the number of links within this hypertext document.\n\n"
      "This method has to be implemented by inherited classes." },
    { "hypertext_get_link_index", _not_implemented_args, METH_VARARGS,
      "O.hypertext_get_link_index (index) -> int\n\n"
      "Gets the index of a hyperlink for the specifc character index.\n\n"
      "Gets the index into the array of hyperlinks that is associated with\n"
      "the character specified by index.\n\n"
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
 * Wrapper for atk_hypertext_get_link().
 */
static AtkHyperlink*
_hypertext_get_link (AtkHypertext *hypertext, gint link_index)
{
    PyObject *result;
    PyObject *obj;
    AtkHyperlink *retval = NULL;

    debug ("_hypertext_get_link\n");

    obj = g_object_get_data (G_OBJECT (hypertext), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "hypertext_get_link", "i", link_index);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkHyperlink_Type))
        retval = ATKHYPERLINK (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_hypertext_get_n_links().
 */
static gint
_hypertext_get_n_links (AtkHypertext *hypertext)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_hypertext_get_n_links\n");

    obj = g_object_get_data (G_OBJECT (hypertext), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "hypertext_get_n_links", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_hypertext_get_link_index().
 */
static gint
_hypertext_get_link_index (AtkHypertext *hypertext, gint char_index)
{
    PyObject *result;
    PyObject *obj;
    gint retval = -1;

    debug ("_hypertext_get_link_index\n");

    obj = g_object_get_data (G_OBJECT (hypertext), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "hypertext_get_link_index", "i",
                                  char_index);
    if (!result)
        return -1;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkHypertextIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkhypertextiface_init (AtkHypertextIface *iface)
{
    iface->get_link = _hypertext_get_link;
    iface->get_n_links = _hypertext_get_n_links;
    iface->get_link_index = _hypertext_get_link_index;
}

/**
 * Gets a modified interface for the AtkHypertextIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkhypertextiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkhypertextiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkHypertext methods to the passed object instance.
 */
void
atkhypertextiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkhypertextiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkhypertextiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkhypertextiface_methods[i].ml_name,
                                func);
        i++;
    }
}
