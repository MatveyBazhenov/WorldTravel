#include "RoundedButton.hpp"

RoundedButton::RoundedButton(wxWindow *parent, wxWindowID id,
                             const wxString &label, const wxColour &bgColor,
                             const wxColour &textColor, int cornerRadius,
                             const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size), bgColor(bgColor), textColor(textColor),
      cornerRadius(cornerRadius), label(label), isPressed(false) {
  currentBgColor = bgColor;

  Bind(wxEVT_PAINT, &RoundedButton::OnPaint, this);
  Bind(wxEVT_ENTER_WINDOW, &RoundedButton::OnMouseEnter, this);
  Bind(wxEVT_LEAVE_WINDOW, &RoundedButton::OnMouseLeave, this);
  Bind(wxEVT_LEFT_DOWN, &RoundedButton::OnMouseDown, this);
  Bind(wxEVT_LEFT_UP, &RoundedButton::OnMouseUp, this);
}

void RoundedButton::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  dc.SetBrush(wxBrush(currentBgColor));
  dc.SetPen(*wxTRANSPARENT_PEN);

  dc.DrawRoundedRectangle(0, 0, GetSize().x, GetSize().y, cornerRadius);

  dc.SetTextForeground(textColor);
  dc.SetFont(GetFont());

  wxSize textSize = dc.GetTextExtent(label);
  int x = (GetSize().x - textSize.x) / 2;
  int y = (GetSize().y - textSize.y) / 2;
  dc.DrawText(label, x, y);
}

void RoundedButton::OnMouseEnter(wxMouseEvent &event) {
  currentBgColor = bgColor.ChangeLightness(110);
  Refresh();
}

void RoundedButton::OnMouseLeave(wxMouseEvent &event) {
  currentBgColor = bgColor;
  isPressed = false;
  Refresh();
}

void RoundedButton::OnMouseDown(wxMouseEvent &event) {
  currentBgColor = bgColor.ChangeLightness(90);
  isPressed = true;
  Refresh();
}

void RoundedButton::OnMouseUp(wxMouseEvent &event) {
  if (isPressed && GetClientRect().Contains(event.GetPosition())) {
    wxCommandEvent evt(wxEVT_BUTTON, GetId());
    ProcessEvent(evt);
  }
  currentBgColor = bgColor.ChangeLightness(110);
  isPressed = false;
  Refresh();
}

void RoundedButton::SetLabel(const wxString &label) {
  this->label = label;
  Refresh();
}

void RoundedButton::SetColors(const wxColour &bg, const wxColour &text) {
  bgColor = bg;
  textColor = text;
  currentBgColor = bg;
  Refresh();
}