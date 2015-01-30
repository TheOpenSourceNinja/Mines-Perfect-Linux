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

//#include <Forms.hpp>  // g++: Exception

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#ifdef VISUAL_CPP
  #include <minmax.h> // visual studio (max)
#endif

using namespace std;

#ifdef LINUX
  #include "linux-compatibility.h"
#endif

#include "logbook.h"
#include "api.h"  // Exception

namespace MinesPerfect // wegen Compiler
{
  namespace Glob
  {
    extern  int   VERSION;
    extern  bool  log_on;  // Erstellt 'auto.log'-Datei (zum Fehlerreproduzieren)
  }
}

using namespace MinesPerfect;

wxString  AUTO_LOG_FNAME = wxT("auto.log");

//******************************************************************************
void Log::write (const wxString& fname) const
//------------------------------------------------------------------------------
{
  wxFile outFile( fname, wxFile::write_append );
  wxFileOutputStream outFileStream( outFile );
  wxTextOutputStream out(outFileStream);
  //ofstream  out (fname.c_str(), ios::out | ios::app);

  if (!outFileStream.IsOk())
    throw LogException ((wxT("Log::write(): ") + fname
                        + wxT(" couldn't open.")).c_str());

  out << (wxUint32) time1 << wxT(" ") << name << wxT(" ") << val << (valid ? wxT("") : wxT("!")) << wxT("\n");
}

//******************************************************************************
int Log::toInt() const
//------------------------------------------------------------------------------
{
  if (!valid)
    return 0;

  int misc;

  if      (name == LOG_FLAG)         return TYPE_FLAG + (val << 2);
  else if (name == LOG_OPEN)         return TYPE_OPEN + (val << 2);
  else if (name == LOG_MODUS)        misc = MISC_MODUS + val;
  else if (name == LOG_MURPHYS_LAW)  misc = MISC_MURPHYS_LAW + val;
  else if (name == LOG_HINT)         misc = MISC_HINT;
  else if (name == LOG_SOLVE_ONE)    misc = MISC_SOLVE_ONE;
  else if (name == LOG_SOLVE_ALL)    misc = MISC_SOLVE_ALL + val;
  else if (name == LOG_AUTO_STAGE)   misc = MISC_AUTO_STAGE + val;
  else if (name == LOG_MAX_STAGE)    misc = MISC_MAX_STAGE + val;
  else if (name == LOG_SHOW_MINES)   misc = MISC_SHOW_MINES;
  else if (name == LOG_OUT_OF_TIME)  misc = MISC_OUT_OF_TIME;
  else if (name == LOG_START_TIMER)  misc = MISC_START_TIMER;
  else                               return 0;

  return TYPE_MISC + (misc << 2);
}

