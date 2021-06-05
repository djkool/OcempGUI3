# TooltipWindow examples
import pygame.mouse
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

__tooltip = None

def _make_tooltip (tip, renderer):
    global __tooltip

    # Destroy the tooltip, if it exists.
    if __tooltip:
        __tooltip.destroy ()

    # Create a new tooltip.
    __tooltip = TooltipWindow (tip)
    x, y = pygame.mouse.get_pos ()
    __tooltip.topleft = x + 8, y - 5
    __tooltip.depth = 99 # Make it the topmost widget.

    renderer.add_widget (__tooltip)

def _destroy_tooltip ():
    # Destroy the tooltip, if it exists.
    global __tooltip
    if __tooltip:
        __tooltip.destroy ()
        __tooltip = None

def create_tooltip_view (renderer):

    frame = VFrame ()
    frame.border = BORDER_NONE
    frame.spacing = 20
    
    button1 = Button ("Move the mouse over me")
    button1.tooltip = "An enhanced description"
    button1.connect_signal (SIG_ENTER, _make_tooltip, button1.tooltip, renderer)
    button1.connect_signal (SIG_LEAVE, _destroy_tooltip)
        
    button2 = Button ("And over me, too")
    button2.tooltip = "Another description"
    button2.connect_signal (SIG_ENTER, _make_tooltip, button2.tooltip, renderer)
    button2.connect_signal (SIG_LEAVE, _destroy_tooltip)

    frame.children = button1, button2

    return frame

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (200, 150)
    re.title = "TooltipWindow examples"
    re.color = (234, 228, 223)
    re.add_widget (create_tooltip_view (re))
    # Start the main rendering loop.
    re.start ()
