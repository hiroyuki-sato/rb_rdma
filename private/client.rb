require 'rb_rdma'
require 'pp'
require 'socket'

buf = RbRDMA::MyBuffer.new
ctx = RbRDMA::Context.open("pib_1")
c_channel = RbRDMA::CompChannel.new(ctx)
pd = RbRDMA::PD.new(ctx)
mr = RbRDMA::MR.new(pd,buf,RbRDMA::ACCESS_LOCAL_WRITE) 

cq = RbRDMA::CQ.new(ctx,1,nil,c_channel,0)
qp = RbRDMA::QP.new(pd,RbRDMA::QPT_RC,cq,cq,"d")
puts qp.qp_num

puts qp.qp_type
puts RbRDMA::QPT_RC

srand($$ * Time.new.to_i)
rand =  rand(0xFFFFFF)

#sock = TCPSocket.open('localhost',11885)
sock = TCPSocket.open('127.0.0.1',11885)

msg = sprintf("rand = %d, lid = %d, qpn = %d\n",rand,ctx.port(1).lid,qp.qp_num)
sock.write(msg);
msg2 = sock.gets
puts msg2
sock.write("done")
sock.close
