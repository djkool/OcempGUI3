/* $Id: papi_private.h,v 1.15.2.5 2007/09/23 07:37:49 marcusva Exp $
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

#ifndef _PAPI_PRIVATE_H_
#define _PAPI_PRIVATE_H_

#include <Python.h>
#include <atk/atk.h>

#ifdef DEBUG
#define debug(x) printf("DEBUG: File %s, Line %d: %s", __FILE__, __LINE__, x)
#else
#define debug(x)
#endif

/* Backwards compatibility with Python 2.3. */
#if PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION == 3
#  define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#  define Py_RETURN_TRUE return Py_INCREF(Py_True), Py_True
#  define Py_RETURN_FALSE return Py_INCREF(Py_False), Py_False
#endif

#define ARRAY_AMOUNT(x) (sizeof (x) / sizeof ((x)[0]))

/**
 * Identifier flag for the get_n_accessible_children method.  As the
 * AtkObjectclass->get_n_children interface receives an AtkObject, we
 * cannot simply use an encapsulation _without_ using the GType system.
 * We do not use the GType system, thus we bind the information directly
 * to the AtkObject and use the g_object_set/get_data functions on it.
 */
#define PAPI_CHILDREN "PAPI_CHILDREN"

/**
 * We have to manage a reference to the encapsulating PyAtkObject in our
 * wrapped AtkObject, so that interfaces and methods, which receive an
 * AtkObject as argument, can directly interact with the PyAtkObject, if
 * necessary. This is esp. useful for the interface bindings, where
 * users have to implement the interface methods as python methods,
 * which we will invoke then.
 */
#define PAPI_PYOBJECT "PAPI_PYOBJECT"

/**
 * Interface types used by the AtkObject resolution system to create
 * interface enabled GTypes on the fly.
 * See the GType related function in papi_atkobject.c
 * papi_constants.c contains the names for exposing the constants.
 */
typedef enum
{
    IFACE_INVALID        = 0,       /* Invalid interface identifier. */
    IFACE_ACTION         = 1 << 0,  /* Identifies the AtkAction interface. */
    IFACE_COMPONENT      = 1 << 1,  /* Identifies the AtkComponent interface. */
    IFACE_DOCUMENT       = 1 << 2,  /* Identifies the AtkDocument interface. */
    IFACE_EDITABLE_TEXT  = 1 << 3,  /* Identifies the AtkEditableText
                                     * interface. */
    IFACE_HYPERTEXT      = 1 << 4,  /* Identifies the AtkHypertext interface. */
    IFACE_IMAGE          = 1 << 5,  /* Identifies the AtkImage interface. */
    IFACE_SELECTION      = 1 << 6,  /* Identifies the AtkSelection interface. */
    IFACE_STREAMABLE     = 1 << 7,  /* Identifies the AtkStreamableContent
                                     * interface. */
    IFACE_TABLE          = 1 << 8,  /* Identifies the AtkTable interface. */
    IFACE_TEXT           = 1 << 9,  /* Identifies the AtkText interface. */
    IFACE_VALUE          = 1 << 10, /* Identifies the AtkValue interface. */
    IFACE_HYPERLINK_IMPL = 1 << 11  /* Identifies the AtkHyperlinkImpl
                                    * interface. */
} AtkInterfaceType;

/**
 * Object types used by the module.
 */
extern PyTypeObject PyAtkObject_Type;
extern PyTypeObject PyAtkStateSet_Type;
extern PyTypeObject PyAtkRelation_Type;
extern PyTypeObject PyAtkRelationSet_Type;
extern PyTypeObject PyAtkHyperlink_Type;
extern PyTypeObject PyAtkAttribute_Type;
extern PyTypeObject PyAtkTextRectangle_Type;
extern PyTypeObject PyAtkTextRange_Type;

/**
 * Different functions, which will export the globally available ATK
 * functions into the python module.
 */
void atkrelationtype_export_funcs (PyObject *module);
void atkrole_export_funcs (PyObject *module);
void atkstatetype_export_funcs (PyObject *module);
void atktextattribute_export_funcs (PyObject *module);
void atkutil_export_funcs (PyObject *module);
void atkbridge_export_funcs (PyObject *module);

/**
 * Creates constants for the python module from the different ATK
 * enumerations.
 */
void constants_export (PyObject *module);

/**
 * Globally used classbinding functions. As ATK forces implementors to
 * override various interfaces of it, those are used to do that job and
 * to point to the overriden or encapsulated interfaces.
 */
PyObject *atkutil_get_listeners (void);
guint atkutil_lookup_signal (const char *name);
void atkobjectclass_init (AtkObjectClass *class);
void atkutilclass_init (AtkUtilClass *class);
int atkutil_root_satisfied (void);

/**
 * Functions to add the different available ATK interfaces to the
 * underlying AtkObject instances and to the Python object wrapper.
 */
AtkInterfaceType atktype_get_num (GType *types);
GType atktype_get_type (AtkInterfaceType ifacenum);
void atktype_add_interface_methods (PyObject *self, AtkInterfaceType ifacenum);

/**
 * Creates PyObject instances fom GValue types. Defined in
 * papi_atkvalueiface.c
 */
PyObject *pyobject_from_gvalue (GValue *value);

/**
 * Retrieval functions for various Atk interfaces, which are called, when
 * new AtkObjects are instantiated using the get_papi_atk_type() call within
 * the PyAtkObject init call.
 */
const GInterfaceInfo *atkactioniface_get_info (void);
void atkactioniface_add_methods (PyObject *self);

const GInterfaceInfo *atkcomponentiface_get_info (void);
void atkcomponentiface_add_methods (PyObject *self);

const GInterfaceInfo *atkdocumentiface_get_info (void);
void atkdocumentiface_add_methods (PyObject *self);

const GInterfaceInfo *atkeditableiface_get_info (void);
void atkeditableiface_add_methods (PyObject *self);

const GInterfaceInfo *atkhypertextiface_get_info (void);
void atkhypertextiface_add_methods (PyObject *self);

const GInterfaceInfo *atkimageiface_get_info (void);
void atkimageiface_add_methods (PyObject *self);

const GInterfaceInfo *atkselectioniface_get_info (void);
void atkselectioniface_add_methods (PyObject *self);

const GInterfaceInfo *atkstreamableiface_get_info (void);
void atkstreamableiface_add_methods (PyObject *self);

const GInterfaceInfo *atktableiface_get_info (void);
void atktableiface_add_methods (PyObject *self);

const GInterfaceInfo *atktextiface_get_info (void);
void atktextiface_add_methods (PyObject *self);

const GInterfaceInfo *atkvalueiface_get_info (void);
void atkvalueiface_add_methods (PyObject *self);

const GInterfaceInfo *atkhyperlinkimpliface_get_info (void);
void atkhyperlinkimpliface_add_methods (PyObject *self);

#endif /* _PAPI_PRIVATE_H_ */
