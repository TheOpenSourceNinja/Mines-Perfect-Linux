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

#include <iostream>
#include <stdlib.h>
#include <vector> // visual studio (nur fuer debug-konfiguration)
#include <fstream> // visual studio (nur fuer debug-konfiguration)
#include <sstream>

#ifdef __GNUG__
  #pragma implementation
  #pragma interface
#endif

#ifdef __GNUWIN32__
  #include <windows.h>
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "../core/api.h"
#include "../gui/gamectrl.h"

#include "dialogs.h"
#include "app.h"
#include "mine32.xpm"


// Konstanten
namespace MinesPerfect
{
  namespace Glob
  {
    extern int VERSION;
  }
}

// globale 
extern MainWindow*  main_win;

//******************************************************************************
AboutDialog::AboutDialog(wxWindow *parent)
//------------------------------------------------------------------------------
    : wxDialog(parent, -1, wxString(wxT("About")))
{
    m_topsizer = new wxBoxSizer(wxVERTICAL);
    m_hsizer1  = new wxBoxSizer(wxHORIZONTAL);
    m_hsizer3  = new wxBoxSizer(wxHORIZONTAL);

//    m_bitmap = new wxStaticBitmap(this, -1, wxIcon("mine32.ico", wxBITMAP_TYPE_ICO));
    m_bitmap = new wxStaticBitmap(this, -1, wxIcon(mine32_xpm));
    m_text1  = new wxStaticText(this, -1, wxString (wxT("Mines-Perfect\n\nVersion ")) 
                   + wxString ((wxChar) (wxT('0') + MinesPerfect::Glob::VERSION / 100 % 10), 1) + wxT(".")
                   + wxString ((wxChar) (wxT('0') + MinesPerfect::Glob::VERSION /  10 % 10), 1) + wxT(".")
                   + wxString ((wxChar) (wxT('0') + MinesPerfect::Glob::VERSION /   1 % 10), 1));
    m_text2  = new wxStaticText(this, -1, wxT("Copyright(c) 1995-2003, Christian Czepluch\n")
                                          wxT("Copyright(c) 2015, James Dearing\n")
                                          wxT("This program is distributed under the terms of the\n")
                                          wxT("GNU General Public License (GPL)  (see copyright.txt)"));
    m_text3  = new wxStaticText(this, -1, wxT("www.czeppi.de"));
    m_button = new wxButton(this, wxID_OK , wxT("OK"));

    m_hsizer1->Add(m_bitmap, 0, wxALL, 5);
    m_hsizer1->Add(0, 0, 2);
    m_hsizer1->Add(m_text1, 0, wxALL, 5);
    m_hsizer1->Add(0, 0, 3);
    
    m_hsizer3->Add(0, 0, 1);
    m_hsizer3->Add(m_text3,  0, wxALL, 5);
    m_hsizer3->Add(0, 0, 1);
    m_hsizer3->Add(m_button, 0, wxALL, 5);
    m_hsizer3->Add(0, 0, 1);
    
    m_topsizer->Add(m_hsizer1, 0, wxEXPAND | wxALL, 5);
    m_topsizer->Add(m_text2,   0,            wxALL, 5);
    m_topsizer->Add(m_hsizer3, 0, wxEXPAND | wxALL, 5);
    
    SetAutoLayout(TRUE);
    SetSizer(m_topsizer);

    m_topsizer->SetSizeHints(this);
    m_topsizer->Fit(this);
}


BEGIN_EVENT_TABLE(ShowBestTimesDialog, wxDialog)
    EVT_BUTTON      (-1,  ShowBestTimesDialog::OnButton)
    EVT_RADIOBUTTON (-1,  ShowBestTimesDialog::OnButton)
    EVT_CHOICE      (-1,  ShowBestTimesDialog::OnChoice)
END_EVENT_TABLE()

