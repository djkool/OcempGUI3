# Event usage example.
from ocempgui.events import Signals

# Create a new event object, which acts as a click event with a specific
# position.
event = Signals.Event ("click", (125, 150))

print "Event signal: %s, data: %s" % (event.signal, event.data)