//******************************************************************************
Log::Log (int num)
//------------------------------------------------------------------------------
{
  valid = true;
  time1 = 0;

  // val, name
  Type type = static_cast<Type> (num & 3);
  num >>= 2;

  if      (type == TYPE_FLAG)  { name = LOG_FLAG; val = num; }
  else if (type == TYPE_OPEN)  { name = LOG_OPEN; val = num; }
  else if (type == TYPE_MISC)
  {
    Misc misc = static_cast<Misc> (num);

    if      (misc == MISC_MODUS + 0)        { name = LOG_MODUS;       val = 0; }
    else if (misc == MISC_MODUS + 1)        { name = LOG_MODUS;       val = 1; }
    else if (misc == MISC_MODUS + 2)        { name = LOG_MODUS;       val = 2; }
    else if (misc == MISC_MODUS + 3)        { name = LOG_MODUS;       val = 3; }
    else if (misc == MISC_MODUS + 4)        { name = LOG_MODUS;       val = 4; }

    else if (misc == MISC_MURPHYS_LAW + 0)  { name = LOG_MURPHYS_LAW; val = 0; }
    else if (misc == MISC_MURPHYS_LAW + 1)  { name = LOG_MURPHYS_LAW; val = 1; }

    else if (misc == MISC_HINT)             { name = LOG_HINT;        val = 0; }
    else if (misc == MISC_SOLVE_ONE)        { name = LOG_SOLVE_ONE;   val = 0; }

    else if (misc == MISC_SOLVE_ALL + 1)    { name = LOG_SOLVE_ALL;   val = 1; }
    else if (misc == MISC_SOLVE_ALL + 2)    { name = LOG_SOLVE_ALL;   val = 2; }
    else if (misc == MISC_SOLVE_ALL + 3)    { name = LOG_SOLVE_ALL;   val = 3; }

    else if (misc == MISC_AUTO_STAGE + 0)   { name = LOG_AUTO_STAGE;  val = 0; }
    else if (misc == MISC_AUTO_STAGE + 1)   { name = LOG_AUTO_STAGE;  val = 1; }
    else if (misc == MISC_AUTO_STAGE + 2)   { name = LOG_AUTO_STAGE;  val = 2; }
    else if (misc == MISC_AUTO_STAGE + 3)   { name = LOG_AUTO_STAGE;  val = 3; }

    else if (misc == MISC_MAX_STAGE + 1)    { name = LOG_MAX_STAGE;   val = 1; }
    else if (misc == MISC_MAX_STAGE + 2)    { name = LOG_MAX_STAGE;   val = 2; }
    else if (misc == MISC_MAX_STAGE + 3)    { name = LOG_MAX_STAGE;   val = 3; }

    else if (misc == MISC_SHOW_MINES + 0)   { name = LOG_SHOW_MINES;  val = 0; }
    else if (misc == MISC_SHOW_MINES + 1)   { name = LOG_SHOW_MINES;  val = 1; }

    else if (misc == MISC_OUT_OF_TIME)      { name = LOG_OUT_OF_TIME; val = 0; }
    else if (misc == MISC_START_TIMER)      { name = LOG_START_TIMER; val = 0; }

    else                                    valid = false;
  }
  else
  {
    valid = false;
  }
}

//******************************************************************************
Logbook::Logbook (uint32 ran, const Options& opt)
//------------------------------------------------------------------------------
{
  rand_seq   = ran;
  options    = opt;
  is_playing = false;
  play_index = 0;
  cur_index  = 0;
  clock0     = clock();

  logs.clear();

  if (Glob::log_on)
    writeOptions (AUTO_LOG_FNAME);
}

//******************************************************************************
const Log& Logbook::getPlayLog() const
//------------------------------------------------------------------------------
{
  ASSERT (play_index < logs.size());

  return logs[play_index];
}

//******************************************************************************
void Logbook::writeOptions (const wxString& fname) const
//------------------------------------------------------------------------------
{
  wxFile outFile( fname, wxFile::write_append );
  wxFileOutputStream outFileStream( outFile );
  wxTextOutputStream out(outFileStream);

  if (!outFileStream.IsOk())
    throw LogException ((wxT("Logbook::writeOptions(): ") + fname
                       + wxT(" konnte nicht geoeffnet werden!")).c_str());

  out << 0 << wxT(" ") << LOG_VERSION      << wxT(" ") << Glob::VERSION          << wxT("\n");
  out << 0 << wxT(" ") << LOG_VARIANT      << wxT(" ") << _VARIANT_              << wxT("\n");
  out << 0 << wxT(" ") << LOG_RAND_SEQ     << wxT(" ") << (wxUint32) rand_seq               << wxT("\n");
  out << 0 << wxT(" ") << LOG_BOARD_NAME   << wxT(" ") << options.getBoardName() << wxT("\n");
  out << 0 << wxT(" ") << LOG_BOARD_CHKSUM << wxT(" ") << 0                      << wxT("\n");
  out << 0 << wxT(" ") << LOG_LEVEL        << wxT(" ") << options.getLevelNr()   << wxT("\n");
  out << 0 << wxT(" ") << LOG_HEIGHT       << wxT(" ") << options.getHeight()    << wxT("\n");
  out << 0 << wxT(" ") << LOG_WIDTH        << wxT(" ") << options.getWidth()     << wxT("\n");
  out << 0 << wxT(" ") << LOG_DEEP         << wxT(" ") << options.getDeep()      << wxT("\n");
  out << 0 << wxT(" ") << LOG_NUM_MINES    << wxT(" ") << options.getNumMines()  << wxT("\n");
  out << 0 << wxT(" ") << LOG_NUM_WHOLES   << wxT(" ") << options.getNumWholes() << wxT("\n");
  out << 0 << wxT(" ") << LOG_MODUS        << wxT(" ") << options.getModus()     << wxT("\n");
  out << 0 << wxT(" ") << LOG_MURPHYS_LAW  << wxT(" ") << options.getMurphysLaw()<< wxT("\n");
  out << 0 << wxT(" ") << LOG_AUTO_STAGE   << wxT(" ") << options.getAutoStage() << wxT("\n");
  out << 0 << wxT(" ") << LOG_MAX_STAGE    << wxT(" ") << options.getMaxStage()  << wxT("\n");
  out << 0 << wxT(" ") << LOG_SHOW_MINES   << wxT(" ") << options.getShowMines() << wxT("\n");
  out << wxT("\n");
}

