#include "rb_rdma.h"
#include "context.h"
#include "device.h"
#include "comp_channel.h"
#include "pd.h"

VALUE mRbRDMA = Qnil;

void Init_rb_rdma(){

  mRbRDMA = rb_define_module("RbRDMA");

    //
    // enum ibv_node_type
    rb_const_set(mRbRDMA, rb_intern("NODE_UNKNOWN"), INT2FIX(IBV_NODE_UNKNOWN)); 
    rb_const_set(mRbRDMA, rb_intern("NODE_CA"), INT2FIX(IBV_NODE_CA)); 
    rb_const_set(mRbRDMA, rb_intern("NODE_SWITCH"), INT2FIX(IBV_NODE_SWITCH)); 
    rb_const_set(mRbRDMA, rb_intern("NODE_ROUTER"), INT2FIX(IBV_NODE_ROUTER)); 
    rb_const_set(mRbRDMA, rb_intern("NODE_RNIC"), INT2FIX(IBV_NODE_RNIC)); 

    // general 
    rb_const_set(mRbRDMA, rb_intern("MAX_PORTS"), INT2FIX(254));
    rb_const_set(mRbRDMA, rb_intern("INVALID_PORT"), INT2FIX(255));
    rb_const_set(mRbRDMA, rb_intern("MAX_PKEYS"), INT2FIX(65536));
    rb_const_set(mRbRDMA, rb_intern("MAX_GUIDS"), INT2FIX(256));
    rb_const_set(mRbRDMA, rb_intern("MAX_PKT_WORDS"), INT2FIX(4222/4));
    
    // LID
    rb_const_set(mRbRDMA, rb_intern("LID_RESERVED"), INT2FIX(0));
    rb_const_set(mRbRDMA, rb_intern("LID_MULTICAST"), INT2FIX(0xC000));
    rb_const_set(mRbRDMA, rb_intern("LID_PERMISSIVE"), INT2FIX(0xFFFF));
    rb_const_set(mRbRDMA, rb_intern("LID_COUNT_UNICAST"), INT2FIX(0xC000));
    rb_const_set(mRbRDMA, rb_intern("LID_COUNT_MULTICAST"), INT2FIX((0xFFFE - 0xC000) + 1));
    
    // Partition Key Constants
    rb_const_set(mRbRDMA, rb_intern("PKEY_DEFAULT"), INT2FIX(0xFFFF));
    rb_const_set(mRbRDMA, rb_intern("PKEY_PARTIAL_DEFAULT"), INT2FIX(0x7FFF));
    rb_const_set(mRbRDMA, rb_intern("PKEY_MEMBERSHIP_BIT"), INT2FIX(1<<15));
    rb_const_set(mRbRDMA, rb_intern("PKEY_INVALID"), INT2FIX(0));

    // Well known QKEY Constants
    rb_const_set(mRbRDMA, rb_intern("IB_DEFAULT_QP0_QKEY"), INT2FIX(0));
    rb_const_set(mRbRDMA, rb_intern("IB_DEFAULT_QP1_QKEY"), INT2FIX(0x80010000));
    
    // LRH LNH Header bits
    rb_const_set(mRbRDMA, rb_intern("LNH_GRH"), INT2FIX(1<<0));
    rb_const_set(mRbRDMA, rb_intern("LNH_mRbRDMA"), INT2FIX(1<<1));

    // GID Constants
//    rb_const_set(mRbRDMA, rb_intern("GID_DEFAULT_PREFIX"), INT2NUM(0xFE80 << 48));
    rb_const_set(mRbRDMA, rb_intern("IB_DEFAULT_SUBN_PREFIX"), INT2NUM(IB_DEFAULT_SUBN_PREFIX));

    // enum ibv_mtu
    rb_const_set(mRbRDMA, rb_intern("MTU_256"), INT2FIX(IBV_MTU_256)); 
    rb_const_set(mRbRDMA, rb_intern("MTU_512"), INT2FIX(IBV_MTU_512)); 
    rb_const_set(mRbRDMA, rb_intern("MTU_1024"), INT2FIX(IBV_MTU_1024)); 
    rb_const_set(mRbRDMA, rb_intern("MTU_2048"), INT2FIX(IBV_MTU_2048)); 
    rb_const_set(mRbRDMA, rb_intern("MTU_4096"), INT2FIX(IBV_MTU_4096)); 

    // PortInfo Link Width Constants
    rb_const_set(mRbRDMA, rb_intern("LINK_WIDTH_1x"),  INT2FIX(1<<0));
    rb_const_set(mRbRDMA, rb_intern("LINK_WIDTH_4x"),  INT2FIX(1<<1));
    rb_const_set(mRbRDMA, rb_intern("LINK_WIDTH_8x"),  INT2FIX(1<<2));
    rb_const_set(mRbRDMA, rb_intern("LINK_WIDTH_12x"), INT2FIX(1<<3));

#if 0
    // PortInfo Link Speed Constants
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_MAX"),   INT2FIX(0));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_2Gb5"),  INT2FIX(2));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_5Gb0"),  INT2FIX(5));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_10Gb0"), INT2FIX(3));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_20Gb0"), INT2FIX(6));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_30Gb0"), INT2FIX(4));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_40Gb0"), INT2FIX(7));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_60Gb0"), INT2FIX(8));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_80Gb0"), INT2FIX(9));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_120Gb0"),INT2FIX(10));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_14Gb0"), INT2FIX(11));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_56Gb0"), INT2FIX(12));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_112Gb0"),INT2FIX(13));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_168Gb0"),INT2FIX(14));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_25Gb0"), INT2FIX(15));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_100Gb0"),INT2FIX(16));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_200Gb0"),INT2FIX(17));
    rb_const_set(mRbRDMA, rb_intern("LINK_SPEED_300Gb0"),INT2FIX(18));
