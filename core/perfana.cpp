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

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <time.h>
#include <stdio.h>
#include <iostream> //Needed for cerr
#include <cstring> //Needed for strlen()
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#ifdef VISUAL_CPP
  #include <iostream> // visual studio (cerr)
  #include <minmax.h> // visual studio (max)
#endif

//#ifdef LINUX
  #include "linux-compatibility.h"
//#endif

using namespace std;

#include "perfana.h"
#include "api.h"

using namespace MinesPerfect;


static vector<int>       call_stack;
static vector<PerfFunc>  functions;

//******************************************************************************
static ulong  TimeStrLen (ulong time)
//------------------------------------------------------------------------------
{
  int  len = 0;

  for (; time; time /= 10)
    len++;

  return max (len + 1, 5);
}

//******************************************************************************
static wxChar*  TimeToA (ulong time, wxChar* const buf, ulong len)
//------------------------------------------------------------------------------
{
  if (len < 5)
    return 0;

  buf [len] = 0;
  buf [--len] = (wxChar) (time % 10 + wxT('0'));   time /= 10;
  buf [--len] = (wxChar) (time % 10 + wxT('0'));   time /= 10;
  buf [--len] = (wxChar) (time % 10 + wxT('0'));   time /= 10;
  buf [--len] = wxT('.');
  buf [--len] = (wxChar) (time % 10 + wxT('0'));   time /= 10;

  for (; time; time /= 10)
    buf [--len] = (wxChar) (time % 10 + wxT('0'));

  while (len > 0)
    buf [--len] = wxT(' ');

  return  buf;
}

//******************************************************************************
Perf::Perf (const wxString& fname)
//------------------------------------------------------------------------------
{
  func_nr   = functions.size();
  is_static = true;
  functions.push_back (PerfFunc (fname));
}

//******************************************************************************
Perf::Perf (const int fnr)
//------------------------------------------------------------------------------
{
  func_nr   = fnr;

  clock0    = getTimeMilliseconds();
  ASSERT (clock0 != -1);

  is_static = false;
  functions[fnr].num_calls++;
  functions[fnr].rec_level++;
  call_stack.push_back (fnr);
}

