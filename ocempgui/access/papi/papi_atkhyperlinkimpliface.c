/* $Id: papi_atkhyperlinkimpliface.c,v 1.1.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static AtkHyperlink*
_hyperlinkimpl_get_hyperlink (AtkHyperlinkImpl *hyperlink);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkhyperlinkimpliface_methods[] =
{
    { "hyperlinkimpl_get_hyperlink", _not_implemented_args, METH_VARARGS,
      "O.hyperlink_get_hyperlink (index) -> AtkHyperlink\n\n"
      "Gets the hyperlink associated with this object.\n\n"
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
 * Wrapper for atk_hyperlinkimpl_get_hyperlink().
 */
static AtkHyperlink*
_hyperlinkimpl_get_hyperlink (AtkHyperlinkImpl *hyperlink)
{
    PyObject *result;
    PyObject *obj;
    AtkHyperlink *retval = NULL;

    debug ("_hyperlinkimpl_get_hyperlink\n");

    obj = g_object_get_data (G_OBJECT (hyperlink), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "hyperlinkimpl_get_hyperlink", NULL);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkHyperlink_Type))
        retval = ATKHYPERLINK (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkHyperlinkImplIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkhyperlinkimpliface_init (AtkHyperlinkImplIface *iface)
{
    iface->get_hyperlink = _hyperlinkimpl_get_hyperlink;
}

/**
 * Gets a modified interface for the AtkHyperlinkImplIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atkhyperlinkimpliface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkhyperlinkimpliface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkHyperlinkImpl methods to the passed object instance.
 */
void
atkhyperlinkimpliface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkhyperlinkimpliface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkhyperlinkimpliface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkhyperlinkimpliface_methods[i].ml_name,
                                func);
        i++;
    }
}
