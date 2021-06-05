# String.draw_string () usage example.
import pygame, pygame.locals
from ocempgui.draw import String

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((400, 100))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('String.draw_string ()')

# Create a text using the ttf located in the current directory.
text = String.draw_string ("This is tuffy.ttf", "tuffy.ttf", 16, 1, (0, 0, 0))
screen.blit (text, (5, 5))

# Create a text using the 'Times' system font
text = String.draw_string ("This is Times", "Times", 16, 1, (255, 0, 0))
screen.blit (text, (5, 35))

# Create a text using the fallback python font by specifying a wrong
# font name (hopefully ;-).
text = String.draw_string ("This is the fallback", "invalid_font_name_here",
                           16, 1, (0, 0, 255))
screen.blit (text, (5, 60))

# Now the same again without antialiasing.
text = String.draw_string ("This is tuffy.ttf (no aa)", "tuffy.ttf",
                           16, 0, (0, 0, 0))
screen.blit (text, (200, 5))

text = String.draw_string ("This is Times (no aa)", "Times",
                           16, 0, (255, 0, 0))
screen.blit (text, (200, 35))

text = String.draw_string ("This is the fallback (no aa)",
                           "invalid_font_name_here", 16, 1, (0, 0, 255))
screen.blit (text, (200, 60))

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
