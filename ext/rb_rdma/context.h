
#if !defined(_RDMA_CONTEXT_H_)
# define _RDMA_CONTEXT_H_

extern void Init_context();
extern VALUE cContext;

struct rdma_context {
  struct ibv_device **devices;
  struct ibv_device *device;
  struct ibv_context *context;

};

#define GET_Context_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rdma_context, \
                       &rdma_context_type,data);

extern const rb_data_type_t rdma_context_type;
#endif
