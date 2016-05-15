#include "rb_rdma.h"
#include "context.h"
#include "pd.h"

static VALUE cMR;

static size_t
memsize_rdma_mr(const void *p){
  return sizeof(struct ibv_mr *);
};

static void
free_rdma_mr(void *ptr){
  struct ibv_mr *mr = ptr;

  ibv_dereg_mr(mr);
};

static const rb_data_type_t rdma_mr_type = {
  "rdma_mr",
  {
    0, 
    free_rdma_mr,
    memsize_rdma_mr
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
mr_s_alloc(VALUE klass){
  VALUE obj;
  obj = TypedData_Wrap_Struct(klass,&rdma_mr_type,0);
  return obj;
}


static VALUE
rdma_mr_initialize(VALUE obj,VALUE obj_pd,VALUE obj_buf,VALUE obj_size,VALUE obj_flag){
  struct ibv_mr *mr;
  struct ibv_pd *pd;
  int access_flag = NUM2INT(obj_flag);
  int size = NUM2INT(obj_size);
  char *buf;

  long sz = sysconf(_SC_PAGESIZE);
  // temporary
  buf = malloc(sz * size);
  
  TypedData_Get_Struct(obj_pd,struct ibv_pd,&rdma_pd_type,pd);

  TypedData_Get_Struct(obj,struct ibv_mr,&rdma_mr_type,mr);

  mr = ibv_reg_mr(pd,buf,size,access_flag);
  if(!mr){
     rb_exc_raise(rb_syserr_new(errno, "mr reg fail"));
    // TODO ERROR
  }
  DATA_PTR(obj) = mr;

  return obj;
}

void Init_mr(){

  cMR = rb_define_class_under(mRbRDMA, "MR", rb_cData);
  rb_define_alloc_func(cMR, mr_s_alloc);
//  rb_define_method(cMR,"initialize", rdma_mr_initialize,3);
  rb_define_method(cMR,"initialize", rdma_mr_initialize,4);

}
