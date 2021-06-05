# Entry examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _toggle_password (button, entry):
    entry.password = button.active

def create_entry_view ():
    states = ("STATE_NORMAL", "STATE_ENTERED", "STATE_ACTIVE",
              "STATE_INSENSITIVE")

    table = Table (2, 3)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)

    # Frame with the states.
    frm_states = VFrame (Label ("States"))
    frm_states.spacing = 5
    frm_states.align = ALIGN_LEFT
    for i in xrange (len (states)):
        entry = Entry (states[i])
        if STATE_TYPES[i] == STATE_INSENSITIVE:
            entry.sensitive = False
        else:
            entry.state = STATE_TYPES[i]
        frm_states.add_child (entry)
    table.add_child (0, 0, frm_states)

    # Frame with different padding.
    frm_padding = VFrame (Label ("Padding"))
    frm_padding.spacing = 5
    frm_padding.align = ALIGN_LEFT
    for i in xrange (5):
        entry = Entry ("Padding: %dpx" % (i * 2))
        entry.padding = i * 2
        frm_padding.add_child (entry)
    table.add_child (0, 1, frm_padding)

    # Password Mode
    frm_password = VFrame (Label ("Password support"))
    frm_padding.spacing = 5
    entry = Entry ("A password")
    button = ToggleButton ("Password mode")
    button.connect_signal (SIG_TOGGLED, _toggle_password, button, entry)
    frm_password.add_child (entry, button)
    table.add_child (0, 2, frm_password)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (400, 400)
    re.title = "Entry examples"
    re.color = (234, 228, 223)
    re.add_widget (create_entry_view ())
    # Start the main rendering loop.
    re.start ()
