# Drawing engine usage example.
from ocempgui.widgets import *
from ocempgui.widgets.Constants import DEFAULTDATADIR

# Get the path where the theme engines are usually installed.
import sys
sys.path.append (DEFAULTDATADIR)
from themes import default

# Override the DefaultEngine with its drawing definitions so that we can
# implement our own draw_label() method.
class DropShadowEngine (default.DefaultEngine):

    def __init__ (self, style):
        default.DefaultEngine.__init__ (self, style)
    
    def draw_label (self, label):
        """Overrides the label drawing method and adds a dropshadow."""
        # We do not care about multiline labels. Thus pass those to the
        # parent.
        if label.multiline:
            return DefaultEngine.draw_label (self, label)

        cls = label.__class__
        state = label.state
        
        # Peek the style of the label so we can get the colors later.
        st = label.style or self.style.get_style (cls)

        # Save the label colour temporarily as we will change it for the
        # dropshadow. Because we are using references, not plain style
        # copies, we have to do this.
        fgcolor = self.style.get_style_entry (cls, st, "fgcolor", state)

        # The 2px added here are used for the dropshadow.
        width = 2 * label.padding + 2
        height = 2 * label.padding + 2

        # Backup the color.
        tmp = None
        if st.has_key ("fgcolor"): # Is fgcolor defined?
            tmp = st["fgcolor"]
        
        # Draw the text.
        front = None
        drop = None
        if label.mnemonic[0] != -1:
            front = self.draw_string_with_mnemonic \
                    (label.text, state, label.mnemonic[0], cls, st)
            # Swap colors.
            st["fgcolor"] = WidgetStyle ({ state : (100, 100, 100) })
            drop = self.draw_string_with_mnemonic \
                   (label.text, state, label.mnemonic[0], cls, st)
        else:
            front = self.draw_string (label.text, state, cls, st)
            # Swap colors.
            st["fgcolor"] = WidgetStyle ({ state : (100, 100, 100) })
            drop = self.draw_string (label.text, state, cls, st)

        # Surface creation done. Restore the colors.
        if not tmp:
            del st["fgcolor"]
        else:
            st["fgcolor"] = tmp

        # Get the size of the surface(s) and add it to the complete
        # width and height, the label will occupy.
        rect = front.get_rect ()
        width += rect.width
        height += rect.height
        
        # Guarantee size.
        width, height = label.check_sizes (width, height)

        # Blit all and return the label surface to the caller.
        surface = self.draw_rect (width, height, label.state, cls, st)
        surface.blit (drop, (label.padding + 2, label.padding + 2))
        surface.blit (front, (label.padding, label.padding))
        return surface

if __name__ == "__main__":
    re = Renderer ()
    re.create_screen (200, 200)
    re.title = "Style example."
    re.color = (234, 228, 223)

    # Set out own drawing engine in the Style class.
    base.GlobalStyle.engine = DropShadowEngine (base.GlobalStyle)

    label = Label ("Example label")
    label.create_style ()["font"]["size"] = 30
    label.topleft = 10, 10

    label2 = Label ("#Mnemonic")
    label2.create_style ()["font"]["size"] = 30
    label2.topleft = 10, 60

    button = CheckButton ("Dropshadow")
    button.topleft = 10, 100
    re.add_widget (label, label2, button)
    re.start ()
