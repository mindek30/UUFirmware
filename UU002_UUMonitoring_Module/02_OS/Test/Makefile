######################################
# target
######################################
TARGET = test

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# Includes
######################################
# C Includes
C_INCLUDES =  \
-I./ \
-I../EKM_Modbus

# ASM includes
ASM_INCLUDES = 

######################################
# source
######################################
# C sources
C_SOURCES =  \
./test.c \
../EKM_Modbus/EKM_Modbus.c 

# ASM sources
ASM_SOURCES =  

#######################################
# Prefix Compiler Setup
#######################################
PREFIX = 

#######################################
# Compiler Setup
#######################################
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
 


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) $(C_INCLUDES) -c -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(TARGET).exe: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) -o $@
	$(SZ) $@

$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rmdir /s $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
