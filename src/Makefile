SUBDIRS=$(shell ls -l | grep ^d |awk '{if($$9 != "build") print $$9}')
ROOT_DIR=$(shell pwd)
BIN=server
BUILD_DIR=$(ROOT_DIR)/../build
OBJS_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin
CUR_SOURCE=${wildcard *.cpp}
CUR_OBJS=${patsubst %.cpp, %.o, $(CUR_SOURCE)}
EXTRA_H_DIR=${ROOT_DIR}/../depend/include
EXTRA_LIB_DIR=${ROOT_DIR}/../depend/lib
BOOST_H_DIR=/home/licheng/workspace/libs/boost_1_63_0/
BOOST_LIB_DIR=/home/licheng/workspace/libs/boost_1_63_0/stage/lib
CC=g++ -std=c++11 -g -DBOOST_LOG_DYN_LINK -I ${EXTRA_H_DIR} -I ${ROOT_DIR} -I ${BOOST_H_DIR}

#将以下变量导出到子shell中，本次相当于导出到子目录下的makefile中
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR EXTRA_H_DIR EXTRA_LIB_DIR BOOST_LIB_DIR

#需要先执行SUBDIRS最后才能是LINK
all:$(SUBDIRS) $(CUR_OBJS) LINK

#递归执行子目录下的makefile文件，这是递归执行的关键
$(SUBDIRS):ECHO
	make -C $@

LINK:ECHO
    #直接去debug目录下执行makefile文件
	make -C $(BUILD_DIR)

ECHO:
	@echo $(SUBDIRS)

$(CUR_OBJS):%.o:%.cpp
	$(CC) -c $^ -o $(OBJS_DIR)/$@

top:$(CUR_OBJS) LINK


clean:
	@rm $(OBJS_DIR)/*.o

run:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${BOOST_LIB_DIR}:${EXTRA_LIB_DIR};${BIN_DIR}/${BIN}

gdb:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${BOOST_LIB_DIR}:${EXTRA_LIB_DIR};gdb ${BIN_DIR}/${BIN}

biz:ECHO
	make -C business/
