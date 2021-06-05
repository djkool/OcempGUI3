# ScrollBar examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *


def create_scrollbar_view ():

    table = Table (2, 2)
    table.spacing = 5
    table.set_row_align (0, ALIGN_TOP)
    table.set_row_align (1, ALIGN_TOP)
    
    # Simple ScrollBars.
    frame = HFrame (Label ("Simple ScrollBars"))
    frame.spacing = 5
    hscroll = HScrollBar (100, 400)
    vscroll = VScrollBar (100, 400)
    frame.children = hscroll, vscroll
    table.add_child (0, 0, frame)
    
    # Insensitive ScrollBars.
    frame = HFrame (Label ("Insensitive ScrollBars"))
    frame.spacing = 5
    hscroll = HScrollBar (100, 400)
    hscroll.sensitive = False
    vscroll = VScrollBar (100, 400)
    vscroll.sensitive = False
    frame.children = hscroll, vscroll
    table.add_child (0, 1, frame)

    # ScrollBars with a null range.
    frame = HFrame (Label ("ScrollBars without a range"))
    frame.spacing = 5
    hscroll = HScrollBar (100, 100)
    vscroll = VScrollBar (100, 100)
    frame.children = hscroll, vscroll
    table.add_child (1, 0, frame)

    # Small ScrollBars.
    frame = HFrame (Label ("Tiny ScrollBars"))
    frame.spacing = 5
    hscroll = HScrollBar (10, 100)
    vscroll = VScrollBar (10, 100)
    frame.children = hscroll, vscroll
    table.add_child (1, 1, frame)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (320, 280)
    re.title = "ScrollBar examples"
    re.color = (234, 228, 223)

    re.add_widget (create_scrollbar_view ())
    # Start the main rendering loop.
    re.start ()
