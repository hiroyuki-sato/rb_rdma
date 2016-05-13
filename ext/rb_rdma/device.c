#include "rb_rdma.h"
#include "context.h"

static VALUE cDevice;

static size_t
memsize_rdma_device(const void *p){
  return sizeof(struct ibv_device_attr);
};

static void
free_rdma_device(void *ptr){
  struct ibv_device_attr *attr = ptr;

  xfree(attr);
};

static const rb_data_type_t rdma_device_type = {
  "rdma_device",
  {
    0, 
    free_rdma_device,
    memsize_rdma_device
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

static VALUE
device_s_alloc(VALUE klass){
  VALUE obj;
  struct ibv_device_attr *attr = ALLOC(struct ibv_device_attr);
  obj = TypedData_Wrap_Struct(klass,&rdma_device_type,attr);
  return obj;
}

static VALUE
rdma_device_initialize(VALUE obj, VALUE obj_ctx){

//  VALUE obj;

  struct rdma_context *ctx;
//  struct ibv_device_attr *attr = ALLOC(struct ibv_device_attr);
  struct ibv_device_attr *attr = NULL;
  
  TypedData_Get_Struct(obj_ctx,struct rdma_context,&rdma_context_type,ctx);
//  obj = TypedData_Wrap_Struct(cDevice, &rdma_device_type, attr);

  TypedData_Get_Struct(obj,struct ibv_device_attr,&rdma_device_type,attr);

  ibv_query_device(ctx->context,attr);

  return obj;
}

static VALUE
rdma_device_fw_ver(VALUE self){

  struct ibv_device_attr *attr;
  
  TypedData_Get_Struct(self,struct ibv_device_attr,&rdma_device_type,attr);

  printf("%s\n",attr->fw_ver);

  return self;
}

void Init_device(){

  cDevice = rb_define_class_under(mRbRDMA, "Device", rb_cData);
  rb_define_alloc_func(cDevice, device_s_alloc);
  rb_define_method(cDevice,"initialize", rdma_device_initialize,1);
  rb_define_method(cDevice,"fw_ver", rdma_device_fw_ver,0);

}
