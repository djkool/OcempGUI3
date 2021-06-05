# Example client to show how to use TwistedRenderer See how many times
# you can click the button in a 5-second period.
# Written by Ben Olsen

import pygame
from random import randint

from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
from ocempgui.object import BaseObject

from twisted.internet.threadedselectreactor import install
install()
from twisted.internet import reactor
from twisted.spread import pb

REPORT = pygame.locals.USEREVENT + 4
SCORE = pygame.locals.USEREVENT + 5

class NetController (BaseObject, pb.Referenceable):
    def __init__ (self, re):
        """
        The network controller. Can call remote PB methods, and because
        it inherits from pb.Referenceable, can also contain remote
        methods callable by the server.
        
        Make sure you set the internal _signals before setting the
        manager, otherwise the signals won't get connected to the event
        manager.
        """
        BaseObject.__init__ (self)
        self._signals[SCORE] = []
        self.connect_signal (SCORE, self.add_one)
        self.manager = re.active_layer[2]
        
        # Set up twisted connection.
        self.factory = pb.PBClientFactory ()
        d = self.factory.getRootObject ()
        d.addCallback (self.setRoot)
        reactor.connectTCP ('localhost', 8008, self.factory)
        self.root = None
        
    def setRoot (self, root):
        self.root = root
        reactor.callLater (0, self.resetter)
        
    def add_one (self, data):
        # Call the server's score method and update score by 1. Although
        # data is passed to this method, it isn't used. Note that it
        # sends self as an arg, so that the server can call a remote
        # method on this object.
        if self.root:
            self.root.callRemote ('score', self, 1)
    
    def resetter (self):
        # This method is called by the reactor every 5 seconds to reset
        # the score on the server.
        if self.root:
            self.root.callRemote ('reset', self)
            self.emit (REPORT, 0)
            reactor.callLater (5, self.resetter)
        
    def remote_report (self, score):
        # This method is called by the server and emits a REPORT event
        # which is picked up by the JumpingButton object.
        self.emit (REPORT, score)
            
    def notify (self, event):
        self.run_signal_handlers (event.signal, event.data)

class JumpingButton (BaseObject):
    def __init__ (self, re):
        """
        The Jumping Button game. When the button is clicked, it emits a
        SCORE event, which is picked up by the network controller. It
        listens for a REPORT event and updates the score display with
        the value given by the server.
        """
        BaseObject.__init__ (self)
        self._signals[REPORT] = []
        self.connect_signal (REPORT, self.adjustScore)
        self.manager = re.active_layer[2]
        
        # create gui elements
        self.btn = self.drawButton ((20, 20))
        self.scoreLabel = Label ("Current score: ")
        re.add_widget (self.btn)
        re.add_widget (self.scoreLabel)
        
    def clicked (self, data):
        x = randint (1, 550 - self.btn.width)
        y = randint (1, 470 - self.btn.height)
        self.btn.topleft = (x, y)
        self.emit (SCORE, data.pos)
        
    def drawButton (self, loc):
        button = Button ("Click me!")
        button.topleft = loc
        button.connect_signal (SIG_MOUSEDOWN, self.clicked)
        return button
    
    def adjustScore (self, score):
        self.scoreLabel.text = "Current score: %s" % score
    
    def notify (self, event):
        self.run_signal_handlers (event.signal, event.data)

if __name__ == '__main__':
    re = TwistedRenderer ()
    re.reactor = reactor

    re.create_screen (550, 470)
    re.title = "Jumping Button"
    re.color = (234, 228, 223)

    net = NetController (re)
    game = JumpingButton (re)
    
    re.start()
