#include "EnterWindow.hpp"
#include "UserData.hpp"

wxBEGIN_EVENT_TABLE(EnterWindow, wxFrame)
    EVT_BUTTON(ID_ENTER, EnterWindow::OnEnter) wxEND_EVENT_TABLE();

EnterWindow::EnterWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Вход", wxDefaultPosition, wxDefaultSize) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));

  sizer = new wxBoxSizer(wxVERTICAL);
  centsizer = new wxGridSizer(0, 1, 10, 10);

  wxStaticText *header = new wxStaticText(this, wxID_ANY, "Вход");
  wxFont headerFont(
      wxFontInfo(15).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));
  header->SetFont(headerFont);
  header->SetForegroundColour(wxColour(50, 50, 50));

  txtLogin = new wxTextCtrl(this, wxID_ANY);
  txtLogin->SetHint("Логин");
  txtLogin->SetMinSize(wxSize(150, 60));
  txtLogin->SetBackgroundColour(*wxWHITE);
  txtLogin->SetForegroundColour(*wxBLACK);

  txtPassword = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                               wxDefaultSize, wxTE_PASSWORD);
  txtPassword->SetHint("Пароль");
  txtPassword->SetMinSize(wxSize(150, 60));
  txtPassword->SetBackgroundColour(*wxWHITE);
  txtPassword->SetForegroundColour(*wxBLACK);

  btnEnter2 = new RoundedButton(this, ID_ENTER, "Войти",
                                wxColour(28, 124, 84), // цвет фона
                                *wxWHITE,              // цвет текста
                                30, // радиус скругления
                                wxDefaultPosition, wxSize(150, 60)); // размер

  centsizer->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
  centsizer->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
  centsizer->Add(btnEnter2, 0, wxALIGN_CENTER | wxALL, 5);

  // Шрифт кнопок
  wxFont btnF(wxFontInfo(12).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));
  txtPassword->SetFont(btnF);
  btnEnter2->SetFont(btnF);
  txtLogin->SetFont(btnF);
  //
  sizer->Add(header, 0, wxALIGN_CENTER | wxALL, 10);

  sizer->Add(centsizer, 1, wxEXPAND | wxALL, 10);

  this->SetSizer(sizer);
  Center();
  this->Fit();
}

void EnterWindow::CustomMessageBox(wxWindow *parent, const wxString &message,
                                   const wxString &title,
                                   const wxString &imagePath) {
  int width = 370;
  wxDialog dlg(parent, wxID_ANY, title, wxDefaultPosition, wxSize(width, 300));

  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticText *text = new wxStaticText(&dlg, wxID_ANY, message);
  text->Wrap(width - 20);
  mainSizer->Add(text, 0, wxALIGN_CENTER | wxALL, 10);

  wxStaticBitmap *bitmap = new wxStaticBitmap(
      &dlg, wxID_ANY, wxBitmap(imagePath, wxBITMAP_TYPE_PNG));
  mainSizer->Add(bitmap, 0, wxALIGN_CENTER | wxALL, 10);

  wxButton *okButton = new wxButton(&dlg, wxID_OK, "ОК");
  mainSizer->Add(okButton, 0, wxALIGN_CENTER | wxALL, 10);

  dlg.SetSizerAndFit(mainSizer);
  dlg.Centre();
  dlg.ShowModal();
}

void EnterWindow::OnEnter(wxCommandEvent &event) {
  wxString login = txtLogin->GetValue();
  wxString password = txtPassword->GetValue();

  if (login.empty() || password.empty()) {
    wxMessageBox("Заполните все поля!", "Ошибка", wxOK | wxICON_ERROR);
    return;
  }

  wxHTTP http;
  http.SetTimeout(10);

  try {
    if (http.Connect("localhost", 8080)) {
      http.SetMethod("POST");
      http.SetHeader("Content-Type", "application/json");

      wxString jsonBody = wxString::Format(
          R"({"login": "%s", "password": "%s"})", login, password);

      if (!http.SetPostText("application/json", jsonBody)) {
        throw std::runtime_error("Failed to set POST data");
      }

      wxString response;
      wxInputStream *stream = http.GetInputStream("/login");
      if (stream) {
        wxStringOutputStream output(&response);
        stream->Read(output);
        delete stream;
      }

      int responseCode = http.GetResponse();
      wxLogDebug("Response code: %d", responseCode);

      switch (responseCode) {
      case 200: { // Success
        // Извлекаем токен из ответа
        wxString token = ExtractTokenFromResponse(response);

        if (token.empty()) {
          wxLogError("Token not found in server response");
          CustomMessageBox(this, "Ошибка: Сервер не вернул токен авторизации",
                           "Ошибка", "../images/Om_Nom_sad_200x200.png");
        } else {
          // Сохраняем токен
          UserData::GetInstance().SetToken(token);
          CustomMessageBox(this, "Успешный вход!\nЗдравствуйте, " + login,
                           "Успех", "../images/Om_Nom_happy_200x200.png");
        }
        break;
      }
      case 400: { // Bad Request
        CustomMessageBox(this,
                         "Ошибка: Неверное имя пользователя или пароль!\nПоля "
                         "должны быть непустыми и содержать до 50 символов!",
                         "Ошибка", "../images/Om_Nom_sad_200x200.png");
        break;
      }
      case 401: { // Unauthorized
        CustomMessageBox(this, "Ошибка: Неверный пароль!", "Ошибка",
                         "../images/Om_Nom_surprised_200x200.png");
        break;
      }
      case 404: { // Not Find
        CustomMessageBox(this,
                         R"(Ошибка: Пользователя с именем ")" + login +
                             R"(" не существует!)",
                         "Ошибка", "../images/Om_Nom_surprised_200x200.png");
        break;
      }
      default: {
        throw std::runtime_error("Не получилось получить ответ от сервера");
        break;
      }
      }
    } else {
      throw std::runtime_error("Не удалось подключиться к серверу");
    }
  } catch (const std::exception &e) {
    CustomMessageBox(this, e.what(), "Ошибка",
                     "../images/Om_Nom_sad_200x200.png");
  } catch (...) {
    CustomMessageBox(this, "Неизвестная ошибка", "Ошибка",
                     "../images/Om_Nom_surprised_200x200.png");
  }

  http.Close();
}
wxString EnterWindow::ExtractTokenFromResponse(const wxString &response) {
  // Ищем начало токена: "token":"
  int start = response.Find("\"token\":\"");
  if (start == wxNOT_FOUND)
    return wxEmptyString;

  // Перемещаемся к началу значения токена
  start += 9; // Длина "\"token\":\""

  // Ищем конец токена (закрывающая кавычка)
  int end = response.find('"', start);
  if (end == wxNOT_FOUND)
    return wxEmptyString;

  // Извлекаем подстроку между start и end
  return response.SubString(start, end - 1);
}