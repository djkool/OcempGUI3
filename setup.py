#!/usr/bin/env python

# $Id: setup.py,v 1.36.2.17 2008/05/10 08:03:45 marcusva Exp $
# setup script for ocempgui

import distutils.sysconfig
from distutils.core import setup, Extension
from distutils.command.install_data import install_data
import os, sys, glob, time

VERSION = "0.2.9"

# Minimum requirements.
ATK_MINIMUM = "1.18.0"
PYGAME_MINIMUM = (1, 7, 1)
PYTHON_MINIMUM = (2, 3)

PAPI_VERSION = "0.0.5"
PAPI_DEBUG = "1"

##
# General configuration stuff.
##
def check_pkgconfig ():
    """Checks for the pkg-config utility."""
    if sys.platform == "win32":
        return os.system ("pkg-config > NUL") == 0
    else:
        return os.system ("pkg-config 2> /dev/null") == 256

def pkg_get_flags (package, flags, repl=None):
    """Gets the general compiler flags for a specific package using the
    pkg-config utility."""
    pipe = os.popen ("pkg-config %s %s" % (flags, package), "r")
    data = pipe.readline ().strip ()
    pipe.close ()
    if repl:
        return data.replace (repl, "").split ()
    return data.split ()

def pkg_get_all_cflags (name):
    """Gets all necessary flags for a compiler using the pkg-config
    utility."""
    return pkg_get_flags (name, "--cflags-only-I", "-I"), \
           pkg_get_flags (name, "--libs-only-L", "-L"), \
           pkg_get_flags (name, "--libs-only-l", "-l")

def get_directory_list (base):
    """Gets a list of subdirectories for the given base path."""
    # Get the needed ocempgui directory.
    realpath = os.path.split (os.path.abspath (sys.argv[0]))[0]

    # First get all the directories.
    paths = glob.glob (os.path.join (realpath, base, "*"))
    dirpaths = []
    for x in paths:
        if os.path.isdir (x):
            dirpaths += get_directory_list (os.path.join (base, x))

    # Although this should not happen, guarantee, that there is no CVS
    # target.
    dirpaths = [x for x in dirpaths if x.find ("CVS") == -1]

    # Do not forget the main directory.
    dirpaths = [os.path.join (realpath, base)] + dirpaths
    return dirpaths

def get_installation_files (base, installpath, filedict):
    """Create a nice list from it suitable for the data_files section of the
    distutils setup."""
    # Get the needed ocempgui directory.
    realpath = os.path.split (os.path.abspath (sys.argv[0]))[0]

    filelist = []
    for key in filedict:
        # We also need to get rid of the current directory prefix and
        # set it it to the correct installation prefix.
        try:
            path = key.split (os.path.join (realpath, base, ""))[1]
        except IndexError:
            # We got the main directory.
            path = ""
        path = os.path.join (installpath, path)

        # Add the files.
        files = []
        for installfile in filedict[key]:
            installfile = installfile.split (os.path.join (realpath, ""), 1)[1]
            files.append (installfile)
        filelist.append ((path, files))
    return filelist

##
# Installation routines.
##
def adjust_paths (datadir, files):
    """Adjusts the datadir paths in the style using files."""
    path = os.path.join (datadir, "share", "ocempgui")

    for f in files:
        fd = open (f, "r+")
        lines = fd.readlines ()
        for i, l in enumerate (lines):
            lines[i] = l.replace ("@DATAPATH@", path)
        fd.seek (0)
        fd.writelines (lines)
        fd.close ()

class InstallData (install_data):
    """Overrides the install_data behaviour to adjust the data paths."""
    def run (self):
        install_lib = self.get_finalized_command ("install_lib")
        bdist = self.get_finalized_command ("bdist")
        files = [os.path.join (install_lib.install_dir, "ocempgui", "widgets",
                               "Constants.py")]

        binary = False
        isrpm = False
        iswininst = False

        print bdist.bdist_base, self.install_dir

        for entry in sys.argv[1:]:
            if entry.startswith ("bdist"):
                binary = True
                if entry == "bdist_wininst":
                    iswininst = True
                elif entry == "bdist_rpm":
                    isrpm = True
            elif entry == "--format=rpm":
                isrpm = True
            elif entry == "--format=wininst":
                iswininst = True
    
        # Binary distribution build.
        if binary:
            path = bdist.bdist_base
            if isrpm:
                path = os.path.join (path, "rpm")
            elif iswininst:
                path = os.path.join (path, "wininst")
            else:
                path = os.path.join (path, "dumb")
            adjust_paths (self.install_dir.replace(path, ""), files)
        else:
            adjust_paths (self.install_dir, files)
        install_data.run (self)

        # Update the .pyc file (s).
        install_lib.byte_compile (files)

