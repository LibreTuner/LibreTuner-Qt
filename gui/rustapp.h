#ifndef RUSTAPP_H
#define RUSTAPP_H

#include <memory>
#include <iostream>
#include <stdint.h>
#include <functional>
#include <vector>

#include "rust_bindings.h"
#include "error.h"

namespace rust {

typedef struct AppRef * app_t;
typedef struct DataLinkRef * datalink_t;
typedef struct PlatformLinkRef * platformlink_t;
typedef struct DownloaderRef * downloader_t;

extern "C" {
app_t create_app();
void drop_app(app_t app);

void drop_datalink(datalink_t datalink);
void drop_platformlink(platformlink_t platformlink);
void drop_downloader(downloader_t downloader);

std::size_t roms_len(app_t app);
const char *rom_name(app_t app, std::size_t rom_id);
std::size_t tunes_len(app_t app, std::size_t rom_id);
unsigned get_tune_id(app_t app, std::size_t rom_id, std::size_t tune_off);
const char *tune_name(app_t app, std::size_t tune_id);

datalink_t app_get_datalink(app_t app, std::size_t id);
platformlink_t app_create_platform_link(app_t app, std::size_t datalink, const char *platform);
void app_list_datalinks(app_t app, void(*)(const char**, std::size_t, void*), void *userdata);
void app_list_platforms(app_t app, void(*)(const char**, const char**, std::size_t, void*), void *userdata);
bool app_download(app_t app, platformlink_t platform, const char* id, const char* name, void(*)(float, void*), void* userdata);

downloader_t platformlink_downloader(platformlink_t platformlink);
bool downloader_download(downloader_t downloader, void(*)(const uint8_t*, std::size_t, void*), void(*)(float, void*), void *userdata);
}


void gDownloadCallback(const uint8_t *data, std::size_t len, void *userdata);
void gProgressCallback(const float progress, void *userdata);
void gListDatalinks(const char **links, std::size_t len, void *userdata);
void gListPlatforms(const char **names, const char **ids, std::size_t len, void *userdata);


using ProgressCallback = std::function<void(float progress)>;
using DownloadCallback = std::function<void(const uint8_t *data, std::size_t length)>;

struct DownloadCallbacks {
    DownloadCallback &download;
    ProgressCallback &progress;
};

struct Downloader {
public:
    Downloader(downloader_t ref) : downloader_{ref, drop_downloader} {
        if (!ref) {
            throw std::runtime_error("Downloading unsupported for platform link");
        }
    }

    inline void download(DownloadCallback dlCallback, ProgressCallback progressCallback = ProgressCallback()) {
        DownloadCallbacks callbacks{dlCallback, progressCallback};

        if (!downloader_download(downloader_.get(), gDownloadCallback, gProgressCallback, &callbacks)) {
            throw std::runtime_error(lastErrorString());
        }
    }

private:
    std::unique_ptr<DownloaderRef, void(*)(downloader_t)> downloader_;
};


struct PlatformLink {
public:
    PlatformLink(platformlink_t ref) : platformlink_{ref, drop_platformlink} {
        if (!ref) {
            throw std::runtime_error(lastErrorString());
        }
    }

    PlatformLink(const PlatformLink&) = delete;
    PlatformLink(PlatformLink&& link) : platformlink_(std::move(link.platformlink_)) {

    }
    PlatformLink &operator=(const PlatformLink&) = delete;
    PlatformLink &operator=(PlatformLink&&) = delete;

    inline Downloader downloader() const {
        return Downloader(platformlink_downloader(platformlink_.get()));
    }

    inline platformlink_t handle() const { return platformlink_.get(); }

private:
    std::unique_ptr<PlatformLinkRef, void(*)(platformlink_t)> platformlink_;
};


struct DataLink {
public:
    DataLink(datalink_t ref) : datalink_{ref} {
        if (!ref) {
            throw std::runtime_error(lastErrorString());
        }
    }

private:
    datalink_t datalink_;
};



struct App {
public:
    App(const App&) = delete;

    App(App&&) = delete;
    App &operator=(const App&) = delete;
    App() {
        app_t app = create_app();
        if (!app) {
            throw std::runtime_error(lastErrorString());
        }
        app_.reset(app);
    }

    std::size_t romsLen() { return roms_len(app_.get()); }
    std::string romName(std::size_t romId) { return wrap_string(rom_name(app_.get(), romId)); }
    std::size_t tunesLen(std::size_t romId) { return tunes_len(app_.get(), romId); }
    unsigned getTuneId(std::size_t romId, std::size_t tuneOff) { return get_tune_id(app_.get(), romId, tuneOff); }
    std::string tuneName(std::size_t tuneId) { return wrap_string(tune_name(app_.get(), tuneId)); }

    DataLink getDatalink(std::size_t id) { return DataLink(app_get_datalink(app_.get(), id)); }
    inline PlatformLink createPlatformLink(std::size_t datalink_id, const char *platform) {
        assert(app_);
        return PlatformLink(app_create_platform_link(app_.get(), datalink_id, platform));
    }

    void download(const PlatformLink &link, const std::string &id, const std::string &name, ProgressCallback callback) {
        if (!app_download(app_.get(), link.handle(), id.c_str(), name.c_str(), gProgressCallback, &callback)) {
            throw std::runtime_error(lastErrorString());
        }
    }

    inline std::vector<std::string> datalinks() {
        std::vector<std::string> datalinks;

        app_list_datalinks(app_.get(), gListDatalinks, &datalinks);
        return datalinks;
    }

    inline std::vector<std::pair<std::string, std::string>> platforms() {
        std::vector<std::pair<std::string, std::string>> platforms;

        app_list_platforms(app_.get(), gListPlatforms, &platforms);
        return platforms;
    }
private:
    std::unique_ptr<AppRef, void (*)(app_t)> app_{nullptr, drop_app};
};

}

#endif // RUSTAPP_H
