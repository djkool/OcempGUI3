# Image.load_image () usage example.
import pygame, pygame.locals
from ocempgui.draw import Image

# Initialize the drawing window.
pygame.init ()
screen = pygame.display.set_mode ((120, 100))
screen.fill ((250, 250, 250))
pygame.display.set_caption ('Image.load_image ()')

# Load an image and blit it on the screen.
image = Image.load_image ("./image.png")
screen.blit (image, (10, 10))

# Show anything.
pygame.display.flip ()

# Wait for input.
while not pygame.event.get ([pygame.locals.QUIT]):
    pass
