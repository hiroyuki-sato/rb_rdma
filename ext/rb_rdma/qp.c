#include "rb_rdma.h"
#include "pd.h"
#include "cq.h"
#include "qp.h"
#include "mr.h"

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
  0
};

static VALUE
qp_s_alloc(VALUE klass){
  VALUE obj;

  struct rb_rdma_data_qp *qp;
  struct ibv_qp_init_attr *init_attr = ALLOC(struct ibv_qp_init_attr);
//  struct ibv_qp_init_attr *init_attr = malloc(sizeof(struct ibv_qp_init_attr));

  printf("ibv_qp_init_attr %p\n",init_attr);
  obj = TypedData_Make_Struct(klass,struct rb_rdma_data_qp,&rdma_qp_type,
                              qp);
  qp->init_attr = init_attr;
  return obj;
}

static int
get_qp_attr(VALUE self,enum ibv_qp_attr_mask attr_mask,struct ibv_qp_attr *attr){
  struct rb_rdma_data_qp *qp;
  GET_QP_DATA(self,qp);
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

  VALUE rb_pd,rb_qt_type, rb_send_cq, rb_recv_cq, rb_srq, rb_sq_sig_all, rb_cap;
//  struct ibv_qp_init_attr init_attr;

  rb_scan_args(argc, argv, "52", &rb_pd,
               &rb_qt_type, &rb_send_cq, &rb_recv_cq, &rb_srq, &rb_sq_sig_all,
               &rb_cap);

  qp_type = NUM2INT(rb_qt_type);

  GET_PD_DATA(rb_pd,data_pd);
  GET_CQ_DATA(rb_send_cq,send_cq);
  GET_CQ_DATA(rb_send_cq,recv_cq);
  GET_QP_DATA(self,data_qp);

printf("***************\n");
  
  data_qp->init_attr->send_cq = send_cq->cq;
  data_qp->init_attr->recv_cq = recv_cq->cq;
  data_qp->init_attr->srq = NULL;
  data_qp->init_attr->cap.max_send_wr = 1;
  data_qp->init_attr->cap.max_recv_wr = 1;
  data_qp->init_attr->cap.max_send_sge = 1;
  data_qp->init_attr->cap.max_recv_sge = 1;
  data_qp->init_attr->cap.max_inline_data = 0;
  data_qp->init_attr->qp_type = qp_type;
  
  data_qp->qp = ibv_create_qp(data_pd->pd,data_qp->init_attr);

  if(!data_qp->qp){
    // TODO ERROR
    rb_exc_raise(rb_syserr_new(errno, "qp alloc fail"));
  }

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

static VALUE
qp_attr_qp_state(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_STATE,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.qp_state);
}

static VALUE
qp_attr_cur_qp_state(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_CUR_STATE,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.cur_qp_state);
}

static VALUE
qp_attr_path_mtu(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_PATH_MTU,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.path_mtu);
}

static VALUE
qp_attr_path_mig_state(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_PATH_MIG_STATE,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.path_mig_state);
}

static VALUE
qp_attr_qkey(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_QKEY,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.qkey);
}

static VALUE
qp_attr_rq_psn(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_RQ_PSN,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.rq_psn);
}

static VALUE
qp_attr_sq_psn(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_SQ_PSN,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.sq_psn);
}

static VALUE
qp_attr_dest_qp_num(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_DEST_QPN,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.dest_qp_num);
}

static VALUE
qp_attr_qp_access_flags(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_ACCESS_FLAGS,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.qp_access_flags);
}

#if 0
static VALUE
qp_attr_cap(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_CAP,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.cap);
}

static VALUE
qp_attr_ah_attr(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_AV,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.ah_attr);
}

static VALUE
qp_attr_alt_ah_attr(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,FIXME,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.alt_ah_attr);
}
#endif

static VALUE
qp_attr_pkey_index(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_PKEY_INDEX,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.pkey_index);
}

static VALUE
qp_attr_alt_pkey_index(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_ALT_PATH,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.alt_pkey_index);
}

static VALUE
qp_attr_en_sqd_async_notify(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_EN_SQD_ASYNC_NOTIFY,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.en_sqd_async_notify);
}

#if 0
static VALUE
qp_attr_sq_draining(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_STATE,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.sq_draining);
}
#endif

static VALUE
qp_attr_max_rd_atomic(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_MAX_QP_RD_ATOMIC,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.max_rd_atomic);
}

static VALUE
qp_attr_max_dest_rd_atomic(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_MAX_DEST_RD_ATOMIC,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.max_dest_rd_atomic);
}

static VALUE
qp_attr_min_rnr_timer(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_MIN_RNR_TIMER,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.min_rnr_timer);
}

static VALUE
qp_attr_port_num(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_PORT,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.port_num);
}

static VALUE
qp_attr_timeout(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_TIMEOUT,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.timeout);
}

static VALUE
qp_attr_retry_cnt(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_RETRY_CNT,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.retry_cnt);
}

static VALUE
qp_attr_rnr_retry(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_RNR_RETRY,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.rnr_retry);
}

