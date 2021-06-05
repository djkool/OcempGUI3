# Draw.draw_triangle () usage example.
import pygame, pygame.locals
from ocempgui.draw import Draw

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((200, 200))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('Draw.draw_triangle ()')

# Draw three triangles.
Draw.draw_triangle (screen, (255, 0, 0), (20, 5), (5, 30), (35, 30), 0)
Draw.draw_triangle (screen, (0, 255, 0), (25, 5), (40, 30), (55, 5), 0)
Draw.draw_triangle (screen, (0, 0, 255), (60, 5), (45, 30), (75, 30), 0)

# Draw a 'tunnel effect' of triangles.
for i in range (30):
     val = i + 3
     color = (val * 4, val * 7, val * 5)
     Draw.draw_triangle (screen, color, (5 + 2 * val, 50 + val),
                         (195 - 2 * val, 50 + val), (100, 195 - 2 * val), 1)

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
