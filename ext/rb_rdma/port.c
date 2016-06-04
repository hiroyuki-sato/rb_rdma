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
  if(!ibv_query_port(ctx->context,port_num,data_port->port_attr)){
    rb_exc_raise(rb_syserr_new(errno, "port query fail"));
  };
  return self;
}

void Init_port(){

  cPort = rb_define_class_under(mRbRDMA, "Port", rb_cData);
  rb_define_alloc_func(cPort, port_s_alloc);
  rb_define_method(cPort,"initialize", rdma_port_initialize,2);

}
