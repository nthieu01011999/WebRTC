###############################################################################
# author: ThanNT
# date: 6/1/2017
###############################################################################

-include config.mk

RPI_SYS_DIR	=
TOOLS_DIR	=

ifdef BUILD_LINUX64
CROSS_COMPILE=
OBJ_DIR		= _build
NAME_MODULE	= fcam-vvtk-linux64
endif
ifdef BUILD_ARM_VVTK
OBJ_DIR		= _build_vvtk
CROSS_COMPILE=arm-linux-
NAME_MODULE	= cmr-p2p
endif


CXX	  =$(CROSS_COMPILE)g++
CC	  =$(CROSS_COMPILE)gcc
AR	  =$(CROSS_COMPILE)ar
STRIP =$(CROSS_COMPILE)strip


OPTIMIZE_OPTION	=	-g -s -O3
WARNNING_OPTION	+=	#-Werror -W -Wno-missing-field-initializers

CXXFLAGS += $(HAVE_FLAGS)

-include sources/ak/Makefile.mk
-include sources/sys/Makefile.mk
-include sources/app/Makefile.mk
-include sources/common/Makefile.mk
-include sources/driver/Makefile.mk
-include sources/libraries/Makefile.mk

ifdef BUILD_LINUX64
GENERAL_FLAGS +=\
	-DBUILD_LINUX64
else
GENERAL_FLAGS +=\
	-DBUILD_ARM_VVTK
endif

ifdef RELEASE
GENERAL_FLAGS += -DRELEASE
endif

# CXX compiler option
CXXFLAGS	+=\
		$(OPTIMIZE_OPTION)	\
		$(WARNNING_OPTION)	\
		-Wall				\
-pipe				\
		-ggdb				\
		$(GENERAL_FLAGS) 

# Library paths
LDFLAGS	+= -Wl,-Map=$(OBJ_DIR)/$(NAME_MODULE).map


ifdef BUILD_LINUX64
DEPENDENCIES=sources/libraries/linux64
endif
ifdef BUILD_ARM_VVTK
#TODO fix vvtk path
DEPENDENCIES=sources/libraries/arm_rts
# LDLIBS +=  $(DEPENDENCIES)/lib/libfreetype.a
endif

ifdef FEATURE_RTMP
	GENERAL_FLAGS += -DFEATURE_RTMP
endif

ifdef FEATURE_AI
GENERAL_FLAGS += -DFEATURE_AI
endif

ifdef BUILD_LINUX64
LDLIBS +=  $(DEPENDENCIES)/lib/libdatachannel.a
LDLIBS +=  $(DEPENDENCIES)/lib/libjuice.a
LDLIBS +=  $(DEPENDENCIES)/lib/libsrtp2.a
LDLIBS +=  $(DEPENDENCIES)/lib/libusrsctp.a

LDLIBS	+=\
		-lpthread			\
		-lrt				\
		-lm					\
		-lcurl				\
		-lcrypto			\
		-lssl				\
		-lmosquittopp				\
		-lmosquitto				
endif

ifdef BUILD_ARM_VVTK
LDLIBS +=  $(DEPENDENCIES)/lib/libmosquittopp.a
LDLIBS +=  $(DEPENDENCIES)/lib/libmosquitto.a

# lib webrtc
LDLIBS +=  $(DEPENDENCIES)/libdatachannel/lib/libdatachannel.a
LDLIBS +=  $(DEPENDENCIES)/libdatachannel/lib/libjuice.a
LDLIBS +=  $(DEPENDENCIES)/libdatachannel/lib/libsrtp2.a
LDLIBS +=  $(DEPENDENCIES)/libdatachannel/lib/libusrsctp.a

ifdef FEATURE_RTMP
LDLIBS +=  $(DEPENDENCIES)/lib/libRTMP.a
LDLIBS +=  $(DEPENDENCIES)/lib/librtmp.a
LDLIBS +=  $(DEPENDENCIES)/lib/libFPTHmac.a
endif

