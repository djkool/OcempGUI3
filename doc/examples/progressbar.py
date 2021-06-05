# ProgressBar examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _update_bar (bar, button):
    if bar.value == 0:
        while bar.value < 100:
            bar.increase ()
        button.text = "Clean the progressbar"
    else:
        while bar.value > 0:
            bar.decrease ()
        button.text = "Fill the progressbar"

def _update_text (bar):
    bar.text = "%.2f" % bar.value + "%"

def _create_vframe (text):
    frame = VFrame (Label (text))
    frame.spacing = 5
    frame.align = ALIGN_LEFT
    return frame

def create_progressbar_view ():
    table = Table (1, 3)
    table.spacing = 5
    
    # Create and display a simple ProgressBar.
    frame = _create_vframe ("ProgressBar")
    progress = ProgressBar ()
    progress.step = 0.5

    # Create a button to start filling.
    btn = Button ("#Fill the ProgressBar")
    btn.connect_signal ("clicked", _update_bar, progress, btn)
    frame.add_child (progress, btn)
    table.add_child (0, 0, frame)

    # ProgressBar with text.
    frame = _create_vframe ("Progressbar with text")
    progress = ProgressBar ()
    progress.text = "0.00%"
    progress.step = 0.5
    progress.connect_signal (SIG_VALCHANGED, _update_text, progress)
                             
    # Create a button to start filling.
    btn = Button ("Fill the ProgressBar")
    btn.connect_signal ("clicked", _update_bar, progress, btn)
    frame.add_child (progress, btn)
    table.add_child (0, 1, frame)

    # Insensitive progressbar.
    frame = _create_vframe ("Insensitive Progressbar")
    progress = ProgressBar ()
    progress.value = 50.0
    progress.text = "50.00%"
    progress.sensitive = False
    frame.add_child (progress)
    table.add_child (0, 2, frame)
    
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (450, 150)
    re.title = "ProgressBar examples"
    re.color = (234, 228, 223)
    re.add_widget (create_progressbar_view ())
    # Start the main rendering loop.
    re.start ()
