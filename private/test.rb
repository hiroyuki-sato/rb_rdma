require 'rb_rdma'
require 'pp'
ctx = RbRDMA::Context.open("pib_0")
dev = RbRDMA::Device.new(ctx)
c_channel = RbRDMA::CompChannel.new(ctx)
pd = RbRDMA::PD.new(ctx)
mr = RbRDMA::MR.new(pd,"",12345,RbRDMA::ACCESS_LOCAL_WRITE) 
pp mr

cq = RbRDMA::CQ.new(ctx,1,nil,c_channel,0)
qp = RbRDMA::QP.new(RbRDMA::QPT_RC,cq,cq,"d")
pp qp
