from ocempgui.widgets import *
from ocempgui.widgets.Constants import *
from TicTacToeSimple import *

def _set_label (data, tictactoe, label):
    if data == TICTACTOE_WIN:
        label.text = "Well done, %s!" % tictactoe.curplayer
    elif data == TICTACTOE_INVALIDSQUARE:
        label.text = "Invalid square, %s." % tictactoe.curplayer
    else:
        label.text = ""

if __name__ == "__main__":
    re = Renderer ()
    re.create_screen (400, 400)
    re.color = (234, 228, 223)

    lbl_title = Label ("The ultimative OcempGUI Tic Tac Toe")
    lbl_title.topleft = 15, 5
    lbl_title.create_style ()["font"]["size"] = 30

    lbl_win = Label ("")
    lbl_win.topleft = 90, 300
    lbl_win.create_style ()
    lbl_win.style["font"]["size"] = 30
    lbl_win.style["fgcolor"][STATE_NORMAL] = (255, 0, 0)
    
    tictactoe = TicTacToe ()
    tictactoe.topleft = 100, 100
    tictactoe.connect_signal (SIG_TICTACTOE, _set_label, tictactoe, lbl_win)
    re.add_widget (lbl_title, lbl_win, tictactoe)
    re.start ()
