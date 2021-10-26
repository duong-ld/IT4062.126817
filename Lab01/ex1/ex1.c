#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void makeStrLower(char* str) {
  for (int i = 0; i < strlen(str); i++) {
    if ('A' <= str[i] && str[i] <= 'Z') {
      str[i] = str[i] + 32;
    }
  }
}

void getMaterial(char input, char* material) {
  switch (input) {
    case '1':
      strcpy(material, "Jets");
      break;
    case '2':
      strcpy(material, "Fog");
      break;
    case '3':
      strcpy(material, "Foam");
      break;
    case '4':
      strcpy(material, "Dry agent");
      break;
    default:
      printf("Invalid HAZCHEM code\n");
      exit(1);
  }
}

void getReactivity(char input, char* reactivity) {
  switch (input) {
    case 'p': case 's': case 'w': case 'y': case 'z':
      strcpy(reactivity, "Can be violently reactive");
      break;
    case 'r': case 't': case 'x':
      strcpy(reactivity, "None");
      break;
    default:
      printf("Invalid HAZCHEM code\n");
      exit(1);
  }
}

void getProtection(char input, int reverse, char* protection) {
  switch (input) {
    case 'p': case 'r': case 'w': case 'x':
      strcpy(protection, "Full protective clothing must be worn");
      break;
    case 's': case 't': case 'y': case 'z':
      if (reverse) {
        strcpy(protection,
               "Breathing apparatus, protective gloves for fire only");
      } else {
        strcpy(protection, "Breathing apparatus");
      }
      break;
    default:
      printf("Invalid HAZCHEM code\n");
      exit(1);
  }
}

void getContainment(char input, char* containment) {
  switch (input) {
    case 'p': case 'r': case 's': case 't':
      strcpy(containment,
             "The dangerous goods may be washed down to a drain with a large quantity of water");
      break;
    case 'w': case 'x': case 'y': case 'z':
      strcpy(containment,
             "A need to avoid spillages from entering drains or water courses");
      break;
    default:
      printf("Invalid HAZCHEM code\n");
      exit(1);
  }
}

void getEvacuation(char input, char* evacuation) {
  if (input == '\0') {
    strcpy(evacuation, "None");
  } else if (input == 'e') {
    strcpy(evacuation, "Consider Evacuation");
  } else {
    printf("Invalid HAZCHEM code\n");
    exit(1);
  }
}

void getInput(char* input, int* reverse) {
  char reverseInput[10] = "\0";
  printf("Enter HAZCHEM code: ");
  scanf("%9s", input);
  makeStrLower(input);

  if (strlen(input) < 2 || strlen(input) > 3) {
    printf("Invalid HAZCHEM code\n");
    exit(1);
  }

  if (input[1] == 's' || input[1] == 't' || input[1] == 'y' ||
      input[1] == 'z') {
    printf("Is the %c reverse coloured?: ", input[1] - 32);
    scanf("%9s", reverseInput);
    makeStrLower(reverseInput);
  } else {
    *reverse = 0;
    return;
  }

  if (strcmp(reverseInput, "yes") == 0) {
    *reverse = 1;
  } else if (strcmp(reverseInput, "no") == 0) {
    *reverse = 0;
  } else {
    printf("Invalid Reverse input >> NO\n");
    *reverse = 0;
  }
}
int main(int argc, char const* argv[]) {
  char input[10] = "\0";
  char material[100] = "\0";
  char reactivity[100] = "\0";
  char protection[100] = "\0";
  char containment[100] = "\0";
  char Evacuation[100] = "\0";
  int reverse = 0;
  getInput(input, &reverse);
  getMaterial(input[0], material);
  getReactivity(input[1], reactivity);
  getProtection(input[1], reverse, protection);
  getContainment(input[1], containment);
  getEvacuation(input[2], Evacuation);
  printf("\n\n***Emergency action advice***\n");
  printf("Material: \t%s\n", material);
  printf("Reactivity: \t%s\n", reactivity);
  printf("Protection: \t%s\n", protection);
  printf("Containment: \t%s\n", containment);
  printf("Evacuation: \t%s\n", Evacuation);
  printf("*****************************\n");
  return 0;
}
