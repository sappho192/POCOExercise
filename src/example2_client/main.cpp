/* WebSocket echo client */

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPClientSession.h>

#include <Poco/Net/WebSocket.h>
using Poco::Net::WebSocket;
#include <Poco/Net/NetException.h>

const Poco::UInt16 PORT = 32980;

int main()
{
    int flags;

    try
    {
        Poco::Net::HTTPClientSession session("localhost", PORT);
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/ws", Poco::Net::HTTPRequest::HTTP_1_1);
        Poco::Net::HTTPResponse response;

        string cmd;

        WebSocket *ws = new WebSocket(session, request, response);

        cout << "WebSocket opened" << endl;

        string payload = "Client: Hello server!";

        ws->sendFrame(payload.c_str(), payload.size(), WebSocket::FRAME_TEXT);

        char buffer[1024] = {
            0,
        };
        int recvDataSice = ws->receiveFrame(buffer, sizeof(buffer), flags);
        cout << "[recv]: " << buffer << endl;

        while (true)
        {
            cmd = "";
            cin >> cmd;

            if (cmd == "exit")
            {
                break;
            }

            ws->sendFrame(cmd.data(), cmd.size(), WebSocket::FRAME_TEXT);

            char buffer[1024] = {
                0,
            };
            int recvDataSize = ws->receiveFrame(buffer, sizeof(buffer), flags);
            if (recvDataSize > 0)
            {
                cout << "[recv]: " << buffer << endl;
            }
        }

        ws->shutdown();
    }
    catch (Poco::Exception &ex)
    {
        cout << ex.displayText() << endl;
        return 0;
    }

    return 0;
}
