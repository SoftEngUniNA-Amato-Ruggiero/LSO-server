#ifndef PROCESSPERSONALITY_H
#define PROCESSPERSONALITY_H

#include "cJSON/cJSON.h"

enum Traits {
    EXTROVERSION, AGREEABLENESS, CONSCIENTIOUSNESS, EMOTIONAL_STABILITY, OPENNESS, NUM_TRAITS
};

enum Attributes {
    EXTROVERTED, CRITICAL, DEPENDABLE, ANXIOUS, COMPLEX, RESERVED, SYMPATHETIC, DISORGANIZED, CALM, CONVENTIONAL, NUM_ATTRIBUTES
};

char *processPersonality(const char json_data[]);
int processJson(const char *json_data);
void calculatePersonality(const int *answers);
void getAnswers(const cJSON *json);
int calculateTrait(const unsigned int a, const unsigned int b);
int recode(const unsigned int i);

#endif