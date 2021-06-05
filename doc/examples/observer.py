# Subject/Observer usage example.
from ocempgui.events import Subject, IObserver

# The subject that should notify observers about state changes.
class MyObject (Subject):
    def __init__ (self):
        Subject.__init__ (self, "MyObject")
        self._x = "Simple Attribute"
        self._y = 1234567890
        self._z = None

    def get_x (self):
        return self._x
    
    def set_x (self, value):
        # Preserve old value.
        old = self._x
        self._x = value
        # Notify about change.
        self.notify ("x", old, value)

    def get_y (self):
        return self._y

    def set_y (self, value):
        # Preserve old value.
        old = self._y
        self._y = value
        # Notify about change.
        self.notify ("y", old, value)

    def get_z (self):
        return self._z

    def set_z (self, value):
        # Preserve old value.
        old = self._z
        self._z = value
        # Notify about change.
        self.notify ("z", old, value)

    x = property (get_x, set_x)
    y = property (get_y, set_y)
    z = property (get_z, set_z)

class OwnObserver (IObserver):
    def __init__ (self):
        pass

    def update (self, subject, prop, oldval, newval):
        if subject == "MyObject": # A MyObject instance, check details.
            if prop == "x":
                # Its x value changed.
                print "The x value of a MyObject instance changed from " \
                      "%s to %s" % (str (oldval), str (newval))
            elif prop == "y":
                # Its y value changed.
                print "The y value of a MyObject instance changed from " \
                      "%s to %s" % (str (oldval), str (newval))
            else:
                # Another value changed.
                print "The %s value of a MyObject instance changed from" \
                      "%s to %s" % (str (prop), str (oldval), str (newval))

class AnotherObserver (IObserver):
    def __init__ (self):
        pass

    def update (self, subject, prop, oldval, newval):
        print "Detail %s of %s changed from %s to %s" % (str (prop), subject,
                                                         str (oldval),
                                                         str (newval))

subject = MyObject ()

# Add tow observers doing
observer1 = OwnObserver ()
observer2 = AnotherObserver ()

subject.add (observer1, observer2)

subject.x = "FooBarBaz"
subject.y = subject.x * 3
subject.z = 100
