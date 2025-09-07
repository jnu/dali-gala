# ESP32-S3-Pico Project Makefile
# Provides convenient shortcuts for common PlatformIO operations

.PHONY: help build upload monitor clean test all

# Default target
all: build

# Show help
help:
	@echo "ESP32-S3-Pico Project Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  build     - Build the project"
	@echo "  upload    - Build and upload to device"
	@echo "  monitor   - Monitor serial output"
	@echo "  clean     - Clean build artifacts"
	@echo "  test      - Run tests"
	@echo "  all       - Build the project (default)"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Usage: make [target]"

# Build the project
build:
	@echo "Building ESP32-S3-Pico project..."
	uv run pio run

# Build and upload to device
upload:
	@echo "Building and uploading to ESP32-S3-Pico..."
	uv run pio run --target upload

# Monitor serial output
monitor:
	@echo "Starting serial monitor..."
	uv run pio device monitor

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	uv run pio run --target clean

# Run tests
test:
	@echo "Running tests..."
	uv run pio test

# Build and upload, then monitor
deploy: upload monitor

# Check device connection
device:
	@echo "Available devices:"
	uv run pio device list

# Update PlatformIO
update:
	@echo "Updating PlatformIO..."
	uv run pio update

# Install dependencies
deps:
	@echo "Installing project dependencies..."
	uv run pio lib install

# Check for updates
check:
	@echo "Checking for library updates..."
	uv run pio lib update --dry-run

