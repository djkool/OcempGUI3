# Box examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
    
def create_box_view ():
    frame = VFrame (Label ("Box example"))
    frame.topleft = 10, 10

    # The Box with 200x200 pixels in size.
    box = Box (200, 200)

    # Widgets to place into it.
    label = ImageLabel ("image.png")
    label.topleft = 10, 10

    button = Button ("A Button")
    button.topleft = 30, 30

    frame1 = VFrame (Label ("A VFrame"))
    frame1.add_child (Label ("Label in the VFrame"))
    frame1.topleft = 60, 80

    chk = CheckButton ("A CheckButton")
    chk.topleft = 130, 110

    box.children = label, button, frame1, chk
    frame.add_child (box)

    return frame

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (300, 300)
    re.title = "Box examples"
    re.color = (234, 228, 223)
    re.show_layer_info = True
    re.add_widget (create_box_view ())
    # Start the main rendering loop.
    re.start ()