//******************************************************************************
Perf::~Perf()
//------------------------------------------------------------------------------
{
  if (is_static || !Glob::perfana_on)
    return;

  unsigned long long   clock1 = getTimeMilliseconds();

  ASSERT (clock0 != -1);
  
  ulong     total  = (ulong) ((1000000.0 / CLOCKS_PER_SEC) * (clock1 - clock0));
  int       fnr    = call_stack.back();
  call_stack.pop_back();
  if (functions[fnr].rec_level == 1)
    functions[fnr].time_total += total;
  functions[fnr].time_self += total;
  functions[fnr].rec_level--;

  if (!call_stack.empty())
  {
    functions[call_stack.back()].time_self -= total;
  }
  else
  {
    // Ausgabe
    wxFileOutputStream outFile(wxT("perfana.txt"));
    wxTextOutputStream out( outFile );

    const wxString  title_name       = wxT("name");
    const wxString  title_percent    = wxT("%");
    const wxString  title_total      = wxT("total/ms");
    const wxString  title_self       = wxT("self/ms");
    const wxString  title_num_calls  = wxT("calls");
    const wxString  title_self_aver  = wxT("self/call");
    const wxString  title_total_aver = wxT("total/call");
    ulong         max_name_len     = title_name.length();
    ulong         max_percent      = max ((ulong)title_percent.length(), (ulong)4U);
    ulong         max_total        = title_total.length();
    ulong         max_self         = title_self.length();
    ulong         max_num_calls    = title_num_calls.length();
    ulong         max_self_aver    = title_self_aver.length();
    ulong         max_total_aver   = title_total_aver.length();
    ulong         total_time;
    set<wxString>    name_set;
    unsigned short bufSize = 40;
    wxChar           buf[bufSize];

    sort (functions.begin(), functions.end(), PerfFuncCmp());

    // max
    for (unsigned i = 0; i < functions.size(); i++)
    {
      max_name_len   = max (max_name_len,   (unsigned long) functions[i].name.length());
      max_total      = max (max_total,      functions[i].time_total);
      max_self       = max (max_self,       functions[i].time_self);
      max_num_calls  = max (max_num_calls,  (ulong) functions[i].num_calls);

      if (functions[i].num_calls == 0)
      {
        max_self_aver  = 0;
        max_total_aver = 0;
      }
      else
      {
        max_self_aver  = max (max_self_aver,  functions[i].time_self
                         / functions[i].num_calls);
        max_total_aver = max (max_total_aver, functions[i].time_total
                         / functions[i].num_calls);
      }

      ulong  set_size = name_set.size();
      name_set.insert (functions[i].name);
      if (set_size == name_set.size()) {
        #ifdef wxUSE_UNICODE
          wcerr << wxT("'") << functions[i].name << wxT("' ist doppelt") << endl;
        #else
          cerr << wxT("'") << functions[i].name << wxT("' ist doppelt") << endl;
        #endif // wxUSE_UNICODE
      }
    }
    total_time = max_total;

    // max -> len
    #ifdef wxUSE_UNICODE
      swprintf (buf, bufSize, wxT("%lu"), max_num_calls);
    #else
      sprintf (buf, wxT("%lu"), max_num_calls);
    #endif // wxUSE_UNICODE
    
    max_total      = max (TimeStrLen (max_total), (ulong) title_total.length());
    max_self       = max (TimeStrLen (max_self ), (ulong) title_self.length());
    max_num_calls  = max ((size_t) bufSize, (size_t) title_num_calls.length());
    max_self_aver  = max (TimeStrLen (max_self_aver ),
                          (ulong) title_self_aver.length());
    max_total_aver = max (TimeStrLen (max_total_aver),
                          (ulong) title_total_aver.length());

    // Title
    out << wxT('\n')
        << title_name       << wxString (max_name_len   - title_name.length()       + 2, wxT(' '))
        << title_percent    << wxString (max_percent    - title_percent.length()    + 2, wxT(' '))
        << title_total      << wxString (max_total      - title_total.length()      + 2, wxT(' '))
        << title_self       << wxString (max_self       - title_self.length()       + 2, wxT(' '))
        << title_num_calls  << wxString (max_num_calls  - title_num_calls.length()  + 2, wxT(' '))
        << title_self_aver  << wxString (max_self_aver  - title_self_aver.length()  + 2, wxT(' '))
        << title_total_aver << wxString (max_total_aver - title_total_aver.length() + 2, wxT(' '))
        << wxT('\n');

    // Rest
    { // visual studio
    for (unsigned i = 0; i < functions.size(); i++)
    {
      out << functions[i].name
          << wxString (max_name_len - functions[i].name.length() + 2, wxT(' '));
      out << (wxUint32) (100 * functions[i].time_self / total_time ) << wxT('.')// out << setw (2) << 100 * functions[i].time_self / total_time << '.'
          << (wxUint32) (1000 * functions[i].time_self / total_time % 10) << wxT("  ");
      out << TimeToA (functions[i].time_total, buf, max_total) << wxT("  ");
      out << TimeToA (functions[i].time_self, buf, max_self) << wxT("  ");
      out << functions[i].num_calls << wxT("  ");// out << setw (max_num_calls) << functions[i].num_calls << "  ";
      out << TimeToA (functions[i].num_calls == 0 ?
                            0 : functions[i].time_self / functions[i].num_calls,
                      buf, max_self_aver) << wxT("  ");
      out << TimeToA (functions[i].num_calls == 0 ?
                           0 : functions[i].time_total / functions[i].num_calls,
                      buf, max_total_aver) << wxT('\n');
    }
    } // visual studio

    // aufraeumen
    { // visual studio
    for (unsigned i = 0; i < functions.size(); i ++)
    {
      functions[i].time_self  = 0;
      functions[i].time_total = 0;
      functions[i].num_calls  = 0;
    }
    } // visual studio
  }
}

