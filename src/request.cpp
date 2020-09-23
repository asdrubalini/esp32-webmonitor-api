#include <curl/curl.h>
#include <iostream>
#include <chrono>

#include "request.h"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    // Don't write output to stdout
    return size * nmemb;
}

response_status request_get_status(char* url) {
    CURL *curl;
    CURLcode res;
    response_status ret;

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        // Set headers
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, USER_AGENT);
        
        curl_easy_setopt(curl, CURLOPT_HEADER, chunk);
        
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // Perform request
        res = curl_easy_perform(curl);

        std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

        // Compute request's duration time
        ret.duration_ms = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start).count();

        // Check if everything is OK and get the status code
        ret.error = (res != CURLE_OK);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ret.status_code);

        curl_slist_free_all(chunk);
    }

    curl_easy_cleanup(curl);

    return ret;
}
