# ActionListener usage example.
import sys
from ocempgui.events import EventManager
from ocempgui.object import ActionListener

count = 0

def emit_pong (event, manager):
    print "emit_pong received: [%s] - emitting pong..." % event
    manager.emit ("pong", "pong_event")

def emit_ping (event, manager):
    global count
    if count > 10:
        sys.exit ()
    count += 1
    print "emit_ping received: [%s] - emitting ping..." % event
    manager.emit ("ping", "ping_event")

# Create an event manager and two ping-pong listeners.
manager = EventManager ()

listener1 = ActionListener ()
listener1.connect_signal ("ping", emit_pong, manager)
listener1.manager = manager

listener2 = ActionListener ()
listener2.connect_signal ("pong", emit_ping, manager)
listener2.manager = manager

# start ping-pong actions
print "Starting Ping-Pong"
manager.emit ("ping", "ping_event")