//******************************************************************************
void ShowBestTimesDialog::Show()
//------------------------------------------------------------------------------
{
  // Da die Sizer-Objekte sich die minimal Groesse beim Add() merken,
  // muss 'm_score_sizer' jedesmal neu aufgebaut werden.
  // Deshalb muss m_time_text und m_name_text als Klassenelemente definiert werden,

  int i;
  for (i = 3 * 4 - 1; i >= 0; --i)
  {
    m_score_sizer->Remove(i);
  }

  wxString  level_str_list[] = { wxT("Beginner"), wxT("Intermediate"), wxT("Expert") };

  for (i = 0; i <= 2; ++i)
  {
    wxString time_str, name_str, lable_str, send_str, level_str;

    level_str = level_str_list[i];
    level_str += wxT(":");
    
    time_str << m_options->getBoardType(m_sel_board)->records[i].time / 1000 << wxT(",")
             << m_options->getBoardType(m_sel_board)->records[i].time % 1000 << wxT(" s");

    if (m_options->getBoardType(m_sel_board)->records[i].time < MAX_MSECS
    &&  m_options->getBoardType(m_sel_board)->records[i].certified_board
    && !m_options->getBoardType(m_sel_board)->records[i].was_send)
      time_str = wxT("* ") + time_str;

    name_str << m_options->getBoardType(m_sel_board)->records[i].name.c_str();

    m_select_buttons[i]->SetValue(i == m_sel_level);

    m_level_text[i]->SetLabel(level_str);
    m_time_text[i]->SetLabel(time_str);
    m_name_text[i]->SetLabel(name_str);

    m_score_sizer->Add(m_select_buttons[i], 0, wxALL, 5);
    m_score_sizer->Add(m_level_text[i],     0, wxALL, 5);
    m_score_sizer->Add(m_time_text[i],      0, wxALL, 5);
    m_score_sizer->Add(m_name_text[i],      0, wxALL, 5);
  }

  Record& rec = m_options->getBoardType(m_sel_board)->records[m_sel_level];

  m_send_button->Enable( rec.time < MAX_MSECS && rec.certified_board );
  m_send_button->SetLabel( rec.time < MAX_MSECS && rec.certified_board && rec.was_send ? 
                           wxT("Set *") : wxT("Send"));
  m_reset_button->Enable( rec.time < MAX_MSECS );
//  m_show_button->Enable( rec.certified_board ); // geht so nicht,
//      weil:         rec.time == MAX_MSECS => !rec.certified_board                  
/*
  m_send_button->Enable    ( rec.time < MAX_MSECS && rec.certified_board );
  m_asterisk_button->Enable( rec.time < MAX_MSECS && rec.certified_board && rec.was_send);
*/
  m_topsizer->Fit(this);
}

//******************************************************************************
ShowBestTimesDialog::ShowBestTimesDialog(wxWindow *parent, 
                                         MinesPerfect::Options* options)
//------------------------------------------------------------------------------
    : wxDialog(parent, -1, wxString(wxT("Best Times"))), m_options(options), 
      m_sel_board(options->getBoardNr()), m_sel_level(options->getLevelNr())
{
  int i;

  // sizer
  m_topsizer      = new wxBoxSizer(wxVERTICAL);
  m_score_sizer   = new wxFlexGridSizer(4);
  m_button_sizer1 = new wxBoxSizer(wxHORIZONTAL);
  m_button_sizer2 = new wxBoxSizer(wxHORIZONTAL);
  m_nobody_sizer  = new wxBoxSizer(wxHORIZONTAL);

  // boardname
  wxString* board_names = new wxString[m_options->getNumBoards()];

  for (i = 0; i < m_options->getNumBoards(); ++i)
    board_names[i] = m_options->getBoardName(i).c_str();

  m_choice = new wxChoice(this, -1, wxPoint(10,10), wxSize(120,-1), 
                          m_options->getNumBoards(), board_names); 
  delete[] board_names;
  m_choice->SetSelection(m_sel_board);

  // scores
  for (i = 0; i < 3; ++i)
  {
    m_select_buttons[i] = new wxRadioButton (this, -1, wxT(""));
    m_level_text[i]     = new wxStaticText  (this, -1, wxT(""));
    m_time_text[i]      = new wxStaticText  (this, -1, wxT(""));
    m_name_text[i]      = new wxStaticText  (this, -1, wxT(""));
  }
  
  // buttons
  m_reset_button = new wxButton(this, -1, wxT("Reset"));
  m_send_button  = new wxButton(this, -1, wxT("Send"));
  m_show_button  = new wxButton(this, -1, wxT("Show"));

  // nobody
  m_nobody_text = new wxStaticText(this, -1, wxT("(nobody will be the last one!)"));

  // es muessen alle Elemente von scoresizer eingefuegt werden,
  // ansonsten gibt's eine Assert-Exception beim
  // entfernen.
  for (i = 0; i < 3 * 4; ++i)
    m_score_sizer->Add(0, 0, 0);

  m_ok_button = new wxButton(this, wxID_OK, wxT("OK"));
  
  m_button_sizer1->Add(0, 0, 1);
  m_button_sizer1->Add(m_show_button, 0, wxALL, 5);
  m_button_sizer1->Add(m_send_button, 0, wxALL, 5);
  m_button_sizer1->Add(0, 0, 1);

  m_button_sizer2->Add(0, 0, 1);
  m_button_sizer2->Add(m_ok_button,    0, wxALL, 5);
  m_button_sizer2->Add(m_reset_button, 0, wxALL, 5);
  m_button_sizer2->Add(0, 0, 1);

  m_nobody_sizer->Add(0, 0, 1);
  m_nobody_sizer->Add(m_nobody_text, 0, wxALL, 5);
  m_nobody_sizer->Add(0, 0, 1);

  // topsizer
  m_topsizer->Add(m_choice,        0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_score_sizer,   0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_button_sizer1, 0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_button_sizer2, 0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_nobody_sizer,  0, wxEXPAND | wxALL, 5);
    
  SetAutoLayout(TRUE);
  SetSizer(m_topsizer);

  m_topsizer->SetSizeHints(this);

  Show();
}

