// Mines-Perfect: a minesweeper clone
// Copyright (C) 1995-2003  Christian Czepluch
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef LOGBOOK_H
#define LOGBOOK_H

#include <string>
#include <fstream> // fuer vector etc.
#include <vector>
#include <time.h>
#include "utils.h"
#include "options.h"

namespace MinesPerfect {


/* Probleme mit Konstanten in Headerfiles
// stat.
const wxString LOG_RAND_SEQ    = wxT("rand_seq";
const wxString LOG_BOARD_NR    = wxT("board_nr";
const wxString LOG_LEVEL       = wxT("level";
const wxString LOG_HEIGHT      = wxT("height";
const wxString LOG_WIDTH       = wxT("width";
const wxString LOG_DEEP        = wxT("deep";
const wxString LOG_NUM_MINES   = wxT("num_mines";
const wxString LOG_NUM_WHOLES  = wxT("num_wholes";

// stat. und dyn.
const wxString LOG_MODUS       = wxT("modus";
const wxString LOG_MURPHYS_LAW = wxT("murphys_law";
const wxString LOG_AUTO_STAGE  = wxT("auto_stage";
const wxString LOG_MAX_STAGE   = wxT("max_stage";
const wxString LOG_SHOW_MINES  = wxT("show_mines";

// dyn.
const wxString LOG_FLAG        = wxT("flag";
const wxString LOG_OPEN        = wxT("open";
const wxString LOG_SOLVE_ONE   = wxT("solve_one";
const wxString LOG_SOLVE_ALL   = wxT("solve_all";
const wxString LOG_OUT_OF_TIME = wxT("out_of_time";
*/

// stat.
#define LOG_VERSION      wxT("version")
#define LOG_VARIANT      wxT("variant")
#define LOG_RAND_SEQ     wxT("rand_seq")
#define LOG_BOARD_NAME   wxT("board_name")
#define LOG_BOARD_CHKSUM wxT("board_chksum")
#define LOG_LEVEL        wxT("level")
#define LOG_HEIGHT       wxT("height")
#define LOG_WIDTH        wxT("width")
#define LOG_DEEP         wxT("deep")
#define LOG_NUM_MINES    wxT("num_mines")
#define LOG_NUM_WHOLES   wxT("num_wholes")

// stat. und dyn.
#define LOG_MODUS       wxT("modus")
#define LOG_MURPHYS_LAW wxT("murphys_law")
#define LOG_AUTO_STAGE  wxT("auto_stage")
#define LOG_MAX_STAGE   wxT("max_stage")
#define LOG_SHOW_MINES  wxT("show_mines")

// dyn.
#define LOG_FLAG        wxT("flag")
#define LOG_OPEN        wxT("open")
#define LOG_SOLVE_ONE   wxT("solve_one")
#define LOG_SOLVE_ALL   wxT("solve_all")
#define LOG_HINT        wxT("hint")
#define LOG_OUT_OF_TIME wxT("out_of_time")
#define LOG_START_TIMER wxT("start_timer")

#define LOGS            wxT("logs")

//******************************************************************************
class LogException : public Exception
//------------------------------------------------------------------------------
{
public:

   LogException (const wxString& t) : Exception (t) {}
};

//******************************************************************************
struct Log
//------------------------------------------------------------------------------
{
  long         time1;
  wxString       name;
  int          val;  // muss mind. 32 Bit lang sein (rand_seq)
  bool         valid;

  Log (void) : time1(0), name(wxT("")), val(0), valid(true) {;}
  Log (wxString n, int v)
  : time1(0), name(n), val(v), valid(true) {;}

  bool isComputerLog() const { return name == LOG_OUT_OF_TIME 
                                      || name == LOG_START_TIMER; }
  void write (const wxString& fname) const;


  enum Type // 2-Bit
  { 
    TYPE_END = 0, TYPE_OPEN, TYPE_FLAG, TYPE_MISC 
  };

  enum Misc // 6-Bit
  { 
    MISC_MODUS       = 0, 
    MISC_MURPHYS_LAW = MISC_MODUS       + 5, // 0- 4
    MISC_HINT        = MISC_MURPHYS_LAW + 2, // 0 - 1
    MISC_SOLVE_ONE,   
    MISC_SOLVE_ALL, 
    MISC_AUTO_STAGE  = MISC_SOLVE_ALL   + 4, // 1 - 3
    MISC_MAX_STAGE   = MISC_AUTO_STAGE  + 4, // 0 - 3
    MISC_SHOW_MINES  = MISC_MAX_STAGE   + 4, // 0 - 3
    MISC_OUT_OF_TIME = MISC_SHOW_MINES  + 2, // 0 - 1
    MISC_START_TIMER 
  };

  int  toInt() const;
  Log (int num);
};

//******************************************************************************
class Logbook
//------------------------------------------------------------------------------
{
  private:

    clock_t      clock0;
    Options      options;
    vector<Log>  logs;
    uint32       rand_seq;
    bool         is_playing;
    unsigned     play_index;  // akt. Index zum Schreiben und Lesen
    unsigned     cur_index;   // Index ab dem neue Eintraege hinzugefuegt werden
                              // sollen.
                              // ! Zeigt immer auf einen gueltigen Log oder auf
                              // ! den ersten
    void    writeOptions (const wxString& fname) const;
    char    Int6ToChar (int num) const;
    int     CharToInt6 (char ch) const;

  public:

    Logbook (uint32 ran, const Options&  opt);

    const Log&      getPlayLog() const;
    uint32          getRandSeq() const { return rand_seq;    }
    const Options&  getOptions() const { return options;     }
    const unsigned  getNumLogs() const { return logs.size(); }
    bool            isPlaying()  const { return is_playing;  }
    bool            eop()        const { return play_index >= cur_index; }
                                 // eop = end-of-play

    void            write        (const wxString& fname);
    void            read         (const wxString& fname);
    void            startPlaying ();
    void            stopPlaying  () { is_playing = false; }
    void            operator>>   (Log& l);
    void            operator<<   (const Log& l);
    bool            undo         (bool all);
    bool            redo         (Log& log);
    bool            invalidateLast ();
//    wxString          getLogsAsStr () const;
//    void            setLogs      (const wxString& text);
    wxString          exportStr () const;
    void            importStr (const wxString& text);
};


} // namespace MinesPerfect

#endif
