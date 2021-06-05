/* $Id: papi_atktableiface.c,v 1.10.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static AtkObject* _table_ref_at (AtkTable *table, gint row, gint column);
static gint _table_get_index_at (AtkTable *table, gint row, gint column);
static gint _table_get_column_at_index (AtkTable *table, gint index_);
static gint _table_get_row_at_index (AtkTable *table, gint index_);
static gint _table_get_n_columns (AtkTable *table);
static gint _table_get_n_rows (AtkTable *table);
static gint _table_get_column_extent_at (AtkTable *table, gint row,
                                         gint column);
static gint _table_get_row_extent_at (AtkTable *table, gint row, gint column);
static AtkObject* _table_get_caption (AtkTable *table);
static const gchar* _table_get_column_description (AtkTable *table,
                                                   gint column);
static const gchar* _table_get_row_description (AtkTable *table, gint row);
static AtkObject* _table_get_column_header (AtkTable *table, gint column);
static AtkObject* _table_get_row_header (AtkTable *table, gint row);
static AtkObject* _table_get_summary (AtkTable *table);
static void _table_set_caption (AtkTable *table, AtkObject *caption);
static void _table_set_row_description (AtkTable *table, gint row,
                                        const gchar *description);
static void _table_set_column_description (AtkTable *table, gint column,
                                           const gchar *description);
static void _table_set_row_header (AtkTable *table, gint row,
                                   AtkObject *header);
static void _table_set_column_header (AtkTable *table, gint column,
                                      AtkObject *header);
static void _table_set_summary (AtkTable *table, AtkObject *accessible);
static gint _table_get_selected_columns (AtkTable *table, gint **selected);
static gint _table_get_selected_rows (AtkTable *table, gint **selected);
static gboolean _table_is_column_selected (AtkTable *table, gint column);
static gboolean _table_is_row_selected (AtkTable *table, gint row);
static gboolean _table_is_selected (AtkTable *table, gint row, gint column);
static gboolean _table_add_column_selection (AtkTable *table, gint column);
static gboolean _table_add_row_selection (AtkTable *table, gint row);
static gboolean _table_remove_column_selection (AtkTable *table, gint column);
static gboolean _table_remove_row_selection (AtkTable *table, gint row);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atktableiface_methods[] =
{
    { "table_ref_at", _not_implemented_args, METH_VARARGS,
      "O.table_ref_at (row, colum) -> AtkObject\n\n"
      "Get a reference to the table cell at row, column.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_index_at", _not_implemented_args, METH_VARARGS,
      "O.table_get_index_at (row, column) -> int\n\n"
      "Gets the index at the specified row and column.\n\n"
      "Gets an integer representing the index at the specified row and\n"
      "column.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_column_at_index", _not_implemented_args, METH_VARARGS,
      "O.table_get_column_at_index (index) -> int\n\n"
      "Gets an integer representing the column at the specified index.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_row_at_index", _not_implemented_args, METH_VARARGS,
      "O.table_get_row_at_index (index) -> int\n\n"
      "Gets an integer representing the row at the specified index.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_n_columns", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.table_get_n_columns () -> int\n\n"
      "Gets the number of columns in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_n_rows", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.table_get_n_rows () -> int\n\n"
      "Gets the number of rows in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_column_extent_at", _not_implemented_args, METH_VARARGS,
      "O.table_get_column_extent_at (row, column) -> int\n\n"
      "Gets the number of columns occupied by the object.\n\n"
      "Gets the number of columns occupied by the accessible object at the\n"
      "specified row and column in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_row_extent_at", _not_implemented_args, METH_VARARGS,
      "O.table_get_row_extent_at (row, column) -> int\n\n"
      "Gets the number of rows occupied by the object.\n\n"
      "Gets the number of rows occupied by the accessible object at a\n"
      "specified row and column in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_caption", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.table_get_caption () -> AtkObject\n\n"
      "Gets the caption for the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_caption", _not_implemented_args, METH_VARARGS,
      "O.table_set_caption (caption) -> None\n\n"
      "Sets the caption for the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_column_description", _not_implemented_args, METH_VARARGS,
      "O.table_get_column_description (column) -> string\n\n"
      "Gets the description text of the specified column in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_column_description", _not_implemented_args, METH_VARARGS,
      "O.table_set_column_description (column, description) -> None\n\n"
      "Sets the description text for the specified column of the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_row_description", _not_implemented_args, METH_VARARGS,
      "O.table_get_row_description (row) -> string\n\n"
      "Gets the description text of the specified row in the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_row_description", _not_implemented_args, METH_VARARGS,
      "O.table_set_row_description (row, description) -> None\n\n"
      "Sets the description text for the specified row of table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_column_header", _not_implemented_args, METH_VARARGS,
      "O.table_get_column_header (column) -> AtkObject\n\n"
      "Gets the column header of a specified column in a table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_column_header", _not_implemented_args, METH_VARARGS,
      "O.table_set_column_header (column, header) -> None\n\n"
      "Sets the specified column header to header.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_row_header", _not_implemented_args, METH_VARARGS,
      "O.table_get_row_header (row) -> AtkObject\n\n"
      "Gets the row header of a specified row in an accessible table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_row_header", _not_implemented_args, METH_VARARGS,
      "O.table_set_row_header (row, header) -> None\n\n"
      "Sets the specified row header to header.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_summary", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.table_get_summary () -> AtkObject\n\n"
      "Gets the summary description of the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_set_summary", _not_implemented_args, METH_VARARGS,
      "O.table_set_summary (summary) -> None\n\n"
      "Sets the summary description of the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_selected_columns", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.table_get_selected_columns () -> list\n\n"
      "Gets the selected columns of the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_get_selected_rows", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.table_get_selected_rows () -> list\n\n"
      "Gets the selected rows of the table.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_is_column_selected", _not_implemented_args, METH_VARARGS,
      "O.table_is_column_selected (column) -> bool\n\n"
      "Checks whether the specified column is selected.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_is_row_selected", _not_implemented_args, METH_VARARGS,
      "O.table_is_row_selected (row) -> bool\n\n"
      "Checks whether the specified row is selected.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_is_selected", _not_implemented_args, METH_VARARGS,
      "O.table_is_selected (row, column) -> bool\n\n"
      "Checks whether the object at the specified row and column is selected."
      "\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_add_column_selection", _not_implemented_args, METH_VARARGS,
      "O.table_add_column_selection (column) -> bool\n\n"
      "Adds the specified column to the selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_add_row_selection", _not_implemented_args, METH_VARARGS,
      "O.table_add_row_selection (row) -> bool\n\n"
      "Adds the specified row to the selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_remove_column_selection", _not_implemented_args, METH_VARARGS,
      "O.table_remove_column_selection (column) -> bool\n\n"
      "Removes the specified column from the selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "table_remove_row_selection", _not_implemented_args, METH_VARARGS,
      "O.table_remove_row_selection (row) -> bool\n\n"
      "Removes the specified row from the selection.\n\n"
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
 * Wrapper for atk_table_ref_at().
 */
