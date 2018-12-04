#include <memory>

#include "rust_bindings.h"

namespace rust {
extern "C" {
	const char *get_last_error_string();
}

std::string lastErrorString() {
    return rust::wrap_string(rust::get_last_error_string());
}

}
