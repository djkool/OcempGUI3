# BaseWidget examples
from ocempgui.widgets import BaseWidget

class MyWidget (BaseWidget):
    def __init__ (self):
        BaseWidget.__init__ (self)

widget = MyWidget ()
widget.topleft = 10, 10
print widget.topleft