def get_papi_defines ():
    """Builds the defines list for the C Compiler."""
    val = [("DEBUG", PAPI_DEBUG), ("VERSION", '"0.0.5"')]
    if sys.platform == "win32":
        val.append (("IS_WIN32", "1"))
    return val

def get_papi_files ():
    """Gets the list of file to use for building the papi accessibility
    module."""
    path = os.path.join ("ocempgui", "access", "papi")
    files = glob.glob (os.path.join (path, "*.c"))
    return files

def get_data_files ():
    """Gets a list of the files beneath data/ to install."""
    installpath = os.path.join ("share", "ocempgui")
    path = "data"
    dirs = get_directory_list (path)
    filedict = {}
    for path in dirs:
        files = glob.glob (os.path.join (path, "*.*"))
        if files:
            filedict[path] = files
    return get_installation_files ("data", installpath, filedict)

def get_documentation_files ():
    """Gets a list of files to install as documentation."""
    installpath = os.path.join ("share", "doc", "ocempgui")
    docpaths = get_directory_list ("doc")

    # Traverse all the directories in the docpath an get the needed files.
    # Every file installed from the docs will have a suffix.
    filedict = {}
    for path in docpaths:
        files = glob.glob (os.path.join (path, "*.*"))
        if files:
            filedict[path] = files
    return get_installation_files ("doc", installpath, filedict)

def run_checks ():
    # Python version check.
    if sys.version_info < PYTHON_MINIMUM: # major, minor check
        raise Exception ("You should have at least Python >= %d.%d.x "
                         "installed." % PYTHON_MINIMUM)

    # Pygame versioning checks.
    pygame_version = None
    try:
        import pygame
        if pygame.version.vernum < PYGAME_MINIMUM:
            raise Exception ("You should have at least Pygame >= %d.%d.%d "
                             "installed" % PYGAME_MINIMUM)
	pygame_version = pygame.version.ver
    except ImportError:
        pass

    # Environment checks for the PAPI interfaces.
    papi = False
    atk_version = "not found"
    if not check_pkgconfig ():
        papi = False
    else:
        val = pkg_get_flags ("atk", "--modversion")
        if val:
            atk_version = val[0]
            if atk_version >= ATK_MINIMUM:
                papi = True

    print "\nThe following information will be used to build OcempGUI:"
    print "\t Python:     %d.%d.%d" % sys.version_info[0:3]
    print "\t Pygame:     %s" % pygame_version
    print "\t ATK:        %s" % atk_version
    print "\t Build Papi: %s\n" % papi

    return papi

if __name__ == "__main__":

    want_papi = False
    try:
        want_papi = run_checks ()
    except Exception, detail:
        print "Error:", detail
        sys.exit (1)

    docfiles = get_documentation_files ()
    datafiles = get_data_files ()

    setupdata = {
        "name" :  "OcempGUI",
        "version" : VERSION,
        "description": "Ocean Empire User Interface Library",
        "author": "Marcus von Appen",
        "author_email": "marcus@sysfault.org",
        "license": "BSD license",
        "url": "http://ocemp.sourceforge.net/gui.html",
        "packages": ["ocempgui",
                     "ocempgui.access",
                     "ocempgui.draw",
                     "ocempgui.events",
                     "ocempgui.object",
                     "ocempgui.widgets",
                     "ocempgui.widgets.components",
                     "ocempgui.widgets.images"],
        "data_files" : datafiles + docfiles,
        "cmdclass" : { "install_data" : InstallData },
        }

    if not want_papi:
        # Do not build the accessibility extension.
        setup (**setupdata)
    else:
        # Try to build the setup with the extension.
        includes, libdirs, libs = pkg_get_all_cflags ("atk")
        gmodflags = pkg_get_all_cflags ("gmodule-2.0")
        includes += gmodflags[0]
        libdirs += gmodflags[1]
        libs += gmodflags[2]
        #includes += distutils.sysconfig.get_python_inc ()

        defines = get_papi_defines ()
    
        warn_flags = ["-W", "-Wall", "-Wpointer-arith", "-Wcast-qual",
                      "-Winline", "-Wcast-align", "-Wconversion",
                      "-Wstrict-prototypes", "-Wmissing-prototypes",
                      "-Wmissing-declarations", "-Wnested-externs",
                      "-Wshadow", "-Wredundant-decls"
                      ]
        compile_args = warn_flags + ["-std=c99","-g"]
        papi = Extension ("ocempgui.access.papi", sources=get_papi_files (),
                          include_dirs=includes, library_dirs=libdirs,
                          libraries=libs, language="c",
                          define_macros=defines,
                          extra_compile_args=compile_args)
  
        setupdata["ext_modules"] = [papi]
        setup (**setupdata)
