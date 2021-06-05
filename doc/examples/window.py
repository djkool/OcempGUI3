# Window examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _destroy_dialog (window):
    window.child.text = "#Close"
    
def create_window_view ():
    # Create and display a simple window.
    window = Window ("Simple window")
    window.child = Button ("#Not clickable")
    window.child.connect_signal (SIG_CLICKED, window.destroy)
    window.topleft = 5, 5
    window.depth = 1
    
    # Create dialog window.
    dialog = DialogWindow ("Modal dialog window")
    dialog.child = Button ("#Close first to use other widgets")
    dialog.child.connect_signal (SIG_CLICKED, _destroy_dialog, window)
    dialog.child.connect_signal (SIG_CLICKED, dialog.destroy)
    dialog.topleft = 100, 5
    dialog.depth = 2
    
    return window, dialog

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (400, 200)
    re.title = "Window examples"
    re.color = (234, 228, 223)
    re.show_layer_info = True
    re.add_widget (*create_window_view ())
    # Start the main rendering loop.
    re.start ()
