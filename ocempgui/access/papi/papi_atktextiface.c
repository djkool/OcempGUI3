/* $Id: papi_atktextiface.c,v 1.11.2.4 2007/09/23 07:37:49 marcusva Exp $
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
static PyObject* _not_implemented_args (PyObject *self, PyObject *args);
static PyObject* _not_implemented_noargs (PyObject *self);
static gchar* _text_get_text (AtkText *text, gint start_offset,
                              gint end_offset);
static gunichar _text_get_character_at_offset (AtkText *text, gint offset);
static gchar* _text_get_text_after_offset (AtkText *text, gint offset,
                                           AtkTextBoundary boundary_type,
                                           gint *start_offset,
                                           gint *end_offset);
static gchar* _text_get_text_at_offset (AtkText *text, gint offset,
                                        AtkTextBoundary boundary_type,
                                        gint *start_offset, gint *end_offset);
static gchar* _text_get_text_before_offset (AtkText *text, gint offset,
                                            AtkTextBoundary boundary_type,
                                            gint *start_offset,
                                            gint *end_offset);
static gint _text_get_caret_offset (AtkText *text);
static void _text_get_character_extents (AtkText *text, gint offset, gint *x,
                                         gint *y, gint *width, gint *height,
                                         AtkCoordType coords);
static AtkAttributeSet* _text_get_run_attributes (AtkText *text, gint offset,
                                                  gint *start_offset,
                                                  gint *end_offset);
static AtkAttributeSet* _text_get_default_attributes (AtkText *text);
static gint _text_get_character_count (AtkText *text);
static gint _text_get_offset_at_point (AtkText *text, gint x, gint y,
                                       AtkCoordType coords);
static AtkTextRange** _text_get_bounded_ranges  (AtkText *text,
                                                 AtkTextRectangle *rect,
                                                 AtkCoordType coord_type,
                                                 AtkTextClipType x_clip_type,
                                                 AtkTextClipType y_clip_type);
static void _text_get_range_extents (AtkText *text, gint start_offset,
                                     gint end_offset, AtkCoordType coord_type,
                                     AtkTextRectangle *rect);
static gint _text_get_n_selections (AtkText *text);
static gchar* _text_get_selection (AtkText *text, gint selection_num,
                                   gint *start_offset, gint *end_offset);
static gboolean _text_add_selection (AtkText *text, gint start_offset,
                                     gint end_offset);
static gboolean _text_remove_selection (AtkText *text, gint selection_num);
static gboolean _text_set_selection (AtkText *text, gint selection_num,
                                     gint start_offset, gint end_offset);
static gboolean _text_set_caret_offset (AtkText *text, gint offset);

/**
 * Interface methods, which are bound to the object at runtime.
 */
