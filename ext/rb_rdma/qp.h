#if !defined(_RDMA_QP_H_)
# define _RDMA_QP_H_

extern void Init_qp();
extern VALUE cQP;

#define GET_QP_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_qp, &rdma_qp_type,data);

extern const rb_data_type_t rdma_qp_type;
#endif

