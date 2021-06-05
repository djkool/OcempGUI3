/* $Id: papi_atkeditableiface.c,v 1.10.2.4 2007/09/23 07:37:48 marcusva Exp $
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
static gboolean _editable_set_run_attributes (AtkEditableText *text,
                                              AtkAttributeSet *attrib_set,
                                              gint start_offset,
                                              gint end_offset);
static void _editable_set_text_contents (AtkEditableText *text,
                                         const gchar *string);
static void _editable_insert_text (AtkEditableText *text, const gchar *string,
                                   gint length, gint *position);
static void _editable_copy_text (AtkEditableText *text, gint start_pos,
                                 gint end_pos);
static void _editable_cut_text (AtkEditableText *text, gint start_pos,
                                gint end_pos);
static void _editable_delete_text (AtkEditableText *text, gint start_pos,
                                   gint end_pos);
static void _editable_paste_text (AtkEditableText *text, gint position);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atkeditableiface_methods[] =
{
    { "editable_set_run_attributes", _not_implemented_args, METH_VARARGS,
      "O.editable_set_run_attributes (attribute_set, start, end) -> bool\n\n"
      "Sets the attributes for a specified range.\n\n"
      "See the ATK_ATTRIBUTE macros (such as ATK_ATTRIBUTE_LEFT_MARGIN) for\n"
      "examples of attributes that can be set. Note that other attributes\n"
      "that do not have corresponding ATK_ATTRIBUTE macros may also be set\n"
      "for certain text widgets.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_set_text_contents", _not_implemented_args, METH_VARARGS,
      "O.editable_set_text_contents (text) -> None\n\n"
      "Sets the contents of the editable.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_insert_text", _not_implemented_args, METH_VARARGS,
      "O.editable_insert_text (text, length, position) -> int\n\n"
      "Inserts text at a given position.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_copy_text", _not_implemented_args, METH_VARARGS,
      "O.editable_copy_text (start, end) -> None\n\n"
      "Copies a range of text.\n\n"
      "Copy text from a start position up to, but not including the end\n"
      "position to the clipboard.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_cut_text", _not_implemented_args, METH_VARARGS,
      "O.editable_cut_text (start, end) -> None\n\n"
      "Cuts a range of text.\n\n"
      "Copy text from a start position up to, but not including the end\n"
      "position to the clipboard and then delete it from the widget.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_delete_text", _not_implemented_args, METH_VARARGS,
      "O.editable_delete_text (start, end) -> None\n\n"
      "Deletes a range of text.\n\n"
      "Delete text from a start position up to, but not including the end\n"
      "position.\n\n"
      "This method has to be implemented by inherited classes." },
    { "editable_paste_text", _not_implemented_args, METH_VARARGS,
      "O.editable_paste_text (position) -> None\n\n"
      "Paste text from the clipboard to the specified position.\n\n"
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
 * Wrapper for atk_editable_text_set_run_attributes().
 */
static gboolean
_editable_set_run_attributes (AtkEditableText *text,
                              AtkAttributeSet *attrib_set,
                              gint start_offset, gint end_offset)
{
    PyObject *result;
    PyObject *obj;
    PyObject *list;
    PyAtkAttribute *attr;
    GSList *cur = attrib_set;
    int i = 0;
    gboolean retval = FALSE;

    debug ("_editable_set_run_attributes\n");

    /* Build the attribute set. */
    list = PyList_New ((int) g_slist_length (cur));
    if (!list)
        return FALSE;
    do
    {
        attr = PyObject_New (PyAtkAttribute, &PyAtkAttribute_Type);
        attr->obj = (AtkAttribute *) cur->data;
        PyList_SetItem (list, i++, (PyObject *) attr);
    }
    while ((cur = g_slist_next (cur)));

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_set_run_attributes", "Oii",
                                  list, start_offset, end_offset);
    if (result)
        retval = result == Py_True;
    Py_DECREF (list);
    Py_XDECREF (result);
    return retval;
}

/**
 * Wrapper for atk_editable_text_set_text_contents().
 */
static void
_editable_set_text_contents (AtkEditableText *text, const gchar *string)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_set_text_contents\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_set_text_contents", "s",
                                  string);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_editable_text_insert_text().
 */
static void
_editable_insert_text (AtkEditableText *text, const gchar *string, gint length,
                       gint *position)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_insert_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_insert_text", "sii", string,
                                  length, *position);
    if (!result)
        return;
    *position = PyInt_AsLong (result);
    Py_DECREF (result);
}

/**
 * Wrapper for atk_editable_text_copy_text().
 */
static void
_editable_copy_text (AtkEditableText *text, gint start_pos, gint end_pos)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_copy_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_copy_text", "ii", start_pos,
                                  end_pos);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_editable_text_cut_text().
 */
static void
_editable_cut_text (AtkEditableText *text, gint start_pos, gint end_pos)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_cut_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_cut_text", "ii", start_pos,
                                  end_pos);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_editable_text_delete_text().
 */
static void
_editable_delete_text (AtkEditableText *text, gint start_pos, gint end_pos)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_delete_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_delete_text", "ii", start_pos,
                                  end_pos);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_editable_text_paste_text().
 */
static void
_editable_paste_text (AtkEditableText *text, gint position)
{
    PyObject *result;
    PyObject *obj;

    debug ("_editable_paste_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "editable_paste_text", "i", position);
    Py_XDECREF (result);
}

/**
 * Bind the different AtkEditableTextIface interfaces to the wrapper
 * interfaces.
 */
static void
_atkeditableiface_init (AtkEditableTextIface *iface)
{
    iface->set_run_attributes = _editable_set_run_attributes;
    iface->set_text_contents = _editable_set_text_contents;
    iface->insert_text = _editable_insert_text;
    iface->copy_text = _editable_copy_text;
    iface->cut_text = _editable_cut_text;
    iface->delete_text = _editable_delete_text;
    iface->paste_text = _editable_paste_text;
}

/**
 * Gets a modified interface for the AtkEditableTextIface, which uses
 * the python wrappers as interfaces.
 */
const GInterfaceInfo*
atkeditableiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atkeditableiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkEditableText methods to the passed object instance.
 */
void
atkeditableiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atkeditableiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atkeditableiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atkeditableiface_methods[i].ml_name,
                                func);
        i++;
    }
}
