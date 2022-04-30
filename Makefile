SRCS = main.c
CFLAGS = -std=gnu99
LIBS = -lncurses -ltinfo

life: $(SRCS)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	@rm -vf life

