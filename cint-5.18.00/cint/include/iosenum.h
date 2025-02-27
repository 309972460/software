/* include/iosenum.h
 *  This file contains platform dependent ios enum value.
 *  Run 'cint include/iosenum.cxx' to create this file. It is done
 *  only once at installation. */
#if !defined(G__GNUC) || (G__GNUC!=4)
#error $CINTSYSDIR/include/iosenum.h compiler version mismatch. Do'cd $CINTSYSDIR/include; cint iosenum.cxx' to restore
#endif
#pragma ifndef G__TMPLTIOS
static int ios::goodbit=0;
static int ios::eofbit=2;
static int ios::failbit=4;
static int ios::badbit=1;
static int ios::in=8;
static int ios::out=16;
static int ios::ate=2;
static int ios::app=1;
static int ios::trunc=32;
static int ios::binary=4;
static int ios::beg=0;
static int ios::cur=1;
static int ios::end=2;
static int ios::boolalpha=1;
static int ios::adjustfield=176;
static int ios::basefield=74;
static int ios::floatfield=260;
static int ios::skipws=4096;
static int ios::left=32;
static int ios::right=128;
static int ios::internal=16;
static int ios::dec=2;
static int ios::oct=64;
static int ios::hex=8;
static int ios::showbase=512;
static int ios::showpoint=1024;
static int ios::uppercase=16384;
static int ios::showpos=2048;
static int ios::scientific=256;
static int ios::fixed=4;
static int ios::unitbuf=8192;
#pragma else
static ios_base::fmtflags ios_base::boolalpha=1;
static ios_base::fmtflags ios_base::dec=2;
static ios_base::fmtflags ios_base::fixed=4;
static ios_base::fmtflags ios_base::hex=8;
static ios_base::fmtflags ios_base::internal=16;
static ios_base::fmtflags ios_base::left=32;
static ios_base::fmtflags ios_base::oct=64;
static ios_base::fmtflags ios_base::right=128;
static ios_base::fmtflags ios_base::scientific=256;
static ios_base::fmtflags ios_base::showbase=512;
static ios_base::fmtflags ios_base::showpoint=1024;
static ios_base::fmtflags ios_base::showpos=2048;
static ios_base::fmtflags ios_base::skipws=4096;
static ios_base::fmtflags ios_base::unitbuf=8192;
static ios_base::fmtflags ios_base::uppercase=16384;
static ios_base::fmtflags ios_base::adjustfield=176;
static ios_base::fmtflags ios_base::basefield=74;
static ios_base::fmtflags ios_base::floatfield=260;
static ios_base::fmtflags ios_base::badbit=1;
static ios_base::fmtflags ios_base::eofbit=2;
static ios_base::fmtflags ios_base::failbit=4;
static ios_base::fmtflags ios_base::goodbit=0;
static ios_base::fmtflags ios_base::app=1;
static ios_base::fmtflags ios_base::ate=2;
static ios_base::fmtflags ios_base::binary=4;
static ios_base::fmtflags ios_base::in=8;
static ios_base::fmtflags ios_base::out=16;
static ios_base::fmtflags ios_base::trunc=32;
static ios_base::fmtflags ios_base::beg=0;
static ios_base::fmtflags ios_base::cur=1;
static ios_base::fmtflags ios_base::end=2;
#pragma endif
