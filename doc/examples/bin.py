# Bin examples.
import pygame
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

class PivotBin (Bin):
    """PivotBin (widget) -> OwnBin

    A Bin implementation example class.

    This class does not support real rotations of widgets. Instead it
    simply rotates their image surface and displays it. Any other
    behaviour and information of the widget stay the same. Thus event
    capable widgets will not work correctly.
    """
    def __init__ (self):
        Bin.__init__ (self)
        self._orientation = ORIENTATION_HORIZONTAL

    def set_orientation (self, orientation=ORIENTATION_HORIZONTAL):
        """P.set_orientation (...) -> None

        Sets the orientation of the attached child.
        """
        if orientation not in ORIENTATION_TYPES:
            raise ValueError("orientation must be a value of ORIENATION_TYPES")
        self._orientation = orientation
        self.dirty = True

    def draw_bg (self):
        width, height = self.padding, self.padding
        cls = self.__class__

        if self.child:
            width += self.child.width
            height += self.child.height
            if self.orientation == ORIENTATION_VERTICAL:
                # Swap width and height on demand
                width, height = height, width

        # Guarantee the set minimum and maximum sizes.
        width, height = self.check_sizes (width, height)
        surface = base.GlobalStyle.engine.draw_rect (width, height, self.state,
                                                     cls, self.style)
        return surface
    
    def draw (self):
        """Draws the PivotBin and its child according to the set orientation."""
        Bin.draw (self)
        if self.child:
            rect = self.image.get_rect ()
            self.child.center = rect.center
            if self.orientation == ORIENTATION_VERTICAL:
                # Rotate the child image on demand.
                image = pygame.transform.rotate (self.child.image, 90)
                rotate_rect = image.get_rect ()
                rotate_rect.center = rect.center
                self.image.blit (image, rotate_rect)
            else:
                self.image.blit (self.child.image, self.child.rect)

    orientation = property (lambda self: self._orientation,
                            lambda self, var: self.set_orientation (var),
                            doc = "The orientation of the child.")

def rotate_bin (bin, button):
    # Set the Bin orientation and replace the button.
    if bin.orientation == ORIENTATION_HORIZONTAL:
        bin.orientation = ORIENTATION_VERTICAL
    else:
        bin.orientation = ORIENTATION_HORIZONTAL
    button.topleft = bin.left, bin.bottom + 10

if __name__ == "__main__":
    bin = PivotBin ()
    bin.topleft = 10, 10
    bin.child = Label ("Simple label in PivotBin")

    button = Button ("Switch orientation")
    button.topleft = bin.left, bin.bottom
    button.connect_signal (SIG_CLICKED, rotate_bin, bin, button)
    
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (300, 300)
    re.title = "Bin implementation example"
    re.color = (234, 228, 223)
    re.add_widget (bin, button)

    # Start the main rendering loop.
    re.start ()
