# Table examples.
from ocempgui.widgets import Renderer, Table, Label, Button
from ocempgui.widgets.Constants import *

def create_table_view ():
    # Crate and display a Table.
    table = Table (9, 2)
    table.spacing = 5
    table.topleft = 5, 5

    label = Label ("Nonaligned wide Label")
    table.add_child (0, 0, label)
    table.add_child (0, 1, Button ("Simple Button"))

    label = Label ("Top align")
    table.add_child (1, 0, label)
    table.set_align (1, 0, ALIGN_TOP)
    table.add_child (1, 1, Button ("Simple Button"))

    label = Label ("Bottom align")
    table.add_child (2, 0, label)
    table.set_align (2, 0, ALIGN_BOTTOM)
    table.add_child (2, 1, Button ("Simple Button"))
    
    label = Label ("Left align")
    table.add_child (3, 0, label)
    table.set_align (3, 0, ALIGN_LEFT)
    table.add_child (3, 1, Button ("Simple Button"))
    
    label = Label ("Right align")
    table.add_child (4, 0, label)
    table.set_align (4, 0, ALIGN_RIGHT)
    table.add_child (4, 1, Button ("Simple Button"))

    label = Label ("Topleft align")
    table.add_child (5, 0, label)
    table.set_align (5, 0, ALIGN_TOP | ALIGN_LEFT)
    table.add_child (5, 1, Button ("Simple Button"))

    label = Label ("Topright align")
    table.add_child (6, 0, label)
    table.set_align (6, 0, ALIGN_TOP | ALIGN_RIGHT)
    table.add_child (6, 1, Button ("Simple Button"))

    label = Label ("Bottomleft align")
    table.add_child (7, 0, label)
    table.set_align (7, 0, ALIGN_BOTTOM |ALIGN_LEFT)
    table.add_child (7, 1, Button ("Simple Button"))

    label = Label ("Bottomright align")
    table.add_child (8, 0, label)
    table.set_align (8, 0, ALIGN_BOTTOM |ALIGN_RIGHT)
    table.add_child (8, 1, Button ("Simple Button"))

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (250, 350)
    re.title = "Table examples"
    re.color = (234, 228, 223)
    re.add_widget (create_table_view ())
    # Start the main rendering loop.
    re.start ()
