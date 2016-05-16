#include "rb_rdma.h"
#include "context.h"

VALUE cPD;

struct rb_rdma_data_pd {
  VALUE context;
  struct ibv_pd *pd;
};

static size_t
memsize_rdma_pd(const void *p){
  return sizeof(struct ibv_pd *);
};

static void
free_rdma_pd(void *ptr){
  struct rb_rdma_data_pd *data_pd = ptr;
  printf("%p\n",data_pd->pd);

  ibv_dealloc_pd(data_pd->pd);
  xfree(data_pd);
};

static void
mark_rdma_pd(void *ptr){
  struct rb_rdma_data_pd *data_pd = ptr;

  rb_gc_mark(data_pd->context);

}


const rb_data_type_t rdma_pd_type = {
  "rdma_pd",
  {
    mark_rdma_pd, 
    free_rdma_pd,
    memsize_rdma_pd
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
pd_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_pd *data_pd = ALLOC(struct rb_rdma_data_pd);
  self = TypedData_Wrap_Struct(klass,&rdma_pd_type,data_pd);
  return self;
}


static VALUE
rdma_pd_initialize(VALUE self, VALUE rb_ctx){

  struct rdma_context *ctx;
  struct rb_rdma_data_pd *data_pd;
  
  TypedData_Get_Struct(rb_ctx,struct rdma_context,&rdma_context_type,ctx);

  TypedData_Get_Struct(self,struct rb_rdma_data_pd,&rdma_pd_type,data_pd);

  data_pd->context = rb_ctx;
  data_pd->pd = ibv_alloc_pd(ctx->context);
  if(!data_pd->pd){
    rb_exc_raise(rb_syserr_new(errno, "pd alloc fail"));
    // TODO ERROR
  }

  return self;
}

void Init_pd(){

  cPD = rb_define_class_under(mRbRDMA, "PD", rb_cData);
  rb_define_alloc_func(cPD, pd_s_alloc);
  rb_define_method(cPD,"initialize", rdma_pd_initialize,1);

}
