#子目录的Makefile直接读取其子目录就行, DEBUG compile can open or close
SUBDIRS=$(shell ls -l | grep ^d | awk '{print $$9}')
#以下同根目录下的makefile的相同代码的解释
CUR_SOURCE=${wildcard *.cc}
CUR_OBJS=${patsubst %.cc, %.o, $(CUR_SOURCE)}
all:$(SUBDIRS) $(CUR_OBJS)
$(SUBDIRS):ECHO
	make -C $@
$(CUR_OBJS):%.o:%.cc
	$(CC) -c $^ -o $(OBJS_DIR)/$@
ECHO:
	@echo $(SUBDIRS)
