#include "processPersonality.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *trait_names[NUM_TRAITS] = {
    "EXTROVERSION", "AGREEABLENESS", "CONSCIENTIOUSNESS", "EMOTIONAL_STABILITY", "OPENNESS"
};

const char *attribute_names[NUM_ATTRIBUTES] = {
    "EXTROVERTED", "CRITICAL", "DEPENDABLE", "ANXIOUS", "COMPLEX", "RESERVED", "SYMPATHETIC", "DISORGANIZED", "CALM", "CONVENTIONAL"
};

int attribute_values[NUM_ATTRIBUTES];
int personality_traits[NUM_TRAITS];

int processPersonality(const char json_data[], char *personality) {
    if (processJson(json_data) != 0){
        return -1;
    }

    calculatePersonality(attribute_values);

    cJSON *json = cJSON_CreateObject();
    for (int i = 0; i < NUM_TRAITS; i++) {
        cJSON_AddNumberToObject(json, trait_names[i], personality_traits[i]);
    }
    
    char *json_string = cJSON_Print(json);
    if (!json_string) {
        perror("Error creating JSON string\n");
        cJSON_Delete(json);
        return -1;
    }
    
    strncpy(personality, json_string, MAX_STRING_LENGTH - 1);

    free(json_string);
    json_string = NULL;    

    personality[MAX_STRING_LENGTH - 1] = '\0';

    cJSON_Delete(json);
    return 0;
}

int processJson(const char json_data[]) {
    if (!json_data) {
        return 1;
    }

    cJSON *json = cJSON_Parse(json_data);
    if (!json) {
        perror("Error parsing JSON\n");
        return 1;
    }

    getAnswers(json);
    cJSON_Delete(json);
    return 0;
}

void getAnswers(const cJSON *json){
    for (int i = 0; i < NUM_ATTRIBUTES; i++){
        attribute_values[i] = cJSON_GetObjectItem(json, attribute_names[i])->valueint;
    }
}

void calculatePersonality(const int answers[]) {
    personality_traits[EXTROVERSION] = calculateTrait(answers[EXTROVERTED], answers[RESERVED]);
    personality_traits[AGREEABLENESS] = calculateTrait(answers[SYMPATHETIC], answers[CRITICAL]);
    personality_traits[CONSCIENTIOUSNESS] = calculateTrait(answers[DEPENDABLE], answers[DISORGANIZED]);
    personality_traits[EMOTIONAL_STABILITY] = calculateTrait(answers[CALM], answers[ANXIOUS]);
    personality_traits[OPENNESS] = calculateTrait(answers[COMPLEX], answers[CONVENTIONAL]);
}

inline int calculateTrait(const unsigned int a, const unsigned int b) {
    return (a + recode(b)) >> 1;
}

inline int recode(const unsigned int i) {
    return 8 - i;
}