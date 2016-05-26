extern void Init_mr();

#define GET_MR_DATA(klass,data) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_mr, &rdma_mr_type,data);

