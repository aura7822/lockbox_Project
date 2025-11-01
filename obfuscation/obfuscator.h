#pragma once
#include <string>
// Obfuscator API
// - obfuscate_text:
//     input: plain text
//     output: obfuscated text and a JSON string containing the mapping (index -> original word)
// - deobfuscate_text:
//     input: obfuscated text and mapping JSON (the same JSON produced by obfuscate_text)
//     output: restored original plain text
//
// NOTE: mapping_json MUST be encrypted at rest (store with the encrypted file's metadata).
//       The functions here do not perform encryption of the mapping.
namespace Obfuscator {
    // Obfuscates "short" plain text. Returns true on success.
    // mapping_json is filled with a JSON array (e.g. ["firstword","secondword",...])
    bool obfuscate_text(const std::string &plain, std::string &out_obfuscated, std::string &mapping_json);

    // Restores text from obfuscated form and mapping JSON. Returns true on success.
    bool deobfuscate_text(const std::string &obfuscated, const std::string &mapping_json, std::string &out_plain);
}
