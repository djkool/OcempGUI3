# Draw.draw_line () usage example.
import pygame, pygame.locals
from ocempgui.draw import Draw

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((200, 200))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('Draw.draw_line ()')

# Draw horizontal lines in different colors and sizes.
for i in range (10):
    val = i * 10
    Draw.draw_line (screen, (0 + val, 50 + val, 40 + 2 * val),
                    (5, val), (195, val), i)

# Draw vertical lines in different colors and sizes.
for i in range (10):
    val = i * 8
    Draw.draw_line (screen, (0 + 2 * val, 30 + val, 35 + 2 * val),
                    (5 + i * 10, 100), (5 + i * 10, 195), i)

# Draw a cross.
Draw.draw_line (screen, (0, 0, 0), (120, 100), (195, 195), 3)
Draw.draw_line (screen, (0, 0, 0), (195, 100), (120, 195), 3)

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
