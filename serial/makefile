CC = g++
CC_OPT = --std=c++11 -Wall --pedantic
PTHREAD = -lpthread

PhonePricePrediction.out: main.o evaluator.o csv_parser.o misc_funcs.o
	$(CC) $(CC_OPT) main.o evaluator.o csv_parser.o misc_funcs.o \
	-o PhonePricePrediction.out $(PTHREAD)

main.o: main.cc
	$(CC) $(CC_OPT) -c main.cc

evaluator.o: ./evaluator.cc
	$(CC) $(CC_OPT) -c ./evaluator.cc

csv_parser.o: ./csv_parser.cc
	$(CC) $(CC_OPT) -c ./csv_parser.cc

misc_funcs.o: ./misc_funcs.cc
	$(CC) $(CC_OPT) -c ./misc_funcs.cc

clean:
	rm -f *.o *.out