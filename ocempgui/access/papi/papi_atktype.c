/* $Id: papi_atktype.c,v 1.7.2.4 2007/09/23 07:37:49 marcusva Exp $
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

/* Possible leak?
 *
 * "PapiType"            (7) 
 * + sizeof (gulong) * 2 (0xffffff...) -> 64 bit = 16, 32 bit = 8?
 * + 1                   (NUL)
 */
#define MAX_NAME_LEN (24)

static inline char* _get_type_name (AtkInterfaceType ifacenum);

/**
 * Generates an unique type name for the GType system, so that a new
 * type instance can be used to create the various AtkObject subclasses,
 * which implement different interface types.
 * In order to keep the generated type amount low which one uses the module,
 * a kind of 
 */
static inline char*
_get_type_name (AtkInterfaceType ifacenum)
{
    static char name[MAX_NAME_LEN + 1];
    sprintf (name, "%s%x", "PapiType", ifacenum);
    name[MAX_NAME_LEN] = '\0';
    return name;
}

AtkInterfaceType
atktype_get_num (GType *types)
{
    int i = 0;
    AtkInterfaceType retval = 0;

    while (types[i] != 0)
    {
        if (types[i] == ATK_TYPE_ACTION)
            retval |= IFACE_ACTION;
        else if (types[i] == ATK_TYPE_COMPONENT)
            retval |= IFACE_COMPONENT;
        else if (types[i] == ATK_TYPE_DOCUMENT)
            retval |= IFACE_DOCUMENT;
        else if (types[i] == ATK_TYPE_EDITABLE_TEXT)
            retval |= IFACE_EDITABLE_TEXT;
        else if (types[i] == ATK_TYPE_HYPERTEXT)
            retval |= IFACE_HYPERTEXT;
        else if (types[i] == ATK_TYPE_IMAGE)
            retval |= IFACE_IMAGE;
        else if (types[i] == ATK_TYPE_SELECTION)
            retval |= IFACE_SELECTION;
        else if (types[i] == ATK_TYPE_STREAMABLE_CONTENT)
            retval |= IFACE_STREAMABLE;
        else if (types[i] == ATK_TYPE_TABLE)
            retval |= IFACE_TABLE;
        else if (types[i] == ATK_TYPE_TEXT)
            retval |= IFACE_TEXT;
        else if (types[i] == ATK_TYPE_VALUE)
            retval |= IFACE_VALUE;
        else if (types[i] == ATK_TYPE_HYPERLINK_IMPL)
            retval |= IFACE_HYPERLINK_IMPL;
        i++;
    }
    return retval;
}


