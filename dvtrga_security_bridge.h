#ifndef DVTRGA_SECURITY_BRIDGE_H
#define DVTRGA_SECURITY_BRIDGE_H

#if defined(__has_include)
#  if __has_include("../../PROPRIETARY_SEC/dvtrga_security.h")
#    include "../../PROPRIETARY_SEC/dvtrga_security.h"
#    include "../../PROPRIETARY_SEC/dvtrga_security.c"
#  else
static inline int dvtrga_sec_verify(void) { return 1; }
#  endif
#else
static inline int dvtrga_sec_verify(void) { return 1; }
#endif

#endif // DVTRGA_SECURITY_BRIDGE_H
