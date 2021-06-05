# ListItemCollection example.
from ocempgui.widgets.components import ListItemCollection, TextListItem

# Item change handler.
def item_has_changed (item):
    print "Item '%s' has changed" % item.text

# List change handler.
def list_has_changed (l):
    print "List now contains %d item(s)" % l.length

collection = ListItemCollection ()

# Set up a notification handler for item changes.
collection.item_changed = item_has_changed

# Set up a notification handler for list changes
collection.list_changed = list_has_changed

for i in xrange (5):
    collection.append (TextListItem ("Item no. %d" % i))

collection[2].text = "New text in item 3"

# Use a tuple as constructor argument.
items = (TextListItem ("String 1"), TextListItem ("String 2"),
         TextListItem ("String 3"))
collection = ListItemCollection (items)
print "New collection:"
print collection