//******************************************************************************
void Logbook::write (const wxString& fname)
//------------------------------------------------------------------------------
// Schreiben eines Logfiles
{
  writeOptions (fname);

  for (unsigned i = 0; i < play_index; i++)
    logs[i].write(fname);
}

//******************************************************************************
void Logbook::read (const wxString& fname)
//------------------------------------------------------------------------------
// Lesen eines Logfiles
{
  if (fname == AUTO_LOG_FNAME)
    throw LogException ((wxString (wxT("Logbook::read(): ")) + AUTO_LOG_FNAME
                       + wxT(" isn't allowed to read.")).c_str());
  
  wxFileInputStream inFile( fname );
  wxTextInputStream in( inFile );

  if (!inFile.IsOk())
    throw LogException ((wxT("Logbook::read(): ") + fname
                       + wxT(" cannot open.")).c_str());

  Log    log;
  Level  lvl;

  logs.clear();

  while (!inFile.Eof())
  {
    // str1, str2, str3
    wxString str1, str2, str3;
    
    str1 = in.ReadWord(); //in >> str1;
    if (str1.IsEmpty())
      break;
    else if (inFile.Eof())
      throw LogException (wxString(wxT("Logbook::read(1): Number of entries must be a ")
                                 wxT("multiple of three. (")) + str1 + wxT(")"));
    str2 = in.ReadWord(); //in >> str2;
    if (inFile.Eof())
      throw LogException (wxT("Logbook::read(2): Number of entries must be a ")
                          wxT("multiple of three."));
      
    str3 = in.ReadWord(); //in >> str3;
 
    // log
    if (str1.find_first_not_of (wxT("0123456789!")) != wxString::npos)
      throw LogException (wxString(wxT("Logbook::read: '")) + str1 + wxT("' must not contains non-digits."));
   
    log.time1 = strToInt (str1);
    log.name = str2;

    if (log.name == LOG_BOARD_NAME)
    {    
      log.val = options.findBoardNr(str3);
          
      if (log.val == -1)
        throw LogException (wxString(wxT("Logbook::read: board '")) + str3 + wxT("' not found!"));
    }
    else
    {
      if (str3.find_first_not_of (wxT("-0123456789!")) != wxString::npos)
        throw LogException (wxString(wxT("Logbook::read: '")) + str3 + wxT("' must not contains non-digits."));
        
      log.val = strToInt (str3);   
    }

    // log auswerten
    if (log.time1 == 0)
    {
      if      (log.name == LOG_RAND_SEQ)    rand_seq                = log.val;
      else if (log.name == LOG_BOARD_NAME)  options.setBoardNr       (log.val);
      else if (log.name == LOG_MODUS)       options.setModus ((Modus) log.val);
      else if (log.name == LOG_LEVEL)       lvl.nr       = (LevelNr) log.val;
      else if (log.name == LOG_HEIGHT)      lvl.height              = log.val;
      else if (log.name == LOG_WIDTH)       lvl.width               = log.val;
      else if (log.name == LOG_DEEP)        lvl.deep                = log.val;
      else if (log.name == LOG_NUM_MINES)   lvl.num_mines           = log.val;
      else if (log.name == LOG_NUM_WHOLES)  lvl.num_wholes          = log.val;
      else if (log.name == LOG_MURPHYS_LAW) options.setMurphysLaw (log.val != 0);
      else if (log.name == LOG_AUTO_STAGE)  options.setAutoStage     (log.val);
      else if (log.name == LOG_MAX_STAGE)   options.setMaxStage      (log.val);
      else if (log.name == LOG_SHOW_MINES)  options.setShowMines  (log.val != 0);
      else if (log.name != LOG_VERSION && log.name != LOG_VARIANT
           &&  log.name != LOG_BOARD_CHKSUM)
        throw LogException (wxString(wxT("Logbook::read: '")) + log.name + wxT("' not recognize."));
    }
    else
    {
      logs.push_back (log);
    }
  }

  options.setLevel (lvl);
  play_index = cur_index = logs.size();

  if (Glob::log_on)
    write (AUTO_LOG_FNAME);
}

