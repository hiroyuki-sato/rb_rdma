#if !defined(_RDMA_CQ_H_)
# define _RDMA_CQ_H_

struct rb_rdma_data_cq {
  VALUE context;
  VALUE comp_channel;
  struct ibv_cq *cq;
};

extern void Init_cq();
extern VALUE cCQ;

extern const rb_data_type_t rdma_cq_type;
#endif

