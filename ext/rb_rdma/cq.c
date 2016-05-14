#include "rb_rdma.h"
#include "context.h"
#include "comp_channel.h"

VALUE cCQ;

static size_t
memsize_rdma_cq(const void *p){
  return sizeof(struct ibv_cq *);
};

static void
free_rdma_cq(void *ptr){
  struct ibv_cq *cq = ptr;

  ibv_destroy_cq(cq);
};

const rb_data_type_t rdma_cq_type = {
  "rdma_cq",
  {
    0, 
    free_rdma_cq,
    memsize_rdma_cq
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
cq_s_alloc(VALUE klass){
  VALUE obj;
  obj = TypedData_Wrap_Struct(klass,&rdma_cq_type,0);
  return obj;
}


static VALUE
rdma_cq_initialize(VALUE self,VALUE obj_ctx,VALUE obj_cqe,VALUE obj_c_channel,
                   VALUE obj_comp_vector){
  struct ibv_cq *cq;
  struct ibv_comp_channel *c_channel;
  struct rdma_context *ctx;
  int cqe;

  cqe = NUM2INT(obj_cqe);

  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);
  TypedData_Get_Struct(obj_c_channel,struct ibv_comp_channel,
                       &rdma_comp_channel_type,c_channel);
  
  TypedData_Get_Struct(self,struct ibv_cq,&rdma_cq_type,cq);

  cq = ibv_create_cq(ctx->context,cqe,NULL,c_channel,0);
  if(!cq){
    // TODO ERROR
  }
  DATA_PTR(self) = cq;

  return self;
}

void Init_cq(){

  cCQ = rb_define_class_under(mRbRDMA, "CQ", rb_cData);
  rb_define_alloc_func(cCQ, cq_s_alloc);
//  rb_define_method(cCQ,"initialize", rdma_cq_initialize,3);
  rb_define_method(cCQ,"initialize", rdma_cq_initialize,5);

}
