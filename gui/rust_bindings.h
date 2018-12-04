#ifndef RUST_BINDINGS_H
#define RUST_BINDINGS_H

#include <string>

#include <QString>

namespace rust {

extern "C" {
void drop_string(const char* raw);
}

inline std::string wrap_string(const char *raw) {
    if (!raw) {
        return std::string();
    }

    std::string res(raw);
    drop_string(raw);
    return raw;
}

inline QString wrap_QString(const char *raw) {
    if (!raw) {
        return QString();
    }

    QString res = QString::fromUtf8(raw);
    drop_string(raw);
    return res;
}

}

#endif
