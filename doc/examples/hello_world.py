# Hello World example.
from ocempgui.widgets import *

# Initialize the drawing window.
re = Renderer ()
re.create_screen (100, 50)
re.title = "Hello World"
re.color = (250, 250, 250)

button = Button ("Hello World")
button.topleft = (10, 10)
re.add_widget (button)

# Start the main rendering loop.
re.start ()
