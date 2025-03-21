#include "RegistrationWindow.hpp"

wxBEGIN_EVENT_TABLE(RegistrationWindow, wxFrame)
    EVT_BUTTON(ID_REG2, RegistrationWindow::OnRegisterButtonClicked)
wxEND_EVENT_TABLE()

RegistrationWindow::RegistrationWindow(wxWindow *parent)
    : wxFrame(
          parent,
          wxID_ANY,
          "Регистрация",
          wxDefaultPosition,
          wxDefaultSize
      ) {
    wxLocale locale;
    locale.Init(wxLANGUAGE_RUSSIAN);

    wxBoxSizer *mainSizer2 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer *centerSizer2 = new wxGridSizer(3, 1, 10, 10);

    txtLogin = new wxTextCtrl(this, wxID_ANY);
    txtLogin->SetHint("Логин");
    txtLogin->SetMinSize(wxSize(200, 100));

    txtPassword = new wxTextCtrl(this, wxID_ANY);
    txtPassword->SetHint("Пароль");
    txtPassword->SetMinSize(wxSize(200, 100));

    btnReg = new wxButton(this, ID_REG2, "Зарегестрироваться");
    btnReg->SetMinSize(wxSize(200, 100));

    centerSizer2->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
    centerSizer2->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
    centerSizer2->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 5);

    mainSizer2->Add(centerSizer2, 1, wxEXPAND | wxALL, 10);
    this->SetSizer(mainSizer2);
    Center();
    this->Fit();
}

void RegistrationWindow::OnRegisterButtonClicked(wxCommandEvent& event) {
    wxString login = txtLogin->GetValue();
    wxString password = txtPassword->GetValue();

    // делаем json
    wxString jsonBody = wxString::Format(
        R"({"username": "%s", "password": "%s"})",
        login, password
    );

    // отправляем на сервер запрос
    wxHTTP http;
    http.SetHeader("Content-Type", "application/json");
    http.SetTimeout(10);

    const wxString url = "http://localhost:8080/api/register";
    wxInputStream* stream = nullptr;
    
    try {
        if (http.Connect("localhost", 8080)) {
            stream = http.PostRequest("/api/register", jsonBody.ToUTF8());
        }

        // смотрим ответ
        const int responseCode = http.GetResponseCode();
        wxString response;
        
        if (stream) {
            wxStringOutputStream out(&response);
            stream->Read(out);
        }

        if (responseCode == 201) {
            wxMessageBox("Регистрация успешна!\nВаш ключ: " + response, "Успех", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Ошибка: " + response, "Ошибка", wxOK | wxICON_ERROR);
        }
    } catch (...) {
        wxMessageBox("Ошибка подключения к серверу", "Ошибка", wxOK | wxICON_ERROR);
    }

    if (stream) delete stream;
    http.Close();
}