#include <cstdlib>

float lossProbability = 0.05;

bool SimulateLoss() {
    float randomValue = (float)rand() / RAND_MAX; 
    return randomValue < lossProbability; 
}

void LoopTransportBlocks(TransportBlock tb) {
    if (SimulateLoss()) {
        Log("Transport Block dropped!");
        return;
    }

    Push_DownlinkQueue(tb);
}