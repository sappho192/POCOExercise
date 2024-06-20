/* WebSocket echo server */

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <Poco/Net/WebSocket.h>
using Poco::Net::WebSocket;
#include <Poco/Net/NetException.h>

const Poco::UInt16 PORT = 32980;

class WebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response)
    {
        try
        {
            WebSocket ws(request, response);

            cout << "WebSocket opened" << endl;

            int flags = 0;
            int n = 0;

            do
            {
                char buffer[1024] = {
                    0,
                };

                n = ws.receiveFrame(buffer, sizeof(buffer), flags);

                if (n > 0)
                {
                    char log[1024] = {
                        0,
                    };
                    sprintf(log, "Frame receieved (length=%d, flags=0x%x).", n, unsigned(flags));
                    cout << log << endl;

                    ws.sendFrame(buffer, n, flags);
                }
            } while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);

            cout << "WebSocket closed" << endl;
        }
        catch (Poco::Net::WebSocketException &ex)
        {
            cout << ex.displayText() << endl;

            switch (ex.code())
            {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.setContentLength(0);
                response.send();
            }
        }
    }
};

class DummyRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response)
    {
        cout << "DummyRequestHandler: " << request.getURI() << endl;

        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        const char *data = "OK.\n";
        response.sendBuffer(data, strlen(data));
    }
};

class SimpleRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request)
    {
        cout << "SimpleRequestHandlerFactory: " << request.getURI() << endl;

        if (request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
        {
            return new WebSocketRequestHandler();
        }
        return new DummyRequestHandler();
    }
};

int main()
{
    Poco::Net::ServerSocket serverSocket(PORT);

    Poco::Net::HTTPServerParams *pParams = new Poco::Net::HTTPServerParams;
    Poco::Timespan timeout(300, 0); // 6mins
    pParams->setTimeout(timeout);

    // Create the HTTPServer
    Poco::Net::HTTPServer server(new SimpleRequestHandlerFactory(), serverSocket, pParams);
    server.start();

    cout << "Server listening on port " << PORT << endl;

    getchar();

    server.stop();

    return 0;
}