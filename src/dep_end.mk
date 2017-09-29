all: $(DEPDIR)%.d

$(DEPDIR)%.d: ;
.PRECIOUS: $(DEPDIR)%.d

include $(wildcard $(patsubst %,$(DEPDIR)%.d,$(basename $(SOURCES))))
