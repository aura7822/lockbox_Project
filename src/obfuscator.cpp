#include "obfuscator.h"
#include <nlohmann/json.hpp>
#include <cctype>
#include <vector>
#include <sstream>
#include <string>

using json = nlohmann::json;

static inline bool is_word_char(char c) {
  return std::isalnum(static_cast<unsigned char>(c)) || c == '\'' || c == '-';
}

std::string Obfuscator::obfuscateText(const std::string &plain) {
  std::string out_obfuscated;
  std::vector<std::string> mapping;
  std::string cur;
  size_t idx = 0;

  for (size_t i = 0; i < plain.size(); ++i) {
    char c = plain[i];
    if (is_word_char(c)) {
      cur.push_back(c);
    } else {
      if (!cur.empty()) {
        ++idx;
        std::ostringstream token;
        token << "__OBF__" << idx << "__";
        out_obfuscated += token.str();
        mapping.push_back(cur);
        cur.clear();
      }
      out_obfuscated.push_back(c);
    }
  }
  if (!cur.empty()) {
    ++idx;
    std::ostringstream token;
    token << "__OBF__" << idx << "__";
    out_obfuscated += token.str();
    mapping.push_back(cur);
    cur.clear();
  }

  // Build and store mapping JSON
  json j = json::array();
  for (auto &w : mapping) j.push_back(w);
  mapping_json_ = j.dump();

  return out_obfuscated;
}

std::string Obfuscator::deobfuscateText(const std::string &obfuscated) {
  std::string out_plain;
  // Parse mapping_json_
  json j;
  try {
    j = json::parse(mapping_json_);
    if (!j.is_array()) return std::string(); // empty on error
  } catch (...) {
    return std::string();
  }

  std::vector<std::string> mapping;
  mapping.reserve(j.size());
  for (auto &el : j) {
    if (el.is_string()) mapping.push_back(el.get<std::string>());
    else mapping.push_back("");
  }

  size_t i = 0;
  const std::string token_prefix = "__OBF__";
  while (i < obfuscated.size()) {
    if (i + token_prefix.size() <= obfuscated.size()
      && obfuscated.compare(i, token_prefix.size(), token_prefix) == 0) {
      size_t start_idx_pos = i + token_prefix.size();
    size_t end_marker = obfuscated.find("__", start_idx_pos);
    if (end_marker == std::string::npos) {
      out_plain.append(obfuscated.substr(i));
      break;
    }
    std::string idx_str = obfuscated.substr(start_idx_pos, end_marker - start_idx_pos);
    size_t idx = 0;
    try {
      idx = std::stoul(idx_str);
    } catch (...) {
      out_plain.append(obfuscated.substr(i, (end_marker + 2) - i));
      i = end_marker + 2;
      continue;
    }
    if (idx == 0 || idx > mapping.size()) {
      out_plain.append(obfuscated.substr(i, (end_marker + 2) - i));
      i = end_marker + 2;
      continue;
    }
    out_plain += mapping[idx - 1];
    i = end_marker + 2;
      } else {
        out_plain.push_back(obfuscated[i]);
        ++i;
      }
  }
  return out_plain;
}

std::string Obfuscator::getMappingJson() const {
  return mapping_json_;
}

void Obfuscator::setMappingJson(const std::string &mapping_json) {
  mapping_json_ = mapping_json;
}

void Obfuscator::clearMapping() {
  mapping_json_.clear();
}
