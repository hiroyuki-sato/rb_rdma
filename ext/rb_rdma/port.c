#include "rb_rdma.h"
#include "context.h"
#include "port.h"

VALUE cPort;

static size_t
memsize_rdma_port(const void *p){
  return sizeof(struct rb_rdma_data_port *);
};

static void
free_rdma_port(void *ptr){
  struct rb_rdma_data_port *data_port = ptr;

  if( data_port->port_attr ){
    xfree(data_port->port_attr);
    data_port->port_attr = NULL;
  }
  xfree(data_port);
};

static void
mark_rdma_port(void *ptr){
  struct rb_rdma_data_port *data_port = ptr;

  rb_gc_mark(data_port->context);
  rb_gc_mark(data_port->port_num);

}


const rb_data_type_t rdma_port_type = {
  "rdma_port",
  {
    mark_rdma_port, 
    free_rdma_port,
    memsize_rdma_port
  },
  0,0,
  0
};

static VALUE
port_s_alloc(VALUE klass){
  VALUE self;
  struct rb_rdma_data_port *data_port;

  self = TypedData_Make_Struct(klass,struct rb_rdma_data_port,
                              &rdma_port_type,data_port);
  data_port->port_attr = ALLOC(struct ibv_port_attr);
  memset(data_port->port_attr,0,sizeof(struct ibv_port_attr));
  return self;
}


static VALUE
rdma_port_initialize(VALUE self, VALUE rb_ctx,VALUE rb_port_num){

  struct rdma_context *ctx;
  struct rb_rdma_data_port *data_port;
  int port_num;
  
  Check_OBJ_Type(rb_ctx,cContext,"not RDMA::Context object");
  GET_Context_DATA(rb_ctx,ctx);

  Check_Type(rb_port_num,T_FIXNUM);
  port_num = FIX2INT(rb_port_num);

  GET_PORT_DATA(self,data_port);

  data_port->context = rb_ctx;
  data_port->port_num = rb_port_num;
  if(ibv_query_port(ctx->context,port_num,data_port->port_attr)){
    rb_exc_raise(rb_syserr_new(errno, "port query fail"));
  };
  return self;
}

static VALUE
rdma_port_num(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self, data_port);
  return data_port->port_num;
}

static VALUE
port_attr_state(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->state);
}

static VALUE
port_attr_max_mtu(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->max_mtu);
}

static VALUE
port_attr_active_mtu(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->active_mtu);
}

static VALUE
port_attr_gid_tbl_len(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->gid_tbl_len);
}

static VALUE
port_attr_port_cap_flags(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->port_cap_flags);
}

static VALUE
port_attr_max_msg_sz(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->max_msg_sz);
}

static VALUE
port_attr_bad_pkey_cntr(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->bad_pkey_cntr);
}

static VALUE
port_attr_qkey_viol_cntr(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->qkey_viol_cntr);
}

static VALUE
port_attr_pkey_tbl_len(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->pkey_tbl_len);
}

static VALUE
port_attr_lid(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->lid);
}

static VALUE
port_attr_sm_lid(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->sm_lid);
}

static VALUE
port_attr_lmc(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->lmc);
}

static VALUE
port_attr_max_vl_num(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->max_vl_num);
}

static VALUE
port_attr_sm_sl(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->sm_sl);
}

static VALUE
port_attr_subnet_timeout(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->subnet_timeout);
}

static VALUE
port_attr_init_type_reply(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->init_type_reply);
}

static VALUE
port_attr_active_width(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->active_width);
}

static VALUE
port_attr_active_speed(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->active_speed);
}

static VALUE
port_attr_phys_state(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->phys_state);
}

static VALUE
port_attr_link_layer(VALUE self)
{
  struct rb_rdma_data_port *data_port;
  GET_PORT_DATA(self,data_port);

  return INT2FIX(data_port->port_attr->link_layer);
}


void Init_port(){

  cPort = rb_define_class_under(mRbRDMA, "Port", rb_cData);
  rb_define_alloc_func(cPort, port_s_alloc);
  rb_define_method(cPort,"initialize", rdma_port_initialize,2);
  rb_define_method(cPort,"port_num", rdma_port_num,0);


  rb_define_method(cPort,"state",port_attr_state,0);
  rb_define_method(cPort,"max_mtu",port_attr_max_mtu,0);
  rb_define_method(cPort,"active_mtu",port_attr_active_mtu,0);
  rb_define_method(cPort,"gid_tbl_len",port_attr_gid_tbl_len,0);
  rb_define_method(cPort,"port_cap_flags",port_attr_port_cap_flags,0);
  rb_define_method(cPort,"max_msg_sz",port_attr_max_msg_sz,0);
  rb_define_method(cPort,"bad_pkey_cntr",port_attr_bad_pkey_cntr,0);
  rb_define_method(cPort,"qkey_viol_cntr",port_attr_qkey_viol_cntr,0);
  rb_define_method(cPort,"pkey_tbl_len",port_attr_pkey_tbl_len,0);
  rb_define_method(cPort,"lid",port_attr_lid,0);
  rb_define_method(cPort,"sm_lid",port_attr_sm_lid,0);
  rb_define_method(cPort,"lmc",port_attr_lmc,0);
  rb_define_method(cPort,"max_vl_num",port_attr_max_vl_num,0);
  rb_define_method(cPort,"sm_sl",port_attr_sm_sl,0);
  rb_define_method(cPort,"subnet_timeout",port_attr_subnet_timeout,0);
  rb_define_method(cPort,"init_type_reply",port_attr_init_type_reply,0);
  rb_define_method(cPort,"active_width",port_attr_active_width,0);
  rb_define_method(cPort,"active_speed",port_attr_active_speed,0);
  rb_define_method(cPort,"phys_state",port_attr_phys_state,0);
  rb_define_method(cPort,"link_layer",port_attr_link_layer,0);

}

