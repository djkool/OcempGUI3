# BaseObject usage example.
from ocempgui.object import BaseObject
from ocempgui.events import EventManager

# Callbacks, which should be invoked for the object.
def ping_callback (obj, additional_data):
    print "The object is: %s" % obj.name
    print "Passed data is: %s" % additional_data

def pong_callback ():
    print "Another callback with no arguments."

# Object implementation, which can listen to specific events.
class OwnObject (BaseObject):
    def __init__ (self, name):
        BaseObject.__init__ (self)
        self.name = name
        # The object should be able to listen to 'ping' and 'pong'
        # events.
        self._signals["ping"] = []
        self._signals["pong"] = []

    def notify (self, event):
        # This simple notify method will not be used in this
        # example. Instead, the signals are invoked directly.
        if event.signal == "ping":
            self.run_signal_handlers ("ping")
        elif event.signal == "pong":
            self.run_signal_handlers ("pong")

manager = EventManager ()

# Create an object and connect callbacks to its both events.
my_obj = OwnObject ("First object")
ev1 = my_obj.connect_signal ("ping", ping_callback, my_obj, "data")
ev2 = my_obj.connect_signal ("pong", pong_callback)

# Connect it to the event manager
my_obj.manager = manager

# Invoke the connected signals handlers for a specific event.
manager.emit ("ping", None)
manager.emit ("pong", None)

# After disconnecting a callback, it will not be invoked anymore.
my_obj.disconnect_signal (ev1)
my_obj.disconnect_signal (ev2)
manager.emit ("ping", None)
manager.emit ("pong", None)
