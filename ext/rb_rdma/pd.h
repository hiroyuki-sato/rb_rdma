
#if !defined(_RDMA_PD_H_)
# define _RDMA_PD_H_

extern void Init_pd();
extern VALUE cPD;

struct rb_rdma_data_pd {
  VALUE context;
  struct ibv_pd *pd;
};

#define GET_PD_DATA(klass,data_pd) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_pd,&rdma_pd_type,data_pd);

extern const rb_data_type_t rdma_pd_type;
#endif


