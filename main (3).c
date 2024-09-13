/*-------------------------------------------
Project 1:Elementary Cellular Automaton
Course: CS 211, Summer 2024, UIC
System: Advanced zyLab system
Author: Jason Park
-------------------------------------------*/
#include <stdio.h>
#include <stdbool.h> 

const int WORLD_SIZE = 65; //constant variable for world size

typedef struct cell_struct{
    bool state[3]; //bool array that contains active status for [left, me, right] cells 
    bool active; //bool variable for current active status for this cell
} cell;

//function converts the integer rule into an 8-bit binary representation
//takes in bool array and the integer rule as parameters
//converts integer rule to binary and returns true if rule is valid, false otherwise
bool setBitArray(bool bitArray[8], int rule) {
    //checks if rule is within 0-225
    if (rule < 0 || rule > 255){ 
        return false; //returns false if rule is invalid
    }
    //integer conversion into 8-bit binary representation
    for(int i = 0; i < 8; i ++){
        bitArray[i] = rule % 2; //store LSB in array
        rule = rule / 2; //shifts the rule to the right
    }
    return true; //returns true if rule was valid
}


//function converts 3-bit state array to its index form(decimal)
//recieves a state array of size 3
//returns the index of rule bit array, essentially binary -> decimal
int stateToIndex(bool state[3]) {
    int decimal = 0; 
    //conversion of 3 bit binary array into decimal value
    decimal += state[0] * 4; //first bit contributes to 2^2
    decimal += state[1] * 2; //second bit contributes to 2^1
    decimal += state[2] * 1; //third bit contributes to 2^0
    return decimal; 
}


//function updates the state array for every cell in world array
//depends on the status of the nearby [left,me,right] cells
//takes in the struct array with world size as parameter
//no returns but updates the state array
void setStates(cell world[WORLD_SIZE]) {
    int right, left; 
    for(int index = 0; index < WORLD_SIZE; index ++){ //loops through each cell
        //cells not on edge
        if(index > 0 && index < WORLD_SIZE -1){
            right = index + 1; //right is next cell
            left = index - 1; //left is previous cell
        //first cells in array
        } else if(index == 0){
            right = index + 1; //right is second cell
            left = WORLD_SIZE - 1 ; //left wraps to last cell
        //last cells in array
        } else if(index == WORLD_SIZE - 1){
            right = 0; //right wraps round to first cell
            left = index - 1; //left is second to last cell
        }
        //update state array for the current cell
        world[index].state[2] = world[right].active; //state[2] is active status for right
        world[index].state[1] = world[index].active; //state[1] is active status for current
        world[index].state[0] = world[left].active; //state[0] is active status for left    
    }
}


//evolves each cell's active status to next generation
//parameters are the struct world array and rule bit array(reversed) 
//no returns, but evolves the world to next generation
void evolveWorld(cell world[WORLD_SIZE], bool ruleBitArray[8]) {
    //loop that determine next state based on given rule
    for(int i = 0; i < WORLD_SIZE; i++){
        int index = stateToIndex(world[i].state); //converts cell's state to an index 
        world[i].state[1] = ruleBitArray[index]; //update the middle status using the index
    }
    //loop that updates active status to a new state
    for(int i = 0; i < WORLD_SIZE; i ++){
        world[i].active = world[i].state[1]; //sets the active status using prev update
    }
}


//additional function created to aid task 6 and 8
//takes in the struct array of world size
//no returns, but prints each active and inactive cells accordingly
void printWorld(cell world[WORLD_SIZE]){
    //loop through each cell and prints '*' for active and ' ' for inactive cells
    for (int i = 0; i < WORLD_SIZE; i++) {
        if (world[i].active) {
            printf("*"); //active
        } else {
            printf(" "); //inactive
        }
    }
    printf("\n");
}


int main() {
    cell world[WORLD_SIZE];//initializes struct array of cells that represents the world
    bool ruleBitArray[8]; //initializes array for 8-bit rule array
    int rule = -1;//initializes rule as invalid
    printf("Welcome to the Elementary Cellular Automaton!\n");

    //asks the user to enter a rule number until the number is valid
    while(setBitArray(ruleBitArray, rule) == false){
        printf("Enter the rule # (0-255): ");
        scanf("%d", &rule);
    }
    printf("\n");
    printf("The bit array for rule #%d is ", rule);

    //prints the reversed rule bit array in correct order
    for(int i = 7; i >= 0; i--){
        printf("%d", ruleBitArray[i]);
    }
     
    //array that represents the top state of cells
    char *topStates[8] = {"***", "** ", "* *", "*  ", " **", " * ", "  *", "   " };
    printf("\n\n");
    printf("The evolution of all possible states are as follows:\n");

    //loop that prints the top state depending on cell state
    for(int i = 0; i < 8; i++){
        printf("|"); //line for new state 
        printf("%s", topStates[i]); //prints the actual state
        if(i != 7){
            printf("|   "); //whitespace between each state
        } else{
            printf("|"); //since last state, no whitespace 
        }
    }
    printf("\n");

    //loop that prints based on evolution rule
    for(int i = 7; i >= 0; i--){
        if(ruleBitArray[i]){
            printf(" |*|    "); //if ruleBit is true, prints '*
        }else if(!ruleBitArray[i] && i != 0){
            printf(" | |    "); //if not the last bit and false, print with more whitespace
        } else{
            printf(" | |"); //if last bit, print with less whitespace
        }
    }
    printf("\n\n");
    
    int totalGeneration; //intialized for storing number of generations
    printf("Enter the number of generations: ");
    scanf("%d", &totalGeneration); //takes input of number of generations
    printf("\nInitializing world & evolving...\n");

    //loop that sets all cells inactive except the middle cell
    for(int i = 0; i < WORLD_SIZE; i++){
        world[i].active = false;
    }
    world[WORLD_SIZE / 2].active = true;
    setStates(world); //calls setStates function to set initial state
    printWorld(world); //calls printWorld to print the initial state of world

    //loop that evolves the world with generation
    for(int i = 0; i < totalGeneration - 1; i++){
        evolveWorld(world, ruleBitArray); //evolves the world to next generation
        setStates(world); //updates the cell states based on neighbors
        printWorld(world); //prints the current world
    }
    return 0;
}
