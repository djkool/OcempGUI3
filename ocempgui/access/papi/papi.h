/* $Id: papi.h,v 1.6.2.4 2007/09/23 07:37:48 marcusva Exp $
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

#ifndef _PAPI_H_
#define _PAPI_H_

#include <Python.h>
#include <gmodule.h>
#include <atk/atk.h>

/**
 * PyObject type, that encapsulates an AtkObject.
 */
typedef struct
{
    PyObject_HEAD
    AtkObject *obj;
    PyObject *dict;
    PyObject *prop_handlers;
    PyObject *weakrefs;
} PyAtkObject;

#define ATKOBJECT(x) (ATK_OBJECT (((PyAtkObject *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkStateSet.
 */
typedef struct
{
    PyObject_HEAD
    AtkStateSet *obj;
} PyAtkStateSet;
#define ATKSTATESET(x) (ATK_STATE_SET (((PyAtkStateSet *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkRelation.
 */
typedef struct
{
    PyObject_HEAD
    AtkRelation *obj;
} PyAtkRelation;
#define ATKRELATION(x) (ATK_RELATION (((PyAtkRelation *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkRelationSet.
 */
typedef struct
{
    PyObject_HEAD
    AtkRelationSet *obj;
} PyAtkRelationSet;
#define ATKRELATIONSET(x) (ATK_RELATION_SET (((PyAtkRelationSet *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkHyperlink.
 */
typedef struct
{
    PyObject_HEAD
    AtkHyperlink *obj;
} PyAtkHyperlink;
#define ATKHYPERLINK(x) (ATK_HYPERLINK (((PyAtkHyperlink *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkAttribute.
 */
typedef struct
{
    PyObject_HEAD
    AtkAttribute *obj;
} PyAtkAttribute;
#define ATKATTRIBUTE(x) (ATK_ATTRIBUTE (((PyAtkAttribute *) x)->obj))

/**
 * PyObject type, that encapsulates an AtkTextRectangle.
 */
typedef struct
{
    PyObject_HEAD
    AtkTextRectangle *obj;
} PyAtkTextRectangle;
#define ATKTEXTRECTANGLE(x) ((AtkTextRectangle *) \
                             ((PyAtkTextRectangle *) x)->obj)

/**
 * PyObject type, that encapsulates an AtkTextRange.
 */
typedef struct
{
    PyObject_HEAD
    AtkTextRange *obj;
} PyAtkTextRange;
#define ATKTEXTRANGE(x) ((AtkTextRange *) ((PyAtkTextRange *) x)->obj)

/**
 * Python module initialization function. This will be called upon the
 * import <module> statement and sets up the papi module.
 */
PyMODINIT_FUNC initpapi (void);

#endif /* _PAPI_H_ */
