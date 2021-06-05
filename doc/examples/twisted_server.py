# Example server to show how to use TwistedRenderer It runs forever, so
# you'll have to kill it by hand.
# Written by Ben Olsen.

from twisted.spread import pb
from twisted.internet import reactor

class Server (pb.Root):
    """
    Very basic PB server. One internal variable, two remote calls.  Make
    sure you run this before you start the client!
    """
    def __init__ (self):
        self.score = 0

    def remote_score (self, client, score):
        # Adjusts the internal score and calls the client's report
        # method.
        self.score += score
        client.callRemote ('report', self.score)
        # print 'new score: %s' % self.score

    def remote_reset (self, client):
        self.score = 0
        # print "score reset to 0"

reactor.listenTCP (8008, pb.PBServerFactory (Server ()))
reactor.run ()
