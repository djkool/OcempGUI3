# $Id: String.py,v 1.12.4.5 2007/01/22 20:00:33 marcusva Exp $
#
# Copyright (c) 2004-2007, Marcus von Appen
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# TODO: make font caching much more elegant

"""String drawing functions with font caching capabilities."""

from pygame import font as PygameFont
from .Constants import *

__font_cache = {}

def create_file_font (fontfile, size, style=FONT_STYLE_NORMAL):
    """create_file_font (...) -> Font

    Creates a new font from a given font file.

    The 'fontfile' is the path to a font file or a python file-like
    object. The resulting pygame.Font will have the specified height
    'size' in pixels. Additional styles can be passed to give the font a
    bold, italic, or underlined style. The 'styles' attribute must be a
    valid combination of the FONT_STYLE_TYPES.
    
    The font will be cached internally, so it can be reused without
    creating it again and again. This also means, that any operation,
    which will modify the pygame.Font() object, will also be applied to
    the cached one. It is possible to avoid this by copying the font
    object using font.copy().

    The following example will try to load a font with a height of 14
    pixels from a subdirectory:

    create_file_font ('fonts/MyFont.ttf', 14)

    Raises a TypeError, if the 'style' argument is not a valid FONT_STYLE_TYPES
    value.
    """
    global __font_cache
    retval = None

    if (style != FONT_STYLE_NORMAL) and not constants_is_font_style (style):
        raise TypeError ("style must be a value from FONT_STYLE_TYPES")
    
    # Try to clean up after 30 fonts, so we get rid of the old ones.
    if len (list(__font_cache.items ())) > 30:
        __font_cache.clear ()

    if (fontfile, size, style) not in __font_cache:
        retval = PygameFont.Font (fontfile, size)
        __font_cache[(fontfile, size, style)] = retval
        apply_font_style (retval, style)
        return retval
    else:
        return __font_cache[(fontfile, size, style)]

def create_system_font (fontname, size, style=FONT_STYLE_NORMAL):
    """create_system_font (...) -> Font

    Creates a new font from a given font name of the system fonts.

    The 'fontname' is a valid system font name of an installed font.
    The resulting pygame.Font will have the specified height 'size' in
    pixels. Additional styles can be passed to give the font a
    bold, italic, or underlined style. The 'styles' attribute must be a
    valid combination of the FONT_STYLE_TYPES.
    
    The font will be cached internally, so it can be reused without
    creating it again and again. This also means, that any operation,
    which will modify the pygame.Font() object, will also be applied to
    the cached one. It is possible to avoid this by copying the font
    object using font.copy().
    This will always return a valid Font object, and will fallback on
    the builtin pygame font if the given font is not found.
    
    The following example will try to load a font with a height of 14:

    create_system_font ('Helvetica', 14)

    Raises a TypeError, if the 'style' argument is not a valid FONT_STYLE_TYPES
    value.
    """
    global __font_cache
    retval = None

    if (style != FONT_STYLE_NORMAL) and not constants_is_font_style (style):
        raise TypeError ("style must be a value from FONT_STYLE_TYPES")
    bold = style & FONT_STYLE_BOLD == FONT_STYLE_BOLD
    italic = style & FONT_STYLE_ITALIC == FONT_STYLE_ITALIC
    underline = style & FONT_STYLE_UNDERLINE == FONT_STYLE_UNDERLINE

    # Try to clean up after 30 fonts, so we get rid of the old ones.
    if len (list(__font_cache.items ())) > 30:
        __font_cache.clear ()

    if (fontname, size, style) not in __font_cache:
        retval = PygameFont.SysFont (fontname, size, bold, italic)
        __font_cache[(fontname, size, style)] = retval
        retval.set_underline (underline)
        return retval
    else:
        return __font_cache[(fontname, size, style)]

def create_font (font, size, style=FONT_STYLE_NORMAL):
    """create_font (...) -> Font

    Wraps the create_file_font() and create_system_font() methods.

    This function tries to create a font using create_file_font() and
    calls create_system_font() upon failure.
    """
    fnt = None
    try:
        fnt = create_file_font (font, size, style)
    except IOError: # Could not find font file.
        fnt = create_system_font (font, size, style)
    return fnt

def draw_string (text, font, size, antialias, color, style=FONT_STYLE_NORMAL):
    """draw_string (...) -> Surface
    
    Creates a surface displaying a string.

    'text' is the text, which should be renderered to a surface. 'font'
    can be any valid font name of the system fonts or a font file (see
    the note). The 'size' is the height of the font in pixels, 'alias'
    is an integer value, which enables or disables antialiasing of the
    font, and 'color' is a pygame color style argument for the
    text. Additional styles can be passed to give the font a bold,
    italic, or underlined style. The 'styles' attribute must be a valid
    combination of the FONT_STYLE_TYPES.

    The following example will create an antialiased string surface
    with a black font color and tries to use an installed 'Helvetica'
    font:

    draw_string ('Test', 'Helvetica', 14, 1, (0, 0, 0))

    Note: The function first tries to resolve the font as font file.
    If that fails, it looks for a system font name, which matches the
    font argument and returns a Font object based on those information
    (or the fallback font of pygame, see pygame.font.SysFont() for
    more information).
    Besides this the function simply calls the pygame Font.render()
    function, thus all documentation about it can applied to this
    function, too.
    """
    fnt = create_font (font, size, style)
    if not text:
        text = ""
    return fnt.render (text, antialias, color)

def draw_string_with_bg (text, font, size, antialias, color, bgcolor,
                         style=FONT_STYLE_NORMAL):
    """draw_string_with_bg (...) -> Surface

    Creates a surface displaying a atring.

    'text' is the text, which should be renderered to a surface. 'font'
    can be any valid font name of the system fonts or a font file (see
    the note). The 'size' is the height of the font in pixels, 'alias'
    is an integer value, which enables or disables antialiasing of the
    font, and 'color' is a pygame color style argument for the text.
    The Surface will be filled with the passed background color
    'bgcolor' Additional styles can be passed to give the font a bold,
    italic, or underlined style. The 'styles' attribute must be a valid
    combination of the FONT_STYLE_TYPES.

    The following example will create an antialiased string surface with
    a black font color and white background color and tries to use an
    installed 'Helvetica' font:

    draw_string ('Test', 'Helvetica', 14, 1, (0, 0, 0), (255, 255, 255))

    Note: The function first tries to resolve the font as font file.
    If that fails, it looks for a system font name, which matches the
    font argument and returns a Font object based on those information
    (or the fallback font of pygame, see pygame.font.SysFont() for
    more information).
    Besides this the function simply calls the pygame Font.render()
    function, thus all documentation about it can applied to this
    function, too.
    """
    fnt = create_font (font, size, style)
    if not text:
        text = ""
    return fnt.render (text, antialias, color, bgcolor)

def apply_font_style (font, style):
    """apply_font_style (font, style) -> None

    Applies font rendering styles to a Font.

    Raises a TypeError, if the 'style' argument is not a valid FONT_STYLE_TYPES
    value.
    """
    if style == FONT_STYLE_NORMAL:
        return
    if not constants_is_font_style:
        raise TypeError ("style must be a value of FONT_STYLE_TYPES")

    bold = style & FONT_STYLE_BOLD == FONT_STYLE_BOLD
    italic = style & FONT_STYLE_ITALIC == FONT_STYLE_ITALIC
    underline = style & FONT_STYLE_UNDERLINE == FONT_STYLE_UNDERLINE
    font.set_bold (bold)
    font.set_italic (italic)
    font.set_underline (underline)
