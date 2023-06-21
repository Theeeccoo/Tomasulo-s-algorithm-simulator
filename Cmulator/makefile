export CFLAGS = -Wall -Wextra -Werror

.PHONY: initializer


all: initializer run clean

initializer:
	gcc $(CFLAGS) $(CURDIR)/*.c -o out

run:
	@./out

clean:
	@rm -f out
