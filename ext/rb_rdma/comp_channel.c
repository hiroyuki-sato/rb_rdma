#include "rb_rdma.h"
#include "context.h"
#include "comp_channel.h"

VALUE cCompChannel;

static size_t
memsize_rdma_comp_channel(const void *p){
  return sizeof(struct rb_rdma_data_comp_channel);
};

static void
free_rdma_comp_channel(void *ptr){
  struct rb_data_comp_channel *data_c_channel = ptr;

  xfree(data_c_channel);
};

static void
mark_rdma_comp_channel(void *ptr){
  struct rb_rdma_data_comp_channel *data_c_channel = ptr;

  rb_gc_mark(data_c_channel->context);
}

const rb_data_type_t rdma_comp_channel_type = {
  "rdma_comp_channel",
  {
    mark_rdma_comp_channel, 
    free_rdma_comp_channel,
    memsize_rdma_comp_channel
  },
  0,0,
  0  
};

static VALUE
comp_channel_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_comp_channel *data_c_channel;

  self = TypedData_Make_Struct(klass,struct rb_rdma_data_comp_channel,
                               &rdma_comp_channel_type,data_c_channel);
  return self;
}


static VALUE
rdma_comp_channel_initialize(VALUE self, VALUE rb_ctx){

  struct rdma_context *data_ctx;
  struct rb_rdma_data_comp_channel *data_c_channel;
  
  TypedData_Get_Struct(rb_ctx,struct rdma_context,&rdma_context_type,data_ctx);

  TypedData_Get_Struct(self,struct rb_rdma_data_comp_channel,
                       &rdma_comp_channel_type,data_c_channel);

  data_c_channel->context = rb_ctx;
  data_c_channel->comp_channel = ibv_create_comp_channel(data_ctx->context);
  if(!data_c_channel->comp_channel){
     rb_exc_raise(rb_syserr_new(errno, "comp_channel alloc fail"));
  }

  return self;
}

void Init_comp_channel(){

  cCompChannel = rb_define_class_under(mRbRDMA, "CompChannel", rb_cData);
  rb_define_alloc_func(cCompChannel, comp_channel_s_alloc);
  rb_define_method(cCompChannel,"initialize", rdma_comp_channel_initialize,1);

}
