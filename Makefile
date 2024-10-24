TARGET := sll-example.out
TARGET_OBJS := ./sll-example/main.c ./libsll/sll.c
GFLAGS += -std=c11 -03 -Wall -Wextra -Wpedantic

$(TARGET): $(TARGET_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $^

clean:
	$(RM) -f *.s *.i *.o *.out

all: $(TARGET) $(TARGET_TEST)

.PHONY: all clean
