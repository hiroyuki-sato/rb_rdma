#if !defined(_RDMA_Comp_Channel_H_)
# define _RDMA_Comp_Channel_H_

struct rb_rdma_data_comp_channel {
  VALUE context;
  struct ibv_comp_channel *comp_channel;
};

#define GET_Comp_Channel_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_comp_channel, \
                       &rdma_comp_channel_type,data);

extern void Init_comp_channel();
extern VALUE cCompChannel;

extern const rb_data_type_t rdma_comp_channel_type;
#endif