#endif

    // enum ibv_rate
    // PathRecord??
    rb_const_set(mRbRDMA, rb_intern("RATE_MAX"), INT2FIX(IBV_RATE_MAX)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_2_5_GBPS"), INT2FIX(IBV_RATE_2_5_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_5_GBPS"), INT2FIX(IBV_RATE_5_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_10_GBPS"), INT2FIX(IBV_RATE_10_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_20_GBPS"), INT2FIX(IBV_RATE_20_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_30_GBPS"), INT2FIX(IBV_RATE_30_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_40_GBPS"), INT2FIX(IBV_RATE_40_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_60_GBPS"), INT2FIX(IBV_RATE_60_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_80_GBPS"), INT2FIX(IBV_RATE_80_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_120_GBPS"), INT2FIX(IBV_RATE_120_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_14_GBPS"), INT2FIX(IBV_RATE_14_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_56_GBPS"), INT2FIX(IBV_RATE_56_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_112_GBPS"), INT2FIX(IBV_RATE_112_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_168_GBPS"), INT2FIX(IBV_RATE_168_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_25_GBPS"), INT2FIX(IBV_RATE_25_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_100_GBPS"), INT2FIX(IBV_RATE_100_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_200_GBPS"), INT2FIX(IBV_RATE_200_GBPS)); 
    rb_const_set(mRbRDMA, rb_intern("RATE_300_GBPS"), INT2FIX(IBV_RATE_300_GBPS)); 

    // enum ibv_port_state
    rb_const_set(mRbRDMA, rb_intern("PORT_NOP"), INT2FIX(IBV_PORT_NOP)); 
    rb_const_set(mRbRDMA, rb_intern("PORT_DOWN"), INT2FIX(IBV_PORT_DOWN)); 
    rb_const_set(mRbRDMA, rb_intern("PORT_INIT"), INT2FIX(IBV_PORT_INIT)); 
    rb_const_set(mRbRDMA, rb_intern("PORT_ARMED"), INT2FIX(IBV_PORT_ARMED)); 
    rb_const_set(mRbRDMA, rb_intern("PORT_ACTIVE"), INT2FIX(IBV_PORT_ACTIVE)); 
    rb_const_set(mRbRDMA, rb_intern("PORT_ACTIVE_DEFER"), INT2FIX(IBV_PORT_ACTIVE_DEFER)); 

    // PortInfo Physical Port State Constants
    // PHYS_PORT_STATE_SLEEP = 1;
    // PHYS_PORT_STATE_POLLING = 2;
    // PHYS_PORT_STATE_DISABLED = 3;
    // PHYS_PORT_STATE_CFG_TRAIN = 4;
    // PHYS_PORT_STATE_LINK_UP = 5;
    // PHYS_PORT_STATE_LINK_ERR_RECOVERY = 6;
    // PHYS_PORT_STATE_PHY_TEST = 7;

    // MAD_METHODS
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET"), INT2FIX(IB_MAD_METHOD_GET));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_SET"), INT2FIX(IB_MAD_METHOD_SET));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET_RESPONSE"), INT2FIX(IB_MAD_METHOD_GET_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_SEND"), INT2FIX(IB_MAD_METHOD_SEND));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_TRAP"), INT2FIX(IB_MAD_METHOD_TRAP));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_TRAP_REPRESS"), INT2FIX(IB_MAD_METHOD_TRAP_REPRESS));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_REPORT"), INT2FIX(IB_MAD_METHOD_REPORT));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_REPORT_RESPONSE"), INT2FIX(IB_MAD_METHOD_REPORT_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET_TABLE"), INT2FIX(IB_MAD_METHOD_GET_TABLE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET_TABLE_RESPONSE"), INT2FIX(IB_MAD_METHOD_GET_TABLE_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET_TRACE_TABLE"), INT2FIX(IB_MAD_METHOD_GET_TRACE_TABLE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GET_TRACE_TABLE_RESPONSE"), INT2FIX(IB_MAD_METHOD_GET_TRACE_TABLE_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GETMULTI"), INT2FIX(IB_MAD_METHOD_GETMULTI));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_GETMULTI_RESPONSE"), INT2FIX(IB_MAD_METHOD_GETMULTI_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_DELETE"), INT2FIX(IB_MAD_METHOD_DELETE));
    rb_const_set(mRbRDMA, rb_intern("MAD_METHOD_DELETE_RESPONSE"), INT2FIX(IB_MAD_METHOD_DELETE_RESPONSE));
    rb_const_set(mRbRDMA, rb_intern("MAD_RESPONSE"), INT2FIX(IB_MAD_RESPONSE));

    // 
    rb_const_set(mRbRDMA, rb_intern("MAD_BASE_VERSION"), INT2FIX(UMAD_BASE_VERSION));

    // TODO
    // rb_const_set(mRbRDMA, rb_intern("MAD_NOTICE_FATAL"), INT2FIX(0));
    // MAD_NOTICE_FATAL = 0;
    // MAD_NOTICE_URGENT = 1;
    // MAD_NOTICE_SECURITY = 2;
    // MAD_NOTICE_SM = 3;
    // MAD_NOTICE_INFO = 4;

    // MAD_STATUS
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_OK"), INT2FIX(IB_MAD_STS_OK));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_BUSY"), INT2FIX(IB_MAD_STS_BUSY));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_REDIRECT"), INT2FIX(IB_MAD_STS_REDIRECT));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_BAD_BASE_VER_OR_CLASS"), INT2FIX(IB_MAD_STS_BAD_BASE_VER_OR_CLASS));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_METHOD_NOT_SUPPORTED"), INT2FIX(IB_MAD_STS_METHOD_NOT_SUPPORTED));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_METHOD_ATTR_NOT_SUPPORTED"), INT2FIX(IB_MAD_STS_METHOD_ATTR_NOT_SUPPORTED));
    rb_const_set(mRbRDMA, rb_intern("MAD_STS_INV_ATTR_VALUE"), INT2FIX(IB_MAD_STS_INV_ATTR_VALUE));

    // SUBNET_ADMIN Class Specific Status Codes
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_SUCCESS"), INT2FIX(UMAD_SA_STATUS_SUCCESS));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_NO_RESOURCES"), INT2FIX(UMAD_SA_STATUS_NO_RESOURCES));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_REQ_INVALID"), INT2FIX(UMAD_SA_STATUS_REQ_INVALID));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_NO_RECORDS"), INT2FIX(UMAD_SA_STATUS_NO_RECORDS));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_TOO_MANY_RECORDS"), INT2FIX(UMAD_SA_STATUS_TOO_MANY_RECORDS));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_INVALID_GID"), INT2FIX(UMAD_SA_STATUS_INVALID_GID));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_INSUF_COMPS"), INT2FIX(UMAD_SA_STATUS_INSUF_COMPS));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_REQ_DENIED"), INT2FIX(UMAD_SA_STATUS_REQ_DENIED));
    rb_const_set(mRbRDMA, rb_intern("UMAD_SA_STATUS_PRI_SUGGESTED"), INT2FIX(UMAD_SA_STATUS_PRI_SUGGESTED));

    // TODO
    // MAD_STATUS_CLASS_SHIFT = 8;
    // MAD_STATUS_CLASS_MASK = 0x7F;
    rb_const_set(mRbRDMA, rb_intern("UMAD_STATUS_CLASS_MASK"), INT2FIX(UMAD_STATUS_CLASS_MASK));

    // MAD_CLASSES
    rb_const_set(mRbRDMA, rb_intern("IB_SMI_CLASS"), INT2FIX(IB_SMI_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_SMI_DIRECT_CLASS"), INT2FIX(IB_SMI_DIRECT_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_SA_CLASS"), INT2FIX(IB_SA_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_PERFORMANCE_CLASS"), INT2FIX(IB_PERFORMANCE_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_BOARD_MGMT_CLASS"), INT2FIX(IB_BOARD_MGMT_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_DEVICE_MGMT_CLASS"), INT2FIX(IB_DEVICE_MGMT_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_CM_CLASS"), INT2FIX(IB_CM_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_SNMP_CLASS"), INT2FIX(IB_SNMP_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_VENDOR_RANGE1_START_CLASS"), INT2FIX(IB_VENDOR_RANGE1_START_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_VENDOR_RANGE1_END_CLASS"), INT2FIX(IB_VENDOR_RANGE1_END_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_CC_CLASS"), INT2FIX(IB_CC_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_VENDOR_RANGE2_START_CLASS"), INT2FIX(IB_VENDOR_RANGE2_START_CLASS));
    rb_const_set(mRbRDMA, rb_intern("IB_VENDOR_RANGE2_END_CLASS"), INT2FIX(IB_VENDOR_RANGE2_END_CLASS));

    // RMPP_FLAGS_ENUM
    rb_const_set(mRbRDMA, rb_intern("RMPP_FLAG_ACTIVE"), INT2FIX(IB_RMPP_FLAG_ACTIVE));
    rb_const_set(mRbRDMA, rb_intern("RMPP_FLAG_FIRST"), INT2FIX(IB_RMPP_FLAG_FIRST));
    rb_const_set(mRbRDMA, rb_intern("RMPP_FLAG_LAST"), INT2FIX(IB_RMPP_FLAG_LAST));


#if 0
    // enum ibv_transport_type
    rb_const_set(mRbRDMA, rb_intern("TRANSPORT_UNKNOWN"), INT2FIX(IBV_TRANSPORT_UNKNOWN)); 
    rb_const_set(mRbRDMA, rb_intern("TRANSPORT_IB"), INT2FIX(IBV_TRANSPORT_IB)); 
    rb_const_set(mRbRDMA, rb_intern("TRANSPORT_IWARP"), INT2FIX(IBV_TRANSPORT_IWARP)); 

    // enum ibv_device_cap_flags
    rb_const_set(mRbRDMA, rb_intern("DEVICE_RESIZE_MAX_WR"), INT2FIX(IBV_DEVICE_RESIZE_MAX_WR)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_BAD_PKEY_CNTR"), INT2FIX(IBV_DEVICE_BAD_PKEY_CNTR)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_BAD_QKEY_CNTR"), INT2FIX(IBV_DEVICE_BAD_QKEY_CNTR)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_RAW_MULTI"), INT2FIX(IBV_DEVICE_RAW_MULTI)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_AUTO_PATH_MIG"), INT2FIX(IBV_DEVICE_AUTO_PATH_MIG)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_CHANGE_PHY_PORT"), INT2FIX(IBV_DEVICE_CHANGE_PHY_PORT)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_UD_AV_PORT_ENFORCE"), INT2FIX(IBV_DEVICE_UD_AV_PORT_ENFORCE)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_CURR_QP_STATE_MOD"), INT2FIX(IBV_DEVICE_CURR_QP_STATE_MOD)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_SHUTDOWN_PORT"), INT2FIX(IBV_DEVICE_SHUTDOWN_PORT)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_INIT_TYPE"), INT2FIX(IBV_DEVICE_INIT_TYPE)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_PORT_ACTIVE_EVENT"), INT2FIX(IBV_DEVICE_PORT_ACTIVE_EVENT)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_SYS_IMAGE_GUID"), INT2FIX(IBV_DEVICE_SYS_IMAGE_GUID)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_RC_RNR_NAK_GEN"), INT2FIX(IBV_DEVICE_RC_RNR_NAK_GEN)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_SRQ_RESIZE"), INT2FIX(IBV_DEVICE_SRQ_RESIZE)); 
    rb_const_set(mRbRDMA, rb_intern("DEVICE_N_NOTIFY_CQ"), INT2FIX(IBV_DEVICE_N_NOTIFY_CQ)); 

    // enum ibv_atomic_cap
    rb_const_set(mRbRDMA, rb_intern("ATOMIC_NONE"), INT2FIX(IBV_ATOMIC_NONE)); 
    rb_const_set(mRbRDMA, rb_intern("ATOMIC_HCA"), INT2FIX(IBV_ATOMIC_HCA)); 
    rb_const_set(mRbRDMA, rb_intern("ATOMIC_GLOB"), INT2FIX(IBV_ATOMIC_GLOB)); 

    // enum {
    rb_const_set(mRbRDMA, rb_intern("LINK_LAYER_UNSPECIFIED"), INT2FIX(IBV_LINK_LAYER_UNSPECIFIED)); 
    rb_const_set(mRbRDMA, rb_intern("LINK_LAYER_INFINmRbRDMAND"), INT2FIX(IBV_LINK_LAYER_INFINmRbRDMAND)); 
    rb_const_set(mRbRDMA, rb_intern("LINK_LAYER_ETHERNET"), INT2FIX(IBV_LINK_LAYER_ETHERNET)); 

    // enum ibv_event_type
    rb_const_set(mRbRDMA, rb_intern("EVENT_CQ_ERR"), INT2FIX(IBV_EVENT_CQ_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_QP_FATAL"), INT2FIX(IBV_EVENT_QP_FATAL)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_QP_REQ_ERR"), INT2FIX(IBV_EVENT_QP_REQ_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_QP_ACCESS_ERR"), INT2FIX(IBV_EVENT_QP_ACCESS_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_COMM_EST"), INT2FIX(IBV_EVENT_COMM_EST)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_SQ_DRAINED"), INT2FIX(IBV_EVENT_SQ_DRAINED)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_PATH_MIG"), INT2FIX(IBV_EVENT_PATH_MIG)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_PATH_MIG_ERR"), INT2FIX(IBV_EVENT_PATH_MIG_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_DEVICE_FATAL"), INT2FIX(IBV_EVENT_DEVICE_FATAL)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_PORT_ACTIVE"), INT2FIX(IBV_EVENT_PORT_ACTIVE)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_PORT_ERR"), INT2FIX(IBV_EVENT_PORT_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_LID_CHANGE"), INT2FIX(IBV_EVENT_LID_CHANGE)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_PKEY_CHANGE"), INT2FIX(IBV_EVENT_PKEY_CHANGE)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_SM_CHANGE"), INT2FIX(IBV_EVENT_SM_CHANGE)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_SRQ_ERR"), INT2FIX(IBV_EVENT_SRQ_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_SRQ_LIMIT_REACHED"), INT2FIX(IBV_EVENT_SRQ_LIMIT_REACHED)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_QP_LAST_WQE_REACHED"), INT2FIX(IBV_EVENT_QP_LAST_WQE_REACHED)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_CLIENT_REREGISTER"), INT2FIX(IBV_EVENT_CLIENT_REREGISTER)); 
    rb_const_set(mRbRDMA, rb_intern("EVENT_GID_CHANGE"), INT2FIX(IBV_EVENT_GID_CHANGE)); 

    // enum ibv_wc_status
    rb_const_set(mRbRDMA, rb_intern("WC_SUCCESS"), INT2FIX(IBV_WC_SUCCESS)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_LEN_ERR"), INT2FIX(IBV_WC_LOC_LEN_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_QP_OP_ERR"), INT2FIX(IBV_WC_LOC_QP_OP_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_EEC_OP_ERR"), INT2FIX(IBV_WC_LOC_EEC_OP_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_PROT_ERR"), INT2FIX(IBV_WC_LOC_PROT_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_WR_FLUSH_ERR"), INT2FIX(IBV_WC_WR_FLUSH_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_MW_BIND_ERR"), INT2FIX(IBV_WC_MW_BIND_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_BAD_RESP_ERR"), INT2FIX(IBV_WC_BAD_RESP_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_ACCESS_ERR"), INT2FIX(IBV_WC_LOC_ACCESS_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_REM_INV_REQ_ERR"), INT2FIX(IBV_WC_REM_INV_REQ_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_REM_ACCESS_ERR"), INT2FIX(IBV_WC_REM_ACCESS_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_REM_OP_ERR"), INT2FIX(IBV_WC_REM_OP_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RETRY_EXC_ERR"), INT2FIX(IBV_WC_RETRY_EXC_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RNR_RETRY_EXC_ERR"), INT2FIX(IBV_WC_RNR_RETRY_EXC_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_LOC_RDD_VIOL_ERR"), INT2FIX(IBV_WC_LOC_RDD_VIOL_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_REM_INV_RD_REQ_ERR"), INT2FIX(IBV_WC_REM_INV_RD_REQ_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_REM_ABORT_ERR"), INT2FIX(IBV_WC_REM_ABORT_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_INV_EECN_ERR"), INT2FIX(IBV_WC_INV_EECN_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_INV_EEC_STATE_ERR"), INT2FIX(IBV_WC_INV_EEC_STATE_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_FATAL_ERR"), INT2FIX(IBV_WC_FATAL_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RESP_TIMEOUT_ERR"), INT2FIX(IBV_WC_RESP_TIMEOUT_ERR)); 
    rb_const_set(mRbRDMA, rb_intern("WC_GENERAL_ERR"), INT2FIX(IBV_WC_GENERAL_ERR)); 

    // enum ibv_wc_opcode
    rb_const_set(mRbRDMA, rb_intern("WC_SEND"), INT2FIX(IBV_WC_SEND)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RDMA_WRITE"), INT2FIX(IBV_WC_RDMA_WRITE)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RDMA_READ"), INT2FIX(IBV_WC_RDMA_READ)); 
    rb_const_set(mRbRDMA, rb_intern("WC_COMP_SWAP"), INT2FIX(IBV_WC_COMP_SWAP)); 
    rb_const_set(mRbRDMA, rb_intern("WC_FETCH_ADD"), INT2FIX(IBV_WC_FETCH_ADD)); 
    rb_const_set(mRbRDMA, rb_intern("WC_BIND_MW"), INT2FIX(IBV_WC_BIND_MW)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RECV"), INT2FIX(IBV_WC_RECV)); 
    rb_const_set(mRbRDMA, rb_intern("WC_RECV_RDMA_WITH_IMM"), INT2FIX(IBV_WC_RECV_RDMA_WITH_IMM)); 

    // enum ibv_wc_flags
    rb_const_set(mRbRDMA, rb_intern("WC_GRH"), INT2FIX(IBV_WC_GRH)); 
    rb_const_set(mRbRDMA, rb_intern("WC_WITH_IMM"), INT2FIX(IBV_WC_WITH_IMM)); 

    // enum ibv_access_flags
    rb_const_set(mRbRDMA, rb_intern("ACCESS_LOCAL_WRITE"), INT2FIX(IBV_ACCESS_LOCAL_WRITE)); 
    rb_const_set(mRbRDMA, rb_intern("ACCESS_REMOTE_WRITE"), INT2FIX(IBV_ACCESS_REMOTE_WRITE)); 
    rb_const_set(mRbRDMA, rb_intern("ACCESS_REMOTE_READ"), INT2FIX(IBV_ACCESS_REMOTE_READ)); 
    rb_const_set(mRbRDMA, rb_intern("ACCESS_REMOTE_ATOMIC"), INT2FIX(IBV_ACCESS_REMOTE_ATOMIC)); 
    rb_const_set(mRbRDMA, rb_intern("ACCESS_MW_BIND"), INT2FIX(IBV_ACCESS_MW_BIND)); 

    // enum ibv_rereg_mr_flags
    rb_const_set(mRbRDMA, rb_intern("REREG_MR_CHANGE_TRANSLATION"), INT2FIX(IBV_REREG_MR_CHANGE_TRANSLATION)); 
    rb_const_set(mRbRDMA, rb_intern("REREG_MR_CHANGE_PD"), INT2FIX(IBV_REREG_MR_CHANGE_PD)); 
    rb_const_set(mRbRDMA, rb_intern("REREG_MR_CHANGE_ACCESS"), INT2FIX(IBV_REREG_MR_CHANGE_ACCESS)); 
    rb_const_set(mRbRDMA, rb_intern("REREG_MR_KEEP_VALID"), INT2FIX(IBV_REREG_MR_KEEP_VALID)); 

    // enum ibv_mw_type
    rb_const_set(mRbRDMA, rb_intern("MW_TYPE_1"), INT2FIX(IBV_MW_TYPE_1)); 
    rb_const_set(mRbRDMA, rb_intern("MW_TYPE_2"), INT2FIX(IBV_MW_TYPE_2)); 


    // enum ibv_srq_attr_mask
    rb_const_set(mRbRDMA, rb_intern("SRQ_MAX_WR"), INT2FIX(IBV_SRQ_MAX_WR)); 
    rb_const_set(mRbRDMA, rb_intern("SRQ_LIMIT"), INT2FIX(IBV_SRQ_LIMIT)); 

    // enum ibv_qp_type
    rb_const_set(mRbRDMA, rb_intern("QPT_RC"), INT2FIX(IBV_QPT_RC)); 
    rb_const_set(mRbRDMA, rb_intern("QPT_UC"), INT2FIX(IBV_QPT_UC)); 
    rb_const_set(mRbRDMA, rb_intern("QPT_UD"), INT2FIX(IBV_QPT_UD)); 
    rb_const_set(mRbRDMA, rb_intern("QPT_RAW_PACKET"), INT2FIX(IBV_QPT_RAW_PACKET)); 

    // enum ibv_qp_attr_mask
    rb_const_set(mRbRDMA, rb_intern("QP_STATE"), INT2FIX(IBV_QP_STATE)); 
    rb_const_set(mRbRDMA, rb_intern("QP_CUR_STATE"), INT2FIX(IBV_QP_CUR_STATE)); 
    rb_const_set(mRbRDMA, rb_intern("QP_EN_SQD_ASYNC_NOTIFY"), INT2FIX(IBV_QP_EN_SQD_ASYNC_NOTIFY)); 
    rb_const_set(mRbRDMA, rb_intern("QP_ACCESS_FLAGS"), INT2FIX(IBV_QP_ACCESS_FLAGS)); 
    rb_const_set(mRbRDMA, rb_intern("QP_PKEY_INDEX"), INT2FIX(IBV_QP_PKEY_INDEX)); 
    rb_const_set(mRbRDMA, rb_intern("QP_PORT"), INT2FIX(IBV_QP_PORT)); 
    rb_const_set(mRbRDMA, rb_intern("QP_QKEY"), INT2FIX(IBV_QP_QKEY)); 
    rb_const_set(mRbRDMA, rb_intern("QP_AV"), INT2FIX(IBV_QP_AV)); 
    rb_const_set(mRbRDMA, rb_intern("QP_PATH_MTU"), INT2FIX(IBV_QP_PATH_MTU)); 
    rb_const_set(mRbRDMA, rb_intern("QP_TIMEOUT"), INT2FIX(IBV_QP_TIMEOUT)); 
    rb_const_set(mRbRDMA, rb_intern("QP_RETRY_CNT"), INT2FIX(IBV_QP_RETRY_CNT)); 
    rb_const_set(mRbRDMA, rb_intern("QP_RNR_RETRY"), INT2FIX(IBV_QP_RNR_RETRY)); 
    rb_const_set(mRbRDMA, rb_intern("QP_RQ_PSN"), INT2FIX(IBV_QP_RQ_PSN)); 
    rb_const_set(mRbRDMA, rb_intern("QP_MAX_QP_RD_ATOMIC"), INT2FIX(IBV_QP_MAX_QP_RD_ATOMIC)); 
    rb_const_set(mRbRDMA, rb_intern("QP_ALT_PATH"), INT2FIX(IBV_QP_ALT_PATH)); 
    rb_const_set(mRbRDMA, rb_intern("QP_MIN_RNR_TIMER"), INT2FIX(IBV_QP_MIN_RNR_TIMER)); 
    rb_const_set(mRbRDMA, rb_intern("QP_SQ_PSN"), INT2FIX(IBV_QP_SQ_PSN)); 
    rb_const_set(mRbRDMA, rb_intern("QP_MAX_DEST_RD_ATOMIC"), INT2FIX(IBV_QP_MAX_DEST_RD_ATOMIC)); 
    rb_const_set(mRbRDMA, rb_intern("QP_PATH_MIG_STATE"), INT2FIX(IBV_QP_PATH_MIG_STATE)); 
    rb_const_set(mRbRDMA, rb_intern("QP_CAP"), INT2FIX(IBV_QP_CAP)); 
    rb_const_set(mRbRDMA, rb_intern("QP_DEST_QPN"), INT2FIX(IBV_QP_DEST_QPN)); 

    // enum ibv_qp_state
    rb_const_set(mRbRDMA, rb_intern("QPS_RESET"), INT2FIX(IBV_QPS_RESET)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_INIT"), INT2FIX(IBV_QPS_INIT)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_RTR"), INT2FIX(IBV_QPS_RTR)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_RTS"), INT2FIX(IBV_QPS_RTS)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_SQD"), INT2FIX(IBV_QPS_SQD)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_SQE"), INT2FIX(IBV_QPS_SQE)); 
    rb_const_set(mRbRDMA, rb_intern("QPS_ERR"), INT2FIX(IBV_QPS_ERR)); 

    // enum ibv_mig_state
    rb_const_set(mRbRDMA, rb_intern("MIG_MIGRATED"), INT2FIX(IBV_MIG_MIGRATED)); 
    rb_const_set(mRbRDMA, rb_intern("MIG_REARM"), INT2FIX(IBV_MIG_REARM)); 
    rb_const_set(mRbRDMA, rb_intern("MIG_ARMED"), INT2FIX(IBV_MIG_ARMED)); 

    // enum ibv_wr_opcode
    rb_const_set(mRbRDMA, rb_intern("WR_RDMA_WRITE"), INT2FIX(IBV_WR_RDMA_WRITE)); 
    rb_const_set(mRbRDMA, rb_intern("WR_RDMA_WRITE_WITH_IMM"), INT2FIX(IBV_WR_RDMA_WRITE_WITH_IMM)); 
    rb_const_set(mRbRDMA, rb_intern("WR_SEND"), INT2FIX(IBV_WR_SEND)); 
    rb_const_set(mRbRDMA, rb_intern("WR_SEND_WITH_IMM"), INT2FIX(IBV_WR_SEND_WITH_IMM)); 
    rb_const_set(mRbRDMA, rb_intern("WR_RDMA_READ"), INT2FIX(IBV_WR_RDMA_READ)); 
    rb_const_set(mRbRDMA, rb_intern("WR_ATOMIC_CMP_AND_SWP"), INT2FIX(IBV_WR_ATOMIC_CMP_AND_SWP)); 
    rb_const_set(mRbRDMA, rb_intern("WR_ATOMIC_FETCH_AND_ADD"), INT2FIX(IBV_WR_ATOMIC_FETCH_AND_ADD)); 

    // enum ibv_send_flags
    rb_const_set(mRbRDMA, rb_intern("SEND_FENCE"), INT2FIX(IBV_SEND_FENCE)); 
    rb_const_set(mRbRDMA, rb_intern("SEND_SIGNALED"), INT2FIX(IBV_SEND_SIGNALED)); 
    rb_const_set(mRbRDMA, rb_intern("SEND_SOLICITED"), INT2FIX(IBV_SEND_SOLICITED)); 
    rb_const_set(mRbRDMA, rb_intern("SEND_INLINE"), INT2FIX(IBV_SEND_INLINE)); 

    // enum {
    rb_const_set(mRbRDMA, rb_intern("SYSFS_NAME_MAX"), INT2FIX(IBV_SYSFS_NAME_MAX)); 
    rb_const_set(mRbRDMA, rb_intern("SYSFS_PATH_MAX"), INT2FIX(IBV_SYSFS_PATH_MAX)); 


#endif 

  Init_context();
  Init_device();
  Init_comp_channel();
  Init_pd();
}
