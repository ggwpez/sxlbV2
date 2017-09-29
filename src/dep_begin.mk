DEPDIR := $(BINDIR).d/
all: $(DEPDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)