//******************************************************************************
void ShowBestTimesDialog::OnChoice(wxCommandEvent& event)
//------------------------------------------------------------------------------
{
  m_sel_board = m_choice->GetSelection();
  Show();
}

//******************************************************************************
void ShowBestTimesDialog::OnButton(wxCommandEvent& event)
//------------------------------------------------------------------------------
{
  for (unsigned i = 0; i < 3; i++)
  {
    if (event.GetEventObject() == m_select_buttons[i])
    {
      m_sel_level = i;
      Show();
      return;
    }
  }

  BoardType*  board_type = m_options->getBoardType(m_sel_board); 
  Record*     rec        = &board_type->records[m_sel_level];

  if (event.GetEventObject() == m_reset_button)
  {
    board_type->deleteRecord(m_sel_level);
    Show();
  }
  else if (event.GetEventObject() == m_send_button
       &&  rec->time < MAX_MSECS && rec->certified_board && rec->was_send)
  {
    rec->was_send = false;

    Show();
  }
  else if (event.GetEventObject() == m_send_button
       ||  event.GetEventObject() == m_show_button)
  {
    #ifndef wxUSE_UNICODE
      ostringstream ost;
    #else
      wostringstream ost;
    #endif
  
    ost << wxT("mines-perfect.sourceforge.net/highscores.php")
        << wxT("?board=")    << m_options->getBoardType(m_sel_board)->name 
        << wxT("&level=")    << m_sel_level;

    // password von Options
    wxString  password = wxT("");

    for (int i = 0; i < m_options->getNumUsers(); ++i)
    {
      User user = m_options->getUser(i);

      if (MinesPerfect::Lower(rec->name) == MinesPerfect::Lower(user.name))
        password = user.password;
    }

    // send button?
    if (event.GetEventObject() == m_send_button)
    {
      // rec->name ok?
      MinesPerfect::User::NameChecker  name_checker;
      wxString                           err_text;

      if (!name_checker.isValidString(rec->name, &err_text))
      {
        // user_vec, user_list
        vector<wxString> user_vec;
        m_options->getUserlist(user_vec);

        wxString* user_list = new wxString[user_vec.size()];

        for (unsigned k = 0; k < user_vec.size(); ++k)
          user_list[k] = wxString(user_vec[k].c_str());

        // new_name
        wxString msg_text =
          wxString(wxT("Your name isn't valid. Please choose another one.\n"))
                   + wxT("(") + err_text.c_str() + wxT(")");

        wxString           new_name = rec->name.c_str();
        GenValidator       validator (&new_name, &name_checker);
        ChooseComboDialog  dlg (this, msg_text, wxT("Invalid name"), 
                                user_vec.size(), user_list, validator);

        // rename User
        if (dlg.ShowModal() != wxID_OK)
        {
          Show();
          return;
        }
        m_options->renameUser (rec->name, new_name.c_str());

        // tidy up
        delete[] user_list;
      }

      // password von Dialog
      if (password == wxT(""))
      {
        wxString  msg_text =
          wxT("You need a password for the case,\n")
          wxT("that another user has the same name like you.\n")
          wxT("Use your email-adress for example.\n");

        wxString                             new_password = wxT("");
        MinesPerfect::User::PasswordChecker  password_checker;

        GenValidator       validator (&new_password, &password_checker);
        ChooseComboDialog  dlg (this, msg_text, wxT("Password?"), 
                                0, 0, validator);

        if (dlg.ShowModal() != wxID_OK || new_password == wxT(""))
        {
          Show();
          return;
        }

        MinesPerfect::User new_user;

        new_user.name     = rec->name;
        new_user.password = new_password;

        m_options->addUser(new_user);

        password = new_password.c_str();
      }

      ost << wxT("&name=")     << rec->name 
          << wxT("&password=") << password
          << wxT("&time=")     << rec->time
          << wxT("&checksum=") << board_type->getChecksum(m_sel_level,Glob::VERSION);
    }
    else if (event.GetEventObject() == m_show_button)
    {
      if (rec->time < MAX_MSECS && rec->certified_board && password != wxT(""))
        ost << wxT("&name=")     << rec->name 
            << wxT("&password=") << password;
    }

    // start browser
    StartBrowser (wxString(wxT("http")), ost.str());

    if (event.GetEventObject() == m_send_button)
      rec->was_send = true;

    Show();
  }
  else
  {
    event.Skip();
  }
}

