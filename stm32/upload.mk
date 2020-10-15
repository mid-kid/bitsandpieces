.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

.PHONY: debug
debug: $(BUILD_DIR)/$(TARGET).bin
	openocd
