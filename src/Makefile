all: serial.c
	gcc serial.c -o _s

vg: serial
	valgrind --leak-check=yes --track-origins=yes ./serial


vg_leak: test_leak
	valgrind --leak-check=yes --track-origins=yes ./test_leak


clean:
	rm -rf _*
