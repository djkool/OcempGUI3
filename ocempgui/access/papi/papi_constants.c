/* $Id: papi_constants.c,v 1.7.2.4 2007/09/23 07:37:49 marcusva Exp $
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

#include <atk/atk-enum-types.h>
#include "papi.h"
#include "papi_private.h"

/**
 * Name mappings used as constants within the python module for the above
 * enumeration.
 */
static char *AtkInterfaceNames[] = 
{
    "ATK_IFACE_INVALID",
    "ATK_IFACE_ACTION",
    "ATK_IFACE_COMPONENT",
    "ATK_IFACE_DOCUMENT",
    "ATK_IFACE_EDITABLE_TEXT",
    "ATK_IFACE_HYPERTEXT",
    "ATK_IFACE_IMAGE",
    "ATK_IFACE_SELECTION",
    "ATK_IFACE_STREAMABLE",
    "ATK_IFACE_TABLE",
    "ATK_IFACE_TEXT",
    "ATK_IFACE_VALUE",
    "ATK_IFACE_HYPERLINK_IMPL"
};

static inline void
_export_constants (PyObject *module, gpointer *data)
{
    guint i;
    GEnumClass *constants = G_ENUM_CLASS (data);
    for (i = 0; i < constants->n_values; i++)
        PyModule_AddIntConstant (module, constants->values[i].value_name,
                                 constants->values[i].value);
    g_type_class_unref (data);
}

void
constants_export (PyObject *module)
{
    int j = 1;

    /* Exports the AtkRole enumeration members as constants. */
    _export_constants (module, g_type_class_ref (atk_role_get_type ()));

    /* Exports the AtkLayer enumeration members as constants. */
    _export_constants (module, g_type_class_ref (atk_layer_get_type ()));

    /* Exports the AtkRelation enumeration members as constants. */
    _export_constants (module,
                       g_type_class_ref (atk_relation_type_get_type ()));

    /* Exports the AtkState enumeration members as constants. */
    _export_constants (module, g_type_class_ref (atk_state_type_get_type ()));

    /* Exports the AtkTextClipType enumeration members as constants. */
    _export_constants (module,
                       g_type_class_ref (atk_text_clip_type_get_type ()));

    /* Exports the AtkTextBoundary enumeration members as constants. */
    _export_constants (module,
                       g_type_class_ref (atk_text_boundary_get_type ()));

    /* Exports the AtkTextAttribute enumeration members as constants. */
    _export_constants (module,
                       g_type_class_ref (atk_text_attribute_get_type ()));

    /* Exports the AtkCoordType enumeration members as constants. */
    _export_constants (module, g_type_class_ref (atk_coord_type_get_type ()));

    /* Export the used interface identifiers. */
    PyModule_AddIntConstant (module, AtkInterfaceNames[0], 0);
    for (unsigned int i = 1; i < ARRAY_AMOUNT (AtkInterfaceNames); i++)
    {
        PyModule_AddIntConstant (module, AtkInterfaceNames[i], j);
        j <<= 1;
    }
}
