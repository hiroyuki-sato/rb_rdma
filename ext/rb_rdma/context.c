#include "rb_rdma.h"
#include "device.h"
#include "port.h"

VALUE cContext;
//static VALUE mRbRDMA;

struct rdma_context {
  struct ibv_device **devices;
  struct ibv_device *device;
  struct ibv_context *context;
};

static void
free_rdma_context(void *ptr){
  struct rdma_context *ctx = ptr;
  printf("free context\n");

  if( ctx->context )
    ibv_close_device(ctx->context);

  if( ctx->devices )
    ibv_free_device_list(ctx->devices);

  xfree(ctx);
};

/*
static void
alloc_rdma_context(void *ptr){
  struct rma_context *ctx = ptr;

};
*/
static size_t
memsize_rdma_context(const void *p){
  return sizeof(struct rdma_context);
};


const rb_data_type_t rdma_context_type = {
  "rdma_context",
  {
    0, 
    free_rdma_context,
    memsize_rdma_context
  },
  0,0,
  0
};

static VALUE
rdma_context_open(VALUE self,VALUE rb_dev_name){
  VALUE obj;
  int i;
  char *dev_name;
  int num_dev;
  struct ibv_device_attr dev_attr;

  struct rdma_context *sval;

  Check_Type(rb_dev_name,T_STRING);
  dev_name = StringValuePtr(rb_dev_name);

  obj = TypedData_Make_Struct(cContext,struct rdma_context,&rdma_context_type,sval);

  sval->devices = ibv_get_device_list(&num_dev);
  sval->device = NULL;

  for( i = 0 ; i < num_dev ; i++ ){
    printf("%s\n",sval->devices[i]->name);
    if( strncmp(sval->devices[i]->name,dev_name,strlen(dev_name)) == 0 ){
      sval->device = sval->devices[i];
      
      break;
    }
  }
  if( sval->device == NULL ){
    // TODO error
      rb_raise(rb_eRuntimeError, "no device");
  } else {
    printf("open device %s\n",sval->device->name);

    sval->context = ibv_open_device(sval->device);
    if(!sval->context){
      rb_raise(rb_eRuntimeError, "open device failed");
    }
    printf("context addr %p\n",sval->context);
  }

  if( ibv_query_device(sval->context,&dev_attr) ){
    rb_raise(rb_eRuntimeError, "query_device failed");
  }

  return obj;
}

static VALUE
rdma_context_device(VALUE self){
  return rb_funcall(cDevice,rb_intern("new"),1,self);
}

static VALUE
rdma_context_port(VALUE self,VALUE port){
  return rb_funcall(cPort,rb_intern("new"),2,self,port);
}

void Init_context(){

//  cContext = rb_define_class_under(mRbRDMA, "Context", rb_cObject);
  cContext = rb_define_class_under(mRbRDMA, "Context", rb_cData);
  rb_define_singleton_method(cContext, "open", rdma_context_open, 1);
  rb_define_method(cContext,"device",rdma_context_device,0);
  rb_define_method(cContext,"port",rdma_context_port,1);


  // http://d.hatena.ne.jp/tueda_wolf/20091230/p1
//  rb_define_alloc_func(cContext, ruby_Object3D_allocate);
//  rb_define_private_method (rb_cObject3D, "initialize",     (VALUE(*)(...))ruby_Object3D_initialize,                0);

}
