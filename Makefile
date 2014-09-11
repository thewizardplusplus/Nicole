main:
	$(MAKE) -C wizard-basic-compiler
	$(MAKE) -C wizard-basic-framework

clean:
	$(MAKE) -C wizard-basic-compiler clean
	$(MAKE) -C wizard-basic-framework clean
