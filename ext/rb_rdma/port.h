#if !defined(_RDMA_PORT_H_)
# define _RDMA_PORT_H_

extern void Init_port();
extern VALUE cPort;

struct rb_rdma_data_port {
  VALUE context;
  VALUE port_num;
  struct ibv_port_attr *port_attr;
};

#define GET_PORT_DATA(klass,data_port) \
  TypedData_Get_Struct(klass,struct rb_rdma_data_port,&rdma_port_type,data_port);

extern const rb_data_type_t rdma_port_type;
#endif

