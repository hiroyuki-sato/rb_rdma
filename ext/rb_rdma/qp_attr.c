#include "rb_rdma.h"

VALUE cQPAttr;

static size_t
memsize_rdma_qp_attr(const void *p){
  return sizeof(struct ibv_qp_attr *);
};

static void
free_rdma_qp_attr(void *ptr){
  struct ibv_qp_attr *attr = ptr;

  if( attr ){
    xfree(attr);
    attr = NULL;
  }
};

const rb_data_type_t rdma_qp_attr_type = {
  "rdma_qp_attr",
  {
    NULL, 
    free_rdma_qp_attr,
    memsize_rdma_qp_attr
  },
  0,0,
  0
};

static VALUE
qp_attr_s_alloc(VALUE klass){
  VALUE self;
  struct ibv_qp_attr *qp_attr;

  self = TypedData_Make_Struct(klass,struct ibv_qp_attr,
                              &rdma_qp_attr_type,qp_attr);
  memset(qp_attr,0,sizeof(struct ibv_qp_attr));
  return self;
}


#if 0
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
#endif

void Init_qp_attr(){

  cQPAttr = rb_define_class_under(mRbRDMA, "QPAttr", rb_cData);
  rb_define_alloc_func(cQPAttr, qp_attr_s_alloc);
//  rb_define_method(cQPAttr,"initialize", rdma_pd_initialize,1);

}
