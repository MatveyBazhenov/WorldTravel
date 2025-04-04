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

    wxHTTP http;
    http.SetTimeout(10);

    try {
        if (http.Connect("localhost", 8080)) {
            http.SetMethod("POST");
            http.SetHeader("Content-Type", "application/json");

            wxString jsonBody = wxString::Format(
                    R"({"login": "%s", "password": "%s"})", login, password
            );

            if (!http.SetPostText("application/json", jsonBody)) {
                throw std::runtime_error("Failed to set POST data");
            }

            wxInputStream* stream = http.GetInputStream("/login");
            int responseCode = http.GetResponse();

            wxString response;
            if (stream && stream->IsOk()) {
                wxStringOutputStream output(&response);
                stream->Read(output);
                delete stream;

                wxLogDebug("Response code: %d", responseCode);

                if (responseCode == 200) {
                    wxMessageBox("Успешный вход!", "Успех", wxOK | wxICON_INFORMATION);
                } else {
                    wxMessageBox(
                            wxString::Format("Ошибка %d: %s", responseCode, response),
                            "Ошибка", wxOK | wxICON_ERROR
                    );
                }
            } else {
                throw std::runtime_error("Failed to receive response stream.");
            }
        } else {
            throw std::runtime_error("Не удалось подключиться к серверу");
        }
    } catch (const std::exception &e) {
        wxMessageBox(e.what(), "Ошибка", wxOK | wxICON_ERROR);
    } catch (...) {
        wxMessageBox("Неизвестная ошибка", "Ошибка", wxOK | wxICON_ERROR);
    }

    http.Close();
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