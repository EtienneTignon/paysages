main: main.cpp Calc_feature.cpp Fitness.cpp Instance.cpp Solution.cpp Voisinage.cpp
	g++ -o main main.cpp Calc_feature.cpp Fitness.cpp Instance.cpp Solution.cpp Voisinage.cpp -g -fopenmp -fsanitize=address -Wall -std=c++11
	#-fsanitize=address

clear:
	rm main