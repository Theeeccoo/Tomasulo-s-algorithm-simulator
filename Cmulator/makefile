export CFLAGS = -Wall -Wextra -Werror

.PHONY: initializer


all: initializer

initializer:
	gcc $(CFLAGS) $(CURDIR)/*.c -o out

clean:
	@rm -f out
