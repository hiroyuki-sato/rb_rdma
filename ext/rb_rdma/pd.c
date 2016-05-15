#include "rb_rdma.h"
#include "context.h"

VALUE cPD;

static size_t
memsize_rdma_pd(const void *p){
  return sizeof(struct ibv_pd *);
};

static void
free_rdma_pd(void *ptr){
  struct ibv_pd *pd = ptr;
  printf("%p\n",pd);

  ibv_dealloc_pd(pd);
};

const rb_data_type_t rdma_pd_type = {
  "rdma_pd",
  {
    0, 
    free_rdma_pd,
    memsize_rdma_pd
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
pd_s_alloc(VALUE klass){
  VALUE obj;
  obj = TypedData_Wrap_Struct(klass,&rdma_pd_type,0);
  return obj;
}


static VALUE
rdma_pd_initialize(VALUE obj, VALUE obj_ctx){

  struct rdma_context *ctx;
  struct ibv_pd *pd;
  
  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);

  TypedData_Get_Struct(obj,struct ibv_pd,&rdma_pd_type,pd);

  pd = ibv_alloc_pd(ctx->context);
  if(!pd){
    rb_exc_raise(rb_syserr_new(errno, "pd alloc fail"));
    // TODO ERROR
  }

  DATA_PTR(obj) = pd;

  printf("pd: %p\n",pd);

  return obj;
}

void Init_pd(){

  cPD = rb_define_class_under(mRbRDMA, "PD", rb_cData);
  rb_define_alloc_func(cPD, pd_s_alloc);
  rb_define_method(cPD,"initialize", rdma_pd_initialize,1);

}
