# ScrolledWindow examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
import label

def create_scrolledwindow_view ():
    table = Table (2, 2)
    table.spacing = 5
    
    # Create and display a simple ScrolledWindow.
    window = ScrolledWindow (200, 200)
    window.child = label.create_label_view ()
    table.add_child (0, 0, window)
    
    # Create and display a simple ScrolledWindow with always scrolling.
    always = ScrolledWindow (200, 200)
    always.scrolling = SCROLL_ALWAYS
    always.child = label.create_label_view ()
    table.add_child (0, 1, always)
    
    # Create and display a simple ScrolledWindow with no scrolling.
    never = ScrolledWindow (200, 200)
    never.scrolling = SCROLL_NEVER
    never.child = label.create_label_view ()
    table.add_child (1, 0, never)

    # Create an insensitive ScrolledWindow
    insensitive = ScrolledWindow (200, 200)
    insensitive.scrolling = SCROLL_ALWAYS
    insensitive.sensitive = False
    insensitive.child = label.create_label_view ()
    table.add_child (1, 1, insensitive)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (450, 450)
    re.title = "ScrolledWindow examples"
    re.color = (234, 228, 223)
    table = create_scrolledwindow_view ()
    table.topleft = 5, 5
    re.add_widget (table)
    # Start the main rendering loop.
    re.start ()
