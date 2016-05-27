#if !defined(_RDMA_MR_H_)
# define _RDMA_MR_H_

extern void Init_mr();
extern VALUE cMR;
extern const rb_data_type_t rdma_mr_type;

struct rb_rdma_data_mr {
  VALUE pd;
  VALUE buf;
  struct ibv_mr *mr;
};

#define GET_MR_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_mr, &rdma_mr_type,data);

#endif
