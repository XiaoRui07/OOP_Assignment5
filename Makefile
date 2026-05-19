# 1. 基本設定
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Wno-missing-braces
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# 2. 自動偵測檔案
# wildcard 會抓取資料夾內所有的 .cpp 檔，例如 main.cpp, Player.cpp
SRCS = $(wildcard *.cpp)

# 3. 定義執行檔位置
BIN_DIR = bin
TARGET = $(BIN_DIR)/my_game

# 4. 編譯規則
all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BIN_DIR)
	# 這裡會編譯所有的 .cpp 檔案並連結成一個執行檔
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
	@echo "編譯完成！包含檔案: $(SRCS)"

clean:
	rm -rf $(BIN_DIR) 
