PROJECT_NAME = mbfs
DATASET_NAME = cora
WEIGHTED = 

all: hls backend_synthesis copy

csim:
	vivado_hls -f run_csim.tcl $(PROJECT_NAME) $(WEIGHTED)

cosim:
	vivado_hls -f run.tcl $(PROJECT_NAME) $(WEIGHTED)

hls:
	vivado_hls -f run.tcl $(PROJECT_NAME) $(WEIGHTED)
	-rm -rf $(PROJECT_NAME)/$(PROJECT_NAME)_hls.prj
	mv $(PROJECT_NAME)_hls.prj $(PROJECT_NAME)/$(PROJECT_NAME)_hls.prj
	-mv vivado_hls.log $(PROJECT_NAME)/vivado_hls.log

backend_synthesis:
	vivado -mode batch -source script.tcl -tclargs $(PROJECT_NAME)

copy:
	#cp $(PROJECT_NAME)/$(PROJECT_NAME)_hls.prj/solution1/impl/misc/drivers/test_v1_0/src/xtest_hw.h $(PROJECT_NAME)/xtest_hw.h
	cp $(PROJECT_NAME)/vivado/$(PROJECT_NAME).srcs/sources_1/bd/$(PROJECT_NAME)/hw_handoff/$(PROJECT_NAME).hwh $(PROJECT_NAME)/$(PROJECT_NAME).hwh
	cp $(PROJECT_NAME)/vivado/$(PROJECT_NAME).runs/impl_1/$(PROJECT_NAME)_wrapper.bit $(PROJECT_NAME)/$(PROJECT_NAME).bit
	#cp $(PROJECT_NAME)/vivado/$(PROJECT_NAME).tcl $(PROJECT_NAME)/$(PROJECT_NAME).tcl

deploy:
	python ./template/deploy.py $(PROJECT_NAME) $(DATASET_NAME)
