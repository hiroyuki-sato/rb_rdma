require 'mkmf'
#have_library('ibverbs','ibv_get_device_list')
have_library('ibverbs')
#have_library('rdmacm')
#append_library('ibverbs')
#$libs = append_library($libs, "ibverbs")
#find_library('ibverbs','ibv_get_device_list')
#$LDFLAGS << ' -libverbs'
create_makefile('rb_rdma')
