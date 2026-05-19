# 1. 宣告 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 設定專案名稱 (也就是助教說的 [your_project_name])
project(my_game)

# 3. 指定使用 C++17 標準 (對應你原本 g++ 指令的 -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 告訴 CMake 要編譯出名叫 my_game 的執行檔，來源是 main.cpp
# (如果你有把其他類別拆成 .cpp，例如 GameObject.cpp，也要加在後面，用空白隔開)
add_executable(my_game main.cpp)

# 5. 連結 Raylib 與 Linux 系統函式庫 
# (完美對應你原本指令的 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11)
target_link_libraries(my_game raylib GL m pthread dl rt X11)
