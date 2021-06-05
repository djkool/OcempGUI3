# FileDialog examples.
import os
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

def _set_files (result, dialog, entry):
    string = ""
    if result == DLGRESULT_OK:
        string = "".join(["\"%s\" " % f for f in dialog.get_filenames ()])
    else:
        string = "Nothing selected"
    dialog.destroy ()
    entry.text = string
    
def _open_filedialog (renderer, entry):
    buttons = [Button ("#OK"), Button ("#Cancel")]
    buttons[0].minsize = 80, buttons[0].minsize[1]
    buttons[1].minsize = 80, buttons[1].minsize[1]
    results = [DLGRESULT_OK, DLGRESULT_CANCEL]

    dialog = FileDialog ("Select your file(s)", buttons, results)
    dialog.depth = 1 # Make it the top window.
    dialog.topleft = 100, 20
    dialog.filelist.selectionmode = SELECTION_MULTIPLE
    dialog.connect_signal (SIG_DIALOGRESPONSE, _set_files, dialog, entry)
    renderer.add_widget (dialog)

def create_file_view (renderer):
    table = Table (1, 2)
    
    hframe = HFrame (Label ("FileList"))
    hframe.add_child (FileList (200, 200))
    table.add_child (0, 0, hframe)
    
    hframe2 = HFrame (Label ("FileDialog"))
    label = Label ("Selection:")
    entry = Entry ()
    entry.minsize = 200, entry.minsize[1]
    button = Button ("#Browse")
    button.connect_signal (SIG_CLICKED, _open_filedialog, renderer, entry)
    hframe2.add_child (label, entry, button)
    table.add_child (0, 1, hframe2)
    
    table.set_row_align (0, ALIGN_TOP)
    return table

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (550, 300)
    re.title = "FileDialog examples"
    re.color = (234, 228, 223)
    re.add_widget (create_file_view (re))
    # Start the main rendering loop.
    re.start ()
