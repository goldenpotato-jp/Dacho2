# Makefile

# コンパイラの設定
CXX      = g++

# 最適化フラグ (-Ofast, -march=native, -flto で最高速化し、-Wall で警告を適度に抑制)
CXXFLAGS = -Ofast -march=native -flto -std=c++17 -Wall -I.
LDFLAGS  = -Ofast -march=native -flto

# 実行ファイル名とソースファイルの指定
TARGET   = dacho2
SRCS     = $(shell find . -name "*.cpp")
OBJS     = $(SRCS:.cpp=.o)

# デフォルトターゲット
all: $(TARGET)

# リンク処理（-flto を適用するため LDFLAGS を指定）
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

# コンパイル処理
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ビルド生成物の削除
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean