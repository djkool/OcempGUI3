/* $Id: papi_atkhyperlink.c,v 1.5.2.5 2007/09/23 07:37:48 marcusva Exp $
 *
 * Copyright (c) 2006-2007, Marcus von Appen
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
static PyObject* _atkhyperlink_new (PyTypeObject *type, PyObject *args,
                                    PyObject *kwds);
static int _atkhyperlink_init (PyAtkHyperlink *self, PyObject *args,
                               PyObject *kwds);
static void _atkhyperlink_dealloc (PyAtkHyperlink *self);
static PyObject *_atkhyperlink_get_uri (PyAtkHyperlink *self, PyObject *args);
static PyObject *_atkhyperlink_get_object (PyAtkHyperlink *self,
                                           PyObject *args);
static PyObject *_atkhyperlink_get_end_index (PyAtkHyperlink *self);
static PyObject *_atkhyperlink_get_start_index (PyAtkHyperlink *self);
static PyObject *_atkhyperlink_get_n_anchors (PyAtkHyperlink *self);

static PyObject *_atkhyperlink_is_valid (PyAtkHyperlink *self, void *closure);
static PyObject *_atkhyperlink_is_inline (PyAtkHyperlink *self, void *closure);
static PyObject *_atkhyperlink_is_selected_link (PyAtkHyperlink *self,
                                                 void *closure);

/**
 * Methods, which are bound to the PyAtkHyperlink type.
 */
static PyMethodDef _atkhyperlink_methods[] =
{
    { "get_uri", (PyCFunction) _atkhyperlink_get_uri, METH_VARARGS,
      "H.get_uri (index) -> string\n\n"
      "Get the URI associated with the anchor specified by index of link.\n\n"
      "Multiple anchors are primarily used by client-side image maps.\n\n"
      "This method has to be implemented by inherited classes." },
    { "get_object", (PyCFunction) _atkhyperlink_get_object, METH_VARARGS,
      "H.get_object (index) -> AtkObject\n\n"
      "Returns the item associated with this hyperlinks nth anchor.\n\n"
      "For instance, the returned AtkObject will implement AtkText if link\n"
      "is a  text hyperlink, AtkImage if link_ is an image hyperlink etc.\n"
      "Multiple anchors are primarily used by client-side image maps.\n\n"
      "This method has to be implemented by inherited classes." },
    { "get_end_index", (PyCFunction) _atkhyperlink_get_end_index, METH_NOARGS,
      "H.get_end_index () -> int\n\n"
      "Gets the index with the hypertext document at which this link ends.\n\n"
      "This method has to be implemented by inherited classes." },
    { "get_start_index", (PyCFunction) _atkhyperlink_get_start_index,
      METH_NOARGS,
      "H.get_start_index () -> int\n\n"
      "Gets the index with the hypertext document at which this link "
      "begins.\n\n"
      "This method has to be implemented by inherited classes." },
      { "get_n_anchors", (PyCFunction) _atkhyperlink_get_n_anchors, METH_NOARGS,
      "H.get_n_anchors () -> int\n\n"
      "Gets the number of anchors associated with this hyperlink.\n\n"
      "This method has to be implemented by inherited classes." },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the AtkHyperlink.
 */
static PyGetSetDef _atkhyperlink_getsets[] =
{
    { "is_valid", (getter) _atkhyperlink_is_valid, NULL,
      "H.is_valid () -> bool\n\n"
      "Checks, whether the link is still valid.\n\n"
      "Since the document that a link is associated with may have changed\n"
      "this method returns True if the link is still valid (with respect\n"
      "to the document it references) and False otherwise.", NULL },
    { "is_inline", (getter) _atkhyperlink_is_inline, NULL,
      "H.is_inline () -> bool\n\n"
      "Indicates whether the link displays its content inline.\n\n"
      "Indicates whether the link currently displays some or all of its\n"
      "content inline. Ordinary HTML links will usually return False, but\n"
      "an inline <src> HTML element will return True.", NULL },
    { "is_selected_link", (getter) _atkhyperlink_is_selected_link, NULL,
      "H.is_selected_link ()\n\n"
      "Determines whether this AtkHyperlink is selected.\n\n"
      "THIS METHOD IS DEPRECATED", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkHyperlink wrapper type, that encapsulates the AtkHyperlink.
 */
PyTypeObject PyAtkHyperlink_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkHyperlink",        /* tp_name */
    sizeof (PyAtkHyperlink),    /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkhyperlink_dealloc, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "AtkHyperlink () -> AtkHyperlink\n\n"
    "ATK accessibility hyperlink.\n\n"
    "An ATK object which encapsulates a link or set of links in a hypertext\n"
    "document.",
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _atkhyperlink_methods,      /* tp_methods */
    0,                          /* tp_members */
    _atkhyperlink_getsets,      /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    (initproc) _atkhyperlink_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkhyperlink_new,          /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0                           /* tp_del */
};

/**
 * Creates a new PyAtkHyperlink.
 */
static PyObject*
_atkhyperlink_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyAtkHyperlink *self;

    debug ("_atkhyperlink_new\n");

    self = (PyAtkHyperlink *) type->tp_alloc (type, 0);
    return (PyObject *) self;
}

/**
 * Initializes the PyAtkHyperlink members, such as the AtkHyperlink.
 */
static int
_atkhyperlink_init (PyAtkHyperlink *self, PyObject *args, PyObject *kwds)
{
    debug ("_atkhyperlink_init\n");

    if (!self->obj)
        self->obj = g_object_new (ATK_TYPE_HYPERLINK, NULL);
    return 0;
}

/**
 * Deallocates the PyAtkHyperlink and its members.
 */
static void
_atkhyperlink_dealloc (PyAtkHyperlink *self)
{
    debug ("_atkhyperlink_dealloc\n");

    if (self->obj)
        g_object_unref (self->obj);
    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Wrapper for atk_hyperlink_get_uri().
 */
static PyObject*
_atkhyperlink_get_uri (PyAtkHyperlink *self, PyObject *args)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_get_object().
 */
static PyObject*
_atkhyperlink_get_object (PyAtkHyperlink *self, PyObject *args)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_get_end_index().
 */
static PyObject*
_atkhyperlink_get_end_index (PyAtkHyperlink *self)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_get_start_index().
 */
static PyObject*
_atkhyperlink_get_start_index (PyAtkHyperlink *self)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_is_valid().
 */
static PyObject*
_atkhyperlink_is_valid (PyAtkHyperlink *self, void *closure)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_is_inline().
 */
static PyObject*
_atkhyperlink_is_inline (PyAtkHyperlink *self, void *closure)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_get_n_anchors().
 */
static PyObject*
_atkhyperlink_get_n_anchors (PyAtkHyperlink *self)
{
    /* TODO */
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_hyperlink_is_selected_link().
 */
static PyObject*
_atkhyperlink_is_selected_link (PyAtkHyperlink *self, void *closure)
{
    /* TODO */
    Py_RETURN_NONE;
}
