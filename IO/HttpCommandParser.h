#ifndef HttpCommandParser_h
#define HttpCommandParser_h
#include <IO/WebResponse.h>

#ifndef MAX_PARAM_SIZE
#define MAX_PARAM_SIZE 10
#endif

#ifndef MAX_REQUEST_INPUT
#define MAX_REQUEST_INPUT 80
#endif

class HttpCommandParser
{
  private:
    // Example GET line: GET /?foo=bar HTTP/1.1
    static const char *resolveRoute(const char *data)
    {
        // find where the parameters start
        const char *paramsPos = strchr(data, '/');
        if (paramsPos == NULL)
            return NULL; // no parameters
        // find the trailing space
        const char *spacePos = strchr(paramsPos, ' ');
        if (spacePos == NULL)
            return NULL; // no space found

        // work out how long the parameters are and see if too long
        int paramLength = spacePos - paramsPos - 1;
        if (paramLength > MAX_PARAM_SIZE)
            return NULL;
        // copy parameters into a buffer
        static char param[MAX_PARAM_SIZE];
        memcpy(param, paramsPos + 1, paramLength); // skip the "/"
        param[paramLength] = 0;                    // null terminator

        return &param[0];
    } // end of processIncommingGet

    // here to process incoming serial data after a terminator received
    static HttpVerb resolveVerb(const char *data)
    {
        HttpVerb result = HttpVerb::Unknown;
        if (strlen(data) < 4)
            return result;

        if (memcmp(data, "GET ", 4) == 0)
            result = HttpVerb::GET;
        else if (memcmp(data, "PUT ", 4) == 0)
            result = HttpVerb::PUT;
        else if (memcmp(data, "POST ", 5) == 0)
            result = HttpVerb::POST;
        else
            result = HttpVerb::Unknown;
        return result;
    } // end of processData

    static void processIncomingByte(const byte inByte, HttpCommand *result)
    {
        static char input_line[MAX_REQUEST_INPUT];
        static unsigned int input_pos = 0;
        static int lineNumber = 0;
        switch (inByte)
        {
            // an http request ends with a blank line
        case '\n':                     // end of text
            input_line[input_pos] = 0; // terminating null byte
            if (input_pos == 0)
            {
                lineNumber = 0;
                result->completed = true;
                return;
            }
            // terminator reached! process input_line here ...
            if (++lineNumber == 1) //First line contains verb, request url and type.
            {
                result->verb = resolveVerb(input_line);
                result->route = resolveRoute(result->verb == HttpVerb::POST ? &input_line[5] : &input_line[4]);
            }
            // reset buffer for next time
            input_pos = 0;
            break;
        case '\r': // discard carriage return
            break;

        default:
            // keep adding if not full ... allow for terminating null byte
            if (input_pos < (MAX_REQUEST_INPUT - 1))
                input_line[input_pos++] = inByte;
            break;
        } // end of switch

        result->completed = false;
    }

    static void processIncomingRequest(Client &client, HttpCommand *command)
    {
        while (client.connected() && !command->completed)
        {
            while (client.available() > 0 && !command->completed)
                processIncomingByte(client.read(), command);
        } // end of while client connected
    }

  public:
    HttpCommandParser(/* args */) {}
    ~HttpCommandParser() {}

    HttpCommand parse(Client &client)
    {
        HttpCommand command;
        processIncomingRequest(client, &command);
        return command;
    }
};

#endif