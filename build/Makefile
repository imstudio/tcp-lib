OBJS=*.o
ODIR=obj
BOOST_LIB_LINK=-lboost_regex -lboost_log_setup -lboost_log -lboost_thread -lboost_filesystem -lboost_system
PROTOBUF_LIB_LINK=-pthread -I/usr/local/include -L/usr/local/lib -lprotobuf -pthread -lpthread
#PROTOBUF_LIB_LINK=`pkg-config --cflags --libs protobuf`
$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
	g++ -o $@ $^ -lpthread ${PROTOBUF_LIB_LINK} ${BOOST_LIB_LINK} -L ${BOOST_LIB_DIR} -L ${EXTRA_LIB_DIR} 
