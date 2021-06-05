# GenericDialog examples.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _close (result, dialog, label):
    if result == DLGRESULT_OK:
        label.text = "You pressed OK!"
    elif result == DLGRESULT_CANCEL:
        label.text = "You pressed Cancel!"
    elif result == DLGRESULT_CLOSE:
        dialog.destroy ()

def create_dialog_view ():
    buttons = [Button ("#OK"), Button ("#Cancel"), Button ("Clo#se")]
    results = [DLGRESULT_OK, DLGRESULT_CANCEL, DLGRESULT_CLOSE]
    dialog = GenericDialog ("Generic dialog", buttons, results)
    lbl = Label ("Press the buttons to see the action.")
    dialog.content.add_child (lbl)
    dialog.connect_signal (SIG_DIALOGRESPONSE, _close, dialog, lbl)
    dialog.topleft = 30, 30
    dialog.depth = 1
    return dialog

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (300, 300)
    re.title = "GenericDialog examples"
    re.color = (234, 228, 223)
    re.add_widget (create_dialog_view ())
    # Start the main rendering loop.
    re.start ()
