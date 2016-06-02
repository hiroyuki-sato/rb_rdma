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


static VALUE
rdma_qp_attr_initialize(VALUE self){

  return self;
}

static VALUE
rdma_s_rtr_qp_attr(VALUE klass,
                   VALUE rb_mtu,
                   VALUE rb_dqpn,
                   VALUE rb_dpsn,
                   VALUE rb_dlid,
                   VALUE rb_sl,
                   VALUE rb_port){
  VALUE obj;
  struct ibv_qp_attr *attr;

  obj = qp_attr_s_alloc(klass);
  
  TypedData_Get_Struct(obj,struct ibv_qp_attr, &rdma_qp_attr_type,attr);

  attr->qp_state = IBV_QPS_RTR;
  attr->path_mtu = NUM2INT(rb_mtu);
  attr->dest_qp_num = NUM2INT(rb_dqpn);
  attr->rq_psn = NUM2INT(rb_dpsn);
  attr->max_dest_rd_atomic = 1; /* TODO */
  attr->min_rnr_timer = 12; /* TODO */
  attr->ah_attr.is_global = 0;
  attr->ah_attr.dlid = NUM2INT(rb_dlid);
  attr->ah_attr.sl = NUM2INT(rb_sl);
  attr->ah_attr.src_path_bits = 0;
  attr->ah_attr.port_num = NUM2INT(rb_port);

  return obj; 

}

void Init_qp_attr(){

  cQPAttr = rb_define_class_under(mRbRDMA, "QPAttr", rb_cData);
  rb_define_alloc_func(cQPAttr, qp_attr_s_alloc);
  rb_define_method(cQPAttr,"initialize", rdma_qp_attr_initialize,1);
  rb_define_singleton_method(cQPAttr,"rtr_qp_attr", rdma_s_rtr_qp_attr,6);

}
