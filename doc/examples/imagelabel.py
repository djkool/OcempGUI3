# ImageLabel examples.
import os
from ocempgui.draw import Image
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame
    
def create_imagelabel_view ():
    image = Image.load_image ("./image.png")
    table = Table (1, 3)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)

    # Frame with the states.
    frm_states = _create_vframe ("States")
    for i, s in enumerate (STATE_TYPES):
        lbl = ImageLabel (image)
        if s == STATE_INSENSITIVE:
            lbl.sensitive = False
        else:
            lbl.state = s
        frm_states.add_child (lbl)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = _create_vframe ("Padding")
    for i in xrange (5):
        lbl = ImageLabel (image)
        lbl.border = BORDER_FLAT
        lbl.padding = i * 2
        frm_padding.add_child (lbl)
    table.add_child (0, 1, frm_padding)

    # Borders.
    frm_borders = _create_vframe ("Borders")
    for border in BORDER_TYPES:
        lbl = ImageLabel (image)
        lbl.border = border
        frm_borders.add_child (lbl)
    table.add_child (0, 2, frm_borders)
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (500, 350)
    re.title = "ImageLabel examples"
    re.color = (234, 228, 223)
    re.add_widget (create_imagelabel_view ())
    # Start the main rendering loop.
    re.start ()
