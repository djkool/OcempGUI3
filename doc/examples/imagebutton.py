# ImageButton examples.
import pygame, os
from ocempgui.draw import Image
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame

def create_button_view ():
    states = ("STATE_NORMAL", "STATE_ENTERED", "STATE_ACTIVE",
              "STATE_INSENSITIVE")

    image = Image.load_image ("./image.png")
    table = Table (2, 2)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)
    table.set_row_align (1, ALIGN_TOP)
    
    # Frame with the states.
    frm_states = _create_vframe ("States")
    for i, s in enumerate (states):
        btn = ImageButton (image)
        if STATE_TYPES[i] == STATE_INSENSITIVE:
            btn.sensitive = False
        else:
            btn.state = STATE_TYPES[i]
        btn.text = s
        frm_states.add_child (btn)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = _create_vframe ("Padding")
    for i in xrange (4):
        btn = ImageButton (image)
        btn.padding = i * 2
        frm_padding.add_child (btn)
    table.add_child (0, 1, frm_padding)

    # Mnemonics.
    frm_mnemonic = _create_vframe ("Mnemonics")
    btn = ImageButton (image)
    btn.text = "#Simple Mnemonic"
    btn2 = ImageButton (image)
    btn2.text = "#Activate using <ALT><Underlined Key>"
    frm_mnemonic.add_child (btn, btn2)
    table.add_child (1, 0, frm_mnemonic)

    # Multiline labeled ImageButton
    frm_multiline = _create_vframe ("Multiline label")
    button = ImageButton (image)
    button.text = "Multiple lines\nwith a #mnemonic"
    button.child.multiline = True
    frm_multiline.add_child (button)
    table.add_child (1, 1, frm_multiline)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (570, 400)
    re.title = "ImageButton examples"
    re.color = (234, 228, 223)
    re.add_widget (create_button_view ())
    # Start the main rendering loop.
    re.start ()
