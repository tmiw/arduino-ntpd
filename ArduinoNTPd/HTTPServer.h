/*
 * File: HTTPServer.h
 * Description:
 *   HTTP server implementation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <Arduino.h>
#include "config.h"

class HttpServer;
typedef void (*UrlHandlerFn)(HttpServer *server);

struct UrlHandler
{
    UrlHandler(char *p, UrlHandlerFn fn)
        : path(p), function(fn) { }
        
    char *path;
    UrlHandlerFn function;
};

class HttpServer
{
public:
    HttpServer(UrlHandler *handlers, int count)
        : urlHandlers_(handlers),
          numUrls_(count),
          seenBlankLine_(false),
          currentSplit_(0),
          httpServerPort_(HTTP_SERVER_PORT)
    {
        // empty
    }
    
    /*
     * Begins listening for HTTP requests.
     */
    void beginListening();
    
    /*
     * Processes a single HTTP request.
     */
    void processOneRequest();
    
    /*
     * Send OK response to client.
     */
    void responseOK()
    {
        sendHttpResponseHeaders_(200, "OK");
    }

    /*
     * Send error response to client.
     */
    void responseError()
    {
        sendHttpResponseHeaders_(500, "Internal Server Error");
    }

    /*
     * Prints output to client.
     */
    template<typename T>
    void print(T val) { currentClient_.print(val); }

    /*
     * Prints output to client.
     */
    template<typename T>
    void println(T val) { currentClient_.println(val); }

private:
    EthernetServer httpServerPort_;
    EthernetClient currentClient_;
    UrlHandler *urlHandlers_;
    int numUrls_;
    bool seenBlankLine_;
    int currentSplit_;
    
    String httpMethod_;
    String requestPath_;
    
    /*
     * Routes HTTP request to correct destination.
     */
    void routeRequest_();
    
    /*
     * Resets server state.
     */
    void resetServerState_()
    {
        if (currentClient_ && currentClient_.connected())
        {
            delay(1);
            currentClient_.stop();
        }
        
        seenBlankLine_ = false;
        currentSplit_ = 0;
        httpMethod_ = "";
        requestPath_ = "";
    }
    
    /*
     * Sends HTTP response headers to client.
     */
    void sendHttpResponseHeaders_(int code, char *description);
};

#endif // HTTP_SERVER_H
