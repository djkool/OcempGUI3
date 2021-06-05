# StatusBar examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _update_tip (statusbar, button):
    statusbar.pop_tip ()
    statusbar.push_tip ("Button '%s' was clicked" % button.text)

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame

def create_statusbar_view ():
    table = Table (1, 3)
    table.spacing = 5
    
    # Create and display a simple StatusBar with some children.
    frame = _create_vframe ("StatusBar")
    status = StatusBar ()
    button1 = Button ("Button")
    button1.connect_signal (SIG_CLICKED, _update_tip, status, button1)
    button2 = CheckButton ("Empty")
    button2.connect_signal (SIG_CLICKED, _update_tip, status, button2)

    status.add_child (button1, button2)
    
    button = Button ("Testbutton")
    button.connect_signal (SIG_CLICKED, _update_tip, status, button)
    frame.add_child (button, status)
    
    table.add_child (0, 0, frame)
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (450, 150)
    re.title = "StatusBar examples"
    re.color = (234, 228, 223)
    re.add_widget (create_statusbar_view ())
    # Start the main rendering loop.
    re.start ()
