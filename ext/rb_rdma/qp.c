#include "rb_rdma.h"
#include "pd.h"
#include "cq.h"

VALUE cQP;

struct rb_rdma_data_qp {
  VALUE send_cq;
  VALUE recv_cq;
  VALUE srq;
  VALUE pd;
  struct ibv_qp *qp;
  struct ibv_qp_init_attr *init_attr;
};

static size_t
memsize_rdma_qp(const void *p){
  return sizeof(struct rb_rdma_data_qp);
};

static void
free_rdma_qp(void *ptr){
  struct rb_rdma_data_qp *qp = ptr;

  if( qp->qp ){
    ibv_destroy_qp(qp->qp);
  }
  xfree(qp->init_attr);
  xfree(qp);
};
static void
mark_rdma_qp(void *ptr){
  struct rb_rdma_data_qp *data_qp = ptr;

  rb_gc_mark(data_qp->pd);
  rb_gc_mark(data_qp->recv_cq);
  rb_gc_mark(data_qp->send_cq);
  rb_gc_mark(data_qp->srq);
}

const rb_data_type_t rdma_qp_type = {
  "rdma_qp",
  {
    mark_rdma_qp, 
    free_rdma_qp,
    memsize_rdma_qp
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
qp_s_alloc(VALUE klass){
  VALUE obj;

  struct rb_rdma_data_qp *qp = ALLOC(struct rb_rdma_data_qp);
  struct ibv_qp_init_attr *init_attr = ALLOC(struct ibv_qp_init_attr);
  qp->init_attr = init_attr;
  qp->qp = NULL;
   
  obj = TypedData_Wrap_Struct(klass,&rdma_qp_type,qp);
  return obj;
}

static int
get_qp_attr(VALUE self,enum ibv_qp_attr_mask attr_mask,struct ibv_qp_attr *attr){
  struct rb_rdma_data_qp *qp;
  TypedData_Get_Struct(self,struct rb_rdma_data_qp,&rdma_qp_type,qp);
  return ibv_query_qp(qp->qp,attr,attr_mask,qp->init_attr);
}

/*
static void
init_qp(rb_rdma_data_qp *qp,enum ibv_qp_type qt_type,ibv_cq *send_cq, ibv_cq *recv_cq, 
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
  struct rb_rdma_data_qp *data_qp;
  int qp_type;
  struct rb_rdma_data_cq *send_cq;
  struct rb_rdma_data_cq *recv_cq;
  struct ibv_srq *srq; /* FIXME */
  struct rb_rdma_data_pd *data_pd;
  struct ibv_qp_init_attr attr2;
  VALUE rb_pd,rb_qt_type, rb_send_cq, rb_recv_cq, rb_srq, rb_sq_sig_all, rb_cap;
//  struct ibv_qp_init_attr init_attr;

  rb_scan_args(argc, argv, "52", &rb_pd,
               &rb_qt_type, &rb_send_cq, &rb_recv_cq, &rb_srq, &rb_sq_sig_all,
               &rb_cap);

  qp_type = NUM2INT(rb_qt_type);

  TypedData_Get_Struct(rb_pd,struct rb_rdma_data_pd,&rdma_pd_type,data_pd);
  TypedData_Get_Struct(rb_send_cq,struct rb_rdma_data_cq,&rdma_cq_type,send_cq);
  TypedData_Get_Struct(rb_recv_cq,struct rb_rdma_data_cq,&rdma_cq_type,recv_cq);
  
  TypedData_Get_Struct(self,struct rb_rdma_data_qp,&rdma_qp_type,data_qp);

#if 0
//  qp->init_attr.send_cq = send_cq->cq;
//  qp->init_attr.recv_cq = recv_cq->cq;
  qp->init_attr.send_cq = cq;
  qp->init_attr.recv_cq = cq;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_send_wr = 1;
  qp->init_attr.cap.max_recv_wr = 1;
  qp->init_attr.cap.max_send_sge = 1;
  qp->init_attr.cap.max_recv_sge = 1;
  qp->init_attr.qp_type = qp_type;
//  qp->init_attr.qp_type = IBV_QPT_RC;
#endif

printf("***************\n");
  
  data_qp->init_attr->send_cq = send_cq->cq;
  data_qp->init_attr->recv_cq = recv_cq->cq;
  data_qp->init_attr->cap.max_send_wr = 1;
  data_qp->init_attr->cap.max_recv_wr = 1;
  data_qp->init_attr->cap.max_send_sge = 1;
  data_qp->init_attr->cap.max_recv_sge = 1;
  data_qp->init_attr->qp_type = qp_type;

//#if 0
  attr2.send_cq = send_cq->cq;
  attr2.recv_cq = recv_cq->cq;
  attr2.cap.max_send_wr = 1;
  attr2.cap.max_recv_wr = 1;
  attr2.cap.max_send_sge = 1;
  attr2.cap.max_recv_sge = 1;
  attr2.qp_type = IBV_QPT_RC;
//#endif
  
//  printf("%d\n",data_qp->init_attr->cap.max_recv_sge = 1);

  printf("pd %p\n",data_pd->pd);
  printf("send_cq %p\n",send_cq->cq);
  printf("recv_cq %p\n",recv_cq->cq);
printf("test\n");
  data_qp->qp = ibv_create_qp(data_pd->pd,data_qp->init_attr);
printf("******\n");
//  data_qp->qp = ibv_create_qp(data_pd->pd,&attr2);

  printf("context in pd: %p",data_pd->pd->context);
printf("test2\n");
  if(!data_qp->qp){
    // TODO ERROR
    rb_exc_raise(rb_syserr_new(errno, "qp alloc fail"));
  }
//  DATA_PTR(self) = qp;

  { 
    struct ibv_qp_attr attr = {
      .qp_state        = IBV_QPS_INIT,
      .pkey_index      = 0,
      .port_num        = 1, /* TODO */
      .qp_access_flags = 0
    };

    if(ibv_modify_qp(data_qp->qp,&attr,
                     IBV_QP_STATE   |
                     IBV_QP_PKEY_INDEX |
                     IBV_QP_PORT    |
                     IBV_QP_ACCESS_FLAGS)){
      rb_exc_raise(rb_syserr_new(errno, "Failed to modify QP to INIT"));
    }
  }
  printf("oK\n");

  return self;
}



/*
static VALUE
attr_qp_state(VALUE self){
  struct ibv_qp_attr attr;
  struct rb_rdma_data_qp *qp;
  
  TypedData_Get_Struct(self,struct rb_rdma_data_qp,&rdma_qp_type,qp);
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


