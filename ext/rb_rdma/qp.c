#include "rb_rdma.h"
#include "pd.h"
#include "cq.h"

VALUE cQP;

struct rdma_qp {
  struct ibv_qp *qp;
  struct ibv_qp_init_attr *init_attr;
};

static size_t
memsize_rdma_qp(const void *p){
  return sizeof(struct rema_qp *);
};

static void
free_rdma_qp(void *ptr){
  struct rdma_qp *qp = ptr;

  ibv_destroy_qp(qp->qp);
  xfree(qp->init_attr);
  xfree(qp);
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

  struct rdma_qp *qp = ALLOC(struct rdma_qp);
  struct ibv_qp_init_attr *init_attr = ALLOC(struct ibv_qp_init_attr);
  qp->init_attr = init_attr;
   
  obj = TypedData_Wrap_Struct(klass,&rdma_qp_type,qp);
  return obj;
}

static int
get_qp_attr(VALUE self,enum ibv_qp_attr_mask attr_mask,struct ibv_qp_attr *attr){
  struct rdma_qp *qp;
  TypedData_Get_Struct(self,struct rdma_qp,&rdma_qp_type,qp);
  return ibv_query_qp(qp->qp,attr,attr_mask,qp->init_attr);
}

/*
static void
init_qp(rdma_qp *qp,enum ibv_qp_type qt_type,ibv_cq *send_cq, ibv_cq *recv_cq, 
        ibv_srq *srq, int sq_sig_all, struct ibv_qp_cap *cap){
  qp->init-attr
  qp->init_attr.send_cq = send_cq;
  qp->init_attr.recv_cq = recv_cq;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_recv_wr = 1;
  qp->init_attr.cap.max_send_sge = 1;
  qp->init_attr.cap.max_recv_sge = 1;
  qp->init_attr.qp_type = qp_type;

}
*/

static VALUE
rdma_qp_initialize(int argc, VALUE *argv, VALUE self){
  struct rdma_qp *qp;
  int qp_type;
  struct ibv_cq *send_cq;
  struct ibv_cq *recv_cq;
  struct ibv_srq *srq;
  struct ibv_pd *pd;
  struct ibv_qp_init_attr attr2;
  VALUE rb_pd,rb_qt_type, rb_send_cq, rb_recv_cq, rb_srq, rb_sq_sig_all, rb_cap;
//  struct ibv_qp_init_attr init_attr;

  rb_scan_args(argc, argv, "52", &rb_pd,
               &rb_qt_type, &rb_send_cq, &rb_recv_cq, &rb_srq, &rb_sq_sig_all,
               &rb_cap);

  qp_type = NUM2INT(rb_qt_type);

  TypedData_Get_Struct(rb_send_cq,struct ibv_cq,&rdma_cq_type,send_cq);
  TypedData_Get_Struct(rb_recv_cq,struct ibv_cq,&rdma_cq_type,recv_cq);
  TypedData_Get_Struct(rb_pd,struct ibv_pd,&rdma_pd_type,pd);
  
                                 
  TypedData_Get_Struct(self,struct rdma_qp,&rdma_qp_type,qp);

#if 0
  qp->init_attr.send_cq = send_cq;
  qp->init_attr.recv_cq = recv_cq;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_recv_wr = 1;
  qp->init_attr.cap.max_send_sge = 1;
  qp->init_attr.cap.max_recv_sge = 1;
  qp->init_attr.qp_type = qp_type;
#endif
  
  qp->init_attr->send_cq = send_cq;
  qp->init_attr->recv_cq = recv_cq;
  qp->init_attr->cap.max_send_wr = 1;
  qp->init_attr->cap.max_recv_wr = 1;
  qp->init_attr->cap.max_send_sge = 1;
  qp->init_attr->cap.max_recv_sge = 1;
  qp->init_attr->qp_type = qp_type;

//#if 0
  attr2.send_cq = send_cq;
  attr2.recv_cq = recv_cq;
  attr2.cap.max_send_wr = 1;
  attr2.cap.max_recv_wr = 1;
  attr2.cap.max_send_sge = 1;
  attr2.cap.max_recv_sge = 1;
  attr2.qp_type = IBV_QPT_RC;
//#endif
  
//  printf("%d\n",qp->init_attr->cap.max_recv_sge = 1);

  printf("pd %p\n",pd);
  printf("send_cq %p\n",send_cq);
  printf("recv_cq %p\n",recv_cq);
printf("test\n");
  qp->qp = ibv_create_qp(pd,qp->init_attr);
//  qp->qp = ibv_create_qp(pd,&attr2);

printf("test2\n");
  if(!qp->qp){
    // TODO ERROR
    rb_exc_raise(rb_syserr_new(errno, "qp alloc fail"));
  }
//  DATA_PTR(self) = qp;

  return self;
}



/*
static VALUE
attr_qp_state(VALUE self){
  struct ibv_qp_attr attr;
  struct rdma_qp *qp;
  
  TypedData_Get_Struct(self,struct rdma_qp,&rdma_qp_type,qp);
  ibv_query_qp(qp->qp,&attr,IBV_QP_STATE,qp->init_attr);

  return INT2NUM(attr.qp_state);
}
*/

static VALUE
attr_qp_state(VALUE self){
  struct ibv_qp_attr attr;
  int ret;
  ret = get_qp_attr(self,IBV_QP_STATE,&attr);
  if( ret != 0 ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2NUM(attr.qp_state);
}

void Init_qp(){

  cQP = rb_define_class_under(mRbRDMA, "QP", rb_cData);
  rb_define_alloc_func(cQP, qp_s_alloc);
//  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);
  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);
  rb_define_method(cQP,"qp_state", attr_qp_state,0);

}


