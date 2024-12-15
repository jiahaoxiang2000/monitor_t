.PHONY: pio-build pio-upload bu

pio-build:
	platformio run

pio-upload: pio-build
	platformio run --target upload

bu: pio-build pio-upload
	@echo "Build and upload completed"
