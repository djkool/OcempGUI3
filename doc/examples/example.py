# OcempGUI example application introducing all widgets and their
# capabilities.
import sys
sys.path += ['/Users/Marcus/libs/lib/python2.4/site-packages']
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
from ocempgui.widgets.components import *
import button, checkbutton, imagebutton, radiobutton, togglebutton, imagelabel
import label, entry, progressbar, scrolledlist, scrolledwindow, window
import frame, table, scale, imagemap, filedialog, genericdialog, graph2d
import scrollbar, tooltipwindow, transparency, box, alignment

class MainScreen (object):
    def __init__ (self, renderer):
        self.renderer = renderer
        self.items =  { "Alignment" : self.alignment_selected,
                        "Box" : self.box_selected,
                        "Button" : self.button_selected,
                        "CheckButton" : self.checkbutton_selected,
                        "Entry" : self.entry_selected,
                        "Frames" : self.frames_selected,
                        "FileList & Dialog" : self.filelist_selected,
                        "GenericDialog" : self.genericdialog_selected,
                        "Graph2D" : self.graph2d_selected,
                        "ImageButton" : self.imagebutton_selected,
                        "ImageMap" : self.imagemap_selected,
                        "ImageLabel" : self.imagelabel_selected,
                        "Label" : self.label_selected,                        
                        "ProgressBar" : self.progressbar_selected,
                        "RadioButton" : self.radiobutton_selected,
                        "Scale" : self.scale_selected,
                        "ScrollBar" : self.scrollbar_selected,
                        "ScrolledList" : self.scrolledlist_selected,
                        "ScrolledWindow" : self.scrolledwindow_selected,
                        "Table" : self.table_selected,
                        "ToggleButton" : self.togglebutton_selected,
                        "TooltipWindow" : self.tooltipwindow_selected,
                        "Transparency" : self.transparency_selected,
                        "Windows" : self.windows_selected }
        self.states = ("STATE_NORMAL", "STATE_ENTERED", "STATE_ACTIVE",
                       "STATE_INSENSITIVE")
        self.current = None
        self.frame = None
        self.status = None

        # Set a better font than the default one.
        base.GlobalStyle.styles["default"]["font"]["name"] = "tuffy.ttf"
        base.GlobalStyle.styles["default"]["font"]["size"] = 12

    def update_status (self, text):
        self.status.pop_tip ()
        self.status.push_tip (text)

    def label_selected (self):
        self.frame.add_child (label.create_label_view ())

    def button_selected (self):
        self.frame.add_child (button.create_button_view ())

    def box_selected (self):
        self.frame.add_child (box.create_box_view ())
    
    def imagebutton_selected (self):
        self.frame.add_child (imagebutton.create_button_view ())

    def imagemap_selected (self):
        self.frame.add_child (imagemap.create_imagemap_view ())

    def imagelabel_selected (self):
        self.frame.add_child (imagelabel.create_imagelabel_view ())

    def checkbutton_selected (self):
        self.frame.add_child (checkbutton.create_button_view ())
    
    def togglebutton_selected (self):
        self.frame.add_child (togglebutton.create_button_view ())

    def radiobutton_selected (self):
        self.frame.add_child (radiobutton.create_button_view ())

    def entry_selected (self):
        self.frame.add_child (entry.create_entry_view ())

    def progressbar_selected (self):
        self.frame.add_child (progressbar.create_progressbar_view ())

    def scale_selected (self):
        self.frame.add_child (scale.create_scale_view ())

    def scrollbar_selected (self):
        self.frame.add_child (scrollbar.create_scrollbar_view ())
    
    def scrolledlist_selected (self):
        self.frame.add_child (scrolledlist.create_scrolledlist_view ())

    def scrolledwindow_selected (self):
        self.frame.add_child (scrolledwindow.create_scrolledwindow_view ())

    def windows_selected (self):
        windows = window.create_window_view ()
        for w in windows:
            w.topleft = w.top + 180, w.left + 110
        self.renderer.add_widget (*windows)

    def genericdialog_selected (self):
        dialog = genericdialog.create_dialog_view ()
        dialog.topleft = 180, 110
        self.renderer.add_widget (dialog)

    def graph2d_selected (self):
        self.frame.add_child (graph2d.create_graph2d_view ())
    
    def filelist_selected (self):
        self.frame.add_child (filedialog.create_file_view (self.renderer))
    
    def frames_selected (self):
        self.frame.add_child (frame.create_frame_view ())
    
    def table_selected (self):
        self.frame.add_child (table.create_table_view ())

    def tooltipwindow_selected (self):
        self.frame.add_child (tooltipwindow.create_tooltip_view (self.renderer))

    def transparency_selected (self):
        widgets = transparency.create_transparent_view ()
        self.frame.add_child (widgets[0])
        widgets[1].topleft = widgets[1].left + self.frame.left, \
                             widgets[1].top + self.frame.top
        self.renderer.add_widget (widgets[1])

    def alignment_selected (self):
        self.frame.add_child (alignment.create_alignment_view ())
    
    def select_widget (self, itemlist):
        # We have a single selection only, so there is always only one item
        # in the list.
        item = itemlist.get_selected ()[0]
        self.update_status ("'%s' was selected" % item.text)
        self.current = item.text
        for child in self.frame.children:
            self.frame.remove_child (child)
            child.destroy ()
        self.items[item.text]()

    def init (self):
        # Title.
        lbl_title = Label ("OcempGUI widget example application")
        lbl_title.create_style()["font"]["size"] = 30
        lbl_title.topleft = 150, 5
        self.renderer.add_widget (lbl_title)
    
        # Selection list.
        list_widgets = ScrolledList (150, 450)
        list_widgets.topleft = 15, 100
        list_widgets.index = 1
        list_widgets.selectionmode = SELECTION_SINGLE
        list_widgets.connect_signal (SIG_SELECTCHANGED, self.select_widget,
                                     list_widgets)
        keys = self.items.keys ()
        keys.sort ()
        for item in keys:
            list_widgets.items.append (TextListItem (item))
        self.renderer.add_widget (list_widgets)

        # Window for the entries.
        self.frame = HFrame ()
        self.frame.minsize = (615, 450)
        self.frame.topleft = 170, 100
        self.renderer.add_widget (self.frame)

        # Code/Widget switch.
        btn_switch_code = RadioButton ("Show #Code")
        btn_switch_code.topleft = 170, 70
        btn_switch_code.index = 3
        btn_switch_code.sensitive = False
        btn_switch_widget = RadioButton ("Show #Widget", btn_switch_code)
        btn_switch_widget.topleft = 265, 70
        btn_switch_widget.index = 4
        btn_switch_widget.active = True
        self.renderer.add_widget (btn_switch_code, btn_switch_widget)

        # Status bar.
        self.status = StatusBar ()
        self.status.push_tip ("Start your journey by clicking on an item.")
        self.status.minsize = (770, self.status.minsize[1])
        self.status.tip_width = 650
        self.status.date_width = 120
        self.status.topleft = 15, 577
        self.renderer.add_widget (self.status)

if __name__ == "__main__":
    # Used to ensure the correct display of the date in the StatusBar
    # widget.
    import locale
    locale.setlocale (locale.LC_ALL, '')

    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (800, 610)
    re.title = "OcempGUI widgets examples"
    re.color = (234, 228, 223)
    re.show_layer_info = True

    mainscreen = MainScreen (re)
    mainscreen.init ()
    
    # Start the main rendering loop.
    re.start ()
