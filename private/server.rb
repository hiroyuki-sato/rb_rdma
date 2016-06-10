require 'rb_rdma'
require 'pp'
require 'socket'

buf = RbRDMA::MyBuffer.new
ctx = RbRDMA::Context.open("pib_0")
port = ctx.port(1)
c_channel = RbRDMA::CompChannel.new(ctx)
pd = RbRDMA::PD.new(ctx)
mr = RbRDMA::MR.new(pd,buf,RbRDMA::ACCESS_LOCAL_WRITE) 

cq = RbRDMA::CQ.new(ctx,1,nil,c_channel,0)
qp = RbRDMA::QP.new(pd,RbRDMA::QPT_RC,cq,cq,"d")
puts qp.qp_num

puts qp.qp_type
puts RbRDMA::QPT_RC

srand($$ * Time.new.to_i)
puts rand(0xFFFFFF)

gate = TCPServer.open(11885)

sock = gate.accept
gate.close

msg2 = sprintf("rand = %d, lid = %d, qpn: %d\n",rand,ctx.port(1).lid,qp.qp_num)
rtr_mask = RbRDMA::QP_STATE           |
           RbRDMA::QP_AV              |
           RbRDMA::QP_PATH_MTU        |
           RbRDMA::QP_DEST_QPN        |
           RbRDMA::QP_RQ_PSN          |
           RbRDMA::QP_MAX_QP_RD_ATOMIC|
           RbRDMA::QP_MIN_RNR_TIMER   

while msg = sock.gets
  break if msg =~ /done/
  msg.chomp!
  puts msg
  d = Hash[msg.split(/\s*,\s*/).map{ |s| s.split(/\s*=\s*/)}]
  puts msg2
  
  sock.write(msg2)
end

pp d["qpn"]
pp d["rand"]
pp d["lid"]
qp_attr = RbRDMA::QPAttr.rtr_qp_attr(port.active_mtu,d["qpn"].to_i,d["rand"].to_i,d["lid"].to_i,0,1);
qp.modify_qp(qp_attr,rtr_mask)
sock.close
