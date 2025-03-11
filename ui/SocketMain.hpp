#ifndef SOCKETMAIN_HPP
#define SOCKETMAIN_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/log.h>
#include <wx/sstream.h>
#include "wx/socket.h"

#if wxUSE_IPV6
typedef wxIPV4address IPaddress;
#else
typedef wxIPV4address IPaddress;
#endif
class SocketMain : public wxEvtHandler {
public:
    SocketMain();
    ~SocketMain();

    void ServerConnect(wxSocketEvent &event);
    void ServerSockedData(wxSocketEvent &event);
    void readMsg(wxSocketBase *sock);

private:
    wxSocketServer *server;
    int clients;
    wxDECLARE_EVENT_TABLE();
};

enum { SERVER_ID = 100, SOCKET_ID };

#endif