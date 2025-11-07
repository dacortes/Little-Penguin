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
NAME := helloKeyboard
obj-m += $(NAME).o
PWD := $(CURDIR)
KDIR := /lib/modules/$(shell uname -r)/build
SUDO := $(shell [ "$$(id -u)" -ne 0 ] && echo "sudo")

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install: all
	@printf "$(BLUE)[INFO]$(END) >> Installing $(NAME).ko to /lib/modules/$(shell uname -r)/extra/\n"
	@$(SUDO) mkdir -p /lib/modules/$(shell uname -r)/extra
	@$(SUDO) cp -v $(NAME).ko /lib/modules/$(shell uname -r)/extra/
	@$(SUDO) depmod -a
	@printf "$(GREEN)[SUCCESS]$(END) >> Installed and depmod updated\n"

uninstall:
	@if [ -f /lib/modules/$(shell uname -r)/extra/$(NAME).ko ]; then \
		$(SUDO) rm -v /lib/modules/$(shell uname -r)/extra/$(NAME).ko; \
		$(SUDO) depmod -a; \
		printf "$(GREEN)[SUCCESS]$(END) >> Removed $(NAME).ko from modules tree\n"; \
	else \
		printf "$(YELLOW)[INFO]$(END) >> No installed module found to remove\n"; \
	fi

up:
	@if lsmod | awk '{print $$1}' | grep -q $(NAME); then \
		printf "$(BLUE)[INFO]$(END) >> Module $(NAME) already loaded\n"; \
	else \
		if [ -f $(NAME).ko ]; then \
			$(SUDO) insmod $(NAME).ko && printf "$(GREEN)[SUCCESS]$(END) >> Module $(NAME) loaded successfully\n"; \
		else \
			printf "$(RED)[ERROR]$(END) >> Module file $(NAME).ko not found\n"; \
		fi \
	fi

down:
	@if lsmod | awk '{print $$1}' | grep -q $(NAME); then \
		$(SUDO) rmmod $(NAME) && printf "$(GREEN)[SUCCESS]$(END) >> Unloaded $(NAME).ko\n"; \
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

.ONESHELL:

udev-install:
	@printf "$(BLUE)[INFO]$(END) >> Installing udev rule to /etc/udev/rules.d/99-$(NAME).rules\n"
	@$(SUDO) tee /etc/udev/rules.d/99-$(NAME).rules > /dev/null <<'EOF'
	ACTION=="add", SUBSYSTEMS=="usb", ATTRS{bInterfaceClass}=="03", ATTRS{bInterfaceProtocol}=="01", RUN+="/sbin/modprobe helloKeyboard"
	EOF
	@$(SUDO) udevadm control --reload-rules && $(SUDO) udevadm trigger --action=add
	@printf "$(GREEN)[SUCCESS]$(END) >> udev rule installed and reloaded\n"

udev-remove:
	@if [ -f /etc/udev/rules.d/99-$(NAME).rules ]; then \
		$(SUDO) rm -v /etc/udev/rules.d/99-$(NAME).rules; \
		$(SUDO) udevadm control --reload-rules; \
		printf "$(GREEN)[SUCCESS]$(END) >> Removed udev rule\n"; \
	else \
		printf "$(YELLOW)[INFO]$(END) >> No udev rule to remove\n"; \
	fi

enable-boot:
	@printf "$(BLUE)[INFO]$(END) >> Enabling module load at boot via /etc/modules-load.d/$(NAME).conf\n"
	@$(SUDO) tee /etc/modules-load.d/$(NAME).conf > /dev/null <<'EOF'
	$(NAME)
	EOF
	@printf "$(GREEN)[SUCCESS]$(END) >> Module will be loaded at boot (systemd modules-load)\n"

disable-boot:
	@if [ -f /etc/modules-load.d/$(NAME).conf ]; then \
		$(SUDO) rm -v /etc/modules-load.d/$(NAME).conf; \
		printf "$(GREEN)[SUCCESS]$(END) >> Disabled module load at boot\n"; \
	else \
		printf "$(YELLOW)[INFO]$(END) >> Module not configured for boot\n"; \
	fi

re: clean all
	@printf "$(BLUE)[INFO]$(END) >> Rebuilt module $(NAME).ko\n"

help:
	@printf "$(PURPLE)================= [ $(LIGHT)HELP MENU$(END) $(PURPLE)] =================$(END)\n"
	@printf "$(YELLOW)make all$(END)\t\t→ Compile the kernel module ($(NAME).ko)\n"
	@printf "$(YELLOW)make install$(END)\t→ Install module into /lib/modules/$(shell uname -r)/extra and run depmod\n"
	@printf "$(YELLOW)make uninstall$(END)\t→ Remove installed module and run depmod\n"
	@printf "$(YELLOW)make up$(END)\t\t→ Load module from cwd with insmod\n"
	@printf "$(YELLOW)make down$(END)\t\t→ Unload module with rmmod\n"
	@printf "$(YELLOW)make info$(END)\t\t→ Show modinfo for local .ko\n"
	@printf "$(YELLOW)make udev-install$(END)\t→ Install udev rule to autoload on keyboard plug\n"
	@printf "$(YELLOW)make udev-remove$(END)\t→ Remove udev rule\n"
	@printf "$(YELLOW)make enable-boot$(END)\t→ Load module at boot (systemd modules-load.d)\n"
	@printf "$(YELLOW)make disable-boot$(END)\t→ Remove boot load config\n"
	@printf "$(PURPLE)============================================================$(END)\n"

.PHONY: all clean install uninstall up down info udev-install udev-remove enable-boot disable-boot re help
.SILENT:
