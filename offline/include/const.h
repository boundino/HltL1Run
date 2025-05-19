
namespace plot {
  std::map<std::string, std::string> g_leg = {
                                 {"HIJING", "H#scale[0.9]{IJING}"},
                                 {"Starlight", "S#scale[0.9]{TARLIGHT}"},
                                 {"-", " "},
  };
  std::string leg(std::string t) {
    for (auto l : g_leg) {
      t = xjjc::str_replaceall(t, l.first, l.second);
    }
    return t;
  }
}
