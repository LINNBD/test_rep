.PHONY: modules clean install uninstall
KDIR ?= /lib/modules/`uname -r`/build
modules:
	$(MAKE) -C $(KDIR) M=$$PWD/module modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD/module clean

install:
	-dkms remove logout/1.0 --all
	dkms install module
	cp signout /usr/local/bin
	cp logout.service /etc/systemd/system
	systemctl daemon-reload
	systemctl enable logout.service
	systemctl start logout.service
uninstall:
	-dkms remove logout/1.0 --all
	-rm -f /usr/local/bin/signout
	-rm -f /etc/systemd/system/logout.service
	-systemctl daemon-reload
