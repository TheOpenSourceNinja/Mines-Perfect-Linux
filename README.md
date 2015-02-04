# Mines-Perfect-Linux
This is an attempt to port Mines-Perfect (http://mines-perfect.sourceforge.net/) to Linux. I (James Dearing) decided to fork the project in part because my attempts to contact the original creator have so far been unsuccessful, in part because the original project did not use version control software like Git, and in part because I would prefer to keep all my OSS projects on one web site. If we successfully port the game to Linux, then I will try again to contact the original creator of Mines-Perfect and get this fork reintegrated into the original project.

All contributions are welcome.

Unfortunately porting this to Linux requires that every string support Unicode. Thus every string has been converted to a wxString, so every file - contrary to what is stated in the original readme below - now depends on the wxWidgets library.

# Original Readme

            Mines-Perfect - a minesweeper-clone
            -----------------------------------

Introduction:

  The sources contain all files which are necessary to build
  the mineperf.exe file.
  The following files are only in the precompiled distribution(s):

  - additional boards
  - help-files
  - changes-file


File-structure:

  - core:  all files with have nothing to do with a GUI
  - gui:   all files which represent a control but use no extern library
  - wxwin: all files which use the wxWindows-library


You need for build:

  - Visual C++ 6.0 (use wxwin/mineperf.dsw)
  - wxwindows 2.40 (Sourceforge)

  or

  - Dev-C++ 4.9.7/gcc 2.95 (Sourceforge)  (use wxwin/mineperf.dev)
  - wxwindows 2.29 (Sourceforge)  (there exist a plug-in for Dev-C++)

  or

  - mingw/gcc 2.95 (use wxwin/makefile.win)
  - wxwindows 2.29 (Sourceforge)


!! You cannot send highscores with a selfcompiled mineperf.exe !!


(old contact info removed)
