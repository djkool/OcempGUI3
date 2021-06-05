"""A simple gui chat client. It's built off the simpleclient.py in the
Twisted examples, and uses TwistedRenderer to tie the GUI and network
elements together.

The GUI has two screens: LoginScreen and ChatScreen. When the Protocol
detects a server connection, it emits an event to switch to the
ChatScreen. When the Protocol detects it has lost a connection, it emits
an event to switch to the LoginScreen.

The client and server doesn't really keep track of which users are
connected. This could be done with some more complex data being sent
across the wire (maybe even just a dict), but for this example, only
chat text is sent. Also, I didn't bother making the chat window scroll
down automatically when new items are added.  This may be something that
can be done with ScrolledList, but I haven't looked at it closely
enough.

written by Ben Olsen
"""

import pygame
from pygame.locals import *

from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
from ocempgui.widgets.components import TextListItem
from ocempgui.object import BaseObject
from ocempgui.events import INotifyable

from twisted.internet._threadedselect import install
install()
from twisted.internet import reactor, protocol

# local signals
SIG_REC = 'rec'
SIG_SEND = 'send'
COMMAND = 'command'

class LoginScreen (BaseObject):
    """The Login Screen contains Entries for server, port, and user
    name. The button emits a signal containing this info, which is
    picked up by the factory and used in reactor.connectTCP. It inherits
    from BaseObject to make signal processing easier.
    """
    def __init__ (self, re):
        BaseObject.__init__ (self)
        # Remember to set _signals before setting the manager!
        self._signals[COMMAND] = []
        self.connect_signal (COMMAND, self.processCommand)
        self.manager = re.active_layer[2]
        self.renderer = re
        
        # self.frame is the main widget for this class.
        self.frame = VFrame (Label ("Enter login information"))
        # labels on the left, entry fields on the right.
        self.table = Table (3, 3)
        self.table.set_column_align (0, ALIGN_RIGHT)
        self.table.set_column_align (1, ALIGN_LEFT)
        self.table.add_child (0, 0, Label ("Server:"))
        self.table.add_child (1, 0, Label ("Port:"))
        self.table.add_child (2, 0, Label ("Username:"))
        
        self.serverEntry = Entry ('localhost')
        self.portEntry = Entry ('8008')
        self.nameEntry = Entry ()
        self.table.add_child (0, 1, self.serverEntry)
        self.table.add_child (1, 1, self.portEntry)
        self.table.add_child (2, 1, self.nameEntry)
        self.frame.add_child (self.table)
        
        self.button = Button ("Connect")
        self.button.connect_signal (SIG_MOUSEDOWN, self.sendConnect)
        self.frame.add_child (self.button)
        
        # Set the frame roughly in the middle.
        self.frame.topleft = (190, 70)
    
    def sendConnect (self, data):
        # Called when the button gets SIG_MOUSEDOWN.
        self.emit (COMMAND, (self.serverEntry.text,
                             int (self.portEntry.text), self.nameEntry.text))
    
    def processCommand (self, data):
        # This function adds or removes self.frame from the renderer
        # based on command signals sent from the Protocol.
        if data == 'switch_to_chat':
            self.renderer.remove_widget (self.frame)
        if data == 'switch_to_login':
            self.renderer.add_widget (self.frame)
    
    def notify (self, event):
        self.run_signal_handlers (event.signal, event.data)
    
