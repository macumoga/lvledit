basic:
	g++ src/lvledit.cpp -c -g -Ofast -o obj/lvledit.lib -Wall -Wextra -Wpedantic
	g++ src/main.cpp obj/*.lib -g -Ofast -o lvledit.uba -Wall -Wextra -Wpedantic
