all:
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o token.o token.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o file_reader.o file_reader.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o lexico.o lexico.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o semantic.o semantic.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o machines.o machines.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o spa.o spa.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o table_of_symbols.o table_of_symbols.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o linked_list.o linked_list.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o util.o util.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o file_writer.o file_writer.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o spa_stack.o spa_stack.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o main.o main.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o stack.o stack.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o transducer.o transducer.c
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -o semantic_actions.o semantic_actions.c
	gcc -o Compiler.exe util.o transducer.o token.o table_of_symbols.o stack.o spa_stack.o spa.o semantic_actions.o semantic.o main.o machines.o linked_list.o lexico.o file_writer.o file_reader.o