//******************************************************************************
void Logbook::startPlaying (void)
//------------------------------------------------------------------------------
{
  if (Glob::log_on)
    writeOptions (AUTO_LOG_FNAME);

  play_index = 0;
  is_playing = true;
  clock0 = clock(); // Zeit synchronisieren
}

//******************************************************************************
void Logbook::operator>> (Log& log)
//------------------------------------------------------------------------------
// Lesen eines Log-Eintrages
{
  ASSERT (play_index < logs.size());
  ASSERT (play_index < cur_index);

//   logs[play_index].time1 = getTime();  //???

  log = logs[play_index];
  play_index++;

  Log  log2 = log;
  
  if (Glob::log_on)
    log.write (AUTO_LOG_FNAME); // auto_log protokolliert auch das Lesen.

  // Zeit synchronisieren
  clock0 = clock() - log.time1 * CLK_TCK / 1000;
}

//******************************************************************************
void Logbook::operator<< (const Log& log)
//------------------------------------------------------------------------------
// schreiben eines Log-Eintrages
{
  // check play_index
  if (isPlaying())
  {
    ASSERT (play_index < cur_index || log.isComputerLog());
  }
  else
  {
    // Leseaktionen muessen beendet sein!
    ASSERT (play_index == cur_index);
  }

  Log  log2 = log;

  // logs, play_index
  if (isPlaying())
  {
    if (log2.name == LOG_START_TIMER)
      log2.val = 0; // Wert steckt schon in time1

    // Waehrend eines Abspielens duerfen nur Out-Of-Time und Start-Timer
    // Logs geschrieben werden!
    if (!logs[play_index].isComputerLog())
      throw LogException (wxString(wxT("logs[].name == '")) + logs[play_index].name 
                         + wxT("' invalid!"));
    // Kommen Computerlogs in der selben Reihenfolge?  
    else if (logs[play_index].name != log2.name)
      throw LogException (wxT("Logbook::operator<<: log.name invalid"));
    else if (logs[play_index].val != log2.val)
      throw LogException (wxT("Logbook::operator<<: log.val invalid"));

    // Log schreiben
    if (Glob::log_on)
    {
      log2.time1 = logs[play_index].time1; 
      log2.write (AUTO_LOG_FNAME);
    }

    play_index++;
  }
  else // not playing
  {
    // AUTO_LOG_FNAME
    if (log.name == LOG_START_TIMER)
    {
      // Hier wird die clock0-Zeit vom Timer uebergeben
      log2.time1 = (log.val - clock0) * 1000 / CLK_TCK;
      log2.val    = 0; // Wert steckt schon in time1
    }
    else
    {
      log2.time1 = (clock() - clock0) * 1000 / CLK_TCK;
    }

    if (log2.time1 == 0)
      log2.time1 = 1;   // 0 wird fuer die Startoptionen benutzt

    if (Glob::log_on)
      log2.write (AUTO_LOG_FNAME);

    if (cur_index < logs.size())
      logs.erase (logs.begin() + cur_index, logs.end());  // moegliche redo-
                                                        // Actionen loeschen
    logs.push_back (log2);
    play_index = cur_index = logs.size();
  }
}

