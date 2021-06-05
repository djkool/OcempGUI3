# Embedding example of the Renderer in an own pygame mainloop with
# partial screen assignment.
import sys, random
import pygame, pygame.locals
from ocempgui.draw import Draw
from ocempgui.widgets import *

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((400, 400))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('Draw.draw_rect ()')

# Draw a rectangle, which can moev around on the screen.
rect = Draw.draw_rect (55, 40, (255, 0, 0))
screen.blit (rect, (20, 20))
x, y = 20, 20

# Partial screen assignment for the Renderer.
renderer = Renderer ()
surface = pygame.Surface ((200, 200))
renderer.screen = surface
renderer.color = 100, 100, 100

# Some widgets.
button = Button ("Hello :-)")
button.topleft = 10, 10
entry = Entry ("Awesome...")
entry.topleft = 30, 50
renderer.add_widget (button, entry)

# Blit the Renderer's contents at the desired position.
renderer.topleft = 70, 70
screen.blit (renderer.screen, renderer.topleft)

# Set up the tick event for timer based widgets.
pygame.time.set_timer (Constants.SIG_TICK, 10)

while True:
    events = pygame.event.get ()
    for event in events:
        if event.type == pygame.locals.QUIT:
            sys.exit ()
    # Pass all events to the Renderer.
    renderer.distribute_events (*events)

    # Clean the old red rect area.
    screen.fill ((250, 250, 250), (x, y, 55, 40))

    # Blit the rect at a new position
    x += random.randint (-8, 9)
    y += random.randint (-8, 9)
    # Do not let it leave the bounds.
    if x < 0: x = 0
    if y < 0: y = 0
    if x > 345: x = 345
    if y > 360: y = 360
    screen.blit (rect, (x, y))

    # Blit the renderer, too
    screen.blit (renderer.screen, renderer.topleft)
    pygame.display.flip ()
    pygame.time.delay (30)
