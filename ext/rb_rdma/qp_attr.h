
#if !defined(_RDMA_QP_ATTR_H_)
# define _RDMA_PD_H_

extern void Init_qp_attr();
extern VALUE cQPAttr;
extern const rb_data_type_t rdma_qp_attr_type;

#define GET_QP_ATTR_DATA(klass,qp_attr) \
  TypedData_Get_Struct(klass,struct ibv_qp_attr,&rdma_qp_attr_type,qp_attr);

#endif


