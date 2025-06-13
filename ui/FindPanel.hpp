#ifndef FINDPANEL_HPP
#define FINDPANEL_HPP

#include "RoundedButton.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <wx/button.h>
#include <wx/calctrl.h>
#include <wx/datectrl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/protocol/http.h>
#include <wx/spinctrl.h>
#include <wx/sstream.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

const int ID_FIND = wxID_HIGHEST + 20;
const int ID_ACCOUNT = wxID_HIGHEST + 21;

class FindPanel : public wxPanel {
public:
  FindPanel(wxNotebook *parent);
  ~FindPanel();

private:
  wxLocale _locale;
  wxStaticText *findText;
  wxTextCtrl *txtFrom;
  wxTextCtrl *txtTo;
  wxSpinCtrl *spinMoney;
  wxCalendarCtrl *calendar;

  wxBoxSizer *mainSizer4;
  wxBoxSizer *topSizer4;
  wxGridSizer *centerSizer4;
  wxBoxSizer *bottomSizer4;

  wxBitmap m_backgroundBitmap;
  wxBitmap m_backgroundBitmap2;

  void OnFindClick(wxCommandEvent &event);
  void OnSize(wxSizeEvent &event);
  void OnAccountClick(wxMouseEvent &event);
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output);
};

#endif