BEGIN_EVENT_TABLE(SelfdefinedDialog, wxDialog)
    EVT_BUTTON(-1, SelfdefinedDialog::OnButton)
END_EVENT_TABLE()

//******************************************************************************
SelfdefinedDialog::SelfdefinedDialog(wxWindow *parent, 
                                     MinesPerfect::Level* lvl)
//------------------------------------------------------------------------------
    : wxDialog(parent, -1, wxString(wxT("Selfdefined"))) //, m_board_type(board_type)
{
  m_level = lvl;

  // inputsizer
  wxString  pages_str, rows_str, cols_str, mines_str, wholes_str;
  
  rows_str   << m_level->height;
  cols_str   << m_level->width;
  pages_str  << m_level->deep;
  mines_str  << m_level->num_mines;
  wholes_str << m_level->num_wholes;
  
  m_inputsizer = new wxFlexGridSizer(2, 4, 5);
  
  m_inputsizer->Add(new wxStaticText(this, -1, wxT("Rows:")), 0, 
                    wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
  m_rowsctrl = new wxTextCtrl(this, -1, rows_str);
  m_inputsizer->Add(m_rowsctrl, 1, wxGROW | wxALIGN_CENTER_VERTICAL);
  
  m_inputsizer->Add(new wxStaticText(this, -1, wxT("Columns:")), 0, 
                    wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
  m_colsctrl = new wxTextCtrl(this, -1, cols_str);
  m_inputsizer->Add(m_colsctrl, 1, wxGROW | wxALIGN_CENTER_VERTICAL);
  
  if (m_level->deep > 0)
  {
    m_inputsizer->Add(new wxStaticText(this, -1, wxT("Pages:")), 0, 
                      wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    m_pagesctrl = new wxTextCtrl(this, -1, pages_str);
    m_inputsizer->Add(m_pagesctrl, 1, wxGROW | wxALIGN_CENTER_VERTICAL);
  }
  
  m_inputsizer->Add(new wxStaticText(this, -1, wxT("Mines:")), 0, 
                    wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
  m_minesctrl = new wxTextCtrl(this, -1, mines_str);
  m_inputsizer->Add(m_minesctrl, 1, wxGROW | wxALIGN_CENTER_VERTICAL);
  
  if (m_level->num_wholes > -1)
  {
    m_inputsizer->Add(new wxStaticText(this, -1, wxT("Wholes:")), 0, 
                      wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    m_wholesctrl = new wxTextCtrl(this, -1, wholes_str);
    m_inputsizer->Add(m_wholesctrl, 1, wxGROW | wxALIGN_CENTER_VERTICAL);
  }
  
  // buttonsizer
  m_buttonsizer = new wxBoxSizer(wxVERTICAL);

  m_buttonsizer->Add(0, 0, 1);
  m_buttonsizer->Add(new wxButton(this, wxID_OK , wxT("OK")), 0, wxALL, 5);
  m_buttonsizer->Add(0, 0, 1);
  m_buttonsizer->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALL, 5);
  m_buttonsizer->Add(0, 0, 1);
  
  // topsizer
  m_topsizer = new wxBoxSizer(wxHORIZONTAL);
  
  m_topsizer->Add(m_inputsizer,  0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_buttonsizer, 0, wxEXPAND | wxALL, 5);
    
  SetAutoLayout(TRUE);
  SetSizer(m_topsizer);

  m_topsizer->SetSizeHints(this);
  m_topsizer->Fit(this);
}

//******************************************************************************
void SelfdefinedDialog::OnButton(wxCommandEvent& event)
//------------------------------------------------------------------------------
{
  long val;
  
  if (m_rowsctrl->GetValue().ToLong(&val))
    m_level->height = val;
  
  if (m_colsctrl->GetValue().ToLong(&val))
    m_level->width = val;
  
  if (m_level->deep > 0
  &&  m_pagesctrl->GetValue().ToLong(&val))
    m_level->deep = val;
  
  if (m_minesctrl->GetValue().ToLong(&val))
    m_level->num_mines = val;
  
  if (m_level->num_wholes > -1
  &&  m_wholesctrl->GetValue().ToLong(&val))
    m_level->num_wholes = val;
  
  event.Skip();
}

BEGIN_EVENT_TABLE(GenValidator, wxValidator)
    EVT_CHAR(GenValidator::OnChar)
END_EVENT_TABLE()

//******************************************************************************
bool GenValidator::Validate(wxWindow *parent)
//------------------------------------------------------------------------------
{
  wxString  errmsg = wxT("");
  wxString  text;

  if (m_validatorWindow == 0)
     return false;
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)))
     text = ((wxComboBox*) m_validatorWindow)->GetValue().c_str();
  else
    return false;

  if (m_checker != 0)
  {
    if (errmsg != wxT("") 
    ||  !m_checker->isValidString (text, &errmsg))
    {
      m_validatorWindow->SetFocus();
      errmsg = wxString(wxT("'")) + text + wxT("' ") + errmsg;

      wxMessageBox( errmsg.c_str(), wxT("Validation conflict"), 
                    wxOK | wxICON_EXCLAMATION, parent);

      return false;
    }
  }

  return true;
}

//******************************************************************************
bool GenValidator::TransferToWindow(void)
//------------------------------------------------------------------------------
{
  if (m_validatorWindow == 0 || m_text == 0)
    return false;
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)))
     ((wxComboBox*) m_validatorWindow)->SetValue(*m_text);
  else
    return false;

  return true;
}

