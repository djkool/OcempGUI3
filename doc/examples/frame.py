# Frame examples.
import os
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def create_frame_view ():
    table = Table (2, 3)
    table.topleft = 5, 5
    table.spacing = 5
    
    # Create and display two 'standard' frames.
    hframe = HFrame (Label ("Horizontal Frame"))
    table.add_child (0, 0, hframe)

    lbl = Label ("Vertical Frame" + os.linesep + "with 5 px spacing")
    lbl.multiline = True
    vframe = VFrame (lbl)
    vframe.spacing = 5
    table.add_child (0, 1, vframe)
    
    for i in xrange(3):
        btn = Button ("Button %d" % i)
        hframe.add_child (btn)
        btn2 = Button ("Button %d" % i)
        vframe.add_child (btn2)

    # Create framed frames.
    framed1 = VFrame (Label ("VFrame"))
    framed2 = HFrame ()
    framed3 = VFrame (Label ("VFrame as HFrame.widget"))
    framed3.add_child (Label ("Child of a VFrame"))
    framed2.widget = framed3
    framed2.add_child (Button ("Button 1"), Button ("Button 2"))
    button = Button ("Simple Button")
    framed1.add_child (framed2, button)
    table.add_child (1, 0, framed1)

    # Create a Frame with alignment.
    frame_align = VFrame (Label ("VFrame with right alignment"))
    frame_align.align = ALIGN_RIGHT
    label1 = Label ("Label")
    label2 = Label ("Even longer label")
    button = CheckButton ("A CheckButton")
    frame_align.add_child (label1, label2, button)
    table.add_child (1, 1, frame_align)

    # Add insensitive frames.
    hframe = HFrame (Label ("Insensitive HFrame"))
    hframe.sensitive = False
    table.add_child (0, 2, hframe)
    
    vframe = VFrame (Label ("Insensitive VFrame"))
    vframe.sensitive = False
    table.add_child (1, 2, vframe)
    
    for i in xrange(3):
        btn = Button ("Button %d" % i)
        hframe.add_child (btn)
        btn2 = Button ("Button %d" % i)
        vframe.add_child (btn2)
   
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (600, 300)
    re.title = "Frame examples"
    re.color = (234, 228, 223)
    re.add_widget (create_frame_view ())

    # Start the main rendering loop.
    re.start ()

