#ifndef bro_pcre_h
#define bro_pcre_h
#include <assert.h>
#include <stdio.h>
#include <string>
// TODO: use configure to figure out the location of pcre.h
// GnuWin32
#include "pcre.h"
namespace binpac {
using namespace std;

class RE_Matcher {
 public:
  RE_Matcher(const char *pat) {
    pattern_ = "^";
    pattern_ += "(";
    pattern_ += pat;
    pattern_ += ")";
    pcre_ = NULL;
    pextra_ = NULL;
  }
  ~RE_Matcher() {
    if (pcre_) {
      pcre_free(pcre_);
    }
  }
  int Compile() {
    const char *err = NULL;
    int erroffset = 0;
    pcre_ = pcre_compile(pattern_.c_str(),
                         0,  // options,
                         &err, &erroffset, NULL);
    if (pcre_ == NULL) {
      fprintf(stderr, "Error in RE_Matcher::Compile(): %d:%s\n", erroffset,
              err);
      return 0;
    }
    return 1;
  }

  int MatchPrefix(const unsigned char *s, int n) {
    // const char *err=NULL;
    assert(pcre_);
    const int MAX_NUM_OFFSETS = 30;
    int offsets[MAX_NUM_OFFSETS];
    int ret = pcre_exec(pcre_,
                        pextra_,  // pcre_extra
                        // NULL,  // pcre_extra
                        (const char *)s, n,
                        0,  // offset
                        0,  // options
                        offsets, MAX_NUM_OFFSETS);
    if (ret < 0) {
      return -1;
    }
    assert(offsets[0] == 0);
    return offsets[1];
  }

 protected:
  pcre *pcre_;
  string pattern_;
  pcre_extra *pextra_;
};
}  // namespace binpac
#endif
