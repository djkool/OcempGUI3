# Profiling script.
import hotshot, hotshot.stats, sys

if __name__ == "__main__":
    if len (sys.argv) < 2:
        print "Usage: profiler.py scriptfile"
        sys.exit ()
    command = "execfile ('%s')" % sys.argv[1]
    prof = hotshot.Profile ("ocempgui.cache")
    prof.run (command)
    prof.close()
    stats = hotshot.stats.load ("ocempgui.cache")
    s = stats.strip_dirs ().sort_stats ("time", "calls")
    s.print_stats (10).print_callers (10)

