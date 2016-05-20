#include "rb_rdma.h"
#include "context.h"
#include "cq.h"
#include "comp_channel.h"

VALUE cCQ;

static size_t
memsize_rdma_cq(const void *p){
  return sizeof(struct rb_rdma_data_cq);
};

static void
free_rdma_cq(void *ptr){
  struct rb_rdma_data_cq *data_cq = ptr;

  if( data_cq->cq )
    ibv_destroy_cq(data_cq->cq);
  xfree(data_cq);
};
static void
mark_rdma_cq(void *ptr){
  struct rb_rdma_data_cq *data_cq = ptr;

  rb_gc_mark(data_cq->context);
  rb_gc_mark(data_cq->comp_channel);
}

const rb_data_type_t rdma_cq_type = {
  "rdma_cq",
  {
    mark_rdma_cq, 
    free_rdma_cq,
    memsize_rdma_cq
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
cq_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_cq *data_cq;
  self = TypedData_Make_Struct(klass,struct rb_rdma_data_cq,&rdma_cq_type,
                               data_cq);
  return self;
}


static VALUE
rdma_cq_initialize(VALUE self,VALUE rb_ctx,VALUE rb_cqe,VALUE rb_cq_context,
                   VALUE rb_c_channel, VALUE rb_comp_vector){
  struct rb_rdma_data_cq *data_cq;
  struct rb_rdma_data_comp_channel *data_c_channel;
  struct rdma_context *data_ctx;
  int cqe;

  cqe = NUM2INT(rb_cqe);

  TypedData_Get_Struct(rb_ctx,struct rdma_context,&rdma_context_type,data_ctx);
  TypedData_Get_Struct(rb_c_channel,struct rb_rdma_data_comp_channel,
                       &rdma_comp_channel_type,data_c_channel);
  TypedData_Get_Struct(self,struct rb_rdma_data_cq,&rdma_cq_type,data_cq);

  data_cq->context = rb_ctx;
  data_cq->comp_channel = rb_c_channel;
//  data_cq->cq = ibv_create_cq(data_ctx->context,cqe,NULL,
//                              data_c_channel->comp_channel,0);
  printf("context in cq %p\n",data_ctx->context);
  data_cq->cq = ibv_create_cq(data_ctx->context,1,NULL,
                              NULL,0);
  if(!data_cq->cq){
     printf("cq alloc error\n");
     rb_exc_raise(rb_syserr_new(errno, "cq alloc fail"));
//     rb_syserr_new(errno,"test");
    // TODO ERROR
  }
  printf("alloc cq %p\n",data_cq->cq);
  return self;
}

void Init_cq(){

  cCQ = rb_define_class_under(mRbRDMA, "CQ", rb_cData);
  rb_define_alloc_func(cCQ, cq_s_alloc);
//  rb_define_method(cCQ,"initialize", rdma_cq_initialize,3);
  rb_define_method(cCQ,"initialize", rdma_cq_initialize,5);

}
