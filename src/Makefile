

all: arbiter_verilator_tb

VERILATOR=verilator
VERILATOR_ROOT ?= $(shell bash -c 'verilator -V|grep VERILATOR_ROOT | head -1 | sed -e "s/^.*=\s*//"')
VINC := $(VERILATOR_ROOT)/include


./obj_dir/Varbiter_verilator_tb.cpp: arbiter_verilator_tb.v
	$(VERILATOR) --trace -cc arbiter_verilator_tb.v

obj_dir/Varbiter_verilator_tb__ALL.a: obj_dir/Varbiter_verilator_tb.cpp
	make --no-print-directory -C obj_dir -f Varbiter_verilator_tb.mk

arbiter_verilator_tb: obj_dir/Varbiter_verilator_tb__ALL.a arbiter_verilator_tb.cpp
	g++ -I$(VINC) -I obj_dir $(VINC)/verilated.cpp	\
		$(VINC)/verilated_vcd_c.cpp arbiter_verilator_tb.cpp	\
		obj_dir/Varbiter_verilator_tb__ALL.a -o arbiter_verilator_tb


clean: 
	rm -rf obj_dir
	rm -f arbiter_verilator_tb arbiter_verilator_tb.vcd