//******************************************************************************
bool GenValidator::TransferFromWindow(void)
//------------------------------------------------------------------------------
{
  if (m_validatorWindow == 0 || m_text == 0)
    return false;
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)))
     *m_text = ((wxComboBox*) m_validatorWindow)->GetValue();
  else
    return false;

  return true;
}

//******************************************************************************
void GenValidator::OnChar(wxKeyEvent& event)
//------------------------------------------------------------------------------
{
  if (m_validatorWindow)
  {
    int      ch     = (int)event.KeyCode();
    wxString errmsg = wxT("");

    // we don't filter special keys and Delete
    if (ch >= WXK_SPACE && ch != WXK_DELETE && ch <= WXK_START
    && m_checker != 0 && !m_checker->isValidChar(ch))
    {
      if (!wxValidator::IsSilent())
        wxBell();

      // eat message
      return;
    }
  }

  event.Skip();
}

BEGIN_EVENT_TABLE(ChooseComboDialog, wxDialog)
END_EVENT_TABLE()

//******************************************************************************
ChooseComboDialog::ChooseComboDialog
//------------------------------------------------------------------------------
  (
    wxWindow*                   parent, 
    const wxString&             msg, 
    const wxString&             title, 
    int                         n,        // number elements of choices
    const wxString*             choices,  // list of strings to show
    const wxValidator&          validator
  )
  : wxDialog(parent, -1, title), m_n(n), m_list(choices)
{
  m_msg_text = new wxStaticText(this, -1, msg);
  m_combo    = new wxComboBox(this, -1, wxT("nobody"), 
                              wxDefaultPosition, wxDefaultSize, 
                              n, choices, 0, validator);

  // buttonsizer
  m_buttonsizer = new wxBoxSizer(wxHORIZONTAL);

  m_buttonsizer->Add(0, 0, 1);
  m_buttonsizer->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxALL, 5);
  m_buttonsizer->Add(0, 0, 1);
  m_buttonsizer->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALL, 5);
  m_buttonsizer->Add(0, 0, 1);
  
  // topsizer
  m_topsizer = new wxBoxSizer(wxVERTICAL);
  
  m_topsizer->Add(m_msg_text,    0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_combo,       0, wxEXPAND | wxALL, 5);
  m_topsizer->Add(m_buttonsizer, 0, wxEXPAND | wxALL, 5);
    
  SetAutoLayout(TRUE);
  SetSizer(m_topsizer);

  m_topsizer->SetSizeHints(this);
  m_topsizer->Fit(this);

  m_combo->SetFocus();
}

