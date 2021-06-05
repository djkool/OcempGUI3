# Renderer examples.
from ocempgui.widgets import Renderer

# Initialize the drawing window.
re = Renderer ()
screen = re.create_screen (200, 200)
re.title = "Renderer"
re.color = (250, 250, 250)

# Start the main rendering loop.
re.start ()