static VALUE
qp_attr_alt_port_num(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_ALT_PATH,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.alt_port_num);
}

static VALUE
qp_attr_alt_timeout(VALUE self)
{
  struct ibv_qp_attr attr;
  if ( get_qp_attr(self,IBV_QP_ALT_PATH,&attr) ){
     rb_exc_raise(rb_syserr_new(errno, "qp attr get fail"));
  }
  return INT2FIX(attr.alt_timeout);
}

static VALUE
qp_attr_qp_num(VALUE self)
{
  struct rb_rdma_data_qp *data_qp;
  GET_QP_DATA(self,data_qp);
  return INT2FIX(data_qp->qp->qp_num);
}

static VALUE
qp_attr_qp_type(VALUE self)
{
  struct rb_rdma_data_qp *data_qp;
  GET_QP_DATA(self,data_qp);
  return INT2FIX(data_qp->qp->qp_type);
}

static VALUE
rdma_qp_post_recv(VALUE self,VALUE rb_mr,VALUE rb_wr_id){

  int wr_id;
  int i;
  int n = 1;
  struct ibv_recv_wr *bad_wr;

  struct ibv_sge list;
  struct ibv_recv_wr wr;
  struct rb_rdma_data_qp *data_qp;
  struct rb_rdma_data_mr *data_mr;
  struct my_buffer *my_buf;

  GET_QP_DATA(self,data_qp);
  GET_MR_DATA(rb_mr,data_mr);

  TypedData_Get_Struct(data_mr->buf,struct my_buffer, &my_buffer_type,my_buf);

  Check_Type(rb_wr_id,T_FIXNUM);
  wr_id = NUM2INT(rb_wr_id);

  list.addr = (uintptr_t) my_buf->buf;
  list.length = my_buf->len;
  list.lkey = data_mr->mr->lkey;

  memset(&wr,0,sizeof(struct ibv_recv_wr));
  wr.wr_id = wr_id;
  wr.sg_list  = &list;
  wr.num_sge  = 1;

  for( i = 0 ; i < n ; ++i ){
    if( ibv_post_recv(data_qp->qp,&wr,&bad_wr) ){
    }
  }
  return Qnil;
}

void Init_qp(){

  cQP = rb_define_class_under(mRbRDMA, "QP", rb_cData);
  rb_define_alloc_func(cQP, qp_s_alloc);
//  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);
  rb_define_method(cQP,"initialize", rdma_qp_initialize,-1);
  rb_define_method(cQP,"post_recv",rdma_qp_post_recv,2);

  rb_define_method(cQP,"qp_state",qp_attr_qp_state,0);
  rb_define_method(cQP,"cur_qp_state",qp_attr_cur_qp_state,0);
  rb_define_method(cQP,"path_mtu",qp_attr_path_mtu,0);
  rb_define_method(cQP,"path_mig_state",qp_attr_path_mig_state,0);
  rb_define_method(cQP,"qkey",qp_attr_qkey,0);
  rb_define_method(cQP,"rq_psn",qp_attr_rq_psn,0);
  rb_define_method(cQP,"sq_psn",qp_attr_sq_psn,0);
  rb_define_method(cQP,"dest_qp_num",qp_attr_dest_qp_num,0);
  rb_define_method(cQP,"qp_access_flags",qp_attr_qp_access_flags,0);
//  rb_define_method(cQP,"cap",qp_attr_cap,0);
//  rb_define_method(cQP,"ah_attr",qp_attr_ah_attr,0);
//  rb_define_method(cQP,"alt_ah_attr",qp_attr_alt_ah_attr,0);
  rb_define_method(cQP,"pkey_index",qp_attr_pkey_index,0);
  rb_define_method(cQP,"alt_pkey_index",qp_attr_alt_pkey_index,0);
  rb_define_method(cQP,"en_sqd_async_notify",qp_attr_en_sqd_async_notify,0);
//  rb_define_method(cQP,"sq_draining",qp_attr_sq_draining,0);
  rb_define_method(cQP,"max_rd_atomic",qp_attr_max_rd_atomic,0);
  rb_define_method(cQP,"max_dest_rd_atomic",qp_attr_max_dest_rd_atomic,0);
  rb_define_method(cQP,"min_rnr_timer",qp_attr_min_rnr_timer,0);
  rb_define_method(cQP,"port_num",qp_attr_port_num,0);
  rb_define_method(cQP,"timeout",qp_attr_timeout,0);
  rb_define_method(cQP,"retry_cnt",qp_attr_retry_cnt,0);
  rb_define_method(cQP,"rnr_retry",qp_attr_rnr_retry,0);
  rb_define_method(cQP,"alt_port_num",qp_attr_alt_port_num,0);
  rb_define_method(cQP,"alt_timeout",qp_attr_alt_timeout,0);
  //
  // from ibv_qp
  rb_define_method(cQP,"qp_num",qp_attr_qp_num,0);
  rb_define_method(cQP,"qp_type",qp_attr_qp_type,0);


}


