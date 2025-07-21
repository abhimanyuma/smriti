#include <string>
#include <algorithm>

#include "utils_string.h"

namespace Smriti {

void to_uppercase(std::string& str) {
    std::transform(
        str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::toupper(c); }
    );
}

} // namespace Smriti
