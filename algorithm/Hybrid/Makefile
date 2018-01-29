CC=gcc

CFLAGS +=\
		-g\
		-std=c99\
		-Wno-discarded-qualifiers\

SRCS +=\
		FAST_Create.c\
		FAST_Destroy.c\
		FAST_EndRequest.c\
		FAST_Get.c\
		FAST_Set.c\
		FAST_Remove.c\
		fast_AllocDataBlockEntry.c\
		fast_AllocRWLogBlockEntry.c\
		fast_AllocSWLogBlockEntry.c\
		fast_SearchDataBlock.c\
		fast_SearchRWLogBlock.c\
		fast_SearchSWLogBlock.c\
		fast_Util.c\
		fast_SwitchSWLogBlock.c\
		fast_MergeSWLogBlock.c\
		fast_MergeRWLogBlock.c\

OBJS :=\
	$(patsubst %.c,%.o,$(SRCS))\

TARGETOBJ:=\
	$(addprefix ../../object/,$(OBJS))\

all: $(TARGETOBJ) 

libnom.a : $(TARGETOBJ)
	$(AR) r $(@) $(TARGETOBJ)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

../../object/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	@$(RM) libnom.a
	@$(RM) *.o
