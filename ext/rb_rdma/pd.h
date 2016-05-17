
#if !defined(_RDMA_PD_H_)
# define _RDMA_PD_H_

extern void Init_pd();
extern VALUE cPD;

struct rb_rdma_data_pd {
  VALUE context;
  struct ibv_pd *pd;
};

extern const rb_data_type_t rdma_pd_type;
#endif


