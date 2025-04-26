#include "RegistrationWindow.hpp"

RegistrationWindow::RegistrationWindow(wxWindow *parent)
    : wxFrame(
          parent,
          wxID_ANY,
          "Регистрация",
          wxDefaultPosition,
          wxDefaultSize
      ) {
    _locale.Init(wxLANGUAGE_RUSSIAN);

    wxBoxSizer *mainSizer2 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer *centerSizer2 = new wxGridSizer(3, 1, 10, 10);

    txtLogin = new wxTextCtrl(this, wxID_ANY);
    txtLogin->SetHint("Логин");
    txtLogin->SetMinSize(wxSize(200, 100));

    txtPassword = new wxTextCtrl(this, wxID_ANY);
    txtPassword->SetHint("Пароль");
    txtPassword->SetMinSize(wxSize(200, 100));

    btnReg = new wxButton(this, ID_REG2, "Зарегистрироваться");
    btnReg->SetMinSize(wxSize(200, 100));

    centerSizer2->Add(txtLogin, 0, wxALIGN_CENTER | wxALL, 5);
    centerSizer2->Add(txtPassword, 0, wxALIGN_CENTER | wxALL, 5);
    centerSizer2->Add(btnReg, 0, wxALIGN_CENTER | wxALL, 5);

    mainSizer2->Add(centerSizer2, 1, wxEXPAND | wxALL, 10);
    this->SetSizer(mainSizer2);
    Center();
    this->Fit();
}


void RegistrationWindow::OnRegisterButtonClicked(wxCommandEvent &event) {
    wxString login = txtLogin->GetValue();
    wxString password = txtPassword->GetValue();

    if (login.empty() || password.empty()) {
        wxMessageBox("Заполните все поля!", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    wxString jsonBody = wxString::Format(
            R"({"username": "%s", "password": "%s"})", login, password
    );

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
            wxInputStream* stream = http.GetInputStream("/registration");
            if(stream){
                wxStringOutputStream output(&response);
                stream->Read(output);
                delete stream;
            }

            int responseCode = http.GetResponse();
            wxLogDebug("Response code: %d", responseCode);
            
            switch (responseCode) {
                case 200:{ //Success
                    nlohmann::json response_json = nlohmann::json::parse(response.ToStdString());
                    if(response_json["status"].get<std::string>() == "ok"){
                        wxMessageBox(
                                "Регистрация успешна!\nВаш ключ: " + response_json["user_key"].get<std::string>() , "Успех",
                                wxOK | wxICON_INFORMATION
                        );
                    }
                    break;
                }
                case 400:{ //Bad Request
                    wxMessageBox( 
                            "Ошибка: Неверное имя пользователя или пароль! Поля должны быть непустыми и содержать до 50 символов!",
                            "Ошибка",wxOK | wxICON_ERROR
                    );
                    break;
                }
                case 409:{ //Conflict
                    wxMessageBox(
                            R"(Ошибка: Пользоваетль с именем ")" + login + R"(" уже существует!)" ,
                            "Ошибка",wxOK | wxICON_ERROR
                    );
                    break;
                }
                default:{
                    throw std::runtime_error("Не получилось получить ответ от сервера =(");
                    break;
                }
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



wxBEGIN_EVENT_TABLE(RegistrationWindow, wxFrame)
    EVT_BUTTON(ID_REG2, RegistrationWindow::OnRegisterButtonClicked)
        wxEND_EVENT_TABLE()