#libs3
LDLIBS +=  $(DEPENDENCIES)/lib/libs3.a

LDLIBS += -L$(DEPENDENCIES)/lib
LDLIBS	+=\
		-lpthread			\
		-lrt				\
		-lm					\
		-lcurl				\
		-lcrypto			\
		-lssl				\

#stb
LDLIBS	+=\
		-DSTB_IMAGE_IMPLEMENTATION			\
		-DSTB_IMAGE_WRITE_IMPLEMENTATION				\
		-lm					\
		-I$(PWD)/sources/libraries/arm_rts/include/stb	\
	
# lib SDK
LDLIBS	+=\
		-lvvtkhal

endif
		
all: create $(OBJ_DIR)/$(NAME_MODULE)

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

	@echo "DEPENDENCIES" $(DEPENDENCIES)

	rm -rf $(OBJ_DIR)/$(NAME_MODULE)

$(OBJ_DIR)/%.o: %.cpp
	@echo CXX $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS) -std=c++17 $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@echo CC $<
	@$(CC) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS)

$(OBJ_DIR)/$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CXX) -o $@ $^ $(CXXFLAGS)
	@echo "\n\n"
	@echo LIB $(LDLIBS) "\n\n"audio_stream
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

ifdef RELEASE
	@echo "\033[1;32m----------------- Project is build Release -----------------\033[0m"
else
	@echo "\033[1;31m----------------- Project is build Debug -----------------\033[0m"
endif


.PHONY: copy
copy: $(OBJ_DIR)/$(NAME_MODULE)
	sudo cp $(OBJ_DIR)/$(NAME_MODULE) $(HOME)/FTEL/mountNFS/bin
	
.PHONY: flash
flash:
	@sudo LD_LIBRARY_PATH=/usr/local/lib/ $(OBJ_DIR)/$(NAME_MODULE)

.PHONY: debug
debug:
	sudo gdb $(OBJ_DIR)/$(NAME_MODULE)

.PHONY: install
install:
	cp $(OBJ_DIR)/$(NAME_MODULE) /mnt/usr/bin

.PHONY: clean
clean: clean-agent
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

.PHONY: strip
strip:
	$(STRIP) $(OBJ_DIR)/$(NAME_MODULE)

show_variables:
	@echo "DEPENDENCIES: $(DEPENDENCIES)"
	@echo "LDLIBS: $(LDLIBS)"

show-build:
ifdef RELEASE
	@echo "\033[1;32m----------------- Project is build Release -----------------\033[0m"
else
	@echo "\033[1;31m----------------- Project is build Debug -----------------\033[0m"
endif

agent-cp:
	cp -r $(OBJ_DIR)/$(NAME_MODULE) ../agent/bin/fcam


check:
ifneq ($(BUILD_ERROR), '')
	$(error error is $(BUILD_ERROR))
endif

build-agent: check agent-cp

	@echo "\033[1;32mBuild model [$(FW_MODEL)]\033[0m"

	@mkdir -p $(BUILD_DIR)

	@../auto_gen.sh ../$(PROJECT_AGENT)/version  $(VENDOR) $(FW_MODEL) $(FW_NAME) $(VERSION) $(DATE_CURR) $(BUILD_TIME)
	mksquashfs ../$(PROJECT_AGENT) $(BUILD_DIR)/fpt_agent_$(FW_NAME).bin -comp xz 

	@echo "\033[1;32mBuild model [$(FW_MODEL)]\033[0m"

# scp $(BUILD_DIR)/* huynt@192.168.1.141:/home/huynt/Workspace/FPT/Github/Camera/vvtk_nfs

ifdef RELEASE
	@echo "\033[1;32m----------------- Project is build Release -----------------\033[0m"
else
	@echo "\033[1;31m----------------- Project is build Debug -----------------\033[0m"
endif

clean-agent:
	rm -rf $(BUILD_DIR)