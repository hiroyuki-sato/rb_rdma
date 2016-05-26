#if !defined(_RDMA_CQ_H_)
# define _RDMA_CQ_H_

struct rb_rdma_data_cq {
  VALUE context;
  VALUE comp_channel;
  struct ibv_cq *cq;
};

#define GET_CQ_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_cq, &rdma_cq_type,data);

extern void Init_cq();
extern VALUE cCQ;

extern const rb_data_type_t rdma_cq_type;
#endif

