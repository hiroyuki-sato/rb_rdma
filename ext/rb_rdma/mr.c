#include "rb_rdma.h"
#include "context.h"
#include "pd.h"
#include "mr.h"

VALUE cMR;

static size_t
memsize_rdma_mr(const void *p){
  return sizeof(struct rb_rdma_data_mr);
};

static void
free_rdma_mr(void *ptr){
  struct rb_rdma_data_mr *data_mr = ptr;

  if( data_mr->mr )
    ibv_dereg_mr(data_mr->mr);
  xfree(data_mr);
};

static void
mark_rdma_mr(void *ptr){
  struct rb_rdma_data_mr *data_mr = ptr;

  rb_gc_mark(data_mr->pd);
  rb_gc_mark(data_mr->buf); // temporary
}

const rb_data_type_t rdma_mr_type = {
  "rdma_mr",
  {
    mark_rdma_mr, 
    free_rdma_mr,
    memsize_rdma_mr
  },
  0,0,
  0
};

static VALUE
mr_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_mr *data_mr;
  self = TypedData_Make_Struct(klass,struct rb_rdma_data_mr,&rdma_mr_type,
                               data_mr);
  data_mr->mr = NULL;
  return self;
}


static VALUE
rdma_mr_initialize(VALUE self,VALUE rb_pd,VALUE rb_buf,VALUE rb_flag){
  struct rb_rdma_data_mr *data_mr;
  struct rb_rdma_data_pd *data_pd;
  int access_flag;

  // temporary
  struct my_buffer *buf;

  Check_OBJ_Type(rb_pd,cPD,"not RDMA::PD object");
  GET_PD_DATA(rb_pd,data_pd);

  Check_Type(rb_flag,T_FIXNUM);
  access_flag = NUM2INT(rb_flag);

  // temporary
  TypedData_Get_Struct(rb_buf,struct my_buffer, &my_buffer_type,buf);

  GET_MR_DATA(self,data_mr);

  data_mr->pd = rb_pd;
  printf("pd in mr %p\n",data_pd->pd);
  data_mr->mr = ibv_reg_mr(data_pd->pd,buf->buf,buf->len,access_flag);
  if(!data_mr->mr){
     rb_exc_raise(rb_syserr_new(errno, "mr reg fail"));
    // TODO ERROR
  }

  data_mr->buf = rb_buf;

  return self;
}

static struct ibv_mr *
get_mr(VALUE self){
  struct rb_rdma_data_mr *data_mr;
  GET_MR_DATA(self,data_mr);
  return data_mr->mr;
}

static VALUE
rdma_mr_lkey(VALUE self){
  return INT2NUM(get_mr(self)->lkey);
}

static VALUE
rdma_mr_rkey(VALUE self){
  return INT2NUM(get_mr(self)->rkey);
}

void Init_mr(){

  cMR = rb_define_class_under(mRbRDMA, "MR", rb_cData);
  rb_define_alloc_func(cMR, mr_s_alloc);
  rb_define_method(cMR,"initialize", rdma_mr_initialize,3);

  rb_define_method(cMR,"lkey",rdma_mr_lkey,0);
  rb_define_method(cMR,"rkey",rdma_mr_rkey,0);

}
