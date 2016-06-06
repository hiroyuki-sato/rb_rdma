#if !defined(_RDMA_DEVICE_H_)
# define _RDMA_DEVICE_H_

extern void Init_device();
extern VALUE cDevice;

struct rb_rdma_data_device {
  VALUE context;
  struct ibv_device_attr *attr;
};


#define GET_DEVICE_DATA(klass,data_pd) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_pd,&rdma_pd_type,data_pd);

extern const rb_data_type_t rdma_pd_type;
#endif

