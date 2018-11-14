all:
	clear
	g++ -std=c++11 -o proj4 proj4.cpp main.cpp

GREEN=\033[0;32m
RED=\033[0;31m
NC=\033[0m
test:
	clear
	@echo "COMPILING...\n"
	@g++ -std=c++11 -o proj4 proj4.cpp main.cpp

	@echo "RUNNNIG TESTS..."

	@echo "\n"
	@echo "----------------------"
	@echo "----------------------"
	@echo "|  TESTING   REPORT  |"
	@echo "----------------------"
	@echo "TEST-NAME\tRESULT"
	@echo "----------------------"




	@./proj4 -t traces/409.trace -s > traces/testing.txt
	@if diff traces/409-s.out traces/testing.txt > /dev/null; then echo "$(GREEN)409 -s\t\tPASSED"; else echo "$(RED)409 -s\t\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/alive.trace -l > traces/testing.txt
	@if diff traces/alive-l.out traces/testing.txt > /dev/null; then echo "$(GREEN)alive -l\tPASSED"; else echo "$(RED)alive -l\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/kashmir.trace -l > traces/testing.txt
	@if diff traces/kashmir-l-ip.out traces/testing.txt > /dev/null; then echo "$(GREEN)kashmir -l\tPASSED"; else echo "$(RED)kashmir -l\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/kashmir.trace -p > traces/testing.txt
	@if diff traces/kashmir-p.out traces/testing.txt > /dev/null; then echo "$(GREEN)kashmir -p\tPASSED"; else echo "$(RED)kashmir -p\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/kashmir.trace -m > traces/testing.txt
	@if diff traces/kashmir-m.out traces/testing.txt > /dev/null; then echo "$(GREEN)kashmir -m\tPASSED"; else echo "$(RED)kashmir -m\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/low.trace -s > traces/testing.txt
	@if diff traces/low-s.out traces/testing.txt > /dev/null; then echo "$(GREEN)low -s\t\tPASSED"; else echo "$(RED)low -s\t\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/low.trace -l > traces/testing.txt
	@if diff traces/low-l.out traces/testing.txt > /dev/null; then echo "$(GREEN)low -l\t\tPASSED"; else echo "$(RED)low -l\t\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/low.trace -p > traces/testing.txt
	@if diff traces/low-p.out traces/testing.txt > /dev/null; then echo "$(GREEN)low -p\t\tPASSED"; else echo "$(RED)low -p\t\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/low.trace -m > traces/testing.txt
	@if diff traces/low-m.out traces/testing.txt > /dev/null; then echo "$(GREEN)low -m\t\tPASSED"; else echo "$(RED)low -m\t\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/refugee.trace -s > traces/testing.txt
	@if diff traces/refugee-s.out traces/testing.txt > /dev/null; then echo "$(GREEN)refugee -s\tPASSED"; else echo "$(RED)refugee -s\tFAILED"; fi
	@rm traces/testing.txt


	
	@./proj4 -t traces/shout.trace -s > traces/testing.txt
	@if diff traces/shout-s.out traces/testing.txt > /dev/null; then echo "$(GREEN)shout -s\tPASSED"; else echo "$(RED)shout -s\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/shout.trace -l > traces/testing.txt
	@if diff traces/shout-l-ip.out traces/testing.txt > /dev/null; then echo "$(GREEN)shout -l\tPASSED"; else echo "$(RED)shout -l\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/shout.trace -p > traces/testing.txt
	@if diff traces/shout-p.out traces/testing.txt > /dev/null; then echo "$(GREEN)shout -p\tPASSED"; else echo "$(RED)shout -p\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/shout.trace -m > traces/testing.txt
	@if diff traces/shout-m.out traces/testing.txt > /dev/null; then echo "$(GREEN)shout -m\tPASSED"; else echo "$(RED)shout -m\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/thunder.trace -s > traces/testing.txt
	@if diff traces/thunder-s.out traces/testing.txt > /dev/null; then echo "$(GREEN)thunder -s\tPASSED"; else echo "$(RED)thunder -s\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/thunder.trace -l > traces/testing.txt
	@if diff traces/thunder-l-ip.out traces/testing.txt > /dev/null; then echo "$(GREEN)thunder -l\tPASSED"; else echo "$(RED)thunder -l\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/thunder.trace -p > traces/testing.txt
	@if diff traces/thunder-p.out traces/testing.txt > /dev/null; then echo "$(GREEN)thunder -p\tPASSED"; else echo "$(RED)thunder -p\tFAILED"; fi
	@rm traces/testing.txt

	@./proj4 -t traces/thunder.trace -m > traces/testing.txt
	@if diff traces/thunder-m.out traces/testing.txt > /dev/null; then echo "$(GREEN)thunder -m\tPASSED"; else echo "$(RED)thunder -m\tFAILED"; fi
	@rm traces/testing.txt



	@./proj4 -t traces/truckin.trace -p > traces/testing.txt
	@if diff traces/truckin-p.out traces/testing.txt > /dev/null; then echo "$(GREEN)truckin -p\tPASSED"; else echo "$(RED)truckin -p\tFAILED"; fi
	@rm traces/testing.txt

clean:
	clear
	rm proj4
