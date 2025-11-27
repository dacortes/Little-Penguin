################################################################################
#                               BOLD COLORS                                    #
################################################################################

END = \033[m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
PURPLE = \033[35m
CIAN = \033[36m

################################################################################
#  FONT                                                                        #
################################################################################

LIGTH = \033[1m
DARK = \033[2m
ITALIC = \033[3m

################################################################################
#                               VARIABLES                                      #
################################################################################

NAME := mymounts

SOURCES = \
           src/$(NAME).c

OBJECTS = $(SOURCES:.c=.o)
obj-m += $(NAME).o
$(NAME)-objs := $(OBJECTS)
ccflags-y := -I$(PWD)/include

PWD := $(CURDIR)
SUDO := $(shell [ "$$(id -u)" -ne 0 ] && echo "sudo")

all: up

compile:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


mount:
	@if mount | grep "none on /sys/kernel/debug"; then \
                printf "$(BLUE)[INFO]$(END) The debugfs module is already mounted\n"; \
        else \
                mount -t debugfs none /sys/kernel/debug; \
                printf "$(GREEN)[SUCCESS]$(END) module debugfs mounted\n"; \
        fi

up: compile
	@if lsmod | awk '{print $$1}' | grep -w $(NAME); then \
                printf "$(BLUE)[INFO]$(END) >> Modulo $(NAME) is already loaded\n"; \
        else \
                if [ -f $(NAME).ko ]; then \
                        insmod $(NAME).ko && printf "$(GREEN)[SUCCESS]$(END) >> Module $(NAME) loaded successfully\n";\
                else \
                        printf "$(RED)[ERROR]$(END) !! Module file $(NAME).ko not found\n"; \
                fi \
        fi
down:
	@if lsmod | awk '{print $$1}' | grep -w $(NAME); then \
                rmmod $(NAME) && printf "$(GREEN)[SUCCESS]$(END) >> Unloaded $(NAME).ko\n"; \
        else \
                printf "$(RED)[ERROR]$(END) >> Module $(NAME) is not loaded\n"; \
        fi

info:
	@if [ -f $(NAME).ko ]; then \
                printf "$(GREEN)[SUCCESS]$(END) >> Found $(NAME).ko, displaying module information\n"; \
                modinfo $(NAME).ko; \
        else \
                printf "$(RED)[ERROR]$(END) >> Module file $(NAME).ko not found\n"; \
        fi

re: clean all
	@printf "$(BLUE)[INFO]$(END) >> Rebuilt module $(NAME).ko\n"

help:
	@printf "$(PURPLE)================= [ $(LIGTH)HELP MENU$(END) $(PURPLE)] =================$(END)\n"
	@printf "$(YELLOW)make all$(END)\t\t→ Compile and load the kernel module ($(NAME).ko) with debugfs mount\n"
	@printf "$(YELLOW)make compile$(END)\t→ Only compile the kernel module ($(NAME).ko)\n"
	@printf "$(YELLOW)make clean$(END)\t→ Remove compiled files\n"
	@printf "$(YELLOW)make mount$(END)\t→ Mount debugfs at /sys/kernel/debug if not mounted\n"
	@printf "$(YELLOW)make up$(END)\t\t→ Load the module into the kernel (auto uses sudo if needed)\n"
	@printf "$(YELLOW)make down$(END)\t\t→ Unload the module from the kernel (auto uses sudo if needed)\n"
	@printf "$(YELLOW)make info$(END)\t\t→ Display module information using modinfo\n"
	@printf "$(YELLOW)make re$(END)\t\t→ Clean and recompile the module\n"
	@printf "$(YELLOW)make SUDO=sudo up$(END)\t→ Force use of sudo when loading manually\n"
	@printf "$(YELLOW)make help$(END)\t\t→ Show this help message\n"
	@printf "$(PURPLE)============================================================$(END)\n"

################################################################################
#                               CONFIG                                         #
################################################################################

.SILENT:
.PHONY: all clean up down info re help
