#GC.disable
require 'rb_rdma'
require 'pp'

#rdma_s_rtr_qp_attr(VALUE klass,
#                   VALUE rb_mtu,
#                   VALUE rb_dqpn,
#                   VALUE rb_dpsn,
#                   VALUE rb_dlid,
#                   VALUE rb_sl,
#                   VALUE rb_port){
attr = RbRDMA::QPAttr.rtr_qp_attr(RbRDMA::MTU_1024,1234,1234,1234,1,1);
pp attr

buf = RbRDMA::MyBuffer.new
ctx = RbRDMA::Context.open("pib_0")
dev = ctx.device
pp dev.fw_ver
#port = RbRDMA::Port.new(ctx,1)
port = ctx.port(1)
pp port.port_num
pp port.lid
#dev = RbRDMA::Device.new("test")
#dev = RbRDMA::Device.new(ctx)
#pp dev.fw_ver
#pp dev.max_qp

c_channel = RbRDMA::CompChannel.new(ctx)
pd = RbRDMA::PD.new(ctx)
mr = RbRDMA::MR.new(pd,buf,RbRDMA::ACCESS_LOCAL_WRITE) 
pp mr
pp mr.lkey
pp mr.rkey

cq = RbRDMA::CQ.new(ctx,1,nil,c_channel,0)
pp cq
qp = RbRDMA::QP.new(pd,RbRDMA::QPT_RC,cq,cq,"d")
qp.post_recv(mr,12345);
=begin
puts qp.qp_state
puts '*******'
#pp qp
=end
