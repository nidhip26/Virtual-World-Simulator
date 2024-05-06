/*Nidhi Patel
*
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct room{
    int id;
    int state;
    struct room *NorthRoom;
    struct room *SouthRoom;
    struct room *EastRoom;
    struct room *WestRoom;
    struct creature *CreatureinRoom[10];
    int CreatureSize;
}room;


typedef struct creature{
    int id;
    int type;
    struct room *location;
}creature;


//Global variables
int respect = 40;
room* Rooms;
creature *Creatures;
creature *PC;

void printRespect(){ //prints respect value
    printf("Respect is now %d. ", respect);
}

void CreatureRoom(int numRooms, int numCreatures){ //puts creatures in room at the start
    for (int i = 0; i < numRooms; i++){
        for (int j = 0; j < numCreatures; j++){
            if (Creatures[j].location == &Rooms[i]){
                int var = Rooms[i].CreatureSize;
                Rooms[i].CreatureinRoom[var] = &Creatures[j];
                Rooms[i].CreatureSize += 1;
            }
        }
    }
}

char *StrCreatureType(int CreType){ //gives string name of creature type
    char *StrCreature;

    if (CreType == 0){
        StrCreature = "PC";
    }
    else if (CreType == 1) {
        StrCreature= "animal";
    }
    else{
        StrCreature = "human";
    }
    return StrCreature;
}

void adjustCreatureArray(room *old_room);

void look(void){ //prints out room's state, creatures,and neighbors
    printf("Room ");
    printf("%d", PC->location->id);
    int state;
    state = PC->location->state;
    char *StrState;
    if (state == 0){
        StrState = "clean";
    }
    else if (state == 1) {
        StrState= "half-dirty";
    }
    else {
        StrState = "dirty";
    }

    printf(", %s, ", StrState);
    printf("neighbors ");
    int TotalNeighbors = 0;
    if (PC->location->NorthRoom != NULL){
        TotalNeighbors +=1;
        printf("%d to the north, ", PC->location->NorthRoom->id);
    }
    if (PC->location->SouthRoom != NULL){
        TotalNeighbors +=1;
        printf("%d to the south, ", PC->location->SouthRoom->id);
    }
    if (PC->location->EastRoom != NULL){
        TotalNeighbors +=1;
        printf("%d to the east, ", PC->location->EastRoom->id);
    }
    if (PC->location->WestRoom != NULL){
        TotalNeighbors +=1;
        printf("%d to the west, ", PC->location->WestRoom->id);
    }
    if (TotalNeighbors == 0){
        printf("none, ");
    }
    printf(" contains:");

    int max_size = PC->location->CreatureSize;
    for (int i = 0; i<max_size; i++){ //printing creatures in the room
        adjustCreatureArray(PC->location);
        if (PC->location->CreatureinRoom[i]!= NULL) {
            if (PC->location->CreatureinRoom[i]->type == 0){
                printf("\nPC %d", PC->location->CreatureinRoom[i]->id);
            }
            else if (PC->location->CreatureinRoom[i]->type == 1){
                printf("\nAnimal %d", PC->location->CreatureinRoom[i]->id);
            }
            else if (PC->location->CreatureinRoom[i]->type == 2){
                printf("\nHuman %d", PC->location->CreatureinRoom[i]->id);
            }
            //printf("\n%s %d", StrCreatureType(PC->location->CreatureinRoom[i]->type),PC->location->CreatureinRoom[i]->id);
        }
    }
}

void creatureLeavesRoom(creature *old, room *old_room){ //references to creature in old room are deleted
    for (int i = 0; i < old_room->CreatureSize; i++){
        if (old->id == old_room->CreatureinRoom[i]->id){
            old_room->CreatureSize = old_room->CreatureSize -1;
            old_room->CreatureinRoom[i] = NULL;
            old ->location = NULL;
            break;
        }
    }
}

void creatureLeavesandEnters(creature *old, room *old_room, room *new_room){
    //references to old room are deleted and new references to new room added
    for (int i = 0; i < old_room->CreatureSize; i++){
        if (old_room->CreatureinRoom[i] != NULL){
            if (old->id == old_room->CreatureinRoom[i]->id) {
                old_room->CreatureSize = old_room->CreatureSize -1;
                old_room->CreatureinRoom[i] = NULL;
                old->location = NULL;
                break;
            }
        }
    }
    //linking creature to new room
    old->location = new_room;
    new_room ->CreatureinRoom[new_room->CreatureSize] = old;
    new_room->CreatureSize = new_room -> CreatureSize+1;
}

void PCtoNorth(){ //moves PC to the north
    if (PC->location->NorthRoom != NULL){
        if (PC->location->NorthRoom->CreatureSize == 10){
            printf("Room to the north is full!");
        }
        else{
            PC->location = PC-> location->NorthRoom;
            creatureLeavesandEnters(PC, PC->location->SouthRoom, PC->location);
            adjustCreatureArray(PC->location->SouthRoom);
            PC-> location->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the north!");
        }
    }
    else{
        printf("No room to the north!");
    }
}

void PCtoSouth(){ //moves PC to the south
    if (PC->location->SouthRoom != NULL){
        if (PC->location->SouthRoom->CreatureSize == 10){
            printf("Room to the south is full!");
        }
        else{
            PC->location = PC-> location->SouthRoom;
            creatureLeavesandEnters(PC, PC->location->NorthRoom, PC->location);
            adjustCreatureArray(PC->location->NorthRoom);
            PC-> location->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the south!");
        }
    }
    else{
        printf("No room to the south!");
    }
}

void PCtoEast(){ //moves PC to the east
    if (PC->location->EastRoom != NULL){
        if (PC->location->EastRoom->CreatureSize == 10){
            printf("Room to the east is full!");
        }
        else{
            PC->location = PC-> location->EastRoom;
            creatureLeavesandEnters(PC, PC->location->WestRoom, PC->location);
            adjustCreatureArray(PC->location->WestRoom);
            PC-> location->CreatureSize = PC-> location->CreatureSize + 1;
            printf("You leave towards the east!");
        }
    }
    else{
        printf("No room to the east!");
    }
}

void PCtoWest(){ //moves PC to the west
    if (PC->location->WestRoom != NULL){
        if (PC->location->WestRoom->CreatureSize == 10){
            printf("Room to the west is full!");
        }
        else{
            PC->location = PC-> location->WestRoom;
            creatureLeavesandEnters(PC, PC->location->EastRoom, PC->location);
            adjustCreatureArray(PC->location->EastRoom);
            PC-> location->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the west!");
        }
    }
    else{
        printf("No room to the west!");
    }
}

void adjustCreatureArray(room *old_room){ //null creatures are removed from creature array
    int new_index = 0;
    for (int i = 0; i< old_room->CreatureSize; i++){
        if (old_room ->CreatureinRoom[i] != NULL){
            old_room->CreatureinRoom[new_index] = old_room->CreatureinRoom[i];
            new_index += 1;
        }
    }
}

int checkForType(int type, room *curr){ //checks if creature of type exists in room
    for (int i = 0; i < 10; i++){
        if (curr->CreatureinRoom[i]->type == type){
            return 1;
            break;
        }
        else{
            return 0;
        }
    }
}

void CreaturesDiscontent(int type, room *curr){ //negative reactions to actions
    for (int i = 0; i<curr->CreatureSize; i++){
        if ((curr->CreatureinRoom[i] !=NULL) &&(curr->CreatureinRoom[i]->type == type)){
            if (type==1) { //animal growls
                printf("\n%s %d growls. ", StrCreatureType(type), curr->CreatureinRoom[i]->id);
                respect -= 1;
                printRespect();

            }
            else if (type ==2){ //npc grumbles
                printf("\n%s %d grumbles. ", StrCreatureType(type), curr->CreatureinRoom[i]->id);
                respect -= 1;
                printRespect();
            }
        }
    }
}

void CreaturesContent(int type, room *curr){ //positive reactions to actions
    for (int i = 0; i<curr->CreatureSize; i++){
        if ((curr->CreatureinRoom[i] !=NULL) &&(curr->CreatureinRoom[i]->type == type)){
            if (type==1) { //animal licks
                printf("\n%s %d licks face. ", StrCreatureType(type), curr->CreatureinRoom[i]->id);
                respect += 1;
                printRespect();
            }
            else if (type ==2){ //npc smiles
                printf("\n%s %d smiles. ", StrCreatureType(type), curr->CreatureinRoom[i]->id);
                respect += 1;
                printRespect();
            }
        }
    }
}

void move(creature *moving);
void clean(){ //PC cleans current room
    if (PC->location->state == 0){
        PC->location->state = 0;
    }
    else if (PC->location->state == 1){
        PC->location->state = 0;

        CreaturesDiscontent(2,PC->location);
        //moving NPCs elsewhere
        for (int i = 0; i < PC->location->CreatureSize; i++){
            if (PC->location->CreatureinRoom[i]!=NULL && PC->location->CreatureinRoom[i]->type == 2){
                move(PC->location->CreatureinRoom[i]);
            }
        }

        CreaturesContent(1,PC->location);

    }
    else if(PC->location->state == 2){
        PC->location->state = 1;
        CreaturesDiscontent(2,PC->location);
        CreaturesContent(1,PC->location);

    }
}

void dirty(){ //PC makes room dirty
    if (PC->location->state == 0){
        PC->location->state = 1;
        CreaturesDiscontent(1,PC->location);
        CreaturesContent(2,PC->location);

    }
    else if (PC->location->state == 1){
        PC->location->state = 2;
        CreaturesDiscontent(1,PC->location);
            //move animals
        for (int i = 0; i < PC->location->CreatureSize; i++){
            if (PC->location->CreatureinRoom[i] != NULL && PC->location->CreatureinRoom[i]->type == 1){
                move(PC->location->CreatureinRoom[i]);
            }
        }

        CreaturesContent(2,PC->location);

    }
    else if(PC->location->state == 2){
        PC->location->state = 2;
    }
}


void creatureChangesState(creature *moving){ //creature changes state to satisfy them
    if (moving->location->state == 2){
        if (moving->type == 1){
            moving->location->state = moving->location->state-1;
        }
    }
    else if (moving->location->state == 0){
        if (moving->type == 2){
            moving->location->state =moving->location->state + 1;
        }
    }
}

int checkForEmpty(room *curr){ //checks if room is full
    if (curr->CreatureSize == 10){
        return 0; //false
    }
    else{
        return 1; //true
    }
}

void randomize(int array[], int j) { //mixes up array order
    for (int i = j - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap arr[i] and arr[j]
        int val = array[i];
        array[i] = array[j];
        array[j] = val;
    }
}

void move(creature *moving){ //moves creature in random room
    int num[4] = {1, 2, 3, 4};
    srand(time(NULL));

    randomize(num, 4); //mixes the array order
    for (int i = 0; i<4; i++){
        if (moving->location->NorthRoom == NULL && moving->location->SouthRoom == NULL
        && moving->location->EastRoom == NULL &&moving->location->WestRoom == NULL){
            printf("\n%s %d has drilled a hole and left the room!", StrCreatureType(moving->type), moving->id);
            creatureLeavesRoom(moving, moving->location);
            //adjustCreatureArray(moving->location);
            break;
        }
//      else if (!(checkForEmpty(moving->location->NorthRoom)) && !(checkForEmpty(moving->location->SouthRoom)) && !(checkForEmpty(moving->location->EastRoom))&& !(checkForEmpty(moving->location->WestRoom))){
//            printf("\n%s %d has drilled a hole and left the room!", StrCreatureType(moving->type), moving->id);
//            creatureLeavesRoom(moving, moving->location);
//        }
        else if (num[i]==1){
            if (moving->location->NorthRoom != NULL && checkForEmpty(moving->location->NorthRoom)){
                printf("%d leave towards the north.", moving->id);
                creatureLeavesandEnters(moving, moving->location, moving->location->NorthRoom);
                creatureChangesState(moving);
                adjustCreatureArray(moving->location->SouthRoom);
                break;
            }
        }
        else if (num[i]==2){
            if (moving->location->SouthRoom != NULL && checkForEmpty(moving->location->SouthRoom)){
                printf("%d leave towards the south.", moving->id);
                creatureLeavesandEnters(moving, moving->location, moving->location->SouthRoom);
                creatureChangesState(moving);
                adjustCreatureArray(moving->location->NorthRoom);
                break;
            }
        }
        else if (num[i]==3){
            if (moving->location->EastRoom != NULL && checkForEmpty(moving->location->EastRoom)){
                printf("%d leave towards the east.", moving->id);
                creatureLeavesandEnters(moving, moving->location, moving->location->EastRoom);
                creatureChangesState(moving);
                adjustCreatureArray(moving->location->WestRoom);
                break;
            }
        }
        else if (num[i]==4){
            if (moving->location->WestRoom != NULL && checkForEmpty(moving->location->WestRoom)){
                printf("%d leave towards the west.", moving->id);
                creatureLeavesandEnters(moving, moving->location, moving->location->WestRoom);
                creatureChangesState(moving);
                adjustCreatureArray(moving->location->EastRoom);
                break;
            }
        }
        else{
            printf("\n%s %d has drilled a hole and left the room!", StrCreatureType(moving->type), moving->id);
            creatureLeavesRoom(moving, moving->location);
            adjustCreatureArray(moving->location);
            break;
        }
    }
}

void CreatureClean(int id){ //makes creature clean
    int old_state = PC->location->state;
    int type = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                break;
            }
        }
    }

    if (old_state == 0){
        printf("\nRoom is clean already! Nothing changes.");
    }
    else if(old_state == 1){
        PC->location->state = 0;
        if (type == 1){ //animal cleans
            printf("\n%d licks your face a lot. ", id);
            respect += 2;
            CreaturesContent(1, PC->location);
            CreaturesDiscontent(2, PC->location);
            //printRespect();
        }
        else if (type ==2){ //human cleans
            printf("\n%d grumbles a lot. ", id);
            respect -= 2;
            CreaturesContent(1, PC->location);
            CreaturesDiscontent(2, PC->location);
            //printRespect();
        }
        for (int i = 0; i<PC->location->CreatureSize; i++){ //moves npcs out of room
            if ((PC->location->CreatureinRoom[i] !=NULL)&&(PC->location->CreatureinRoom[i]->type == 2)){
                move(PC->location->CreatureinRoom[i]);
            }
        }
    }
    else if(old_state == 2){
        PC->location->state = 1;
        if (type == 1){ //animal cleans
            printf("\n%d licks your face a lot. ", id);
            respect += 2;
            CreaturesContent(1, PC->location);
            CreaturesDiscontent(2, PC->location);
            //printRespect();
        }
        else if (type ==2){ //human cleans
            printf("\n%d grumbles a lot. ", id);
            respect -= 2;
            CreaturesContent(1, PC->location);
            CreaturesDiscontent(2, PC->location);
            //printRespect();
        }

    }
}

void CreatureDirty(int id){ //makes creature dirty the room
    int old_state = PC->location->state;
    int type = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                break;
            }
        }
    }

    if (old_state == 2){
        printf("\nRoom is dirty already! Nothing changes.");
    }
    else if(old_state == 1){
        PC->location->state = 2;
        if (type == 1){ //animal dirtys
            printf("\n%d growls a lot. ", id);
            respect -= 2;
            CreaturesContent(2, PC->location);
            CreaturesDiscontent(1, PC->location);
            printRespect();
        }
        else if (type ==2){ //human dirtys
            printf("\n%d smiles a lot. ", id);
            respect += 2;
            CreaturesContent(2, PC->location);
            CreaturesDiscontent(1, PC->location);
            //printRespect();
        }
        for (int i = 0; i<PC->location->CreatureSize; i++){
            if ((PC->location->CreatureinRoom[i] !=NULL)&&(PC->location->CreatureinRoom[i]->type == 1)){
                move(PC->location->CreatureinRoom[i]);
            }
        }
    }
    else if(old_state == 0){
        PC->location->state = 1;
        if (type == 1){ //animal dirtys
            printf("\n%d growls a lot. ", id);
            respect -= 2;
            CreaturesContent(2, PC->location);
            CreaturesDiscontent(1, PC->location);
            //printRespect();
        }
        else if (type ==2){ //human dirtys
            printf("\n%d smiles a lot. ", id);
            respect += 2;
            CreaturesContent(2, PC->location);
            CreaturesDiscontent(1, PC->location);
            //printRespect();
        }
    }
}

void CreatureNorth(int id){ //
    int type = 0;
    int index = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                index = i;
                break;
            }
        }
    }

    if (PC->location->NorthRoom != NULL){
        if (PC->location->NorthRoom->CreatureSize == 10){
            printf("Room to the north is full!");
        }
        else{
            PC->location->CreatureinRoom[index]->location = PC-> location->NorthRoom;
            creatureLeavesandEnters(PC->location->CreatureinRoom[index], PC->location, PC->location->NorthRoom);
            adjustCreatureArray(PC->location);
            PC-> location->NorthRoom->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the north!");
        }
    }
    else{
        printf("No room to the north!");
    }
}
void CreatureSouth(int id){ //
    int type = 0;
    int index = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                index = i;
                break;
            }
        }
    }

    if (PC->location->SouthRoom != NULL){
        if (PC->location->SouthRoom->CreatureSize == 10){
            printf("Room to the north is full!");
        }
        else{
            PC->location->CreatureinRoom[index]->location = PC-> location->SouthRoom;
            creatureLeavesandEnters(PC->location->CreatureinRoom[index], PC->location, PC->location->SouthRoom);
            adjustCreatureArray(PC->location);
            PC-> location->SouthRoom->CreatureSize = PC-> location->CreatureSize+1;
            printf("Creature leave towards the south!");
        }
    }
    else{
        printf("No room to the south!");
    }
}

void CreatureEast(int id){ //
    int type = 0;
    int index = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                index = i;
                break;
            }
        }
    }

    if (PC->location->EastRoom != NULL){
        if (PC->location->EastRoom->CreatureSize == 10){
            printf("Room to the east is full!");
        }
        else{
            PC->location->CreatureinRoom[index]->location = PC-> location->EastRoom;
            creatureLeavesandEnters(PC->location->CreatureinRoom[index], PC->location, PC->location->EastRoom);
            adjustCreatureArray(PC->location);
            PC-> location->EastRoom->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the east!");
        }
    }
    else{
        printf("No room to the east!");
    }
}

void CreatureWest(int id){ //
    int type = 0;
    int index = 0;
    for (int i = 0; i<PC->location->CreatureSize; i++){
        if (PC->location->CreatureinRoom[i] != NULL) {
            if (PC->location->CreatureinRoom[i]->id == id) {
                type = PC->location->CreatureinRoom[i]->type;
                index = i;
                break;
            }
        }
    }

    if (PC->location->WestRoom != NULL){
        if (PC->location->WestRoom->CreatureSize == 10){
            printf("Room to the west is full!");
        }
        else{
            PC->location->CreatureinRoom[index]->location = PC-> location->WestRoom;
            creatureLeavesandEnters(PC->location->CreatureinRoom[index], PC->location, PC->location->WestRoom);
            adjustCreatureArray(PC->location);
            PC-> location->WestRoom->CreatureSize = PC-> location->CreatureSize+1;
            printf("You leave towards the west!");
        }
    }
    else{
        printf("No room to the west!");
    }
}

int main( )
{
    printf("Input:\n");
    int numRooms;
    printf("How many rooms?: ");
    scanf("%d", &numRooms);
    Rooms = (room*) malloc(numRooms * sizeof(room));

    printf("Input the Room States/Neighbors: ");
    printf("\n5 integers should be inputted for each room; \n0 for clean, 1 for half-dirty, 2 for dirty. -1 for no neighbor.\n");

    //struct room RoomArray[numRooms];
    for (int i = 0; i < numRooms; i++) {

        int State = 0;
        int NorthState = 0;
        int SouthState = 0;
        int EastState = 0;
        int WestState = 0;
        printf("Room %d: ", i);
        scanf("%d %d %d %d %d", &State, &NorthState, &SouthState, &EastState, &WestState);
        Rooms[i].id = i;
        Rooms[i].state = State;

        if (NorthState==-1){
            Rooms[i].NorthRoom = NULL;
        }
        else{
            Rooms[i].NorthRoom = &Rooms[NorthState];
        }
        if (SouthState==-1){
            Rooms[i].SouthRoom = NULL;
        }
        else{
            Rooms[i].SouthRoom = &Rooms[SouthState];
        }
        if (EastState==-1){
            Rooms[i].EastRoom = NULL;
        }
        else{
            Rooms[i].EastRoom = &Rooms[EastState];
        }
        if (WestState==-1){
            Rooms[i].WestRoom = NULL;
        }
        else{
            Rooms[i].WestRoom = &Rooms[WestState];
        }

    }

    int numCreatures;
    printf("\n# of Creatures: ");
    scanf("%d", &numCreatures);
    Creatures = (creature*) malloc(numCreatures * sizeof(creature));

    printf("Enter the creatureType and location for each creature.");
    printf("\n(Two integers; creatureType is 0 for the PC, 1 for an animal, 2 for an NPC)\n");


    for (int i= 0; i < numCreatures; i++) {
        int CreatureType;
        int RoomLocation;
        printf("Creature %d: ", i);
        scanf("%d %d", &CreatureType, &RoomLocation);
        Creatures[i].type = CreatureType;
        if (Creatures[i].type == 0){
            PC = &Creatures[i];
        }
        Creatures[i].location = &Rooms[RoomLocation];
        Creatures[i].id = i;
    }

    //moves creatures to the rooms
    CreatureRoom(numRooms, numCreatures);


    while(respect >0) {

        char choice[20];
        printf("\nEnter Command: ");
        scanf("%s", choice);
        int switchCase = 0;
        int input_id = 0;
        if (strcmp(choice, "look") == 0) {
            switchCase = 1;
        } else if (strcmp(choice, "clean") == 0) {
            switchCase = 2;
        }
        else if (strcmp(choice, "dirty") == 0) {
            switchCase = 15;
        }
        else if ((strcmp(choice, "north") == 0)){
            switchCase = 3;
        }
        else if (strcmp(choice, "south") == 0) {
            switchCase = 4;
        }
        else if (strcmp(choice, "east") == 0) {
            switchCase = 5;
        }
        else if (strcmp(choice, "west") == 0) {
            switchCase = 6;
        }
        else if (strcmp(choice, "exit") == 0) {
            switchCase = 7;
        }
        else if (strcmp(choice, "help") == 0) {
            switchCase = 10;
        }
        else if (choice[0] > 47 && choice[0]<58){
            input_id = atoi(choice);
            int colon_index=0;
            char action[10];

            for (int j = 0; j < 20; j++){
                if (choice[j] == ':'){
                    colon_index = j;
                    break;
                }
            }
            int k = 0;
            for (int j = colon_index+1; j < 20; j++){
                if ((choice[j] =='\n') || (choice[j]=='\0')){
                    break;
                }
                action[k] = choice[j];
                k++;
            }
            //printf("%d\n", colon_index);
            //printf("%s\n", action);

            if (strstr(choice, "clean") == 0){
                switchCase = 8;
            }
            else if ((strstr(choice, "dirty") == 0)){
                switchCase  = 9;
            }
            else if ((strstr(choice, "north") == 0)){
                switchCase  = 11;
            }
            else if ((strstr(choice, "south") == 0)){
                switchCase  = 12;
            }
            else if ((strstr(choice, "east") == 0)){
                switchCase  = 13;
            }
            else if ((strstr(choice, "west") == 0)){
                switchCase  = 14;
            }
        }
        switch (switchCase) {
            case 1: //look
                look();
                break;
            case 2: //clean
                clean();
                break;
            case 3: //north
                PCtoNorth();
                break;
            case 4: //south
                PCtoSouth();
                break;
            case 5: //east
                PCtoEast();
                break;
            case 6: //west
                PCtoWest();
                break;
            case 7: //exit
                if (respect>40){
                    printf("Congrats, you won! Respect was %d", respect);
                }
                else{
                    printf("You lost! Your respect isn't over 40. Respect is %d", respect);
                }
                free(Rooms);
                free(Creatures);
                exit(0);
                break;
            case 8: //NUMBER:dirty
                CreatureDirty(input_id);
                break;
            case 9: //NUMBER:clean
                CreatureClean(input_id);
                break;
            case 10:
                printf("List of commands: \n"
                       "look: See the name, state, and description of the current room\n"
                       "clean: Can clean the current room and change the state\n"
                       "north: player moves to the room in the north direction\n"
                       "south: player moves to the room in the south direction\n"
                       "east: player moves to the room in the east direction\n"
                       "west: player moves to the room in the west direction\n"
                       "id of creature:clean/dirty: Specifies which creature should clean or dirty the room\n"
                       "id of creature:(north/south/east/west): Specifies which creature should move where");
                break;
            case 11: //creature:north
                printf("yeah");
                CreatureNorth(input_id);
                break;
            case 12://creature:south
                CreatureSouth(input_id);
                break;
            case 13://creature:east
                CreatureEast(input_id);
                break;
            case 14://creature:west
                CreatureWest(input_id);
                break;
            case 15: //dirty
                dirty();
                break;
            default:
                printf("Don't recognize input! Try again. \n");
                printf("->");
                break;

        }

    }
    printf("You lost! Respect was less than 0.");
}
