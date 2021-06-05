# RadioButton examples.
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
    group = None
    for i, s in enumerate (states):
        btn = RadioButton (s, group)
        if i == 0:
            group = btn
        if STATE_TYPES[i] == STATE_INSENSITIVE:
            btn.sensitive = False
        else:
            btn.state = STATE_TYPES[i]
        frm_states.add_child (btn)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = _create_vframe ("Padding")
    group = None
    for i in xrange (5):
        btn = RadioButton ("Padding: %dpx" % (i * 2), group)
        if i == 0:
            group = btn
        btn.padding = i * 2
        frm_padding.add_child (btn)
    table.add_child (0, 1, frm_padding)

    # Mnemonics.
    frm_mnemonic = _create_vframe ("Mnemonics")
    btn = RadioButton ("#Simple Mnemonic")
    btn2 = RadioButton ("#Activate using <ALT><Underlined Key>", btn)
    frm_mnemonic.add_child (btn, btn2)
    table.add_child (0, 2, frm_mnemonic)

    # Multiline labeled buttons
    frm_multiline = _create_vframe ("Multiline labels")
    strings = ("Single lined RadioButton", "Two lines on\na RadioButton",
               "Two lines with a\n#mnemonic")
    group = None
    for i, s in enumerate (strings):
        btn = RadioButton (s, group)
        if i == 0:
            group = btn
        btn.child.multiline = True
        frm_multiline.add_child (btn)
    table.add_child (1, 0, frm_multiline)

    # Empty buttons with different minimum sizes
    group = None
    frm_empty = _create_vframe ("Empty Buttons")
    for i in xrange (5):
        button = RadioButton (None, group)
        button.minsize = (20 * i, 10 * i)
        if i == 0:
            group = button
        frm_empty.add_child (button)
    table.add_child (1, 2, frm_empty)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (550, 380)
    re.title = "RadioButton examples"
    re.color = (234, 228, 223)
    re.add_widget (create_button_view ())
    # Start the main rendering loop.
    re.start ()
