#if !defined(_RDMA_H_)
# define _RDMA_H_

#include "ruby.h"
#include <infiniband/verbs.h>
#include <infiniband/mad.h>
#include <infiniband/umad_types.h>
#include <infiniband/umad_sa.h>

extern VALUE mRbRDMA;

extern const rb_data_type_t my_buffer_type;
extern VALUE cMyBuffer;
extern void Init_my_buffer();
struct my_buffer {
  char *buf;
  int  len;
};
  
#define Check_OBJ_Type(klass,klass_type,err_msg) \
  if (!RTEST(rb_obj_is_kind_of(klass, klass_type))) { \
    rb_raise(rb_eTypeError, err_msg); \
  }

#ifdef DEBUG
  #define DEBUG_CMD(x) x;
  #define TRACE_CMD() printf("func:%s %s:%d\n",__func__,__FILE__,__LINE__);
#else
  #define DEBUG_CMD(x) 
  #define TRACE_CMD() 
#endif
#endif
