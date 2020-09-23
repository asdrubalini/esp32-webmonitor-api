#ifndef REQUEST_H
#define REQUEST_H

#define USER_AGENT "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36"

struct response_status {
    bool error;
    unsigned long status_code;
    unsigned int duration_ms;
};

// Internal stuff
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

// Send HTTP GET request to url and return response_status struct
response_status request_get_status(char* url);

#endif // REQUEST_H
