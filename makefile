all:
	+$(MAKE) -C src
	+$(MAKE) -C test
src:
	+$(MAKE) -C src
test:
	+$(MAKE) -C test
