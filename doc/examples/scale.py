# Scale examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _update_value (scale, label):
    label.text = "Value %f" % scale.value

def create_scale_view ():
    table = Table (4, 4)
    table.topleft = 5, 5
    table.spacing = 5
    
    # Create and display a simple HScale.
    hscale = HScale (0, 100)
    table.add_child (0, 0, hscale)
    lbl = Label ("Value: %f" % hscale.value)
    table.add_child (1, 0, lbl)
    hscale.connect_signal (SIG_VALCHANGED, _update_value, hscale, lbl)
    
    # Create and display an insensitive HScale.
    hscale = HScale (0, 100)
    hscale.sensitive = False
    table.add_child (0, 1, hscale)
    lbl = Label ("Value: %f" % hscale.value)
    table.add_child (1, 1, lbl)
    hscale.connect_signal (SIG_VALCHANGED, _update_value, hscale, lbl)
    
    # Scale with different value range.
    hscale = HScale (-200, 200)
    table.add_child (0, 2, hscale)
    lbl = Label ("Value: %f" % hscale.value)
    table.add_child (1, 2, lbl)
    hscale.connect_signal (SIG_VALCHANGED, _update_value, hscale, lbl)

    # Scale with a small value range.
    hscale = HScale (0, 0.1)
    hscale.step = 0.0001
    table.add_child (0, 3, hscale)
    lbl = Label ("Value: %f" % hscale.value)
    table.add_child (1, 3, lbl)
    hscale.connect_signal (SIG_VALCHANGED, _update_value, hscale, lbl)
    
    # Create and display a simple VScale.
    vscale = VScale (0, 100)
    table.add_child (2, 0, vscale)
    lbl = Label ("Value: %f" % vscale.value)
    table.add_child (3, 0, lbl)
    vscale.connect_signal (SIG_VALCHANGED, _update_value, vscale, lbl)
    
    # Create and display an insensitive VScale.
    vscale = VScale (0, 100)
    vscale.sensitive = False
    table.add_child (2, 1, vscale)
    lbl = Label ("Value: %f" % vscale.value)
    table.add_child (3, 1, lbl)
    vscale.connect_signal (SIG_VALCHANGED, _update_value, vscale, lbl)

    # Scale with different value range.
    vscale = VScale (-200, 200)
    table.add_child (2, 2, vscale)
    lbl = Label ("Value: %f" % vscale.value)
    table.add_child (3, 2, lbl)
    vscale.connect_signal (SIG_VALCHANGED, _update_value, vscale, lbl)

    # Scale with a small value range.
    vscale = VScale (0, 0.1)
    vscale.step = 0.0001
    table.add_child (2, 3, vscale)
    lbl = Label ("Value: %f" % vscale.value)
    table.add_child (3, 3, lbl)
    vscale.connect_signal (SIG_VALCHANGED, _update_value, vscale, lbl)

    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (550, 200)
    re.title = "Scale examples"
    re.color = (234, 228, 223)
    re.add_widget (create_scale_view ())
    # Start the main rendering loop.
    re.start ()
