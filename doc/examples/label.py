# Label examples.
import os
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame
    
def create_label_view ():
    states = ("STATE_NORMAL", "STATE_ENTERED", "STATE_ACTIVE",
              "STATE_INSENSITIVE")

    table = Table (2, 3)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)
    table.set_row_align (1, ALIGN_TOP)

    # Frame with the states.
    frm_states = _create_vframe ("States")
    for i, s in enumerate (states):
        lbl = Label (s)
        if STATE_TYPES[i] == STATE_INSENSITIVE:
            lbl.sensitive = False
        else:
            lbl.state = STATE_TYPES[i]
        frm_states.add_child (lbl)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = _create_vframe ("Padding")
    for i in xrange (5):
        lbl = Label ("Padding: %dpx" % (i * 2))
        lbl.padding = i * 2
        frm_padding.add_child (lbl)
    table.add_child (0, 1, frm_padding)

    # Frame with mnemonic support.
    frm_mnemonics = _create_vframe ("Mnemonics")
    strings = ("#Simple Mnemonic", "A ## is displayed using '####'",
               "M#ultiple M#nemonics #have no #effect")
    for s in strings:
        lbl = Label (s)
        frm_mnemonics.add_child (lbl)
    table.add_child (0, 2, frm_mnemonics)

    # Frame with multiline labels.
    frm_multiline = _create_vframe ("Multiline labels")
    frm_multiline.align = ALIGN_NONE
    strings = ("Single line", "First lines\nSecond line",
               "First line\nSecond line\nThird Line",
               "Two lines with a\n#mnemonic")
    for s in strings:
        lbl = Label (s)
        lbl.multiline = True
        frm_multiline.add_child (lbl)
    table.add_child (1, 0, frm_multiline)
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (500, 350)
    re.title = "Label examples"
    re.color = (234, 228, 223)
    re.add_widget (create_label_view ())
    # Start the main rendering loop.
    re.start ()
