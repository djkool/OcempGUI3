# Theme usage example.
from ocempgui.widgets import base, Renderer, Button, Label

# Load the theme.
base.GlobalStyle.load ("theme_example.rc")

# Create screen.
re = Renderer ()
re.create_screen (200, 100)
re.title = "Theme example"
re.color = (250, 250, 250)

# Create widgets.
button = Button ("Button")
button.topleft = 5, 5
label = Label ("Label")
label.topleft = 100, 5

re.add_widget (button, label)
re.start ()
