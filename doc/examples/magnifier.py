# Magnifier example.
import locale, pygame, sys
from ocempgui.access import Magnifier
from ocempgui.widgets import Renderer, base
from example import MainScreen

# Some localization adjustment for the example.
locale.setlocale (locale.LC_ALL, '')

# Create the magnifier we will use later on.
magnifier = Magnifier ()
magnifier.size = 50, 50

# Initialize the drawing window.
re = Renderer ()
re.create_screen (800, 610)
re.title = "Magnifier example"
re.color = (234, 228, 223)
re.show_layer_info = True
mainscreen = MainScreen (re)
mainscreen.init ()

# Wait for input.
while True:
    events = pygame.event.get ()
    for event in events:
        if event.type == pygame.QUIT:
            sys.exit ()

    # Guarantee, that the Renderer operates on a correct screen.
    magnifier.restore ()
    
    re.distribute_events (*events)

    # Notify the magnifier about the events.
    magnifier.notify (*events)

    # Update the screen.
    pygame.display.flip ()
