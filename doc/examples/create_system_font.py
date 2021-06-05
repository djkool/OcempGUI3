# String.create_system_font () usage example.
import pygame
from ocempgui.draw import String

# Initialize the pygame engine.
pygame.init ()

# Create some fonts.
fonts = {}
names = ( "Arial", "Helvetica", "Sans", "Serif", "Times" )
for name in names:
    fonts[name] = String.create_system_font (name, 14)

# Output the fonts as well as their object address.
for name in fonts:
    print "Loaded: %s at %s" % (name, fonts[name])
