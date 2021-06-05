# String.draw_string_with_bg () usage example.
import pygame, pygame.locals
from ocempgui.draw import String

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((100, 100))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('String.draw_string_with_bg ()')

# Create texts using the 'Times' system font and different background
# colors.
text = String.draw_string_with_bg ("This is Times", "Times", 16, 1, (0, 0, 0),
                                   (200, 200, 200))
screen.blit (text, (5, 5))

text = String.draw_string_with_bg ("This is Times", "Times", 16, 1, (0, 0, 0),
                                   (0, 200, 0))
screen.blit (text, (5, 60))

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
