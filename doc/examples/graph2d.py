# Graph2D examples.
import sys, math
from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
try:
    import Numeric
except ImportError:
    Numeric = None
    try:
        import numpy
    except ImportError:
        print "Numeric or numpy could not be found"
        sys.exit (-1)

__function = "func_1"

def change(graph):
    global __function
    if __function == "func_1":
        graph.eval_func = lambda x: x / ((- 3 * x**2.0 + 2) * math.e**x)
        __function = "func_2"
    else:
        graph.eval_func = lambda x: x**4.0 - 3 * x**2.0 + 2 * x
        __function = "func_1"
    
def create_graph2d_view ():
    frame = VFrame (Label ("Graph2D"))
    frame.topleft = 10, 10

    # Create the graph.
    graph = Graph2D (400, 400)

    # Lock it, because we set some necessary information and want to
    # avoid excessive update() calls.
    graph.lock ()

    # Scale units for the x and y axis.
    graph.scale_units = ("cm", "kp")

    # Point of origin.
    graph.origin = 200, 200

    # We want to see negative values.
    graph.negative = True

    # The evaluation function and data to use.
    graph.eval_func = lambda x: x**4.0 - 3 * x**2.0 + 2 * x
    if Numeric:
        graph.data = Numeric.arrayrange (-10, 10, .001).tolist()
    else:
        graph.data = numpy.arange (-10, 10, .001).tolist()
    # Done, unlock.
    graph.unlock ()

    button = Button ("Change function")
    button.connect_signal (SIG_CLICKED, change, graph)
    
    frame.add_child (graph, button)
    
    return frame

if __name__ == "__main__":
    # Initialize the drawing window.
    re = Renderer ()
    re.create_screen (450, 500)
    re.title = "Graph2D examples"
    re.color = (234, 228, 223)
    re.add_widget (create_graph2d_view ())
    # Start the main rendering loop.
    re.start ()
