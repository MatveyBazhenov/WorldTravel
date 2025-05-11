#include <wx/wx.h>

class RoundedButton : public wxPanel {
public:
  RoundedButton(wxWindow *parent, wxWindowID id, const wxString &label,
                const wxColour &bgColor, const wxColour &textColor,
                int cornerRadius = 15, const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize);

  void SetLabel(const wxString &label);
  void SetColors(const wxColour &bg, const wxColour &text);

private:
  void OnPaint(wxPaintEvent &event);
  void OnMouseEnter(wxMouseEvent &event);
  void OnMouseLeave(wxMouseEvent &event);
  void OnMouseDown(wxMouseEvent &event);
  void OnMouseUp(wxMouseEvent &event);

  wxColour bgColor;
  wxColour textColor;
  wxColour currentBgColor;
  int cornerRadius;
  wxString label;
  bool isPressed;
};