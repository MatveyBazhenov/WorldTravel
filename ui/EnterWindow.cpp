#include "EnterWindow.hpp"

wxBEGIN_EVENT_TABLE(EnterWindow, wxFrame)
    EVT_BUTTON(ID_ENTER, EnterWindow::OnEnter)
        EVT_THREAD(wxID_ANY, EnterWindow::OnThreadEvent) wxEND_EVENT_TABLE();

EnterWindow::EnterWindow(wxWindow *parent)
    : wxFrame(parent, wxID_ANY, "Вход", wxDefaultPosition, wxDefaultSize) {
    wxLocale locale;
    locale.Init(wxLANGUAGE_RUSSIAN);
    sizer = new wxBoxSizer(wxVERTICAL);
    centsizer = new wxGridSizer(3, 1, 10, 10);

    txtLogin = new wxTextCtrl(this, wxID_ANY);
    txtLogin->SetHint("Логин");
    txtLogin->SetMinSize(wxSize(200, 100));

    txtPassword = new wxTextCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_PASSWORD
    );
    txtPassword->SetHint("Пароль");
    txtPassword->SetMinSize(wxSize(200, 100));

    btnEnter2 = new wxButton(this, ID_ENTER, "Войти");
    btnEnter2->SetMinSize(wxSize(200, 100));

    centsizer->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
    centsizer->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
    centsizer->Add(btnEnter2, 0, wxALIGN_CENTER | wxALL, 5);

    sizer->Add(centsizer, 1, wxEXPAND | wxALL, 10);

    this->SetSizer(sizer);
    Center();
    this->Fit();
}

void EnterWindow::OnEnter(wxCommandEvent &event) {
    wxString login = txtLogin->GetValue();
    wxString password = txtPassword->GetValue();

    if (login.empty() || password.empty()) {
        ShowError("Логин и пароль не могут быть пустыми!");
        return;
    }

    std::thread([this, login, password]() {
        wxHTTP http;
        http.SetHeader("Content-Type", "application/json");

        if (!http.Connect("localhost", 8080)) {
            auto evt = new wxThreadEvent();
            evt->SetString("Не удалось подключиться к серверу");
            evt->SetInt(-1);
            wxQueueEvent(this, evt);
            return;
        }

        http.SetMethod("POST");

        wxString postData = wxString::Format(
            "{\"login\":\"%s\", \"password\":\"%s\"}", login, password
        );
        http.SetPostText(postData, "application/json");

        wxInputStream *inputStream = http.GetInputStream("/login");
        int status = http.GetResponse();

        wxString response;
        if (inputStream && inputStream->IsOk()) {
            wxStringOutputStream output;
            inputStream->Read(output);
            response = output.GetString();
        }

        auto evt = new wxThreadEvent();
        evt->SetInt(status);
        evt->SetString(response);
        wxQueueEvent(this, evt);

        http.Close();
    }).detach();
}

void EnterWindow::OnThreadEvent(wxThreadEvent &event) {
    int status = event.GetInt();
    wxString response = event.GetString();

    switch (status) {
        case 200:
            wxMessageBox("Успешный вход!", "Успех", wxOK | wxICON_INFORMATION);
            break;
        case 400:
            ShowError("Некорректные данные. Проверьте ввод.");
            break;
        case 401:
            ShowError("Неверный пароль.");
            break;
        case 404:
            ShowError("Пользователь не найден.");
            break;
        case -1:
        case -2:
            ShowError(response);
            break;
        default:
            ShowError(
                wxString::Format("Ошибка сервера: %d\n%s", status, response)
            );
    }
}

void EnterWindow::ShowError(const wxString &message) {
    wxMessageBox(message, "Ошибка", wxOK | wxICON_ERROR);
}
