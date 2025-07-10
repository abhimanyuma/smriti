#ifndef SMRITI_UTILS_CONVERTORS_H
#define SMRITI_UTILS_CONVERTORS_H

#include <cstdint>
#include <cstddef>
#include <optional>

namespace Smriti
{

std::optional<int64_t> parse_int(char*& input, size_t length);

} // namespace Smriti

#endif // SMRITI_UTILS_CONVERTORS_H