#include "rb_rdma.h"
#include "context.h"
#include "pd.h"

VALUE cPD;

static size_t
memsize_rdma_pd(const void *p){
  return sizeof(struct rb_rdma_data_pd *);
};

static void
free_rdma_pd(void *ptr){
  struct rb_rdma_data_pd *data_pd = ptr;

  if( data_pd->pd )
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
  0
};

static VALUE
pd_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_pd *data_pd;

  self = TypedData_Make_Struct(klass,struct rb_rdma_data_pd,
                              &rdma_pd_type,data_pd);
  data_pd->pd = NULL;
  return self;
}


static VALUE
rdma_pd_initialize(VALUE self, VALUE rb_ctx){

  struct rdma_context *ctx;
  struct rb_rdma_data_pd *data_pd;
  
  Check_OBJ_Type(rb_ctx,cContext,"not RDMA::Context object");
  GET_Context_DATA(rb_ctx,ctx);

  GET_PD_DATA(self,data_pd);

  data_pd->context = rb_ctx;
  printf("ibv_ctx in pd %p\n",ctx->context);
  data_pd->pd = ibv_alloc_pd(ctx->context);
  if(!data_pd->pd){
    rb_exc_raise(rb_syserr_new(errno, "pd alloc fail"));
    // TODO ERROR
  }
  printf("alloc_pd %p\n",data_pd->pd);

  return self;
}

void Init_pd(){

  cPD = rb_define_class_under(mRbRDMA, "PD", rb_cData);
  rb_define_alloc_func(cPD, pd_s_alloc);
  rb_define_method(cPD,"initialize", rdma_pd_initialize,1);

}
