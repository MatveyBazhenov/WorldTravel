#ifndef RESULTPANEL_HPP
#define RESULTPANEL_HPP

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <wx/buffer.h>
#include <wx/button.h>
#include <wx/defs.h>
#include <wx/intl.h>
#include <wx/mstream.h>
#include <wx/panel.h>
#include <wx/protocol/http.h>
#include <wx/simplebook.h>
#include <wx/sizer.h>
#include <wx/socket.h>
#include <wx/sstream.h>
#include <wx/statline.h>

const int ID_DESCRIBTION = wxID_HIGHEST + 1;
const int ID_BACK2 = wxID_HIGHEST + 2;
const int ID_SAVEALL = wxID_HIGHEST + 3;
const int ID_TICKET = wxID_HIGHEST + 4;
const int ID_OPTION1 = wxID_HIGHEST + 5;
const int ID_OPTION2 = wxID_HIGHEST + 6;
const int ID_SAVEOPTION1 = wxID_HIGHEST + 7;
const int ID_SAVEOPTION2 = wxID_HIGHEST + 8;

class ResultPanel : public wxPanel {
public:
  ResultPanel(wxSimplebook *parent);
  ~ResultPanel();
  ResultPanel(wxSimplebook *parent, const std::string &responseData);

private:
  wxLocale _locale;
  void OnBack2(wxCommandEvent &event);
  nlohmann::json m_jsonData;
  void OnShowDescription(wxCommandEvent &event);
  wxButton *btnTicket;
  wxButton *btnOption1;
  wxButton *btnOption2;
  wxButton *btnDescribtion;
  wxButton *btnBack2;
  wxButton *btnSaveAll;
  wxButton *btnSaveOption1;
  wxButton *btnSaveOption2;
  wxBoxSizer *mainSizer;
  wxBoxSizer *topSizer;
  wxGridSizer *centerSizer;
  wxBoxSizer *bottomSizer;
  void OnSaveOption1(wxCommandEvent &event);
  void CustomMessageBox(wxWindow *parent, const wxString &message,
                        const wxString &title, const wxString &imagePath);
};

#endif