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
  struct rb_rdma_data_device *data_device;

  obj = TypedData_Make_Struct(klass,struct rb_rdma_data_device,
                              &rdma_device_type,data_device);
  memset(attr,0,sizeof(struct ibv_device_attr));
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
dev_attr_fw_ver(VALUE self){
  printf("%p\n",get_device_attr(self));

  return rb_str_new2(get_device_attr(self)->fw_ver);
}

static VALUE
dev_attr_node_guid(VALUE self)
{
  return INT2NUM(get_device_attr(self)->node_guid);
}

static VALUE
dev_attr_sys_image_guid(VALUE self)
{
  return INT2NUM(get_device_attr(self)->sys_image_guid);
}

static VALUE
dev_attr_max_mr_size(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_mr_size);
}

static VALUE
dev_attr_page_size_cap(VALUE self)
{
  return INT2NUM(get_device_attr(self)->page_size_cap);
}

static VALUE
dev_attr_vendor_id(VALUE self)
{
  return INT2NUM(get_device_attr(self)->vendor_id);
}

static VALUE
dev_attr_vendor_part_id(VALUE self)
{
  return INT2NUM(get_device_attr(self)->vendor_part_id);
}

static VALUE
dev_attr_hw_ver(VALUE self)
{
  return INT2NUM(get_device_attr(self)->hw_ver);
}

static VALUE
dev_attr_max_qp(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_qp);
}

static VALUE
dev_attr_max_qp_wr(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_qp_wr);
}

static VALUE
dev_attr_device_cap_flags(VALUE self)
{
  return INT2NUM(get_device_attr(self)->device_cap_flags);
}

static VALUE
dev_attr_max_sge(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_sge);
}

static VALUE
dev_attr_max_sge_rd(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_sge_rd);
}

static VALUE
dev_attr_max_cq(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_cq);
}

static VALUE
dev_attr_max_cqe(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_cqe);
}

static VALUE
dev_attr_max_mr(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_mr);
}

static VALUE
dev_attr_max_pd(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_pd);
}

static VALUE
dev_attr_max_qp_rd_atom(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_qp_rd_atom);
}

static VALUE
dev_attr_max_ee_rd_atom(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_ee_rd_atom);
}

static VALUE
dev_attr_max_res_rd_atom(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_res_rd_atom);
}

static VALUE
dev_attr_max_qp_init_rd_atom(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_qp_init_rd_atom);
}

static VALUE
dev_attr_max_ee_init_rd_atom(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_ee_init_rd_atom);
}

static VALUE
dev_attr_atomic_cap(VALUE self)
{
  return INT2NUM(get_device_attr(self)->atomic_cap);
}

static VALUE
dev_attr_max_ee(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_ee);
}

static VALUE
dev_attr_max_rdd(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_rdd);
}

static VALUE
dev_attr_max_mw(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_mw);
}

static VALUE
dev_attr_max_raw_ipv6_qp(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_raw_ipv6_qp);
}

static VALUE
dev_attr_max_raw_ethy_qp(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_raw_ethy_qp);
}

static VALUE
dev_attr_max_mcast_grp(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_mcast_grp);
}

static VALUE
dev_attr_max_mcast_qp_attach(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_mcast_qp_attach);
}

static VALUE
dev_attr_max_total_mcast_qp_attach(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_total_mcast_qp_attach);
}

static VALUE
dev_attr_max_ah(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_ah);
}

static VALUE
dev_attr_max_fmr(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_fmr);
}

static VALUE
dev_attr_max_map_per_fmr(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_map_per_fmr);
}

static VALUE
dev_attr_max_srq(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_srq);
}

static VALUE
dev_attr_max_srq_wr(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_srq_wr);
}

static VALUE
dev_attr_max_srq_sge(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_srq_sge);
}

static VALUE
dev_attr_max_pkeys(VALUE self)
{
  return INT2NUM(get_device_attr(self)->max_pkeys);
}

static VALUE
dev_attr_local_ca_ack_delay(VALUE self)
{
  return INT2NUM(get_device_attr(self)->local_ca_ack_delay);
}

static VALUE
dev_attr_phys_port_cnt(VALUE self)
{
  return INT2NUM(get_device_attr(self)->phys_port_cnt);
}

