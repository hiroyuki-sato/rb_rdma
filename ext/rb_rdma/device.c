#include "rb_rdma.h"
#include "context.h"

static VALUE cDevice;

struct rb_rdma_data_device {
  VALUE context;
  struct ibv_device_attr *attr;
};

static size_t
memsize_rdma_device(const void *p){
  return sizeof(struct rb_rdma_data_device);
};

static void
free_rdma_device(void *ptr){
  struct rb_rdma_data_device *data_device = ptr;

  printf("-----free called\n");

  xfree(data_device);
};

static void
mark_rdma_device(void *ptr){
  struct rb_rdma_data_device *data_device = ptr;
  
  printf("mark called\n");
  rb_gc_mark(data_device->context);
}

static const rb_data_type_t rdma_device_type = {
  "rdma_device",
  {
    mark_rdma_device, 
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
  struct rb_rdma_data_device *data_device = ALLOC(struct rb_rdma_data_device);
  obj = TypedData_Wrap_Struct(klass,&rdma_device_type,data_device);
  data_device->attr = attr;
  return obj;
}

static VALUE
rdma_device_initialize(VALUE obj, VALUE rb_ctx){

  struct rdma_context *ctx;
//  struct ibv_device_attr *attr = NULL;
  struct rb_rdma_data_device *data_device;
  
  TypedData_Get_Struct(rb_ctx,struct rdma_context,&rdma_context_type,ctx);

  TypedData_Get_Struct(obj,struct rb_rdma_data_device,&rdma_device_type,data_device);

  if(ibv_query_device(ctx->context,data_device->attr))
    rb_exc_raise(rb_syserr_new(errno, "query_device failed"));

  data_device->context = rb_ctx;

  return obj;
}

static struct ibv_device_attr *
get_device_attr(VALUE self){
  struct rb_rdma_data_device *data_device;
  TypedData_Get_Struct(self,struct rb_rdma_data_device,&rdma_device_type,data_device);

  return data_device->attr;
}

static VALUE
device_attr_fw_ver(VALUE self){
  printf("%p\n",get_device_attr(self));

  return rb_str_new2(get_device_attr(self)->fw_ver);
}

static VALUE
device_attr_max_qp(VALUE self){
  printf("%p\n",get_device_attr(self));
  return INT2NUM(get_device_attr(self)->max_qp);
}

void Init_device(){

  cDevice = rb_define_class_under(mRbRDMA, "Device", rb_cData);
  rb_define_alloc_func(cDevice, device_s_alloc);
  rb_define_method(cDevice,"initialize", rdma_device_initialize,1);
  rb_define_method(cDevice,"fw_ver", device_attr_fw_ver,0);

  rb_define_method(cDevice,"max_qp", device_attr_max_qp,0);

}
