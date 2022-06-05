#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>

std::string redirect_loop(char const * origin_url) {
    std::string current_url = origin_url;
    CURL* curl = curl_easy_init();
    while (current_url.find("news.google.com") != std::string::npos) {
        curl_easy_setopt(curl, CURLOPT_URL, current_url.data());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.73.0");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        CURLcode res = curl_easy_perform(curl);
        char* location;
        long response_code;
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            break;
        }
        else {
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            if ((res == CURLE_OK) &&
                ((response_code / 100) != 3)) {
                break;
            }

            res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);

            if ((res == CURLE_OK) && location) {
                current_url = location;
            }
        }
    }
    curl_easy_cleanup(curl);
    return current_url;
}

int main() {

    curl_global_init(CURL_GLOBAL_DEFAULT);

    for (; std::cin.good(); ) {
        std::string url;
        std::cin >> url;

        std::string r = redirect_loop(url.data());
        printf("%s\n", r.data());
    }

    curl_global_cleanup();

    return 0;
}