/* GType system. */
GType
atktype_get_type (AtkInterfaceType ifacenum)
{
    GType type;
    const char *name;

    static const GTypeInfo info =
        {
            sizeof (AtkObjectClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) NULL,
            (GClassFinalizeFunc) NULL,
            NULL, /* class data */
            sizeof (AtkObject), /* instance size */
            0, /* nb preallocs */
            (GInstanceInitFunc) NULL,
            NULL /* value table */
        };

    name = _get_type_name (ifacenum);
    type = g_type_from_name (name);
    if (type) /* Type already exists. */
        return type;

    /* Create new type. */
    type = g_type_register_static (ATK_TYPE_OBJECT, name, &info, 0);

    /* Add the interfaces. */
    if ((ifacenum & IFACE_ACTION) == IFACE_ACTION)
        g_type_add_interface_static (type, ATK_TYPE_ACTION,
                                     atkactioniface_get_info ());
    
    if ((ifacenum & IFACE_COMPONENT) == IFACE_COMPONENT)
        g_type_add_interface_static (type, ATK_TYPE_COMPONENT,
                                     atkcomponentiface_get_info ());
    
    if ((ifacenum & IFACE_DOCUMENT) == IFACE_DOCUMENT)
        g_type_add_interface_static (type, ATK_TYPE_DOCUMENT,
                                     atkdocumentiface_get_info ());
    
    if ((ifacenum & IFACE_EDITABLE_TEXT) == IFACE_EDITABLE_TEXT)
        g_type_add_interface_static (type, ATK_TYPE_EDITABLE_TEXT,
                                     atkeditableiface_get_info ());
    
    if ((ifacenum & IFACE_HYPERTEXT) == IFACE_HYPERTEXT)
        g_type_add_interface_static (type, ATK_TYPE_HYPERTEXT,
                                     atkhypertextiface_get_info ());
    
    if ((ifacenum & IFACE_IMAGE) == IFACE_IMAGE)
        g_type_add_interface_static (type, ATK_TYPE_IMAGE,
                                     atkimageiface_get_info ());
    
    if ((ifacenum & IFACE_SELECTION) == IFACE_SELECTION)
        g_type_add_interface_static (type, ATK_TYPE_SELECTION,
                                     atkselectioniface_get_info ());
    
    if ((ifacenum & IFACE_STREAMABLE) == IFACE_STREAMABLE)
        g_type_add_interface_static (type, ATK_TYPE_STREAMABLE_CONTENT,
                                     atkstreamableiface_get_info ());
    
    if ((ifacenum & IFACE_TABLE) == IFACE_TABLE)
        g_type_add_interface_static (type, ATK_TYPE_TABLE,
                                     atktableiface_get_info ());

    if ((ifacenum & IFACE_TEXT) == IFACE_TEXT)
        g_type_add_interface_static (type, ATK_TYPE_TEXT,
                                     atktextiface_get_info ());
    
    if ((ifacenum & IFACE_VALUE) == IFACE_VALUE)
        g_type_add_interface_static (type, ATK_TYPE_VALUE,
                                     atkvalueiface_get_info ());

    if ((ifacenum & IFACE_HYPERLINK_IMPL) == IFACE_HYPERLINK_IMPL)
        g_type_add_interface_static (type, ATK_TYPE_HYPERLINK_IMPL,
                                     atkhyperlinkimpliface_get_info ());

    return type;
}

void
atktype_add_interface_methods (PyObject *self, AtkInterfaceType ifacenum)
{
#ifdef DEBUG
    printf ("Adding interface methods to object: ");
    {
        AtkInterfaceType num = ifacenum;
        while (num)
        {
            printf ("%d", num % 2);
            num /= 2;
        }
    }
    printf ("\n");
#endif

    /* Register interfaces for it. */
    if ((ifacenum & IFACE_ACTION) == IFACE_ACTION)
        atkactioniface_add_methods (self);
    if ((ifacenum & IFACE_COMPONENT) == IFACE_COMPONENT)
        atkcomponentiface_add_methods (self);
    if ((ifacenum & IFACE_DOCUMENT) == IFACE_DOCUMENT)
        atkdocumentiface_add_methods (self);
    if ((ifacenum & IFACE_EDITABLE_TEXT) == IFACE_EDITABLE_TEXT)
        atkeditableiface_add_methods (self);
    if ((ifacenum & IFACE_HYPERTEXT) == IFACE_HYPERTEXT)
        atkhypertextiface_add_methods (self);
    if ((ifacenum & IFACE_IMAGE) == IFACE_IMAGE)
        atkimageiface_add_methods (self);
    if ((ifacenum & IFACE_SELECTION) == IFACE_SELECTION)
        atkselectioniface_add_methods (self);
    if ((ifacenum & IFACE_STREAMABLE) == IFACE_STREAMABLE)
        atkstreamableiface_add_methods (self);
    if ((ifacenum & IFACE_TABLE) == IFACE_TABLE)
        atktableiface_add_methods (self);
    if ((ifacenum & IFACE_TEXT) == IFACE_TEXT)
        atktextiface_add_methods (self);
    if ((ifacenum & IFACE_VALUE) == IFACE_VALUE)
        atkvalueiface_add_methods (self);
    if ((ifacenum & IFACE_HYPERLINK_IMPL) == IFACE_HYPERLINK_IMPL)
        atkhyperlinkimpliface_add_methods (self);
}
