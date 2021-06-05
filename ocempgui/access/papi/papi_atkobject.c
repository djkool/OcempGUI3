/* $Id: papi_atkobject.c,v 1.19.2.4 2007/09/23 07:37:48 marcusva Exp $
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

/* Forward declarations for AtkObject bindings. */
static PyObject* _not_implemented_noargs (PyObject *self);
static PyObject* _atkobject_new (PyTypeObject *type, PyObject *args,
                                 PyObject *kwds);
static int _atkobject_init (PyAtkObject *self, PyObject *args, PyObject *kwds);
static void _atkobject_dealloc (PyAtkObject *self);
static PyObject* _atkobject_get_dict (PyAtkObject *self, void *closure);
static PyObject* _atkobject_ref_accessible_child (PyAtkObject *self,
                                                  PyObject *args);
static PyObject* _atkobject_ref_relation_set (PyAtkObject *self);
static PyObject* _atkobject_connect_property_change_handler (PyAtkObject *self,
                                                             PyObject *args);
static PyObject* _atkobject_remove_property_change_handler (PyAtkObject *self,
                                                            PyObject *args);
static PyObject* _atkobject_notify_state_change (PyAtkObject *self,
                                                 PyObject *args);
static PyObject* _atkobject_add_relationship (PyAtkObject *self,
                                              PyObject *args);
static PyObject* _atkobject_remove_relationship (PyAtkObject *self,
                                                 PyObject *args);

static PyObject* _atkobject_get_name (PyAtkObject *self, void *closure);
static int _atkobject_set_name (PyAtkObject *self, PyObject *value,
                                void *closure);
static PyObject* _atkobject_get_description (PyAtkObject *self, void *closure);
static int _atkobject_set_description (PyAtkObject *self, PyObject *value,
                                       void *closure);
static PyObject* _atkobject_get_parent (PyAtkObject *self, void *closure);
static int _atkobject_set_parent (PyAtkObject *self, PyObject *value,
                                  void *closure);
static PyObject* _atkobject_get_n_accessible_children (PyAtkObject *self,
                                                       void *closure);
static PyObject* _atkobject_get_layer (PyAtkObject *self, void *closure);
static PyObject* _atkobject_get_mdi_zorder (PyAtkObject *self, void *closure);
static PyObject* _atkobject_get_role (PyAtkObject *self, void *closure);
static int _atkobject_set_role (PyAtkObject *self, PyObject *value,
                                void *closure);
static PyObject* _atkobject_get_index_in_parent (PyAtkObject *self,
                                                 void *closure);
static PyObject* _atkobject_get_attributes (PyAtkObject *self, void *closure);

/* Nonstandard enhancements. */
static PyObject* _atkobject_emit (PyAtkObject *self, PyObject *args);

static int _class_get_n_children (AtkObject *obj);
static AtkObject* _class_ref_child (AtkObject *obj, int i);
static int _class_get_index_in_parent (AtkObject *obj);

/* Signal handler for the property changes. */
static void _atkobject_handle_property_change (AtkObject *obj,
                                               AtkPropertyValues *value);

/**
 * Methods, which are bound to the PyAtkObject type.
 */
