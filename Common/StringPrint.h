#ifndef StringPrint_h
#define StringPrint_h

#include <Arduino.h>

class StringPrint : public Print
{
    public:
        virtual size_t write(uint8_t c)
        {
            str += (char)c;
            return str.length();
        }

        const char * c_str()
        {
            return str.c_str();
        }

    private:
        String str = "";
};

#endif