//******************************************************************************
bool Logbook::undo(bool all)
//------------------------------------------------------------------------------
// bereitet Logbook fuer ein Undo vor
// anschliessend muss es noch einmal durchgespielt werden
{
  if (isPlaying())
    throw LogException (wxT("Logbook::undo(): is read only!"));
  else if (cur_index > logs.size())
    throw LogException (wxT("Logbook::undo(): invalid cur_index"));
  else if (play_index != cur_index)
    throw LogException (wxT("Logbook::undo(): invalid play_index"));
    
  // first_valid
  bool     found_valid = false;
  unsigned first_valid; 

  for (first_valid = 0; first_valid < cur_index; ++first_valid)
  {
    if (!logs[first_valid].isComputerLog() && logs[first_valid].valid)
    {
      found_valid = true;
      break;
    }
  }

  // Keinen gueltigen -> Ende
  if (!found_valid)
    return false; 

/*
  // Keine gueltigen Logs -> nur auto.log akt.
  if (!found_valid)
  {
    if (Glob::log_on)
    {
      writeOptions(AUTO_LOG_FNAME);

      for (unsigned i = 0; i < cur_index && logs[i].isComputerLog(); ++i)
        logs[i].write(AUTO_LOG_FNAME);
    }
    
    // setze cur_index
    cur_index  = i;
    play_index = cur_index; // play_index mitanpassen
    
    return false; // keine wesentliche Aenderung
  }
*/
  if (cur_index < logs.size() 
  && (!logs[cur_index].valid || logs[cur_index].isComputerLog()))
    throw LogException (wxT("Logbook::undo(): invalid log!"));

  if (all)
  {
    cur_index  = first_valid;
    play_index = cur_index; // mitanpassen
  }
  else
  {
    while (true)
    { 
      if (cur_index == first_valid)
      {
        if (Glob::log_on)
          writeOptions (AUTO_LOG_FNAME);
        return false;
      }
         
      --cur_index;
      play_index = cur_index; // mitanpassen
      
      if (logs[cur_index].valid && !logs[cur_index].isComputerLog())
        break;
    }
  }
    
  return true;
}

//******************************************************************************
bool Logbook::redo (Log& log)
//------------------------------------------------------------------------------
// bereitet Logbook fuer ein Redo vor
// anschliessend muss log noch durchgefuehrt werden
{
  if (isPlaying())
    throw LogException (wxT("Logbook::redo(): is read only!"));

  if (cur_index >= logs.size())
    return false; // kein redo durchgefuehrt

/*
  // Falls cur_index == 0 und ungueltig ist, wird er auf den naechsten guelten
  // gesetzt
  if (cur_index == 0)
  {
    while (cur_index < logs.size() 
           && (!logs[cur_index].valid || logs[cur_index].isComputerLog()))
    {
      if (!logs[cur_index].isComputerLog() && Glob::log_on)
        logs[cur_index].write(AUTO_LOG_FNAME);
        
      ++cur_index;
      play_index = cur_index; // play_index mitaktualisieren
    }
    
    if (cur_index >= logs.size())
      return false; // kein redo durchgefuehrt
  }
*/  
  log = logs[cur_index];

  // cur_index muss immer auf einem gueltigen stehen
  if (!log.valid || log.isComputerLog())
    throw LogException (wxString(wxT("Logbook::redo(): invalid log! (")) + log.name + wxT("')"));

  // cur_index wird auf den naechsten freien oder den naechsten gueltigen Log
  // gesetzt.  
  do
  {
    if (!logs[cur_index].isComputerLog() && Glob::log_on)
      logs[cur_index].write(AUTO_LOG_FNAME);
      
    ++cur_index;
    play_index = cur_index; // play_index mitaktualisieren
  }
  while (cur_index < logs.size() 
         && (!logs[cur_index].valid || log.isComputerLog()));
  
  // Zeit synchronisieren
  clock0 = clock() - log.time1 * CLK_TCK / 1000;
  
  return true;
}

//******************************************************************************
bool Logbook::invalidateLast()
//------------------------------------------------------------------------------
{
  if (play_index < 1 || logs.size() < play_index)
    return false;

  logs[play_index - 1].valid = false;    
  return true;
}

