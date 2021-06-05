# Draw.draw_rect () usage example.
import random
import pygame, pygame.locals
from ocempgui.draw import Draw

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((200, 200))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('Draw.draw_rect ()')

# Draw rectangles with various colors.
rect = Draw.draw_rect (55, 40, (255, 0, 0))
screen.blit (rect, (5, 5))

rect = Draw.draw_rect (55, 40, (0, 255, 0))
screen.blit (rect, (65, 5))

rect = Draw.draw_rect (55, 40, (0, 0, 255))
screen.blit (rect, (125, 5))

# Draw encapsulated rectangles.
for i in range (30):
    val = i + 3
    rnd = (random.randint (0, 5), random.randint (0, 5), random.randint (0, 5))
    color = (rnd[0] * i + 100,  rnd[1] * i + 100, rnd[2] * i + 100)
    rect = Draw.draw_rect (100 - 2 * val, 100 - 2 * val, color)
    screen.blit (rect, (5 + val, 50 + val))

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
