#include "../Headers/GameFlow.h"

int main() {
	GameFlow* flow = GameFlow::getInstance();
	flow->run();
}