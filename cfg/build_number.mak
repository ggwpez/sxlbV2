# Create an auto-incrementing build number.

BUILD_NUMBER := $(shell cat $(BUILD_NUMBER_FILE))

BUILD_NUMBER_FLAGS += -D__BUILD_NUMBER__=$(BUILD_NUMBER)

build_number.target:
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
