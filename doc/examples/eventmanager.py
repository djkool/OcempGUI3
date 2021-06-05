# EventManager usage example.
from ocempgui.events import EventManager, INotifyable

# Create a new event capable object. This can be acquired by adding a
# 'notify ()' method to the object, which receives a single argument.
class OwnObject (INotifyable):
    def __init__ (self):
        self.x = 0
        self.y = 0
    
    def move (self, coords):
       # Moves the object to the desired coordinates (x, y).
       self.x = coords[0]
       self.y = coords[1]
       print "Moved to %d,%d" % (self.x, self.y)

    def notify (self, event):
        # Check the event signal and run a certain action with its data.
        if event.signal == "clicked":
           print "Something was clicked!"
        elif event.signal == "move":
           # Assuming that the event.data contains a coordinate tuple.
           self.move (event.data)

# Create an EventManager and OwnObject instance.
manager = EventManager ()
myobj = OwnObject ()

# Add the object to the EventManager.
manager.add_object (myobj, "move", "clicked")

# Send events to the registered objects via the emit() method.
manager.emit ("clicked", None)
manager.emit ("move", (10, 10))

# Remove the object from the 'clicked' slot.
manager.remove_object (myobj, "clicked")

# Send the 'clicked' event once more.
manager.emit ("clicked", None)

# Remove the object from _all_ slots it still listens on.
manager.remove_object (myobj)

# Send the 'move' event again.
manager.emit ("move", (40, 40))
