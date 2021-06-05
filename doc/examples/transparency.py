# Transparency examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _destroy_dialog (window):
    window.child.text = "#Close"
    
def create_transparent_view ():

    box = Box (180, 80)
    box.border = BORDER_FLAT
    box.topleft = 10, 10

    label = ImageLabel ("image.png")
    label.topleft = 10, 10
    
    button = Button ("A transparent Button")
    button.opacity = 160
    button.topleft = 30, 30

    box.children = label, button
    
    # Create and display a simple window.
    window = Window ("Simple window")
    window.opacity = 100
    window.child = Button ("#Button")
    window.child.connect_signal (SIG_CLICKED, window.destroy)
    window.topleft = 180, 30
    window.depth = 1

    return box, window

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (400, 200)
    re.title = "Transparency examples"
    re.color = (234, 228, 223)
    re.show_layer_info = True
    re.add_widget (*create_transparent_view ())
    # Start the main rendering loop.
    re.start ()
