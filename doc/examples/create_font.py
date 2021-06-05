# String.create_font () usage example.
import pygame
from ocempgui.draw import String

def check (font, name):
    bold = "not bold"
    if font.get_bold ():
        bold = "bold"
    print "%s at %s is %s" % (name, font, bold)

# Initialize the pygame engine.
pygame.init ()

# Create a font from the ttf located in the current directory.
font = String.create_font ("tuffy.ttf", 14)
check (font, "font")

# Now create a second font and manipulate it.
# NOTE: Due to the caching we are using the same font object as above!
font_mod = String.create_font ("tuffy.ttf", 14)
font_mod.set_bold (True)

# Output the bold state of both fonts.
check (font, "font")
check (font_mod, "font_mod")
