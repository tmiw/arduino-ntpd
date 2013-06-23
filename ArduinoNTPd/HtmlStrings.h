/*
 * File: HtmlStrings.h
 * Description:
 *   Static HTML page content.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef HTML_STRINGS_H
#define HTML_STRINGS_H

#define CRLF "\r\n"

#define COMMON_PAGE_STYLE \
    "body { font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 10pt; }" CRLF \
    "a { text-decoration: none; }" CRLF \
    "a:hover { text-decoration: underline; }" CRLF \
    ".nav { width: 100%; border-bottom: 1px solid black; margin-bottom: 1em; }" CRLF \
    ".bottom { width: 100%; border-top: 1px solid black; margin-top: 1em; }" CRLF
    
#define COMMON_PAGE_HEADER \
    "<html>" \
    "<head>" \
    "<meta http-equiv=\"Refresh\" content=\"5\" />" \
    "<title>arduino-ntpd</title>" \
    "<style type=\"text/css\">" \
    "<!--" \
    COMMON_PAGE_STYLE \
    "// -->" \
    "</style>" \
    "</head>" \
    "<body>" \
    "<div class=\"nav\"><a href=\"/time\">Current time</a> | <a href=\"/location\">Current location</a></div>" \

#define TIME_PAGE_HEADER \
    "<h1>Current time</h1>"

#define POSITION_PAGE_HEADER \
    "<h1>Current position</h1>"
    
#define TIME_PAGE_FOOTER \
    "<br/>(All times in UTC.)"
    
#define POSITION_PAGE_FOOTER \
    "<br/>"
    
#define COMMON_PAGE_FOOTER \
    "<div class=\"bottom\">Copyright &copy; 2013 Mooneer Salem. All rights reserved. <a href=\"http://github.com/tmiw/arduino-ntpd\">Source code here</a>.</div>" \
    "</body>" \
    "</html>"
    

#endif // HTML_STRINGS_H
