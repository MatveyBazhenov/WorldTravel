#include "RegistrationWindow.hpp"

RegistrationWindow::RegistrationWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Регистрация", wxDefaultPosition,
              wxDefaultSize) {
  _locale.Init(wxLANGUAGE_RUSSIAN);
  this->SetBackgroundColour(wxColour(242, 242, 242));
  wxBoxSizer *mainSizer2 = new wxBoxSizer(wxVERTICAL);
  wxGridSizer *centerSizer2 = new wxGridSizer(0, 1, 10, 10);

  txtLogin = new wxTextCtrl(this, wxID_ANY);
  txtLogin->SetHint("Логин");
  txtLogin->SetMinSize(wxSize(150, 60));
  txtLogin->SetBackgroundColour(*wxWHITE);
  txtLogin->SetForegroundColour(*wxBLACK);

  txtPassword = new wxTextCtrl(this, wxID_ANY);
  txtPassword->SetHint("Пароль");
  txtPassword->SetMinSize(wxSize(150, 60));
  txtPassword->SetBackgroundColour(*wxWHITE);
  txtPassword->SetForegroundColour(*wxBLACK);

  btnReg = new RoundedButton(this, ID_REG2, "Зарегистрироваться",
                             wxColour(28, 124, 84), // цвет фона
                             *wxWHITE,              // цвет текста
                             30, // радиус скругления
                             wxDefaultPosition, wxSize(200, 60)); // размер

  wxStaticText *header = new wxStaticText(this, wxID_ANY, "Регистрация");
  wxFont headerFont(
      wxFontInfo(15).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));
  header->SetFont(headerFont);
  header->SetForegroundColour(wxColour(50, 50, 50));
  centerSizer2->Add(header, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM,
                    15);

  centerSizer2->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer2->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
  centerSizer2->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 5);

  // Шрифт кнопок
  wxFont btnF(wxFontInfo(12).Family(wxFONTFAMILY_SWISS).FaceName("Roboto"));
  txtPassword->SetFont(btnF);
  btnReg->SetFont(btnF);
  txtLogin->SetFont(btnF);
  //

  mainSizer2->Add(centerSizer2, 1, wxEXPAND | wxALL, 10);
  this->SetSizer(mainSizer2);
  Center();
  this->Fit();
}

void RegistrationWindow::CustomMessageBox(wxWindow *parent,
                                          const wxString &message,
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

void RegistrationWindow::OnRegisterButtonClicked(wxCommandEvent &event) {
  wxString login = txtLogin->GetValue();
  wxString password = txtPassword->GetValue();

  if (login.empty() || password.empty()) {
    wxMessageBox("Заполните все поля!", "Ошибка", wxOK | wxICON_ERROR);
    return;
  }

  wxString jsonBody = wxString::Format(
      R"({"username": "%s", "password": "%s"})", login, password);

  wxHTTP http;
  http.SetTimeout(10);

  try {
    if (http.Connect("localhost", 8080)) {
      http.SetMethod("POST");
      http.SetHeader("Content-Type", "application/json");

      if (!http.SetPostText("application/json", jsonBody)) {
        throw std::runtime_error("Failed to set POST data");
      }

      wxString response;
      wxInputStream *stream = http.GetInputStream("/registration");
      if (stream) {
        wxStringOutputStream output(&response);
        stream->Read(output);
        delete stream;
      }

      int responseCode = http.GetResponse();
      wxLogDebug("Response code: %d", responseCode);

      switch (responseCode) {
      case 200: { // Success
        nlohmann::json response_json =
            nlohmann::json::parse(response.ToStdString());
        if (response_json["status"].get<std::string>() == "ok") {
          // Сохраняем имя пользователя
          UserData::GetInstance().SetUsername(login);

          CustomMessageBox(this,
                           "Регистрация успешна!\nВаш ключ: " +
                               response_json["user_key"].get<std::string>(),
                           "Успех", "../images/Om_Nom_happy_200x200.png");

          // Закрываем окно регистрации (опционально)
          this->Close();
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

      case 409: { // Conflict
        CustomMessageBox(this,
                         R"(Ошибка: Пользователь с именем ")" + login +
                             R"(" уже существует)",
                         "Ошибка", "../images/Om_Nom_sad_200x200.png");
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

wxBEGIN_EVENT_TABLE(RegistrationWindow, wxFrame)
    EVT_BUTTON(ID_REG2, RegistrationWindow::OnRegisterButtonClicked)
        wxEND_EVENT_TABLE()