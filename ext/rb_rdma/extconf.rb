require 'mkmf'
if ENV.has_key?('DEBUG')
puts '** debug **'
  $CFLAGS << " -DDEBUG=1 "
end
have_library('ibverbs')
#have_library('rdmacm')
create_makefile('rb_rdma')