//******************************************************************************
wxChar Logbook::Int6ToChar (int val) const
//------------------------------------------------------------------------------
{
  if      (val <  10)  return wxT('0') + val;
  else if (val <  36)  return wxT('A') + val - 10;
  else if (val <  62)  return wxT('a') + val - 36;
  else if (val == 62)  return wxT('-');
  else if (val == 63)  return wxT('_');

  throw LogException (wxT("Logbook::Int6ToChar: num_bits too large."));
}

//******************************************************************************
int Logbook::CharToInt6 (wxChar ch) const
//------------------------------------------------------------------------------
{
  if      (wxT('0') <= ch && ch <= wxT('9'))  return ch - wxT('0');
  else if (wxT('A') <= ch && ch <= wxT('Z'))  return ch - wxT('A') + 10;
  else if (wxT('a') <= ch && ch <= wxT('z'))  return ch - wxT('a') + 36;
  else if (ch == wxT('-'))               return 62;
  else if (ch == wxT('_'))               return 63;

  throw LogException (wxT("Logbook::CharToInt6: num_bits too large."));
}

//******************************************************************************
wxString Logbook::exportStr () const
//------------------------------------------------------------------------------
{
  wxString    func_name = wxT("Logbook::exportStr: ");
  wxString    text;
  wxChar      buf[20]; // fuer strToInt
  unsigned  i;
  wxChar      sep = wxT(',');

  //--- stat. Informationen ---

  //
  text =  intToStr (Glob::VERSION, buf, 10); 
  text += sep;
  text += intToStr (_VARIANT_, buf, 10);
  text += sep;
  text += options.getBoardName();
  text += sep;
  text += intToStr (options.getLevelNr(), buf, 10);
  text += sep;

  if (options.getLevelNr() == USER_DEFINED)
  {
    text += intToStr (options.getHeight(), buf, 10);
    text += sep;
    text += intToStr (options.getWidth(), buf, 10);
    text += sep;
    text += intToStr (options.getDeep(), buf, 10);
    text += sep;
    text += intToStr (options.getNumMines(), buf, 10);
    text += sep;
    text += intToStr (options.getNumWholes(), buf, 10);
    text += sep;
  }

  text += intToStr (options.getModus(), buf, 10);
  text += sep;
  text += intToStr (options.getMurphysLaw(), buf, 10);
  text += sep;

  text += intToStr (options.getAutoStage(), buf, 10);
  text += sep;
  text += intToStr (options.getMaxStage(), buf, 10);
  text += sep;
  text += intToStr (options.getShowMines(), buf, 10);
  text += sep;
  text += intToStr (rand_seq, buf, 10);
  text += sep;

  //--- dyn. Informationen ---

  // max_val
  int max_val = 0;
  for (i = 0; i < logs.size(); ++i)
  {
    if (logs[i].valid)
      max_val = max (max_val, logs[i].val);
  }

  // num_bits
  int num_bits = 2; // Logs::Type
  while (max_val > 0)
  {
    ++num_bits;
    max_val >>= 1;
  }
  num_bits = max (num_bits, 8); // 8 == Logs::Type + Logs::Misc

  if (num_bits + 6 - 1 > 31) // gibt sonst Probleme mit word
    throw LogException (func_name + wxT("num_bits too large."));

  text += Int6ToChar (num_bits);

  // 
  int word      = 0; // 32-Bit (max. 31 werden benutzt)
  int word_bits = 0; // belegte Bits von word

  for (i = 0; i < logs.size(); ++i)
  {
    int val = logs[i].toInt();

    if (val <= 0)
      continue;

    if (val > (1 << num_bits))
      throw LogException (func_name + wxT("val too large."));

    word += val << word_bits; // val an word anhaengen
    word_bits += num_bits;

    if (word >= (1 << word_bits))
      throw LogException (func_name + wxT("word too large."));

    // vollstaendige Buchstaben ausgeben
    while (word_bits >= 6)
    {
      text += Int6ToChar (word & ((1 << 6) - 1));
      word >>= 6;
      word_bits -= 6;

      if (word > (1 << word_bits))
        throw LogException (func_name + wxT("word too large."));
    }
  }

  if (word_bits >= 6)
    throw LogException (func_name + wxT("word_bits too large."));

  if (word >= (1 << word_bits))
    throw LogException (func_name + wxT("word too large."));

  // letzten unvollstaendigen Buchstaben ausgeben
  if (word_bits > 0)
    text += Int6ToChar (word);

  return text;
}

