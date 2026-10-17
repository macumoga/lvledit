g++ src/lvledit.cpp -c -Og -o obj/lvledit.lib -Wall -Wextra -Wpedantic
g++ src/main.cpp obj/*.lib -Og -o lvledit.exe -Wall -Wextra -Wpedantic
