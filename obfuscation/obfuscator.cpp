#include "obfuscator.h"
#include <nlohmann/json.hpp>
#include<cctype>
#include<vector>
#include<iostream>
using namespace std;
using json = nlohmann::json;
namespace Obfuscator{
static inline bool is_word_char(char c){
   return isalnum(static_cast<unsigned char>(c))||c=='\'' ||c=='-';
}
  bool obfuscate_text(const string &plain, string &out_obfuscated, string &mapping_json){
  out_obfuscated.clear();
  vector<string>mapping;
  string cur;
  size_t idx = 0;
  for(size_t i=0; i<plain.size();++i){
      char c = plain[i];
      if(is_word_char(c)){cur.push_back(c);}
      else{if (!cur.empty()){
      ++idx;
      ostringstream token;
      token<<"__OBF__"<<idx<<"__";
      out_obfuscated += token.str();
      mapping.push_back(cur);
      cur.clear();
      }out_obfuscated.push_back(c);}
  }
  if(!cur.empty()){
  ++idx;
  ostringstream token;
  token<<"__OBF__"<<idx<<"__";
  out_obfuscated += token.str();
  mapping.push_back(cur);
  cur.clear();
  }
  json j = json::array();
  for(auto &w : mapping)j.push_back(w);
  mapping_json = j.dump();
  return true;
  }
  bool deobfuscate_text(const string &obfuscated, const string &mapping_json, string &out_plain){
  out_plain.clear();
  json j;
  try{
  j = json::parse(mapping_json);
  if(!j.is_array())return false;
  }catch(...){return false;}
  vector<string>mapping;
  mapping.reserve(j.size());
  for(auto &el :j){
  if(el.is_string())mapping.push_back(el.get<string>());
  else mapping.push_back("");
  }
  size_t i=0;
  const string token_prefix="__OBF__";
  while(i<obfuscated.size()){
  if(i+token_prefix.size()<=obfuscated.size()
&&obfuscated.compare(i, token_prefix.size(),token_prefix) ==0){
  size_t start_idx_pos =i + token_prefix.size();
  size_t end_marker = obfuscated.find("__",start_idx_pos);
  if(end_marker == string::npos){out_plain.append(obfuscated.substr(i)); break;}
  string idx_str=obfuscated.substr(start_idx_pos,end_marker - start_idx_pos);
  size_t idx = 0;
  try{idx = stoul(idx_str);}catch(...){
  out_plain.append(obfuscated.substr(i,(end_marker +2 )- i));
  i= end_marker + 2;
  continue;
  }if(idx == 0 || idx>mapping.size()){
  out_plain.append(obfuscated.substr(i, (end_marker + 2)- i));
  i= end_marker + 2;
  continue;
  }
  out_plain += mapping[idx - 1];
  i=end_marker +2;
  }else{out_plain.push_back(obfuscated[i]);
  ++i;
  }}
  return true;
  }
}