static PyMethodDef _atktextiface_methods[] =
{
    { "text_get_text", _not_implemented_args, METH_VARARGS,
      "O.text_get_text (start, end) -> string\n\n"
      "Gets the specified text.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_character_at_offset", _not_implemented_args, METH_VARARGS,
      "O.text_get_character_at_offset (offset) -> unicode\n\n"
      "Gets the specified character.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_text_after_offset", _not_implemented_args, METH_VARARGS,
      "O.text_get_text_after_offset (offset, boundary_type) -> string, int, "
      "int\n\n"
      "Gets the specified text after the wanted offset.\n\n"
      "If the boundary_type if ATK_TEXT_BOUNDARY_CHAR the character after\n "
      "the offset is returned.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_START the returned\n"
      "string is from the word start after the offset to the next word start.\n"
      "The returned string will contain the word after the offset if the\n"
      "offset is inside a word or if the offset is not inside a word.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_END the returned\n"
      "string is from the word end at or after the offset to the next word\n"
      "end.\n"
      "The returned string will contain the word after the offset if the\n"
      "offset is inside a word and will contain the word after the word\n"
      "after the offset if the offset is not inside a word.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_SENTENCE_START the returned\n"
      "string is from the sentence start after the offset to the next\n"
      "sentence start.\n"
      "The returned string will contain the sentence after the offset if the\n"
      "offset is inside a sentence or if the offset is not inside a\n"
      "sentence.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_SENTENCE_END the returned\n"
      "string is from the sentence end at or after the offset to the next\n"
      "sentence end.\n"
      "The returned string will contain the sentence after the offset if the\n"
      "offset is inside a sentence and will contain the sentence after the\n"
      "sentence after the offset if the offset is not inside a sentence.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_LINE_START the returned\n"
      "string is from the line start after the offset to the next line start.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_LINE_END the returned\n"
      "string is from the line end at or after the offset to the next line\n"
      "start.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_text_at_offset", _not_implemented_args, METH_VARARGS,
      "O.text_get_text_at_offset (offset, boundary_type) -> string, int,"
      "int\n\n"
      "Gets the specified text at the offset.\n\n"
      "If the boundary_type if ATK_TEXT_BOUNDARY_CHAR the character at the\n"
      "offset is returned.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_START the returned\n"
      "string is from the word start at or before the offset to the word\n"
      "start after the offset.\n"
      "The returned string will contain the word at the offset if the offset\n"
      "is inside a word and will contain the word before the offset if the\n"
      "offset is not inside a word.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_END the returned\n"
      "string is from the word end before the offset to the word end at or\n"
      "after the offset.\n"
      "The returned string will contain the word at the offset if the offset\n"
      "is inside a word and will contain the word after to the offset if the\n"
      "offset is not inside a word.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_SENTENCE_START the returned\n"
      "string is from the sentence start at or before the offset to the\n"
      "sentence start after the offset.\n"
      "The returned string will contain the sentence at the offset if the\n"
      "offset is inside a sentence and will contain the sentence before the\n"
      "offset if the offset is not inside a sentence.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_SENTENCE_END the returned\n"
      "string is from the sentence end before the offset to the sentence end\n"
      "at or after the offset.\n"
      "The returned string will contain the sentence at the offset if the\n"
      "offset is inside a sentence and will contain the sentence after the\n"
      "offset if the offset is not inside a sentence.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_LINE_START the returned\n"
      "string is from the line start at or before the offset to the line\n"
      "start after the offset.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_LINE_END the returned\n"
      "string is from the line end before the offset to the line end at or\n"
      "after the offset.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_text_before_offset", _not_implemented_args, METH_VARARGS,
      "O.text_get_text_before_offset (offset, boundary_type) -> string, int,"
      "int\n\n"
      "Gets the specified text before the offset.\n\n"
      "If the boundary_type if ATK_TEXT_BOUNDARY_CHAR the character before\n"
      "the offset is returned.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_START the returned\n"
      "string is from the word start before the word start before the offset\n"
      "to the word start before the offset.\n"
      "The returned string will contain the word before the offset if the\n"
      "offset is inside a word and will contain the word before the word\n"
      "before the offset if the offset is not inside a word.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_WORD_END the returned\n"
      "string is from the word end before the word end at or before the\n"
      "offset to the word end at or before the offset.\n"
      "The returned string will contain the word before the offset if the\n"
      "offset is inside a word or if the offset is not inside a word.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_SENTENCE_START the returned\n"
      "string is from the sentence start before the sentence start before\n"
      "the offset to the sentence start before the offset.\n"
      "The returned string will contain the sentence before the offset if\n"
      "the offset is inside a sentence and will contain the sentence before\n"
      "the sentence before the offset if the offset is not inside a\n"
      "sentence.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_SENTENCE_END the returned\n"
      "string is from the sentence end before the sentence end at or before\n"
      "the offset to the sentence end at or before the offset.\n"
      "The returned string will contain the sentence before the offset if\n"
      "the offset is inside a sentence or if the offset is not inside a\n"
      "sentence.\n"
      "If the boundary type is ATK_TEXT_BOUNDARY_LINE_START the returned\n"
      "string is from the line start before the line start ar or before the\n"
      "offset to the line start ar or before the offset.\n"
      "If the boundary_type is ATK_TEXT_BOUNDARY_LINE_END the returned\n"
      "string is from the line end before the line end before the offset to\n"
      "the line end before the offset.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_caret_offset", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.text_get_caret_offset () -> int\n\n"
      "Gets the offset position of the caret (cursor).\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_set_caret_offset", _not_implemented_args, METH_VARARGS,
      "O.text_set_caret_offset (offset) -> bool\n\n"
      "Sets the caret (cursor) position to the specified offset.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_character_extents", _not_implemented_args, METH_VARARGS,
      "O.text_get_character_extents (offset, coords) -> x, y, width, height\n\n"
      "Gets the bounding box around the character at the offset.\n\n"
      "Get the bounding box containing the glyph representing the character "
      "at a particular text offset.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_run_attributes", _not_implemented_args, METH_VARARGS,
      "O.text_get_run_attributes (offset) -> AtkAttributeSet, int, int\n\n"
      "Creates an attribute set which consists of the attributes explicitly\n"
      "set at the position offset in the text. start_offset and end_offset\n"
      "are set to the start and end of the range around offset where the\n"
      "attributes are invariant. Note that end_offset is the offset of the\n"
      "first character after the range. See the enum AtkTextAttribute for\n"
      "types of text attributes that can be returned. Note that other\n"
      "attributes may also be returned.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_default_attributes", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.text_get_default_attributes () -> AtkAttributeSet\n\n"
      "Returns an AtkAttributeSet with the default attributes.\n\n"
      "Creates an attribute set which consists of the default values\n"
      "values of attributes for the text. See the enum AtkTextAttribute for\n"
      "types of text attributes that can be returned. Note that other\n"
      "attributes may also be returned.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_character_count", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.text_get_character_count () -> int\n\n"
      "Gets the character count.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_offset_at_point", _not_implemented_args, METH_VARARGS,
      "O.text_get_offset_at_point (x, y, coords) -> int\n\n"
      "Gets the offset of the character located at coordinates x and y.\n\n"
      "x and y are interpreted as being relative to the screen or this\n"
      "widget's window depending on coords.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_bounded_ranges", _not_implemented_args, METH_VARARGS,
      "text_get_bounded_ranges (rect, coords, x_clip, y_clip) -> list\n\n"
      "Get the ranges of text in the specified bounding box." },
    { "text_get_range_extents", _not_implemented_args, METH_VARARGS,
      "text_get_range_extents (start, end, coords) -> AtkTextRectangle\n\n"
      "Get the bounding box for text within the specified range.\n\n" },
    { "text_get_n_selections", (PyCFunction) _not_implemented_noargs,
      METH_NOARGS,
      "O.text_get_n_selections () -> int\n\n"
      "Gets the number of selected regions.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_get_selection", (PyCFunction) _not_implemented_noargs, METH_NOARGS,
      "O.text_get_selection (selection) -> string, int, int\n\n"
      "Gets the text from the specified selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_set_selection", _not_implemented_args, METH_VARARGS,
      "O.text_set_selection (selection, start, end) -> bool\n\n"
      "Changes the start and end offset of the specified selection.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_add_selection", _not_implemented_args, METH_VARARGS,
      "O.text_add_selection (start, end) -> bool\n\n"
      "Adds a selection bounded by the specified offsets.\n\n"
      "This method has to be implemented by inherited classes." },
    { "text_remove_selection", _not_implemented_args, METH_VARARGS,
      "O.text_remove_selection (selection) -> bool\n\n"
      "Removes the specified selection.\n\n"
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
 * Wrapper for atk_text_get_text().
 */
static gchar*
_text_get_text (AtkText *text, gint start_offset, gint end_offset)
{
    PyObject *obj;
    PyObject *result;
    gchar *retval = NULL;

    debug ("_text_get_text\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_text", "ii", start_offset,
                                  end_offset);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return g_strdup (retval);
}

/**
 * Wrapper for atk_text_get_character_at_offset().
 */
static gunichar
_text_get_character_at_offset (AtkText *text, gint offset)
{
    PyObject *obj;
    PyObject *result;
    gunichar retval = 0;

    debug ("_text_get_character_at_offset\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_character_at_offset", "i",
                                  offset);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_text_after_offset().
 */
static gchar*
_text_get_text_after_offset (AtkText *text, gint offset,
                             AtkTextBoundary boundary_type, gint *start_offset,
                             gint *end_offset)
{
    PyObject *obj;
    PyObject *result;
    gchar *retval = NULL;
    
    debug ("_text_get_text_after_offset\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_text_after_offset", "iiii",
                                  offset, boundary_type, start_offset,
                                  end_offset);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return g_strdup (retval);
}

/**
 * Wrapper for atk_text_get_text_at_offset().
 */
static gchar*
_text_get_text_at_offset (AtkText *text, gint offset,
                          AtkTextBoundary boundary_type, gint *start_offset,
                          gint *end_offset)
{
    PyObject *obj;
    PyObject *result;
    gchar *retval = NULL;

    debug ("_text_get_text_at_offset\n");
    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_text_at_offset", "iiii",
                                  offset, boundary_type, start_offset,
                                  end_offset);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return g_strdup (retval);
}

/**
 * Wrapper for atk_text_get_text_before_offset().
 */
static gchar*
_text_get_text_before_offset (AtkText *text, gint offset,
                              AtkTextBoundary boundary_type,
                              gint *start_offset, gint *end_offset)
{
    PyObject *obj;
    PyObject *result;
    gchar *retval = NULL;

    debug ("_text_get_text_before_offset\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_text_before_offset", "iiii",
                                  offset, boundary_type, start_offset,
                                  end_offset);
    if (!result)
        return NULL;

    retval = PyString_AsString (result);
    Py_DECREF (result);
    return g_strdup (retval);
}

/**
 * Wrapper for atk_text_get_caret_offset().
 */
static gint
_text_get_caret_offset (AtkText *text)
{
    PyObject *obj;
    PyObject *result;
    gint retval = 0;

    debug ("_text_get_caret_offset\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_caret_offset", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_character_extents().
 */
static void
_text_get_character_extents (AtkText *text, gint offset, gint *x, gint *y,
                             gint *width, gint *height, AtkCoordType coords)
{
    PyObject *obj;
    PyObject *val;
    PyObject *result;

    debug ("_text_get_character_extents\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_character_extents", "ii",
                                  offset, coords);
    *x = 0;
    *y = 0;
    *width = 0;
    *height = 0;

    if (result)
    {
        if (PySequence_Check (result))
            if (PySequence_Size (result) >= 4)
            {
                val = PySequence_GetItem (result, 0);
                *x = PyInt_AsLong (val);
                Py_DECREF (val);

                val = PySequence_GetItem (result, 1);
                *y = PyInt_AsLong (val);
                Py_DECREF (val);

                val = PySequence_GetItem (result, 2);
                *width = PyInt_AsLong (val);
                Py_DECREF (val);

                val = PySequence_GetItem (result, 3);
                *height = PyInt_AsLong (val);
                Py_DECREF (val);
            }
        Py_DECREF (result);
    }
}

/**
 * Wrapper for atk_text_get_run_attributes().
 */
static AtkAttributeSet*
_text_get_run_attributes (AtkText *text, gint offset, gint *start_offset,
                          gint *end_offset)
{
    PyObject *obj;
    PyObject *result;
    PyObject *attr;
    int amount = 0;
    AtkAttribute *at;
    AtkAttributeSet *retval = NULL;

    debug ("_text_get_run_attributes\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_run_attributes", "i", offset);

    if (result)
    {
        if (PySequence_Check (result))
        {
            amount = PySequence_Size (result);
            if (amount >= 3) /* The first two items should be the offsets. */
            {
                attr = PySequence_GetItem (result, 0);
                *start_offset = PyInt_AsLong (attr);
                Py_DECREF (attr);

                attr = PySequence_GetItem (result, 1);
                *end_offset = PyInt_AsLong (attr);
                Py_DECREF (attr);

                /* Walk through the rest of list. */
                for (int i = 2; i < amount; i++)
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
                    at->value =
                        g_strdup (((PyAtkAttribute *) attr)->obj->value);
                    retval = g_slist_append (retval, at);

                    Py_DECREF (attr);
                }
            }
        }
        Py_DECREF (result);
    }
    return retval;
}

/**
 * Wrapper for atk_text_get_default_attributes().
 */
static AtkAttributeSet*
_text_get_default_attributes (AtkText *text)
{
    PyObject *obj;
    PyObject *result;
    PyObject *attr;
    int amount = 0;
    AtkAttribute *at;
    AtkAttributeSet *retval = NULL;

    debug ("_text_get_run_attributes\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_default_attributes", NULL);

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
 * Wrapper for atk_text_get_character_count().
 */
static gint
_text_get_character_count (AtkText *text)
{
    PyObject *obj;
    PyObject *result;
    int retval = 0;

    debug ("_text_get_character_count\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_character_count", NULL);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_offset_at_point().
 */
static gint
_text_get_offset_at_point (AtkText *text, gint x, gint y, AtkCoordType coords)
{
    PyObject *obj;
    PyObject *result;
    int retval = 0;

    debug ("_text_get_offset_at_point\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_offset_at_point", "iii", x, y,
                                  coords);
    if (!result)
        return 0;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_n_selections().
 */
static gint
_text_get_n_selections (AtkText *text)
{
    PyObject *obj;
    PyObject *result;
    gint retval = -1;

    debug ("_text_get_n_selections\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_n_selections", NULL);
    if (!result)
        return -1;

    retval = PyInt_AsLong (result);
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_selection().
 */
static gchar*
_text_get_selection (AtkText *text, gint selection_num, gint *start_offset,
                     gint *end_offset)
{
    PyObject *obj;
    PyObject *result;
    PyObject *val;
    gchar *retval = NULL;

    debug ("_text_get_selection\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_selection", "i",
                                  selection_num);
    if (!result)
        return NULL;

    if (PySequence_Check (result))
        if (PySequence_Size (result) >= 3)
        {
            val = PySequence_GetItem (result, 0);
            retval = PyString_AsString (val);
            Py_DECREF (val);

            val = PySequence_GetItem (result, 1);
            *start_offset = PyInt_AsLong (val);
            Py_DECREF (val);

            val = PySequence_GetItem (result, 2);
            *end_offset = PyInt_AsLong (val);
            Py_DECREF (val);
        }
    Py_DECREF (result);
    return g_strdup (retval);
}

/**
 * Wrapper for atk_text_add_selection().
 */
static gboolean
_text_add_selection (AtkText *text, gint start_offset, gint end_offset)
{
    PyObject *obj;
    PyObject *result;
    gboolean retval = FALSE;

    debug ("_text_add_selection\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_add_selection", "ii",
                                  start_offset, end_offset);
    if (!result)
        return FALSE;
    
    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_remove_selection().
 */
static gboolean
_text_remove_selection (AtkText *text, gint selection_num)
{
    PyObject *obj;
    PyObject *result;
    gboolean retval = FALSE;
    
    debug ("_text_remove_selection\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_remove_selection", "i",
                                  selection_num);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_set_selection().
 */
static gboolean
_text_set_selection (AtkText *text, gint selection_num, gint start_offset,
                     gint end_offset)
{
    PyObject *obj;
    PyObject *result;
    gboolean retval = FALSE;

    debug ("_text_set_selection\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_set_selection", "iii",
                                  selection_num, start_offset, end_offset);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_set_caret_offset().
 */
static gboolean
_text_set_caret_offset (AtkText *text, gint offset)
{
    PyObject *obj;
    PyObject *result;
    gboolean retval = FALSE;

    debug ("_text_set_caret_offset\n");

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_set_caret_offset", "i", offset);
    if (!result)
        return FALSE;

    retval = result == Py_True;
    Py_DECREF (result);
    return retval;
}

/**
 * Wrapper for atk_text_get_bounded_ranges().
 */
static AtkTextRange**
_text_get_bounded_ranges (AtkText *text, AtkTextRectangle *rect,
                          AtkCoordType coord_type, AtkTextClipType x_clip_type,
                          AtkTextClipType y_clip_type)
{
    PyObject *obj;
    PyObject *result;
    PyObject *cur;
    PyAtkTextRectangle *prect;
    AtkTextRange** range = NULL;
    int amount = 0;

    debug ("_text_get_bounded_ranges\n");

    prect = (PyAtkTextRectangle *)
        PyAtkTextRectangle_Type.tp_new (&PyAtkTextRectangle_Type, NULL, NULL);
    prect->obj->x = rect->x;
    prect->obj->y = rect->x;
    prect->obj->width = rect->width;
    prect->obj->height = rect->height;

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_bounded_ranges", "oiii", prect,
                                  coord_type, x_clip_type, y_clip_type);
    Py_DECREF (prect);

    if (!result)
        return NULL;

    if (PyList_Check (result))
    {
        amount = PyList_Size (result);
        range = g_new (AtkTextRange*, amount + 1);

        for (int i = 0; i < amount; i++)
        {
            cur = PyList_GetItem (result, i);
            if (!PyObject_TypeCheck (cur, &PyAtkTextRange_Type))
            {
                atk_text_free_ranges (range);
                Py_DECREF (result);
                return NULL;
            }
            range[i] = g_new (AtkTextRange, 1);
            range[i]->bounds = ATKTEXTRANGE (cur)->bounds;
            range[i]->start_offset = ATKTEXTRANGE (cur)->start_offset;
            range[i]->end_offset = ATKTEXTRANGE (cur)->end_offset;
            range[i]->content = g_strdup (ATKTEXTRANGE (cur)->content);
        }
        range[amount + 1] = NULL;
    }
    else if (PyTuple_Check (result))
    {
        amount = PyTuple_Size (result);
        range = g_new (AtkTextRange*, amount + 1);

        for (int i = 0; i < amount; i++)
        {
            cur = PyTuple_GetItem (result, i);
            if (!PyObject_TypeCheck (obj, &PyAtkTextRange_Type))
            {
                atk_text_free_ranges (range);
                Py_DECREF (result);
                return NULL;
            }
            range[i] = g_new (AtkTextRange, 1);
            range[i]->bounds = ATKTEXTRANGE (cur)->bounds;
            range[i]->start_offset = ATKTEXTRANGE (cur)->start_offset;
            range[i]->end_offset = ATKTEXTRANGE (cur)->end_offset;
            range[i]->content = g_strdup (ATKTEXTRANGE (cur)->content);
        }
        range[amount + 1] = NULL;
    }

    Py_DECREF (result);
    return range;
}

/**
 * Wrapper for atk_text_get_range_extents().
 */
static void
_text_get_range_extents (AtkText *text, gint start_offset, gint end_offset,
                         AtkCoordType coord_type, AtkTextRectangle *rect)
{
    PyObject *result;
    PyObject *obj;

    obj = g_object_get_data (G_OBJECT (text), PAPI_PYOBJECT);
    result = PyObject_CallMethod (obj, "text_get_range_extents", "iii",
                                  start_offset, end_offset, coord_type);
    if (!result)
        return;

    if (PyObject_TypeCheck (result, &PyAtkTextRectangle_Type))
    {
        rect->x = ATKTEXTRECTANGLE (result)->x;
        rect->y = ATKTEXTRECTANGLE (result)->y;
        rect->width = ATKTEXTRECTANGLE (result)->width;
        rect->height = ATKTEXTRECTANGLE (result)->height;
    }

    Py_DECREF (result);
    return;
}

/**
 * Bind the different AtkTextIface interfaces to the wrapper
 * interfaces.
 */
static void
_atktextiface_init (AtkTextIface *iface)
{
    iface->get_text = _text_get_text;
    iface->get_text_after_offset = _text_get_text_after_offset;
    iface->get_text_at_offset = _text_get_text_at_offset;
    iface->get_character_at_offset = _text_get_character_at_offset;
    iface->get_text_before_offset = _text_get_text_before_offset;
    iface->get_caret_offset = _text_get_caret_offset;
    iface->get_run_attributes = _text_get_run_attributes;
    iface->get_default_attributes = _text_get_default_attributes;
    iface->get_character_extents = _text_get_character_extents;
    iface->get_character_count = _text_get_character_count;
    iface->get_offset_at_point = _text_get_offset_at_point;
    iface->get_n_selections = _text_get_n_selections;
    iface->get_selection = _text_get_selection;
    iface->add_selection = _text_add_selection;
    iface->remove_selection = _text_remove_selection;
    iface->set_selection = _text_set_selection;
    iface->set_caret_offset = _text_set_caret_offset;
    iface->get_range_extents = _text_get_range_extents;
    iface->get_bounded_ranges = _text_get_bounded_ranges;
}

/**
 * Gets a modified interface for the AtkTextIface, which uses the
 * python wrappers as interfaces.
 */
const GInterfaceInfo*
atktextiface_get_info (void)
{
    static const GInterfaceInfo info =
        {
            (GInterfaceInitFunc) _atktextiface_init,
            (GInterfaceFinalizeFunc) NULL,
            NULL
        };
    return &info;
}

/**
 * Binds the AtkText methods to the passed object instance.
 */
void
atktextiface_add_methods (PyObject *self)
{
    int i = 0;
    PyObject *func;
    
    while (_atktextiface_methods[i].ml_name != NULL)
    {
        func = PyCFunction_New (&_atktextiface_methods[i], NULL);
        PyObject_SetAttrString (self, _atktextiface_methods[i].ml_name, func);
        i++;
    }
}
