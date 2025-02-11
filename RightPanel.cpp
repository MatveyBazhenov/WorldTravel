#include "RightPanel.h"
const int ID_EXIT = 103;
RightPanel::RightPanel(wxPanel *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150),
              wxBORDER_SUNKEN) {
  wxPanel *parentPanel2;
  parentPanel2 = parent;
  btnEx = new wxButton(this, ID_EXIT, wxT("EXIT"), wxPoint(10, 10));
}
RightPanel::~RightPanel() {}

void RightPanel::OnExit(wxCommandEvent &event) {
  wxPuts("Exit");
  Close(true);
}