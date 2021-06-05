# ScrolledList examples.
from ocempgui.widgets import *
from ocempgui.widgets.components import TextListItem
from ocempgui.widgets.Constants import *

__count = 0

def _add_item (slist):
    global __count
    slist.items.append (TextListItem ("SrolledList element %d" % __count))
    __count += 1

def _remove_item (slist, button2):
    selection = slist.get_selected ()
    for item in selection:
        slist.items.remove (item)
    button2.sensitive = False

def _update_button (slist, button):
    button.sensitive = len (slist.get_selected ()) > 0

def create_scrolledlist_view ():
    table = Table (2, 2)
    table.spacing = 5

    # Create a simple ScrolledList
    vframe = VFrame ()
    vframe.border = BORDER_NONE

    window = ScrolledList (200, 200)

    frame = HFrame ()
    frame.border = BORDER_NONE
    frame.spacing = 5
    button1 = Button ("#Add item")
    button1.connect_signal (SIG_CLICKED, _add_item, window)
    button2 = Button ("#Remove item")
    button2.connect_signal (SIG_CLICKED, _remove_item, window, button2)
    button2.sensitive = False
    window.connect_signal (SIG_SELECTCHANGED, _update_button, window, button2)
    frame.add_child (button1, button2)
    vframe.add_child (window, frame)

    table.add_child (0, 0, vframe)

    # Create a ScrolledList with always scrolling and single selection.
    always = ScrolledList (200, 200)
    always.scrolling = SCROLL_ALWAYS
    always.selectionmode = SELECTION_SINGLE
    for i in xrange (15):
        item = None
        if i % 2 == 0:
            item = TextListItem ("Short item %d" % i)
        else:
            text = "Very, " +3 * "very, very," + "long item %d" % i
            item = TextListItem (text)
        always.items.append (item)
    table.add_child (0, 1, always)
    table.set_align (0, 1, ALIGN_TOP)

    # Create a ScrolledList with different items.
    never = ScrolledList (200, 200)
    never.scrolling = SCROLL_NEVER
    for i in xrange (10):
        item = None
        if i % 2 == 0:
            item = TextListItem ("Default item")
            item.editable = True
        else:
            item = TextListItem ("Item with a different style")
            item.editable = True
            item.get_style ()["fgcolor"][STATE_NORMAL] = (100, 200, 100)
            item.style["font"]["size"] = 20
        never.items.append (item)
    table.add_child (1, 0, never)

    # Create an insensitive ScrolledList.
    insensitive = ScrolledList (200, 200)
    insensitive.scrolling = SCROLL_ALWAYS
    insensitive.sensitive = False
    for i in xrange (15):
        insensitive.items.append (TextListItem ("Insensitive item %d" % i))
    insensitive.select (insensitive.items[3])
    table.add_child (1, 1, insensitive)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (620, 500)
    re.title = "ScrolledList examples"
    re.color = (234, 228, 223)
    table = create_scrolledlist_view ()
    table.topleft = 5, 5
    re.add_widget (table)
    re.start ()
