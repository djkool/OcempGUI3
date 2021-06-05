# Button examples.
import os
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

    table = Table (2, 3)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)
    table.set_row_align (1, ALIGN_TOP)

    # Frame with the states.
    frm_states = _create_vframe ("States")
    for i, s in enumerate (states):
        btn = Button (s)
        if STATE_TYPES[i] == STATE_INSENSITIVE:
            btn.sensitive = False
        else:
            btn.state = STATE_TYPES[i]
        frm_states.add_child (btn)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = _create_vframe ("Padding")
    for i in xrange (5):
        btn = Button ("Padding: %dpx" % (i * 2))
        btn.padding = i * 2
        frm_padding.add_child (btn)
    table.add_child (0, 1, frm_padding)

    # Mnemonics.
    frm_mnemonic = _create_vframe ("Mnemonics")
    btn = Button ("#Simple Mnemonic")
    btn2 = Button ("#Activate using <ALT><Underlined Key>")
    frm_mnemonic.add_child (btn, btn2)
    table.add_child (0, 2, frm_mnemonic)

    # Borders.
    frm_borders = _create_vframe ("Borders")
    btn_raised = Button ("Raised border")
    btn_sunken = Button ("Sunken border")
    btn_sunken.border = BORDER_SUNKEN
    btn_flat = Button ("Flat border")
    btn_flat.border = BORDER_FLAT
    btn_none = Button ("No border")
    btn_none.border = BORDER_NONE
    btn_etchedin = Button ("Etched in")
    btn_etchedin.border = BORDER_ETCHED_IN
    btn_etchedout = Button ("Etched out")
    btn_etchedout.border = BORDER_ETCHED_OUT
    frm_borders.add_child (btn_raised, btn_sunken, btn_flat, btn_none,
                           btn_etchedin, btn_etchedout)
    table.add_child (1, 0, frm_borders)

    # Multiline labeled buttons
    frm_multiline = _create_vframe ("Multiline labels")
    strings = ("Single lined Button", "Two lines on\na Button",
               "Two lines with a\n#mnemonic")
    for s in strings:
        button = Button (s)
        button.child.multiline = True
        frm_multiline.add_child (button)
    table.add_child (1, 1, frm_multiline)

    # Empty buttons with different minimum sizes
    frm_empty = _create_vframe ("Empty Buttons")
    for i in xrange (5):
        button = Button ()
        button.minsize = (20 * i, 10 * i)
        frm_empty.add_child (button)
    table.add_child (1, 2, frm_empty)
    
    return table
    
if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (550, 470)
    re.title = "Button examples"
    re.color = (234, 228, 223)
    re.add_widget (create_button_view ())
    # Start the main rendering loop.
    re.start ()
