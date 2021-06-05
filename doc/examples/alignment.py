# Alignment examples.
from ocempgui.widgets import Renderer, VFrame, HFrame, Button, Alignment, Label
from ocempgui.widgets.Constants import *

def create_alignment_view ():
    # Crate and display the different alignments.
    frm_main = VFrame (Label ("Alignment examples"))
    frm_main.topleft = 10, 10

    # Top alignments.
    align_topleft = Alignment (100, 50)
    align_topleft.align = ALIGN_TOP | ALIGN_LEFT
    align_topleft.child = Button ("topleft")

    align_top = Alignment (100, 50)
    align_top.align = ALIGN_TOP
    align_top.child = Button ("top")

    align_topright = Alignment (100, 50)
    align_topright.align = ALIGN_TOP | ALIGN_RIGHT
    align_topright.child  = Button ("topright")

    frm_top = HFrame ()
    frm_top.children = align_topleft, align_top, align_topright

    # Centered alignments.
    align_left = Alignment (100, 50)
    align_left.align = ALIGN_LEFT
    align_left.child = Button ("left")
    
    align_center = Alignment (100, 50)
    align_center.align = ALIGN_NONE
    align_center.child  = Button ("center")

    align_right = Alignment (100, 50)
    align_right.align = ALIGN_RIGHT
    align_right.child = Button ("right")

    frm_center = HFrame ()
    frm_center.children = align_left, align_center, align_right

    # Bottom alignments.
    align_bottomleft = Alignment (100, 50)
    align_bottomleft.align = ALIGN_BOTTOM | ALIGN_LEFT
    align_bottomleft.child = Button ("bottomleft")

    align_bottom = Alignment (100, 50)
    align_bottom.align = ALIGN_BOTTOM
    align_bottom.child = Button ("bottom")

    align_bottomright = Alignment (100, 50)
    align_bottomright.align = ALIGN_BOTTOM | ALIGN_RIGHT
    align_bottomright.child = Button ("bottomright")

    frm_bottom = HFrame ()
    frm_bottom.children = align_bottomleft, align_bottom, align_bottomright

    frm_main.children = frm_top, frm_center, frm_bottom
    return frm_main

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (350, 300)
    re.title = "Alignment examples"
    re.color = (234, 228, 223)
    re.add_widget (create_alignment_view ())
    # Start the main rendering loop.
    re.start ()