static AtkObject*
_table_ref_at (AtkTable *table, gint row, gint column)
{
    PyObject *result;
    PyObject *obj;
    AtkObject* retval = NULL;

    debug ("_table_ref_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_ref_at", "ii", row, column);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_index_at().
 */
static gint
_table_get_index_at (AtkTable *table, gint row, gint column)
{
    PyObject *result;
    PyObject *obj;
    gint retval = -1;

    debug ("_table_get_index_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_index_at", "ii",
                                  row, column);
    if (!result)
        return -1;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_column_index_at().
 */
static gint
_table_get_column_at_index (AtkTable *table, gint index_)
{
    PyObject *result;
    PyObject *obj;
    gint retval = -1;

    debug ("_table_get_column_index_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_column_index_at", "i",
                                  index_);
    if (!result)
        return -1;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_row_index_at().
 */
static gint
_table_get_row_at_index (AtkTable *table, gint index_)
{
    PyObject *result;
    PyObject *obj;
    gint retval = -1;

    debug ("_table_get_row_index_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_row_index_at", "i", index_);
    if (!result)
        return -1;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_n_columns().
 */
static gint
_table_get_n_columns (AtkTable *table)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_table_get_n_columns\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_n_columns", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_n_rows().
 */
static gint
_table_get_n_rows (AtkTable *table)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_table_get_n_rows\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_n_rows", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_column_extent_at().
 */
static gint
_table_get_column_extent_at (AtkTable *table, gint row, gint column)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_table_get_column_extent_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_column_extent_at", "ii",
                                  row, column);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_row_extent_at().
 */
static gint
_table_get_row_extent_at (AtkTable *table, gint row, gint column)
{
    PyObject *result;
    PyObject *obj;
    gint retval = 0;

    debug ("_table_get_row_extent_at\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_row_extent_at", "ii",
                                  row, column);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_caption().
 */
static AtkObject*
_table_get_caption (AtkTable *table)
{
    PyObject *result;
    PyObject *obj;
    AtkObject* retval = NULL;

    debug ("_table_get_caption\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_caption", NULL);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_column_description().
 */
static const gchar*
_table_get_column_description (AtkTable *table, gint column)
{
    PyObject *result;
    PyObject *obj;
    const gchar* retval = NULL;

    debug ("_table_get_column_description\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_column_description", "i",
                                  column);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_row_description().
 */
static const gchar*
_table_get_row_description (AtkTable *table, gint row)
{
    PyObject *result;
    PyObject *obj;
    const gchar* retval = NULL;

    debug ("_table_get_row_description\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_row_description", "i", row);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_column_header().
 */
static AtkObject*
_table_get_column_header (AtkTable *table, gint column)
{
    PyObject *result;
    PyObject *obj;
    AtkObject* retval = NULL;

    debug ("_table_get_column_header\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_column_header", "i", column);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_row_header().
 */
static AtkObject*
_table_get_row_header (AtkTable *table, gint row)
{
    PyObject *result;
    PyObject *obj;
    AtkObject* retval = NULL;

    debug ("_table_get_row_header\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_row_header", "i", row);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_get_summary().
 */
static AtkObject*
_table_get_summary (AtkTable *table)
{
    PyObject *result;
    PyObject *obj;
    AtkObject* retval = NULL;

    debug ("_table_get_summary\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_summary", NULL);
    if (!result)
        return NULL;

    if (PyObject_TypeCheck (result, &PyAtkObject_Type))
        retval = ATKOBJECT (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_set_caption().
 */
static void
_table_set_caption (AtkTable *table, AtkObject *caption)
{
    PyObject *result;
    PyObject *obj;

    debug ("_table_set_caption\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_set_caption", "s", caption);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_set_row_description().
 */
static void
_table_set_row_description (AtkTable *table, gint row,
                            const gchar *description)
{
    PyObject *result;
    PyObject *obj;

    debug ("_table_set_row_description\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_set_row_description", "is", row,
                                  description);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_set_column_description().
 */
static void
_table_set_column_description (AtkTable *table, gint column,
                               const gchar *description)
{
    PyObject *result;
    PyObject *obj;

    debug ("_table_set_column_description\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_set_column_description", "is",
                                  column, description);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_set_row_header().
 */
static void
_table_set_row_header (AtkTable *table, gint row, AtkObject *header)
{
    PyObject *result;
    PyObject *obj;
    PyAtkObject* atkobj = NULL;

    debug ("_table_set_row_header\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);

    /* Create the PyAtkObject. */
    atkobj = g_object_get_data (G_OBJECT (header), PAPI_PYOBJECT);
    if (!atkobj)
    {
        atkobj = (PyAtkObject *)
            PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
        atkobj->obj = g_object_ref (header);
        g_object_set_data (G_OBJECT (header), PAPI_PYOBJECT, atkobj);
    }

    result = PyObject_CallMethod (obj, "table_set_row_header", "iO",
                                  row, atkobj);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_set_column_header().
 */
static void
_table_set_column_header (AtkTable *table, gint column, AtkObject *header)
{
    PyObject *result;
    PyObject *obj;
    PyAtkObject* atkobj = NULL;

    debug ("_table_set_column_header\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);

    /* Create the PyAtkObject. */
    atkobj = g_object_get_data (G_OBJECT (header), PAPI_PYOBJECT);
    if (!atkobj)
    {
        atkobj = (PyAtkObject *)
            PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
        atkobj->obj = g_object_ref (header);
        g_object_set_data (G_OBJECT (header), PAPI_PYOBJECT, atkobj);
    }

    result = PyObject_CallMethod (obj, "table_set_column_header", "iO",
                                  column, atkobj);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_ref_at().
 */
static void
_table_set_summary (AtkTable *table, AtkObject *accessible)
{
    PyObject *result;
    PyObject *obj;
    PyAtkObject* atkobj = NULL;

    debug ("_table_set_summary\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);

    /* Create the PyAtkObject. */
    atkobj = g_object_get_data (G_OBJECT (accessible), PAPI_PYOBJECT);
    if (!atkobj)
    {
        atkobj = (PyAtkObject *)
            PyAtkObject_Type.tp_new (&PyAtkObject_Type, NULL, NULL);
        atkobj->obj = g_object_ref (accessible);
        g_object_set_data (G_OBJECT (accessible), PAPI_PYOBJECT, atkobj);
    }

    result = PyObject_CallMethod (obj, "table_set_summary", "iO", atkobj);
    Py_XDECREF (result);
}

/**
 * Wrapper for atk_table_get_selected_columns().
 */
static gint
_table_get_selected_columns (AtkTable *table, gint **selected)
{
    PyObject *result;
    PyObject *obj;
    gboolean istuple = FALSE;
    gboolean islist = FALSE;
    int amount = 0;

    debug ("_table_get_selected_columns\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_selected_columns", NULL);
    if (!result)
        return 0;

    /* Check the type. */
    istuple = PyTuple_Check (result);
    if (!istuple)
        islist = PyList_Check (result);
    if (!istuple && !islist)
        return 0;

    /* Build the list. */
    amount = (istuple) ? PyTuple_Size (result) : PyList_Size (result);
    if (amount == 0)
    {
        Py_DECREF (result);
        return 0;
    }

    *selected = malloc (amount * sizeof (gint));
    if (!selected)
    {
        Py_DECREF (result);
        return 0;
    }

    if (istuple)
        for (int i = 0; i < amount; i++)
            *selected[i] = PyInt_AsLong (PyTuple_GetItem (result, i));
    else
        for (int i = 0; i < amount; i++)
            *selected[i] = PyInt_AsLong (PyList_GetItem (result, i));

    Py_DECREF (result);
    return amount;
}

/**
 * Wrapper for atk_table_get_selected_rows().
 */
static gint
_table_get_selected_rows (AtkTable *table, gint **selected)
{
    PyObject *result;
    PyObject *obj;
    gboolean istuple = FALSE;
    gboolean islist = FALSE;
    int amount = 0;

    debug ("_table_get_selected_rows\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_get_selected_rows", NULL);
    if (!result)
        return 0;

    /* Check the type. */
    istuple = PyTuple_Check (result);
    if (!istuple)
        islist = PyList_Check (result);
    if (!istuple && !islist)
        return 0;

    /* Build the list. */
    amount = (istuple) ? PyTuple_Size (result) : PyList_Size (result);
    if (amount == 0)
    {
        Py_DECREF (result);
        return 0;
    }

    *selected = malloc (amount * sizeof (gint));
    if (!selected)
    {
        Py_DECREF (result);
        return 0;
    }

    if (istuple)
        for (int i = 0; i < amount; i++)
            *selected[i] = PyInt_AsLong (PyTuple_GetItem (result, i));
    else
        for (int i = 0; i < amount; i++)
            *selected[i] = PyInt_AsLong (PyList_GetItem (result, i));

    Py_DECREF (result);
    return amount;
}

/**
 * Wrapper for atk_table_is_column_selected().
 */
static gboolean
_table_is_column_selected (AtkTable *table, gint column)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_is_column_selected\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_is_column_selected", "i",
                                  column);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_is_row_selected().
 */
static gboolean
_table_is_row_selected (AtkTable *table, gint row)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_is_row_selected\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_is_row_selected", "i", row);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_is_selected().
 */
static gboolean
_table_is_selected (AtkTable *table, gint row, gint column)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_is_selected\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_is_selected", "ii", row, column);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_add_column_selection().
 */
static gboolean
_table_add_column_selection (AtkTable *table, gint column)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_add_column_selection\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_add_column_selection", "i",
                                  column);
    if (!result)
        return FALSE;
    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_add_row_selection().
 */
static gboolean
_table_add_row_selection (AtkTable *table, gint row)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_add_row_selection\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_add_row_selection", "i", row);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_remove_column_selection().
 */
static gboolean
_table_remove_column_selection (AtkTable *table, gint column)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_remove_column_selection\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_remove_column_selection", "i",
                                  column);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_table_remove_row_selection().
 */
static gboolean
_table_remove_row_selection (AtkTable *table, gint row)
{
    PyObject *result;
    PyObject *obj;
    gboolean retval = FALSE;

    debug ("_table_remove_row_selection\n");

    obj = g_object_get_data (G_OBJECT (table), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "table_remove_row_selection", "i", row);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Bind the different AtkTableIface interfaces to the wrapper
 * interfaces.
 */
static void
_atktableiface_init (AtkTableIface *iface)
{
    iface->ref_at = _table_ref_at;
    iface->get_index_at = _table_get_index_at;
    iface->get_column_at_index = _table_get_column_at_index;
    iface->get_row_at_index = _table_get_row_at_index;
    iface->get_n_columns = _table_get_n_columns;
    iface->get_n_rows = _table_get_n_rows;
    iface->get_column_extent_at = _table_get_column_extent_at;
    iface->get_row_extent_at = _table_get_row_extent_at;
    iface->get_caption = _table_get_caption;
    iface->get_column_description = _table_get_column_description;
    iface->get_row_description = _table_get_row_description;
    iface->get_column_header = _table_get_column_header;
    iface->get_row_header = _table_get_row_header;
    iface->get_summary = _table_get_summary;
    iface->set_caption = _table_set_caption;
    iface->set_column_description = _table_set_column_description;
    iface->set_row_description = _table_set_row_description;
    iface->set_column_header = _table_set_column_header;
    iface->set_row_header = _table_set_row_header;
    iface->set_summary = _table_set_summary;
    iface->get_selected_columns = _table_get_selected_columns;
    iface->get_selected_rows = _table_get_selected_rows;
    iface->is_column_selected = _table_is_column_selected;
    iface->is_row_selected = _table_is_row_selected;
    iface->is_selected = _table_is_selected;
    iface->add_row_selection = _table_add_row_selection;
    iface->remove_row_selection = _table_remove_row_selection;
    iface->add_column_selection = _table_add_column_selection;
    iface->remove_column_selection = _table_remove_column_selection;
}

/**
 * Gets a modified interface for the AtkTableIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atktableiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atktableiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkTable methods to the passed object instance.
 */
void
atktableiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atktableiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atktableiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atktableiface_methods[i].ml_name, func);
        i++;
    }
}
