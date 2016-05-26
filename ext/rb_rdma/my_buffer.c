#include "rb_rdma.h"
#include <stdlib.h>
#include <malloc.h>

VALUE cMyBuffer;


static void
free_my_buffer(void *ptr){
  struct my_buffer *my_buf = ptr;

  if( my_buf->buf )
    xfree(my_buf->buf);
  xfree(my_buf);
};

static size_t
memsize_my_buffer(const void *p){
  return sizeof(struct my_buffer);
};


const rb_data_type_t my_buffer_type = {
  "rdma_context",
  {
    0, 
    free_my_buffer,
    memsize_my_buffer
  },
  0,0,
  0
};

static VALUE
my_buffer_s_alloc(VALUE klass){
  struct my_buffer *my_buf;
  VALUE obj;

  obj = TypedData_Make_Struct(klass,struct my_buffer,&my_buffer_type,my_buf);

  my_buf->buf = memalign(1024,10);
  memset(my_buf->buf,0x7b,10);
  my_buf->len = 10;

  return obj;
};

/*
static VALUE
my_buffer_initialize(VALUE self){
  return self;
}
*/

void Init_my_buffer(){

  cMyBuffer = rb_define_class_under(mRbRDMA, "MyBuffer", rb_cData);
  rb_define_alloc_func(cMyBuffer, my_buffer_s_alloc);
//  rb_define_method(cMyBuffer, "initialize",my_buffer_initialize,0);

}
