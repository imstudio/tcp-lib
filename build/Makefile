OBJS=*.o
ODIR=obj
BOOST_LIB_LINK=-lboost_regex -lboost_log_setup -lboost_log -lboost_thread -lboost_filesystem -lboost_system
$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
	g++ -o $@ $^ -lpthread ${BOOST_LIB_LINK} -L ${BOOST_LIB_DIR} -L ${EXTRA_LIB_DIR}
