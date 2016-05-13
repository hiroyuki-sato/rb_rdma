#include "rb_rdma.h"
#include "context.h"

static VALUE cCompChannel;

static size_t
memsize_rdma_comp_channel(const void *p){
  return sizeof(struct ibv_comp_channel);
};

static void
free_rdma_comp_channel(void *ptr){
  struct ibv_comp_channel *c_channel = ptr;

  xfree(c_channel);
};

static const rb_data_type_t rdma_comp_channel_type = {
  "rdma_comp_channel",
  {
    0, 
    free_rdma_comp_channel,
    memsize_rdma_comp_channel
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
comp_channel_s_alloc(VALUE klass){
  VALUE obj;
  struct ibv_comp_channel *c_channel = ALLOC(struct ibv_comp_channel);
  obj = TypedData_Wrap_Struct(klass,&rdma_comp_channel_type,c_channel);
  return obj;
}


static VALUE
rdma_comp_channel_initialize(VALUE obj, VALUE obj_ctx){

  struct rdma_context *ctx;
  struct ibv_comp_channel *c_channel;
  
  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);

  TypedData_Get_Struct(obj,struct ibv_comp_channel,&rdma_comp_channel_type,c_channel);

  c_channel = ibv_create_comp_channel(ctx->context);
  if(!c_channel){
    // TODO ERROR
  }

  return obj;
}

void Init_comp_channel(){

  cCompChannel = rb_define_class_under(mRbRDMA, "CompChannel", rb_cData);
  rb_define_alloc_func(cCompChannel, comp_channel_s_alloc);
  rb_define_method(cCompChannel,"initialize", rdma_comp_channel_initialize,1);

}