class ChatScreen (BaseObject):
    """The ChatScreen only has two widgets: the chat window, which is a
    ScrolledList, and the Entry field. The Entry is tied to the ENTER
    key and emits a signal telling the Protocol to send the attached
    data. Listens for SIG_REC events which are sent by the Protocol and
    adds the data to the ScrolledList.
    """
    def __init__ (self, re):
        BaseObject.__init__ (self)
        # Remember to set _signals before setting the manager!
        self._signals[SIG_REC] = []
        self.connect_signal (SIG_REC, self.addText)
        self._signals[COMMAND] = []
        self.connect_signal (COMMAND, self.processCommand)
        self.manager = re.active_layer[2]
        self.renderer = re
        
        # self.table is the main widget for this class
        self.table = Table(2,2)
        
        self.chatframe = VFrame (Label ("Chat Window"))
        self.chatwindow = ScrolledList (520, 240)
        self.chatwindow.selectionmode = SELECTION_NONE
        self.chatframe.add_child (self.chatwindow)
        
        self.entryFrame = VFrame ()
        self.entry = Entry ()
        self.entry.minsize = (520, 24)
        self.entry.connect_signal ("input", self.sendText)
        self.entryFrame.add_child (self.entry)
        
        self.table.add_child (0, 0, self.chatframe)
        self.table.add_child (1, 0, self.entryFrame)
        
    def addText (self, text):
        # Called when the class gets a SIG_REC from the Protocol.
        self.chatwindow.items.append (TextListItem (text))
        # Enforce update before proceeding, so scrolling will be
        # correct and is not influenced by the sprite update cycles.
        self.chatwindow.update ()
        self.chatwindow.vscrollbar.value = self.chatwindow.vscrollbar.maximum

    def sendText (self):
        # Called when the Entry gets ENTER.
        self.emit (SIG_SEND, self.entry.text)
        self.entry.text = ''
    
    def processCommand (self, data):
        # This function adds or removes self.table from the renderer
        # based on command signals sent from the Protocol.
        if data == 'switch_to_chat':
            self.renderer.add_widget (self.table)
        if data == 'switch_to_login':
            self.renderer.remove_widget (self.table)

    def notify (self, event):
        self.run_signal_handlers (event.signal, event.data)
    
class ChatClient (protocol.Protocol, INotifyable):
    """The Protocol does most of the work. When it detects a connection,
    it emits a signal to switch from LoginScreen to ChatScreen, and vice
    versa for losing a connetion. It watches for SIG_SEND events and
    sends the data across the connection.
    
    Note that because of the way the Factory instantiates the Protocol,
    it doesn't make sense to have this class inherit from
    BaseObject. Instead, once it knows it has a connection, it adds
    itself to the renderer's EventManager.
    """
    def __init__ (self):
        self.user = None
        
    def connectionMade (self):
        # The connection was successful, and we now have communication
        # with a server.
        self.manager.add_object (self, SIG_SEND)
        self.manager.emit (COMMAND, 'switch_to_chat')
        # The factory gets the login info, including user name, so set
        # it here to match the factory one.
        self.user = self.factory.user

    def connectionLost (self, reason):
        self.manager.emit (COMMAND, 'switch_to_login')

    def dataReceived(self, data):
        # When we get data from the network, emit an event to have it
        # displayed by the ChatScreen.
        self.manager.emit (SIG_REC, data)

    def notify (self, event):
        # Since the server doesn't keep track of user names, prepend
        # self.user to the beginning of each message
        if event.signal == SIG_SEND:
            self.transport.write (str (self.user) + '>> ' + str (event.data))

class ChatFactory (protocol.ClientFactory, INotifyable):
    """The Factory is the object used by the reactor. It keeps track of
    the connection with self.connected, so that hitting the LoginScreen
    connect button doesn't try to connect multiple times.
    """
    protocol = ChatClient
    def __init__ (self, re):
        # Set the Protocol's event manager.
        self.protocol.manager = re.active_layer[2]
        self.re = re
        self.connected = 0
        self.user = None

    def clientConnectionFailed (self, connector, reason):
        self.connected = 0

    def clientConnectionLost (self, connector, reason):
        self.connected = 0
    
    def startedConnecting (self, connector):
        self.connected = 1

    def notify (self, event):
        if (event.signal == 'command') and \
           (type(event.data) == tuple) and not self.connected:
            server = event.data[0]
            port = event.data[1]
            self.user = event.data[2]
            # Attempt the connection.
            reactor.connectTCP (server, port, self)

def main ():
    re = TwistedRenderer ()
    re.create_screen (550, 320)
    re.title = "Chat client"
    re.color = (234, 228, 223)
    re.reactor = reactor
    
    # Use something more legible than the default
    base.GlobalStyle.styles["default"]["font"]["name"] = "Arial"
    base.GlobalStyle.styles["default"]["font"]["size"] = 12
    
    # Create the two screen objects.
    chatter = ChatScreen (re)
    login = LoginScreen (re)
    
    # We always start at the login screen.
    re.add_widget (login.frame)

    # Create the factory.
    factory = ChatFactory (re)
    re.active_layer[2].add_object (factory, COMMAND)
    
    re.start()

if __name__ == '__main__':
    main ()
