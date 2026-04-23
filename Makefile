# ============================================================================
#  ft_linear_regression — Makefile
# ============================================================================

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -Iinclude

# ── Directories ──────────────────────────────────────────────────────────────
SRC_DIR  := src
OBJ_DIR  := objects
BIN_DIR  := bin

# ── Shared sources (compiled into both binaries) ─────────────────────────────
SHARED_SRCS := $(SRC_DIR)/LinearRegression.cpp \
               $(SRC_DIR)/utils.cpp

SHARED_OBJS := $(SHARED_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ── Per-binary sources ────────────────────────────────────────────────────────
TRAIN_SRC   := $(SRC_DIR)/train.cpp
PREDICT_SRC := $(SRC_DIR)/predict.cpp

TRAIN_OBJ   := $(OBJ_DIR)/train.o
PREDICT_OBJ := $(OBJ_DIR)/predict.o

# ── Binary names ─────────────────────────────────────────────────────────────
TRAIN_BIN   := $(BIN_DIR)/train
PREDICT_BIN := $(BIN_DIR)/predict

# ── Default target: build everything ─────────────────────────────────────────
all: $(TRAIN_BIN) $(PREDICT_BIN)

# ── Link train binary ─────────────────────────────────────────────────────────
$(TRAIN_BIN): $(TRAIN_OBJ) $(SHARED_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

# ── Link predict binary ───────────────────────────────────────────────────────
$(PREDICT_BIN): $(PREDICT_OBJ) $(SHARED_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

# ── Compile any .cpp in src/ → object_files/ ─────────────────────────────────
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) models
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Create object_files/ directory if it does not exist ──────────────────────
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# ── Create bin/ directory if it does not exist ───────────────────────────────
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ── Create models/ directory if it does not exist ────────────────────────────
models:
	mkdir -p models

# ── Phony targets ─────────────────────────────────────────────────────────────
.PHONY: all clean fclean re

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	rm -rf $(BIN_DIR)
	@echo "Binaries removed."

re: fclean all
