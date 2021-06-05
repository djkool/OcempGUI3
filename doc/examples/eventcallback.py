# EventCallback usage example.
from ocempgui.events import Signals

def on_click (x, y):
    print "Received click at: %d, %d" % (x, y)

# Create new EventCallback objects with no data and data.
cb_no_data = Signals.EventCallback ("click", on_click)
cb_yes_data = Signals.EventCallback ("click", on_click, 10, 20)

# Invoke both callbacks.
cb_no_data.run (50, 50) # We need to pass data because of the on_click
                        # signature.
cb_yes_data.run ()
