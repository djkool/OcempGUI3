"""A simple chat server, very nearly unmodified from
the simpleserver.py example on the Twisted site.
"""

from twisted.internet import reactor, protocol
from twisted.protocols import basic
from twisted.application import service, internet

class ChatServer (protocol.Protocol):
    def connectionMade (self):
        #print "Got new client!"
        self.factory.clients.append (self)

    def connectionLost (self, reason):
        #print "Lost a client!"
        self.factory.clients.remove (self)

    def dataReceived (self, data):
        for c in self.factory.clients:
            c.message (c, data)

    def message (self, user, message):
        self.transport.write (message)


def main():
    """This runs the protocol on port 8008"""
    factory = protocol.ServerFactory()
    factory.protocol = ChatServer
    factory.clients = []
    reactor.listenTCP(8008,factory)
    reactor.run()

if __name__ == '__main__':
    main()
