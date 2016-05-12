#include "rb_rdma.h"
#include "context.h"

static VALUE klass;

struct rdma_device {
  struct ibv_device *device;
  struct ibv_device_attr *attr;
};

static size_t
memsize_rdma_device(const void *p){
  return sizeof(struct rdma_device);
};

static void
free_rdma_device(void *ptr){
  struct rdma_device *dev = ptr;

  free(dev->attr);
  free(dev);

};

static const rb_data_type_t rdma_device_type = {
  "rbma_device",
  {
    0, 
    free_rdma_device,
    memsize_rdma_device
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};


static VALUE
rdma_device_initialize(VALUE self, VALUE obj_ctx){
  VALUE obj;

  struct rdma_context *ctx;
  struct rdma_device *sval = ALLOC(struct rdma_device);
  struct ibv_device_attr *attr = ALLOC(struct ibv_device_attr);
  
  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);
  obj = TypedData_Wrap_Struct(klass, &rdma_device_type, sval);

  ibv_query_device(ctx->context,attr);

  sval->device = ctx->device;
  sval->attr = attr;

  return obj;
}

static VALUE
rdma_device_fw_ver(VALUE self){

  struct rdma_device *dev;
  
//  TypedData_Get_Struct(klass,struct rdma_device,&rdma_device_type,dev);

//  printf("%s\n",dev->attr->fw_ver);

  return self;
}

void Init_device(){

  klass = rb_define_class_under(mRbRDMA, "Device", rb_cObject);
  rb_define_method(klass,"initialize", rdma_device_initialize,1);
  rb_define_method(klass,"fw_ver", rdma_device_fw_ver,0);

}
