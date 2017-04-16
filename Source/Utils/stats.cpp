//
// Created by ghiles on 4/16/17.
//

#include "stats.h"

float stats::mean(float t[], int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += t[i];
    }
    return sum / size;
}