//******************************************************************************
void Logbook::importStr (const wxString& text)
//------------------------------------------------------------------------------
{
  wxString func_name = wxT("Logbook::importStr: ");

  //--- stat. Informationen ---

  // parts
  wxString::size_type  p1, p2;
  vector<wxString>     parts;

  for (p1 = p2 = 0; p2 != wxString::npos; p1 = p2 + 1)
  {
    p2 = text.find (wxT(','), p1);

    parts.push_back (text.substr (p1, p2 - p1));
  }

  if (parts.size() < 4) // min. bis Level
    throw LogException (func_name + wxT("too few parts."));

  unsigned i = 0;

  // Version + Variante
  int version = strToInt (parts[i++].c_str()); 
//  int variant = strToInt (parts[i++].c_str()); // wird nicht gebraucht
  i++; // variant ueberspringen

  if (version > Glob::VERSION)
    throw LogException (func_name + wxT("invalid version."));

  // board_name
  int board_nr = options.findBoardNr (parts[i++]);

  if (board_nr == -1)
    throw LogException (func_name + wxT("invalid boardname."));

  options.setBoardNr(board_nr);

  // level
  Level lvl;

  lvl.nr = (LevelNr) strToInt (parts[i++].c_str());

  if (lvl.nr == USER_DEFINED)
  {
    if (parts.size() != 11 + 5)
      throw LogException (func_name + wxT("number of parts != 16."));

    lvl.height     = strToInt (parts[i++].c_str());
    lvl.width      = strToInt (parts[i++].c_str());
    lvl.deep       = strToInt (parts[i++].c_str());
    lvl.num_mines  = strToInt (parts[i++].c_str());
    lvl.num_wholes = strToInt (parts[i++].c_str());
  }
  else
  {
    if (parts.size() != 11)
      throw LogException (func_name + wxT("number of parts != 11."));
  }
  
  if (!options.setLevel(lvl))
    throw LogException (func_name + wxT("error at setLevel()."));

  // modus + murphys_law
  options.setModus      ((Modus) strToInt (parts[i++].c_str()));
  options.setMurphysLaw (strToInt (parts[i++].c_str()) != 0);

  // auto_stage, max_stage, show_mines
  options.setAutoStage (strToInt (parts[i++].c_str()));
  options.setMaxStage  (strToInt (parts[i++].c_str()));
  options.setShowMines (strToInt (parts[i++].c_str()) != 0);

  // rand_seq
  rand_seq = strToInt (parts[i++].c_str());

  //--- dyn. Informationen ---

  wxString log_data = parts[i++];

  if (i != parts.size())
    throw LogException (func_name + wxT("too much parts."));

  logs.clear();

  if (log_data.size() <= 1) // 1. Zeichen ist nur Laenge
    return;

  //
  unsigned k         = 0;
  int      num_bits  = CharToInt6 (log_data[0]);
  int      word      = 0;      // 31-Bits genuegen
  int      word_bits = 0;      // belegte Bits von word

  if (num_bits + 6 - 1 > 31) // gibt sonst Probleme mit word
    throw LogException (func_name + wxT("num_bits too large."));

  if (num_bits < 8) // 8 == Logs::Type + Logs::Misc
    throw LogException (func_name + wxT("num_bits too small."));

  while (true)
  {
    while (word_bits < num_bits)
    {
      ++k;
      if (k >= log_data.size())
        break; // fertig

      word += CharToInt6 (log_data[k]) << word_bits;
      word_bits += 6;

      if (word >= (1 << word_bits))
        throw LogException (func_name + wxT("word too large."));
    }

    if (k >= log_data.size())
      break; // fertig

    Log new_log (word & ((1 << num_bits) - 1));
    word >>= num_bits;
    word_bits -= num_bits;

    new_log.time1 = 1; // != 0
    logs.push_back (new_log);
  }

  //
  play_index = cur_index = logs.size();

  if (Glob::log_on)
    write (AUTO_LOG_FNAME);
}
