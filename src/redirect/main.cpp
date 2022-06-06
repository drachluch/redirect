#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>

int main() {

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    for (; std::cin.good(); ) {
        std::string url;
        std::cin >> url;

        while (url.find("news.google.com") != std::string::npos) {
            curl_easy_setopt(curl, CURLOPT_URL, url.data());
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.73.0");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            CURLcode res = curl_easy_perform(curl);
            char* location;
            long response_code;
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                break;
            }
            else {
                res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

                if ((res == CURLE_OK) && ((response_code / 100) != 3)) {
                    break;
                }

                res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);

                if ((res == CURLE_OK) && location) {
                    url = location;
                }
            }
        }
        printf("%s\n", url.data());
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}

