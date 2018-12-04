#include "rustapp.h"

namespace rust {
void gDownloadCallback(const uint8_t *data, std::size_t len, void *userdata) {
    DownloadCallbacks *callbacks = reinterpret_cast<DownloadCallbacks*>(userdata);

    if (callbacks->download) {
        callbacks->download(data, len);
    }
}

void gProgressCallback(const float progress, void *userdata) {
    ProgressCallback &callback = *reinterpret_cast<ProgressCallback*>(userdata);

    if (callback) {
        callback(progress);
    }
}

void gListDatalinks(const char **links, std::size_t len, void *userdata)
{
    std::vector<std::string> *vec = reinterpret_cast<std::vector<std::string>*>(userdata);
    vec->reserve(len);
    for (int i = 0; i < len; ++i) {
        vec->push_back(std::string(links[i]));
    }
}

void gListPlatforms(const char **names, const char **ids, std::size_t len, void *userdata)
{
    std::vector<std::pair<std::string, std::string>> *vec = reinterpret_cast<std::vector<std::pair<std::string, std::string>>*>(userdata);
    vec->reserve(len);
    for (int i = 0; i < len; ++i) {
        vec->push_back(std::make_pair(std::string(names[i]), std::string(ids[i])));
    }
}

}
