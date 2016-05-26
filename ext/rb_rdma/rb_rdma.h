#include "ruby.h"
#include <infiniband/verbs.h>
#include <infiniband/mad.h>
#include <infiniband/umad_types.h>
#include <infiniband/umad_sa.h>

extern VALUE mRbRDMA;

extern VALUE cMyBuffer;
extern void Init_my_buffer();
struct my_buffer {
  char *buf;
  int  len;
};
  
