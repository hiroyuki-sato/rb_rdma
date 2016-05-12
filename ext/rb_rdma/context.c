#include "rb_rdma.h"

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

  ibv_close_device(ctx->context);
  ibv_free_device_list(ctx->devices);

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
  "rbma_context",
  {
    0, 
    free_rdma_context,
    memsize_rdma_context
  },
  0,0,
  RUBY_TYPED_FREE_IMMEDIATELY  
};

VALUE rdma_context_open(VALUE self,VALUE dev){
  VALUE obj;
  int i;
  char *dev_name;
  int num_dev;
  

  struct rdma_context *sval = ALLOC(struct rdma_context);
  dev_name = StringValuePtr(dev);
  
  
  obj = TypedData_Wrap_Struct(cContext, &rdma_context_type, sval);
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
    printf("no device *\n");
  } else {
    printf("open device %s\n",sval->device->name);
    sval->context = ibv_open_device(sval->device);
  }

  return obj;
}

void Init_context(){

  cContext = rb_define_class_under(mRbRDMA, "Context", rb_cObject);
  rb_define_singleton_method(cContext, "open", rdma_context_open, 1);

  // http://d.hatena.ne.jp/tueda_wolf/20091230/p1
//  rb_define_alloc_func(cContext, ruby_Object3D_allocate);
//  rb_define_private_method (rb_cObject3D, "initialize",     (VALUE(*)(...))ruby_Object3D_initialize,                0);

}
