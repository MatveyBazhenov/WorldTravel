#include "SocketMain.hpp"

SocketMain::SocketMain() {
    IPaddress addr;
    addr.Service(13567);

    wxPuts(
        "Creating server at " + addr.IPAddress() + wxT(" ") +
        wxString::FromUTF8(std::to_string(addr.Service()))
    );
    server = new wxSocketServer(addr);

    if (!server->IsOk()) {
        wxPuts("error port");
        return;
    }
    IPaddress addReal;
    if (!server->GetLocal(addReal)) {
        wxPuts("Error");
    } else {
        wxPuts(
            "Server listening at " + addReal.IPAddress() + wxT(" ") +
            wxString::FromUTF8(std::to_string(addr.Service()))
        );
    }
}

SocketMain::~SocketMain() {
    delete server;
}

void SocketMain::ServerConnect(wxSocketEvent &event) {
    wxString text = "Server: ";
    wxSocketBase *sockB;
    switch (event.GetSocketEvent()) {
        case wxSOCKET_CONNECTION:
            text.Append(" Connection\n");
            break;

        default:
            text.Append(" Unexpected event\n");
            break;
    }
    wxPuts(text + "\n");
    sockB = server->Accept(false);
    IPaddress addr;
    if (sockB) {
        if (!sockB->GetPeer(addr)) {
            wxPuts("New connection from unknown client accepted.");
        } else {
            wxPuts(
                "new client" + addr.IPAddress() + wxT(" ") +
                wxString::FromUTF8(std::to_string(addr.Service())) +
                wxT(" accept")
            );
        }
        sockB->SetEventHandler(*this, SOCKET_ID);
        sockB->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
        sockB->Notify(true);
        clients++;

    } else {
        wxPuts("error connection.");
        return;
    }
}

void SocketMain::ServerSockedData(wxSocketEvent &event) {
    wxSocketBase *sockB = event.GetSocket();
    switch (event.GetSocketEvent()) {
        case wxSOCKET_INPUT: {
            sockB->SetNotify(wxSOCKET_LOST_FLAG);
            unsigned char c;
            sockB->Read(&c, 1);
            wxPuts(wxString::FromUTF8(std::to_string(c)));
            switch (c) {
                case 0xCE:
                    readMsg(sockB);
                    break;

                default:
                    wxPuts("Unknown test id received from client");
            }
            sockB->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
            break;
        }
        case wxSOCKET_LOST: {
            clients--;
            wxPuts("Deleting socket.");
            sockB->Destroy();
            break;
        }
        default:;
    }
}

void SocketMain::readMsg(wxSocketBase *sock) {
    char buf[4096];
    wxUint32 len = sock->ReadMsg(buf, sizeof(buf)).LastCount();
    if (!len){
        wxLogError("Failed to read message.");
        return;
    }
    wxPuts(wxString::FromUTF8(buf, len));

    sock->WriteMsg(buf, len);
}