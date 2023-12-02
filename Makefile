CC = g++ -std=c++11
Flags = -Wall -Wextra -g
Target = submission

all: $(Target)

$(Target): submission.o
	 $(CC) $(Flags) submission.o -o $@

submission.o: submission.cpp 
	$(CC) $(Flags) -c submission.cpp -o $@

clean:
	rm -rf *.o $(Target)