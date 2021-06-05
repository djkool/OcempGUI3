# papi test example.
import ocempgui.access.papi as papi
import atexit

# Main object - somewhat similar to the GailTopLevel object.
application = papi.AtkObject ()
application.name = "Application object"
application.description = "Application description"
application.role = papi.ATK_ROLE_APPLICATION
application.parent = None

def get_application ():
    global application
    return application

# Register the interfaces and initialize the atk-bridge.
papi.set_atk_root (get_application)
atexit.register (papi.shutdown)
papi.init ()

class SimpleA11y (papi.AtkObject):
    def __init__ (self):
        ifaces = papi.ATK_IFACE_COMPONENT | papi.ATK_IFACE_ACTION
        papi.AtkObject.__init__ (self, ifaces)

        # Implement some interfaces of ATK_IFACE_ACTION and
        # ATK_IFACE_COMPONENT.
        self.action_get_n_actions = self.__get_n_actions
        self.action_get_description = self.__get_description
        self.action_get_name = self.__get_name
        self.component_get_extents = self.__get_pos
        self.ref_state_set = self.__ref_state_set

    def __get_pos (self, coords):
        return 10, 10, 99, 99

    def __ref_state_set (self):
        return papi.AtkStateSet ()
    
    def __get_n_actions (self):
        return 1

    def __get_description (self, i):
        return "Example action."

    def __get_name (self, i):
        return "Example action name"
    
# Window dummy as child of the toplevel application object.
window = SimpleA11y ()

# Retrieve the state set, so we can set it active.
set = window.ref_state_set ()
set.add_state (papi.ATK_STATE_ACTIVE)

# Set some necessary information for accessibility applications.
window.role  = papi.ATK_ROLE_WINDOW
window.name = "Window A11y Object"
window.description = "Window Description"

# Link it with the application object
window.parent = application

# Signal testing - window:create will cause accessibility applications
# to note, that a new window was created for the application.
window.emit ("window:create")

print "Keeping myself alive. Press CTRL-C to exit the application."
while True:
    # Iterate the main processing loop of the ATK wrapper internals,
    # so that external applications can interact with the objects.
    papi.iterate ()
