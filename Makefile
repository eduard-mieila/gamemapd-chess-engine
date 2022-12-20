build:
	g++ -Wall -Wextra -std=c++17 *.cpp -o gamemapd_engine -g

run: build
	./gamemapd_engine

clean:
	rm -f gamemapd_engine
