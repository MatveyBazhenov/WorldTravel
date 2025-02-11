#include "LeftPanel.h"

const int ID_BPLUS = 101;
const int ID_BMINUS = 102;
LeftPanel::LeftPanel(wxPanel *parent)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
  wxPanel *parentPanel;
  parentPanel = parent;
  count = 0;
  btnPlus = new wxButton(this, ID_BPLUS, wxT("+"), wxPoint(10, 10));
  btnMinus = new wxButton(this, ID_BMINUS, wxT("-"), wxPoint(10, 60));
  
}
LeftPanel::~LeftPanel() {}
void LeftPanel::OnPlus(wxCommandEvent &event) {}
void LeftPanel::OnMinus(wxCommandEvent &event) {}