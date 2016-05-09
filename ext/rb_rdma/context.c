#include "rb_rdma.h"

struct rdma_context {
  struct ibv_device **devices;
  struct ibv_context *context;
};

static void
free_rdma_context(void *ptr){
  struct rdma_context *ctx = ptr;

  ibv_close_device(ctx->context);
  ibv_free_device_list(ctx->devices);

};

/*
static void
alloc_rdma_context(void *ptr){
  struct rma_context *ctx = ptr;

};
*/
static size_t
memsize_rdma_context(const void *p){
  return sizeof(struct rdma_context);
};


static const rb_data_type_t rdma_context_type = {
  "rbma_context",
  {
    0, 
    free_rdma_context,
    memsize_rdma_context
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

VALUE rdma_context_open(VALUE args){

}

void Init_context(){
  VALUE klass;
  VALUE mRbRDMA;

  mRbRDMA = rb_define_module("RbRDMA");

  klass = rb_define_class_under(mRbRDMA, "Context", rb_cObject);
  rb_define_singleton_method(klass, "open", rdma_context_open, 1);


}
