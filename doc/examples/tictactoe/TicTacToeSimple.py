from ocempgui.widgets import *
from ocempgui.widgets.Constants import *

SIG_TICTACTOE = "tictactoe"
TICTACTOE_WIN = 1
TICTACTOE_VALIDSQUARE = 0
TICTACTOE_INVALIDSQUARE = -1

class TicTacToe (Table):
    """The famous game as widget.
    """
    def __init__ (self):
        Table.__init__ (self, 3, 3)
        self._curplayer = "Player 1"
        self._finished = False
        
        for i in xrange (3):
            for j in xrange (3):
                button = ImageButton ("")
                button.minsize = 60, 60
                self.add_child (i, j, button)
                button.connect_signal (SIG_CLICKED, self._clicked, button,
                                       i, j)
        self._signals[SIG_TICTACTOE] = []

    def _clicked (self, button, i, j):
        """Sets the image of the button, if not already done.
        """
        if self._finished:
            return
        
        button = self.grid[(i, j)]
        # Check, if it is not already set.
        if button.picture == None:
            if self._curplayer == "Player 1":
                # Use the cross.
                button.picture = "cross.png"
            else:
                button.picture = "circle.png"
            self.run_signal_handlers (SIG_TICTACTOE, TICTACTOE_VALIDSQUARE)
            self._check_input ()

            # Set it after the check, so we can get the correct player name.
            if self._curplayer == "Player 1":
                self._curplayer = "Player 2"
            else:
                self._curplayer = "Player 1"
        else:
            self.run_signal_handlers (SIG_TICTACTOE, TICTACTOE_INVALIDSQUARE)

    def _check_input (self):
        """Checks for three in a row.
        """
        three = False
        image = None

        # Check the columns
        for i in xrange (3):
            if three:
                break
            image = self.grid[(i, 0)].path
            if image:
                three = (self.grid[(i, 1)].path == image) and \
                        (self.grid[(i, 2)].path == image)

        if not three:
            # Check the rows.
            for i in xrange (3):
                if three:
                    break
                image = self.grid[(0, i)].path
                if image:
                    three = (self.grid[(1, i)].path == image) and \
                            (self.grid[(2, i)].path == image)

        if not three:
            # Diagonal left to right
            image = self.grid[(0, 0)].path
            if image:
                three = (self.grid[(1, 1)].path == image) and \
                        (self.grid[(2, 2)].path == image)
            if not three:
                # Diagonal right to left
                image = self.grid[(2, 0)].path
                if image:
                    three = (self.grid[(1, 1)].path == image) and \
                            (self.grid[(0, 2)].path == image)

        if three:
            self._finished = True
            self.run_signal_handlers (SIG_TICTACTOE, TICTACTOE_WIN)

    curplayer = property (lambda self: self._curplayer)
