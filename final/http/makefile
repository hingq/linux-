CC = gcc
CFLAGS = -Wall -Wextra -Isrc

# 定义源文件目录和目标文件目录
SRCDIR = src
BUILDDIR = build

# 源文件和目标文件列表
SRCS := $(wildcard $(SRCDIR)/*.c) #获取src下的所有.c文件
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
TARGET = http #运行名称

# 默认目标
all: $(TARGET)

# 编译可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# 编译每个源文件到目标文件
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -rf $(BUILDDIR) $(TARGET)
