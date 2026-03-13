# Riverbraid-Lite Master Build Script
CC = gcc
CFLAGS = -fPIC -shared -I./include -I./core/include
LDFLAGS = -L. -lrb_core

# Target Library
LIB = librb_core.so

# Source Files
SRC = src/petal_audio.c src/petal_vision.c src/petal_action.c src/rb_reseal.c

.PHONY: all clean test monitor

all: $(LIB)

# 1. Build the Sovereign Core
$(LIB): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(LIB)
	@echo "✅ Core Library Built: $(LIB)"

# 2. Run C-Level Integrity Tests
test: $(LIB)
	$(CC) -I./include -I./core/include $(SRC) tests/test_sovereign_loop.c -o test_sovereign_loop
	./test_sovereign_loop
	@echo "✅ C-Level Integrity Verified."

# 3. Launch the Python Observation Deck
monitor: $(LIB)
	python3 bridge_gateway.py

# 4. Cleanup
clean:
	rm -f $(LIB) test_sovereign_loop test_audio test_reseal test_full_cycle
	@echo "🧹 Environment Sanitized."