static PyMethodDef _atkobject_methods[] =
{
    { "ref_accessible_child", (PyCFunction) _atkobject_ref_accessible_child,
      METH_VARARGS,
      "O.ref_accessible_child (index) -> AtkObject\n\n"
      "Gets a reference to the specified accessible child of the object.\n\n"
      "The accessible children are 0-based so the first accessible child\n"
      "is at index 0, the second at index 1 and so on." },
    { "ref_relation_set", (PyCFunction) _atkobject_ref_relation_set,
      METH_NOARGS,
      "O.ref_relation_set () -> AtkRelationSet\n\n"
      "Gets the AtkRelationSet associated with the object." },
    { "ref_state_set", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.ref_state_set () -> AtkStateSet\n\n"
      "Gets a reference to the state set of the accessible." },
    { "connect_property_change_handler",
      (PyCFunction) _atkobject_connect_property_change_handler, METH_VARARGS,
      "O.connect_property_change_handler (...) -> None\n\n"
      "Specifies a function to be called when a property changes value.\n\n"
      "Currently not implemented."},
    { "remove_property_change_handler",
      (PyCFunction) _atkobject_remove_property_change_handler, METH_VARARGS,
      "O.remove_property_change_handler (...) -> None\n\n"
      "Removes a property change handler.\n\n"
      "Currently not implemented."},
    { "notify_state_change", (PyCFunction) _atkobject_notify_state_change,
      METH_VARARGS,
      "O.notify_state_change (state, value) -> None\n\n"
      "Emits a state-change signal for the specified state." },
    { "add_relationship", (PyCFunction) _atkobject_add_relationship,
      METH_VARARGS,
      "O.add_relationship (type, obj) -> None\n\n"
      "Adds a relationship of a specific type with the specified target." },
    { "remove_relationship", (PyCFunction) _atkobject_remove_relationship,
      METH_VARARGS,
      "O.remove_relationship (type, obj) -> None\n\n"
      "Removes a relationship of a specific type with the specified target." },

    /* Nonstandard enhancement. */
    { "emit", (PyCFunction) _atkobject_emit, METH_VARARGS,
      "O.emit (type) -> None\n\n"
      "Emits a specific signal on the underlying AT-SPI implementation." },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the PyAtkObject.
 */
static PyGetSetDef _atkobject_getsets[] = {
    { "__dict__", (getter) _atkobject_get_dict, NULL, NULL, NULL },
    { "name", (getter) _atkobject_get_name, (setter) _atkobject_set_name,
      "The name of the accessible.", NULL },
    { "description", (getter) _atkobject_get_description,
      (setter) _atkobject_set_description, 
      "The description of the accessible.", NULL },
    { "parent", (getter) _atkobject_get_parent, (setter) _atkobject_set_parent,
      "The parent of the accessible.", NULL },
    { "n_accessible_children", (getter) _atkobject_get_n_accessible_children,
      NULL, "The number of accessible children of the accessible.", NULL },
    { "layer", (getter) _atkobject_get_layer, NULL,
      "The layer of the accessible.", NULL },
    { "mdi_zorder", (getter) _atkobject_get_mdi_zorder, NULL,
      "The z-order of the accessible. The value None will be returned if\n"
      "the layer of the accessible is not ATK_LAYER_MDI.", NULL },
    { "role", (getter) _atkobject_get_role, (setter) _atkobject_set_role,
      "The role of the accessible.", NULL },
    { "index_in_parent", (getter) _atkobject_get_index_in_parent, NULL,
      "The 0-based index of this accessible in its parent or -1\n"
      "if the accessible does not have an accessible parent.", NULL },
    { "attributes", (getter) _atkobject_get_attributes, NULL,
      "A list of AtkAttributes applied to this AtkObject as a whole.", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

/**
 * PyAtkObject wrapper type, that encapsulates the AtkObject.
 */
PyTypeObject PyAtkObject_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "papi.AtkObject",           /* tp_name */
    sizeof (PyAtkObject),       /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _atkobject_dealloc, /* tp_dealloc */
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
    "AtkObject (interfaces) -> AtkObject\n\n"
    "ATK accessibility object.\n\n",
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    offsetof (PyAtkObject, weakrefs), /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _atkobject_methods,         /* tp_methods */
    0,                          /* tp_members */
    _atkobject_getsets,         /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    offsetof (PyAtkObject, dict), /* tp_dictoffset */
    (initproc) _atkobject_init, /* tp_init */
    0,                          /* tp_alloc */
    _atkobject_new,             /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0                           /* tp_del */
};

static inline PyObject*
_construct_object (AtkObject *val)
{
    PyObject *retval;

    debug ("Constructing object...\n");

    if (val)
    {
        retval = g_object_get_data (G_OBJECT (val), PAPI_PYOBJECT);
        if (!retval) /* Not constructed by Papi. */
        {
            AtkInterfaceType ifaces;
            GType *types;

            retval = PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
            ((PyAtkObject *) retval)->obj = g_object_ref (val);

            /* Prepare some data slots for the references. */
            g_object_set_data (G_OBJECT (val), PAPI_CHILDREN, NULL);
            g_object_set_data (G_OBJECT (val), PAPI_PYOBJECT, retval);

            /* Set up the interfaces for the type. */
            types = g_type_interfaces (G_TYPE_FROM_INSTANCE (val), NULL);
            ifaces = atktype_get_num (types);
            g_free (types);
            atktype_add_interface_methods (retval, ifaces);
        }
        else
            Py_INCREF (retval);
        return retval;
    }
    Py_RETURN_NONE;
}

/**
 * Creates a new PyAtkObject.
 */
static PyObject*
_atkobject_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    debug ("_atkobject_new\n");
    return PyType_GenericNew (type, args, kwds);
}

/**
 * Initializes the PyAtkObject members, such as the AtkObject.
 */
static int
_atkobject_init (PyAtkObject *self, PyObject *args, PyObject *kwds)
{
    debug ("_atkobject_init\n");
    AtkInterfaceType ifaces = 0;

    if (!PyArg_ParseTuple (args, "|l:init", &ifaces))
        return -1;

    /* No interfaces provided, create default AtkObject. */
    if (!self->obj)
    {
        if (ifaces == IFACE_INVALID)
            self->obj = g_object_new (ATK_TYPE_OBJECT, NULL);
        else
            self->obj = g_object_new (atktype_get_type (ifaces), NULL);

        /* Prepare some data slots for the references. */
        g_object_set_data (G_OBJECT (self->obj), PAPI_CHILDREN, NULL);
        g_object_set_data (G_OBJECT (self->obj), PAPI_PYOBJECT, self);

        atktype_add_interface_methods ((PyObject *) self, ifaces);
    }
    
    /* Update the interface bindings. */
    return 0;
}

/**
 * Deallocates the PyAtkObject and its members.
 */
static void
_atkobject_dealloc (PyAtkObject *self)
{
    debug ("_atkobject_dealloc\n");

    if (self->obj)
        g_object_unref (self->obj);

    Py_XDECREF (self->dict);
    Py_XDECREF (self->prop_handlers);

    if (self->weakrefs)
        PyObject_ClearWeakRefs ((PyObject *) self);

    self->ob_type->tp_free ((PyObject *) self);
}

/**
 * Getter for AtkObject.__dict__.
 */
static PyObject*
_atkobject_get_dict (PyAtkObject *self, void *closure)
{
    if (!self->dict)
    {
	self->dict = PyDict_New ();
        if (!self->dict)
            return NULL;
    }

    Py_INCREF (self->dict);
    return self->dict;
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
 * Wrapper for atk_object_get_name().
 */
static PyObject*
_atkobject_get_name (PyAtkObject *self, void *closure)
{
    const gchar *val;

    debug ("_atkobject_get_name\n");

    val = atk_object_get_name (ATKOBJECT (self));
    if (val)
        return PyString_FromString (val);
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_object_set_name().
 */
static int
_atkobject_set_name (PyAtkObject *self, PyObject *value, void *closure)
{
    char *val;

    debug ("_atkobject_set_name\n");

    if (!value)
    {
        PyErr_SetString (PyExc_TypeError, "Cannot delete the name attribute");
        return -1;
    }

    val = PyString_AsString (value);
    if (!val)
        return -1;
    atk_object_set_name (ATKOBJECT (self), val);
    return 0;
}

/**
 * Wrapper for atk_object_get_description().
 */
static PyObject*
_atkobject_get_description (PyAtkObject *self, void *closure)
{
    const gchar *val;

    debug ("_atkobject_get_description\n");

    val = atk_object_get_description (ATKOBJECT (self));
    if (val)
        return PyString_FromString (val);
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_object_set_description().
 */
static int
_atkobject_set_description (PyAtkObject *self, PyObject *value, void *closure)
{
    char *val;

    debug ("_atkobject_set_description\n");

    if (!value)
    {
        PyErr_SetString (PyExc_TypeError,
                         "Cannot delete the description attribute");
        return -1;
    }

    val = PyString_AsString (value);
    if (!val)
        return -1;
    atk_object_set_description (ATKOBJECT (self), val);
    return 0;
}

/**
 * Wrapper for atk_object_get_parent().
 */
static PyObject*
_atkobject_get_parent (PyAtkObject *self, void *closure)
{
    AtkObject *val;

    debug ("_atkobject_get_parent\n");

    val = atk_object_get_parent (ATKOBJECT (self));
    return _construct_object (val);
}

/**
 * Wrapper for atk_object_set_parent().
 */
static int
_atkobject_set_parent (PyAtkObject *self, PyObject *value, void *closure)
{
    gpointer data;

    debug ("_atkobject_set_parent\n");

    if (!value)
    {
        PyErr_SetString (PyExc_TypeError,
                         "Cannot delete the parent attribute");
        return -1;
    }

    /* Nullify parent. */
    if (value == Py_None)
    {
        AtkObject *parent = ATKOBJECT (self)->accessible_parent;
        if (parent)
        {
            data = g_object_get_data (G_OBJECT (parent), PAPI_CHILDREN);
            data = g_list_remove (data, self->obj);
            ATKOBJECT (self)->accessible_parent = NULL;
            g_object_set_data (G_OBJECT (parent), PAPI_CHILDREN, data);
        }
        return 0;
    }
    else if (!PyObject_TypeCheck (value, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be an AtkObject");
        return -1;
    }
    else
        atk_object_set_parent (ATKOBJECT (self), ATKOBJECT (value));

    /* Add the object to the list of children of the parent. */
    data = g_object_get_data (G_OBJECT (ATKOBJECT (value)), PAPI_CHILDREN);
    data = g_list_append (data, self->obj);
    g_object_set_data (G_OBJECT (ATKOBJECT (value)), PAPI_CHILDREN, data);

   return 0;
}

/**
 * Wrapper for atk_object_get_n_accessible_children().
 */
static PyObject*
_atkobject_get_n_accessible_children (PyAtkObject *self, void *closure)
{
    gint val;

    debug ("_atkobject_get_n_accessible_children\n");

    val = atk_object_get_n_accessible_children (ATKOBJECT (self));
    return PyInt_FromLong ((long) val);
}

/**
 * Wrapper for atk_object_ref_accessible_child().
 */
static PyObject*
_atkobject_ref_accessible_child (PyAtkObject *self, PyObject *args)
{
    AtkObject *val;
    int i;

    debug ("_atkobject_ref_accessible_child\n");

    if (!PyArg_ParseTuple (args, "i:ref_accessible_child", &i))
        return NULL;

    val = atk_object_ref_accessible_child (ATKOBJECT (self), i);
    return _construct_object (val);
}

/**
 * Wrapper for atk_object_ref_relation_set().
 */
static PyObject*
_atkobject_ref_relation_set (PyAtkObject *self)
{
    PyAtkRelationSet *set;
    AtkRelationSet *aset;

    debug ("_atkobject_ref_relationset\n");

    aset = atk_object_ref_relation_set (ATKOBJECT (self));
    if (!aset)
        Py_RETURN_NONE;
    set = PyObject_New (PyAtkRelationSet, &PyAtkRelationSet_Type);
    set->obj = aset;
    return (PyObject *) set;
}

/**
 * Wrapper for atk_object_get_layer().
 */
static PyObject*
_atkobject_get_layer (PyAtkObject *self, void *closure)
{
    AtkLayer layer;

    debug ("_atkobject_get_layer\n");

    layer = atk_object_get_layer (ATKOBJECT (self));
    return PyInt_FromLong ((long) layer);
}

/**
 * Wrapper for atk_object_get_mdi_zorder().
 */
static PyObject*
_atkobject_get_mdi_zorder (PyAtkObject *self, void *closure)
{
    int z;

    debug ("_atkobject_get_mdi_zorder\n");

    z = atk_object_get_mdi_zorder (ATKOBJECT (self));
    return PyInt_FromLong ((long) z);
}

/**
 * Wrapper for atk_object_get_role().
 */
static PyObject*
_atkobject_get_role (PyAtkObject *self, void *closure)
{
    AtkRole role;

    debug ("_atkobject_get_role\n");

    role = atk_object_get_role (ATKOBJECT (self));
    return PyInt_FromLong ((long) role);
}

/**
 * Wrapper for atk_object_set_role().
 */
static int
_atkobject_set_role (PyAtkObject *self, PyObject *value, void *closure)
{
    debug ("_atkobject_set_role\n");
    
    if (!value)
    {
        PyErr_SetString (PyExc_TypeError, "Cannot delete the role attribute");
        return -1;
    }

    atk_object_set_role (ATKOBJECT (self), PyInt_AsLong (value));
    return 0;
}

/**
 * Wrapper for atk_object_get_index_in_parent().
 */
static PyObject*
_atkobject_get_index_in_parent (PyAtkObject *self, void *closure)
{
    int val;

    debug ("_atkobject_index_in_parent\n");

    val = atk_object_get_index_in_parent (ATKOBJECT (self));
    return PyInt_FromLong ((long) val);
}

/**
 * Wrapper for atk_object_get_attributes().
 */
static PyObject*
_atkobject_get_attributes (PyAtkObject *self, void *closure)
{
    PyObject *list;
    PyAtkAttribute *attr;
    AtkAttributeSet *set;
    guint amount = 0;
    int i = 0;

    debug ("_atkobject_get_attributes\n");
    
    set = atk_object_get_attributes (ATKOBJECT (self));
    amount = g_slist_length (set);

    if (amount == 0)
    {
        Py_RETURN_NONE;
    }

    list = PyList_New ((int) g_slist_length (set));
    if (!list)
        return NULL;

    do
    {
        attr = PyObject_New (PyAtkAttribute, &PyAtkAttribute_Type);
        attr->obj = (AtkAttribute *) set->data;
        PyList_SetItem (list, i++, (PyObject *) attr);
    }
    while ((set = g_slist_next (set)));

    return list;
}

/**
 * Wrapper for atk_object_connect_property_change_handler().
 */
static PyObject*
_atkobject_connect_property_change_handler (PyAtkObject *self, PyObject *args)
{
    PyObject *callback;
    guint id = 0;

    debug ("_atkobject_connect_property_change_handler\n");

    if (!PyArg_ParseTuple (args, "O:connect_property_change_handler",
                           &callback))
        return NULL;

    if (!PyCallable_Check (callback))
    {
        PyErr_SetString (PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    /* No property change handler registered for now. Set up the list. */
    if (!self->prop_handlers)
    {
        self->prop_handlers = PyDict_New ();
        if (!self->prop_handlers)
            return NULL;

        /* Just bind our wrapper only once as it will cycle through
         * the dict and invoke each connected hander function. */
        id = atk_object_connect_property_change_handler
            (ATKOBJECT (self),
             (AtkPropertyChangeHandler *) _atkobject_handle_property_change);
    }
    
    /* Could not append to list. */
    if (PyDict_SetItem (self->prop_handlers,
                        PyInt_FromLong (PyDict_Size (self->prop_handlers)),
                        callback) == -1)
    {
        atk_object_remove_property_change_handler (ATKOBJECT (self), id);
        return NULL;
    }

    return PyInt_FromLong (PyDict_Size (self->prop_handlers));
}

/**
 * Wrapper for atk_object_remove_property_change_handler().
 */
static PyObject*
_atkobject_remove_property_change_handler (PyAtkObject *self, PyObject *args)
{
    int id = 0;

    debug ("_atkobject_remove_property_change_handler\n");

    if (!PyArg_ParseTuple (args, "i:remove_property_change_handler", &id))
        return NULL;
    if (PyDict_DelItem (self->prop_handlers, PyInt_FromLong (id)) == -1)
        return NULL;
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_object_notify_state_change().
 */
static PyObject*
_atkobject_notify_state_change (PyAtkObject *self, PyObject *args)
{
    AtkState state;
    int val;

    debug ("_atkobject_notify_state_change\n");

    if (!PyArg_ParseTuple (args, "ii:notify_state_changed", &state, &val))
        return NULL;

    atk_object_notify_state_change (ATKOBJECT (self), state, val);
    Py_RETURN_NONE;
}

/**
 * Wrapper for atk_object_add_relationship().
 */
static PyObject*
_atkobject_add_relationship (PyAtkObject *self, PyObject *args)
{
    PyObject *target;
    AtkRelationType type;

    debug ("_atkobject_add_relationship\n");

    if (!PyArg_ParseTuple (args, "iO:add_relationship", &type, &target))
        return NULL;

    if (!PyObject_TypeCheck (target, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "argument 2 must be an AtkObject");
        return NULL;
    }

    if (atk_object_add_relationship (ATKOBJECT (self), type,
                                     ATKOBJECT (target)))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Wrapper for atk_object_remove_relationship().
 */
static PyObject*
_atkobject_remove_relationship (PyAtkObject *self, PyObject *args)
{
    PyObject *target;
    AtkRelationType type;

    debug ("_atkobject_remove_relationship\n");

    if (!PyArg_ParseTuple (args, "iO:remove_relationship", &type, &target))
        return NULL;

    if (!PyObject_TypeCheck (target, &PyAtkObject_Type))
    {
        PyErr_SetString (PyExc_TypeError, "argument 2 must be an AtkObject");
        return NULL;
    }

    if (atk_object_remove_relationship (ATKOBJECT (self), type,
                                        ATKOBJECT (target)))
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

/**
 * Emits signals on the registered listener hooks.
 */
static PyObject*
_atkobject_emit (PyAtkObject *self, PyObject *args)
{
    PyObject *dict = NULL;
    PyObject *obj = NULL;
    PyObject *list = NULL;
    int size = 0;
    guint id = 0;
    char *type = NULL;
    GValue *value = NULL;
    GSignalInvocationHint hint;
    
    debug ("_atkobject_emit\n");

    if (!PyArg_ParseTuple (args, "s:emit", &type))
        return NULL;

#ifdef DEBUG
    printf ("DEBUG: Received signal: %s\n", type);
#endif

    id = atkutil_lookup_signal (type);
    if (!id) /* id == 0 */
    {
        PyErr_Format (PyExc_KeyError, "Signal '%s' does not exist.", type);
        return NULL;
    }

    dict = PyDict_GetItemString (atkutil_get_listeners (), type);
    if (!dict)
    {
        PyErr_Format (PyExc_KeyError, "Key '%s' does not exist.", type);
        return NULL;
    }

    list = PyDict_Values (dict);
    size = PyList_Size (list);

    /* Iterate over the attached emission hooks and invoke them. */
    for (int i = 0; i < size; i++)
    {
        obj = PyList_GetItem (list, i);

        /* Create the args for the GSignalEmissionHook. */
        hint.signal_id = id;
        hint.detail = g_quark_from_string (type);
        hint.run_type = G_SIGNAL_RUN_FIRST;

        /* Create the arguments for the hook. */
        value = g_new0 (GValue, 1);
        g_value_init (value, G_TYPE_FROM_INSTANCE (ATKOBJECT (self)));
        g_value_set_instance (value, ATKOBJECT (self));

        /* Invoke it. */
        ((GSignalEmissionHook) PyCObject_AsVoidPtr (obj)) (&hint, 1, value,
                                                           NULL);
        g_free (value);
    }
    Py_DECREF (list);
    Py_RETURN_NONE;
}

/**
 * Interface for AtkObjectClass->get_n_children.
 */
static int
_class_get_n_children (AtkObject *obj)
{
    debug ("_class_get_n_children\n");

    GList *list = g_object_get_data (G_OBJECT (obj), PAPI_CHILDREN);
    if (list)
        return g_list_length (list);
    return 0;
}

/**
 * Interface for AtkObjectClass->ref_child.
 */
static AtkObject*
_class_ref_child (AtkObject *obj, int i)
{
    AtkObject *val = NULL;
    GList *list = NULL;

    debug ("_class_ref_child\n");

    list = g_object_get_data (G_OBJECT (obj), PAPI_CHILDREN);
    if (list)
    {
        val = g_list_nth_data (list, (guint) i);
        if (!val)
            return NULL;
    }
    if (val)
        g_object_ref (val);
    return val;
}

/**
 * Interface for AtkObjectClass->get_index_in_parent.
 */
static int
_class_get_index_in_parent (AtkObject *obj)
{
    GList *list;
    AtkObject *parent;

    debug ("_class_get_index_in_parent\n");

    parent = atk_object_get_parent (obj);
    if (!parent)
        return -1;

    list = g_object_get_data (G_OBJECT (parent), PAPI_CHILDREN);
    if (list)
        return g_list_index (list, obj);
    return -1;
}

/**
 * Interface for AtkObjectClass->ref_state_set.
 */
static AtkStateSet*
_class_ref_state_set (AtkObject *obj)
{                                          
    PyObject *pyobj;
    PyObject *result;
    AtkStateSet *retval = NULL;

    debug ("_class_ref_state_set\n");

    pyobj = g_object_get_data (G_OBJECT (obj), PAPI_PYOBJECT);
    result = PyObject_CallMethod (pyobj, "ref_state_set", NULL);
    if (!result)
        return atk_state_set_new (); /* Returning NULL crashes A11Y apps. */

    if (PyObject_TypeCheck (result, &PyAtkStateSet_Type))
        retval = ATKSTATESET (result);
    g_object_ref (retval);
    Py_DECREF (result);
    return retval;
}

/**
 * Generic callback handler for property changes, which invokes the
 * user-defined python handlers.
 */
static void
_atkobject_handle_property_change (AtkObject *obj, AtkPropertyValues *value)
{
    PyObject *callback = NULL;
    PyObject *retval = NULL;
    PyObject *name = NULL;
    PyObject *oldval = NULL;
    PyObject *newval = NULL;
    int pos = 0;
    PyAtkObject *atkobj = (PyAtkObject *) g_object_get_data (G_OBJECT (obj),
                                                             PAPI_PYOBJECT);

    while (PyDict_Next (atkobj->prop_handlers, &pos, NULL, &callback))
    {
        name = PyString_FromString (value->property_name);
        oldval = pyobject_from_gvalue (&(value->old_value));
        newval = pyobject_from_gvalue (&(value->new_value));

        retval = PyObject_CallFunctionObjArgs (callback, name, oldval, newval,
                                               NULL);
        Py_DECREF (name);
        Py_DECREF (oldval);
        Py_DECREF (newval);
        Py_DECREF (retval);
    }
}

/**
 * Bind the different AtkObjectClass interfaces to their needed
 * implementations.
 */
void
atkobjectclass_init (AtkObjectClass *class)
{
    debug ("Hooking AtkObjectClass interfaces.\n");

    class->get_n_children = _class_get_n_children;
    class->ref_child = _class_ref_child;
    class->get_index_in_parent = _class_get_index_in_parent;
    class->ref_state_set = _class_ref_state_set;
}
