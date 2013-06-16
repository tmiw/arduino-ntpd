/*
 * File: HTTPServer.cpp
 * Description:
 *   HTTP server implementation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#if defined(ARDUINO)

#include <Ethernet.h>
#include "HTTPServer.h"

void HttpServer::beginListening()
{
    httpServerPort_.begin();
}

void HttpServer::processOneRequest()
{
    // Retrieve a connection if we're not already working on one.
    currentClient_ = httpServerPort_.available();
    
    // For the current connection, we'll want to read the first line,
    // pull out the request method and path and ignore everything else
    // up to the blank line.
    if (currentClient_)
    {
        while (currentClient_.connected())
        {
            if (currentClient_.available())
            {
                char c = currentClient_.read();
            
                if (c == '\n' && seenBlankLine_)
                {
                    // Finished reading the HTTP request.
                    routeRequest_();
                    resetServerState_();
                }
                else
                {
                    char tmp[2];
                    tmp[1] = 0;
                    tmp[0] = c;
                
                    if (c != '\r')
                    {
                        seenBlankLine_ = false;
                    }
                    
                    if (c == ' ')
                    {
                        // Split string based on delimiter.
                        currentSplit_++;
                    }
                    else if (c == '\n')
                    {
                        seenBlankLine_ = true;
                    }
                    else if (currentSplit_ == 0)
                    {
                        // Request method
                        httpMethod_ += String(tmp);
                    }
                    else if (currentSplit_ == 1)
                    {
                        // Request path.
                        requestPath_ += String(tmp);
                    }
                    else
                    {
                        // Ignore all other characters of request.
                    }
                }
            }
        }
    }
}

void HttpServer::responseRedirect(char *url)
{
    currentClient_.println("HTTP/1.0 302 Found");
    currentClient_.print("Location: ");
    currentClient_.println(url);
    currentClient_.println("Connection: close");
    currentClient_.println("Content-Type: text/html");
    currentClient_.println();
    currentClient_.print("See <a href=\"");
    currentClient_.print(url);
    currentClient_.print("\">");
    currentClient_.print(url);
    currentClient_.println("</a> for new location.");
}

void HttpServer::routeRequest_()
{
    // We only support GET requests.
    if (httpMethod_ == "GET")
    {
        bool found = false;
        for (int index = 0; index < numUrls_; index++)
        {
            if (requestPath_ == urlHandlers_[index].path)
            {
                // Let valid handler handle this request.
                found = true;
                (*urlHandlers_[index].function)(this);
            }
        }
        
        if (!found)
        {
            // HTTP 404 Not Found
            sendHttpResponseHeaders_(404, "Not Found");
            currentClient_.println("Not found.");
        }
    }
    else
    {
        // HTTP 405 Method Not Allowed
        sendHttpResponseHeaders_(405, "Method Not Allowed");
        currentClient_.println("Not allowed.");
    }
}

void HttpServer::sendHttpResponseHeaders_(int code, char *description)
{
    currentClient_.print("HTTP/1.0 ");
    currentClient_.print(code, DEC);
    currentClient_.print(" ");
    currentClient_.println(description);
    currentClient_.println("Connection: close");
    currentClient_.println("Content-Type: text/html");
    currentClient_.println();
}

#endif // defined(ARDUINO)
