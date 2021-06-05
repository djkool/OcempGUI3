# Complex.FaderSurface usage example.
import pygame, pygame.locals
from ocempgui.draw import Complex, Image

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((120, 100))
screen.fill ((180, 180, 180))
pygame.display.set_caption ('Complex.FaderSurface')

# Create a surface we can use to display.
image = Image.load_image ("./image.png")
r = image.get_rect ()

# Create a new FaderSurface with the same dimensions and an initial
# transparency of 1.
surface = Complex.FaderSurface (r.width, r.height, 1)

# Blit the original on the FaderSurface.
surface.blit (image, (0, 0))

# The default step value is -1, but we want to fade the image in.
surface.step = 1

# Loop until the FaderSurface reached the maximum or minimum alpha
# transparency value.
while surface.update ():

    # Clean up and blit the surface..
    screen.fill ((180, 180, 180))
    screen.blit (surface, (10, 10))
    pygame.display.flip ()

    # Check the bounds. We have to check the maximum values - 1, because
    # 255 and 0 cause the surface to return False and we would exit the
    # loop.
    if surface.alpha == 254 or surface.alpha == 1:
        surface.step = -surface.step

    pygame.time.delay (50 / 4)
    
    # Wait for input.
    if pygame.event.get ([pygame.locals.QUIT]):
        break
