# ImageMap examples.
import os
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _get_type (eventtype):
    if eventtype == SIG_MOUSEDOWN:
        return "SIG_MOUSEDOWN"
    elif eventtype == SIG_MOUSEUP:
        return "SIG_MOUSEUP"
    elif eventtype == SIG_MOUSEMOVE:
        return "SIG_MOUSEMOVE"
    else:
        return "Unknown signal"

def _got_mouseevent (event, imagemap, labels):
    labels[0].text = "Signal: %s" % _get_type (imagemap.last_event.signal)
    if imagemap.last_event.signal != SIG_MOUSEMOVE:
        labels[1].text = "Button: %d" % imagemap.last_event.data.button
    else:
        labels[1].text = "Button: None"
    labels[2].text = "Event pos: %s" % str (imagemap.last_event.data.pos)
    labels[3].text = "Rel. pos: %s" % str (imagemap.relative_position)

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame

def create_imagemap_view ():
    frm_map = _create_vframe ("ImageMap")

    imagemap = ImageMap ("image.png")
    
    lbl_desc = Label ("Move the mouse over the ImageMap and" + os.linesep +
                      "press the mouse buttons to interact with it.")
    lbl_desc.multiline = True
    
    lbl_results = [Label ("Signal:"), Label ("Button:"), Label ("Event pos:"),
                   Label ("Rel. pos:")]
    for label in lbl_results:
        label.create_style()["fgcolor"][STATE_NORMAL] = (255, 0, 0)
    imagemap.connect_signal (SIG_MOUSEDOWN, _got_mouseevent, imagemap,
                             lbl_results)
    imagemap.connect_signal (SIG_MOUSEMOVE, _got_mouseevent, imagemap,
                             lbl_results)
    imagemap.connect_signal (SIG_MOUSEUP, _got_mouseevent, imagemap,
                             lbl_results)

    frm_map.add_child (imagemap, lbl_desc, *lbl_results)
    return frm_map

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (250, 220)
    re.title = "ImageMap examples"
    re.color = (234, 228, 223)
    re.add_widget (create_imagemap_view ())
    # Start the main rendering loop.
    re.start ()