BEGIN_EVENT_TABLE(ExceptionDialog, wxDialog)
    EVT_BUTTON (-1, ExceptionDialog::OnButton)
END_EVENT_TABLE()

//******************************************************************************
ExceptionDialog::ExceptionDialog(wxWindow *parent, const wxString& exc_text)
//------------------------------------------------------------------------------
    : wxDialog(parent, -1, wxString(wxT("Exception"))),
      m_exc_text(exc_text)
{
    m_topsizer    = new wxBoxSizer(wxVERTICAL);
    m_buttonsizer = new wxBoxSizer(wxHORIZONTAL);

    wxString msg_text 
      = exc_text + wxT("\n\n")
        + wxT("Please send a bugreport for making Mines-Perfect better.\n")
        + wxT("(It's very easy. You only need an internet connection.)");

    m_static_text   = new wxStaticText(this, -1, msg_text);
    m_send_button   = new wxButton(this, -1,          wxT("Send"));
    m_save_button   = new wxButton(this, -1,          wxT("Save"));
    m_cancel_button = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

    m_buttonsizer->Add(0, 0, 1);
    m_buttonsizer->Add(m_send_button, 0, wxALL, 5);
    m_buttonsizer->Add(0, 0, 1);
    m_buttonsizer->Add(m_save_button, 0, wxALL, 5);
    m_buttonsizer->Add(0, 0, 1);
    m_buttonsizer->Add(m_cancel_button, 0, wxALL, 5);
    m_buttonsizer->Add(0, 0, 1);
    
    m_topsizer->Add(m_static_text, 0,            wxALL, 5);
    m_topsizer->Add(m_buttonsizer, 0, wxEXPAND | wxALL, 5);
    
    SetAutoLayout(TRUE);
    SetSizer(m_topsizer);

    m_topsizer->SetSizeHints(this);
    m_topsizer->Fit(this);
}

//******************************************************************************
void ExceptionDialog::OnButton(wxCommandEvent& event)
//------------------------------------------------------------------------------
{
  if (event.GetEventObject() == m_send_button)
  {
    wxString str = wxString(wxT("mines-perfect.sourceforge.net/bugreport.php?"))
                   + wxT("exceptiontext=") + m_exc_text 
                   + wxT("&logdata=") 
                   + main_win->game->m_logbook->exportStr();

    StartBrowser (wxString(wxT("http")), str.c_str());
  }
  else if (event.GetEventObject() == m_save_button)
  {
    wxString      start_dir = wxGetCwd();
    wxFileDialog  dlg(main_win, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.log"), wxSAVE);

    int rc = dlg.ShowModal();

    wxString  dlg_path = dlg.GetPath();

    dlg.Destroy();
    wxSetWorkingDirectory(start_dir);
  
    if (rc == wxID_OK)
      main_win->game->save(dlg_path.c_str());
  }
  else
  {
    event.Skip();
  }
}
