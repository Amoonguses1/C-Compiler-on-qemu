install:
	qemu-system-x86_64 \
		-m 2048 \
		-smp 2 \
		-machine q35 \
		-cpu Haswell \
		-drive file=ubuntu-intel.img,format=qcow2 \
		-cdrom ubuntu-22.04.5-live-server-amd64.iso \
		-boot d \
		-net nic -net user \
		-vga virtio

exe:
	qemu-system-x86_64 \
		-m 2048 \
		-smp 2 \
		-machine q35 \
		-cpu Haswell \
		-drive file=ubuntu-intel.img,format=qcow2 \
		-boot c \
		-net nic -net user,hostfwd=tcp::2222-:22 \
		-vga virtio \
  		-virtfs local,path=$$(pwd),security_model=none,mount_tag=hostshare
		

create_img:
	qemu-img create -f qcow2 ubuntu-intel.img 20G


CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
		$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
