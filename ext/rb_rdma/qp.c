#include "rb_rdma.h"
#include "pd.h"
#include "cq.h"

VALUE cQP;

static size_t
memsize_rdma_qp(const void *p){
  return sizeof(struct ibv_qp *);
};

static void
free_rdma_qp(void *ptr){
  struct ibv_qp *qp = ptr;

  ibv_destroy_qp(qp);
};

const rb_data_type_t rdma_qp_type = {
  "rdma_qp",
  {
    0, 
    free_rdma_qp,
    memsize_rdma_qp
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
qp_s_alloc(VALUE klass){
  VALUE obj;
  obj = TypedData_Wrap_Struct(klass,&rdma_qp_type,0);
  return obj;
}


static VALUE
rdma_qp_initialize(int argc, VALUE *argv, VALUE self){
  struct ibv_qp *qp;
  int qp_type;
  struct ibv_cq *send_cq;
  struct ibv_cq *recv_cq;
  struct ibv_srq *srq;
  VALUE rb_qt_type, rb_send_cq, rb_recv_cq, rb_srq, rb_sq_sig_all, rb_cap;

  rb_scan_args(argc, argv, "42", 
               &rb_qt_type, &rb_send_cq, &rb_recv_cq, &rb_srq, &rb_sq_sig_all,
               &rb_cap);

  qp_type = NUM2INT(rb_qt_type);

  TypedData_Get_Struct(rb_send_cq,struct ibv_cq,&rdma_cq_type,send_cq);
  TypedData_Get_Struct(rb_recv_cq,struct ibv_cq,&rdma_cq_type,recv_cq);
                                 
//  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);
//  TypedData_Get_Struct(obj_c_channel,struct ibv_comp_channel,
//                       &rdma_comp_channel_type,c_channel);
  
  TypedData_Get_Struct(self,struct ibv_qp,&rdma_qp_type,qp);

//  qp = ibv_create_qp(ctx->context,qpe,NULL,c_channel,0);
  if(!qp){
    // TODO ERROR
  }
  DATA_PTR(self) = qp;

  return self;
}

void Init_qp(){

  cQP = rb_define_class_under(mRbRDMA, "QP", rb_cData);
  rb_define_alloc_func(cQP, qp_s_alloc);
//  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);
  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);

}
