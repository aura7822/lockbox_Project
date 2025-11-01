#pragma once
#include <string>

class Obfuscator {
public:
    Obfuscator() = default;

    // Obfuscate text. Returns the obfuscated text and stores the mapping internally.
    std::string obfuscateText(const std::string &plain);

    // Deobfuscate text using the stored mapping. Returns restored text.
    // If you previously imported a mapping with setMappingJson(), this will use that mapping.
    std::string deobfuscateText(const std::string &obf);

    // Export mapping JSON (so you can encrypt/store it as metadata).
    std::string getMappingJson() const;

    // Import mapping JSON (after decrypting metadata) so you can deobfuscate.
    void setMappingJson(const std::string &mapping_json);

    // Clear stored mapping.
    void clearMapping();

private:
    std::string mapping_json_;
};