void Init_device(){

  cDevice = rb_define_class_under(mRbRDMA, "Device", rb_cData);
  rb_define_alloc_func(cDevice, device_s_alloc);
  rb_define_method(cDevice,"initialize", rdma_device_initialize,1);

  rb_define_method(cDevice,"fw_ver",dev_attr_fw_ver,0);
  rb_define_method(cDevice,"node_guid",dev_attr_node_guid,0);
  rb_define_method(cDevice,"sys_image_guid",dev_attr_sys_image_guid,0);
  rb_define_method(cDevice,"max_mr_size",dev_attr_max_mr_size,0);
  rb_define_method(cDevice,"page_size_cap",dev_attr_page_size_cap,0);
  rb_define_method(cDevice,"vendor_id",dev_attr_vendor_id,0);
  rb_define_method(cDevice,"vendor_part_id",dev_attr_vendor_part_id,0);
  rb_define_method(cDevice,"hw_ver",dev_attr_hw_ver,0);
  rb_define_method(cDevice,"max_qp",dev_attr_max_qp,0);
  rb_define_method(cDevice,"max_qp_wr",dev_attr_max_qp_wr,0);
  rb_define_method(cDevice,"device_cap_flags",dev_attr_device_cap_flags,0);
  rb_define_method(cDevice,"max_sge",dev_attr_max_sge,0);
  rb_define_method(cDevice,"max_sge_rd",dev_attr_max_sge_rd,0);
  rb_define_method(cDevice,"max_cq",dev_attr_max_cq,0);
  rb_define_method(cDevice,"max_cqe",dev_attr_max_cqe,0);
  rb_define_method(cDevice,"max_mr",dev_attr_max_mr,0);
  rb_define_method(cDevice,"max_pd",dev_attr_max_pd,0);
  rb_define_method(cDevice,"max_qp_rd_atom",dev_attr_max_qp_rd_atom,0);
  rb_define_method(cDevice,"max_ee_rd_atom",dev_attr_max_ee_rd_atom,0);
  rb_define_method(cDevice,"max_res_rd_atom",dev_attr_max_res_rd_atom,0);
  rb_define_method(cDevice,"max_qp_init_rd_atom",dev_attr_max_qp_init_rd_atom,0);
  rb_define_method(cDevice,"max_ee_init_rd_atom",dev_attr_max_ee_init_rd_atom,0);
  rb_define_method(cDevice,"atomic_cap",dev_attr_atomic_cap,0);
  rb_define_method(cDevice,"max_ee",dev_attr_max_ee,0);
  rb_define_method(cDevice,"max_rdd",dev_attr_max_rdd,0);
  rb_define_method(cDevice,"max_mw",dev_attr_max_mw,0);
  rb_define_method(cDevice,"max_raw_ipv6_qp",dev_attr_max_raw_ipv6_qp,0);
  rb_define_method(cDevice,"max_raw_ethy_qp",dev_attr_max_raw_ethy_qp,0);
  rb_define_method(cDevice,"max_mcast_grp",dev_attr_max_mcast_grp,0);
  rb_define_method(cDevice,"max_mcast_qp_attach",dev_attr_max_mcast_qp_attach,0);
  rb_define_method(cDevice,"max_total_mcast_qp_attach",dev_attr_max_total_mcast_qp_attach,0);
  rb_define_method(cDevice,"max_ah",dev_attr_max_ah,0);
  rb_define_method(cDevice,"max_fmr",dev_attr_max_fmr,0);
  rb_define_method(cDevice,"max_map_per_fmr",dev_attr_max_map_per_fmr,0);
  rb_define_method(cDevice,"max_srq",dev_attr_max_srq,0);
  rb_define_method(cDevice,"max_srq_wr",dev_attr_max_srq_wr,0);
  rb_define_method(cDevice,"max_srq_sge",dev_attr_max_srq_sge,0);
  rb_define_method(cDevice,"max_pkeys",dev_attr_max_pkeys,0);
  rb_define_method(cDevice,"local_ca_ack_delay",dev_attr_local_ca_ack_delay,0);
  rb_define_method(cDevice,"phys_port_cnt",dev_attr_phys_port_cnt,0);


}
