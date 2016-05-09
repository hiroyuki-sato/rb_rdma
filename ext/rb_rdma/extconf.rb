require 'mkmf'
create_makefile('rb_rdma')
have_library('ibverbs')
have_library('rdmacm')
