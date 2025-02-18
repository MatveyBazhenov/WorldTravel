#ifndef RESULTPANEL_H
#define RESULTPANEL_H

#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/wx.h>

class ResultPanel : public wxPanel {
public:
  ResultPanel(wxNotebook *parent);
  ~ResultPanel();

private:
  wxButton *btnTicket;
  wxButton *btnOption1;
  wxButton *btnOption2;
  wxButton *btnDescribtion;
  wxButton *btnBack2;
  wxButton *btnSaveAll;
  wxButton *btnSaveOption1;
  wxButton *btnSaveOption2;
};

#endif
