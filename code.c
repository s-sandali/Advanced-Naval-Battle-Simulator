#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Define constants for maximum values
#define PI 3.14159265359
#define GRAVITY 9.81 // m/s^2
#define MAX_ESCORT_SHIPS 30
#define MAX_BATTLESHIP_TYPES 4
#define MAX_ESCORT_SHIP_TYPES 5
#define MAX_SIZE 100
#define MAX_LINE_LENGTH 9000


// to store escortship details
typedef struct {
    char notation[10];
    char Name[50];
    char gunName[50];
    double impactPower;
    int range;
    int minAngle;
    int maxAngle;
    int minVelocity;
    int maxVelocity;
    int velocity;
    double innerRadius; // Inner radius of the attack range
    double outerRadius; // Outer radius of the attack range
    double attackRange;
} Etype;

// to store battleship details
typedef struct {
    char notation[10];
    char name[50];
    char gunName[50];
    double BattleVelocity;
    double Angle;
} Btype;

//to store battleship position
typedef struct {
    int x;
    int y;
    int BattleVelocity;
    double Angle; 
} Battleship;

//to store escortship position
typedef struct {
    int x;
    int y;
    char type;
} EscortShip;

// to store the user generated point postions
typedef struct {
    int x;
    int y;
} Point;


// Function prototypes
void mainMenu();
void startSimulation();
void simulationStatistics(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile);
void instructionsMenu();
void exitProgram();
void setupMenu();
void mission();
void instructionsMenuLevel1();
void instructionsMenuLevel2();
void instructionsMenuLevel3();
void battleshipProperties(Btype battleshipTypes[]);
void escortshipProperties(Etype escortShipTypes[] );
void displayFileContents(const char *filename);
void simulation1(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile);
void simulation2(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile);
void simulation3(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile);
int getUserInput();
void setupSimulation(Etype escortShipTypes[], Btype battleshipTypes[], int BattleVelocity);
Etype *initialCon1(char notation[], char Name[], char gunName[], double impactPower, int range, int BattleVelocity);
Btype *initialCon2(char notation[], char Name[], char gunName[], int BattleVelocity);
char *getRandomEscortShipType(Etype escortShipTypes[]);
void displayBattleshipTypes(Btype battleshipTypes[], int numBattleshipTypes);
int selectBattleshipType(Btype battleshipTypes[], int numBattleshipTypes);
void placeBattleship(Battleship *B, int Dx, int Dy);
void populateCanvas(EscortShip E[], int numShips, int Dx, int Dy, char *canvas, Etype escortShipTypes[], Battleship *B);
void displayCanvas(char *canvas, int Dx, int Dy);
int determineOutcome(double t, double B_attack, Battleship *B, EscortShip E[], int numShips, Etype escortShipTypes[], int *numShipsHit, char *sankShipNotation, bool considerImpact);
double calculateRadius(int angle, int velocity);
double calculateTimeOfFlight(Battleship *B, double initialVelocity);
double calculateAttackRange(Battleship *B, double initialVelocity, double t);
double calculateCumulativeImpact(Battleship *B, EscortShip E[], int numShips, Etype escortShipTypes[]);
void freeMemory(Etype* escortShipTypes, Btype* battleshipTypes);
void generatePath(int k, int Dx, int Dy, Point path[]);
void simulateBattleshipMovement(Point path[], int k, char *canvas, int Dx, int Dy, Battleship *B, EscortShip E[], int numShips, Etype *escortShipTypes, int *numShipsHit, char *sankShipNotation, int BattleVelocity, bool considerImpact);
void saveInitialBattleshipConditions(Battleship *B, int Dx, int Dy, const char *filename, Btype battleshipTypes[], int BattleshipIndex, EscortShip E[], int numShips, Etype escortShipTypes[]);
void saveFinalConditions(char *canvas, int Dx, int Dy, const char *filename, bool considerImpact, double cumulativeImpact);
void saveEscortShipDetails(EscortShip E[], int numShips, const char *filename, Etype escortShipTypes[], bool considerImpact);

int main() {
    mainMenu();
    return 0;
}

// Function to handle user input
int getUserInput() {
    int value;
    scanf("%d", &value);
    return value;
}
// Function that displays the main menu
void mainMenu() {
    // Display the main menu options and handle user input
    while (1) {
        printf("\n=== Welcome to the Naval Warfare Simulator ===\n");
        printf("\nChoose an option by entering the number:\n");
        printf("1. Embark on a Mission\n");
        printf("2. Discover Instructions\n");
        printf("3. View Simulation Statistics\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        int choice=getUserInput();

        switch (choice) {
            case 1:
                startSimulation();
                break;
            case 2:
                instructionsMenu();
                break;
            case 3:
                simulationStatistics("initial_conditions.txt", "final_conditions.txt", "hit_ships.txt");
                break;
            case 4:
                exitProgram();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to start the simulation
void startSimulation() {
    char ans;
    // ask the user to start the simulation or review ship properties
    printf("\n=== Starting Simulation ===\n");
    printf("Would you like to review Battleship and Escort Ship properties? (Y/N): ");
    scanf(" %c", &ans);
          
     // Check user's choice   
    if (ans == 'Y' || ans == 'y') {
        setupMenu();
    } else {
        printf("Do you wish to start the simulation? (Y/N): ");
        scanf(" %c", &ans);
        if (ans == 'Y' || ans == 'y') {
            mission();
        } else {
            printf("Returning to the main menu...\n");
            mainMenu();
        }
    }
}

// Function to display setup menu
void setupMenu() {
// Display options for ship properties and ask for user input
       printf("\n=== WarShip Profiles ===\n");
    printf("1. View Battleship Properties\n");
    printf("2. View Escort Ship Properties\n");
    printf("3. Return to Main Menu\n");
    printf("Enter your choice: ");
     
    // Allocate memory for ship types   
    Etype *escortShipTypes = malloc(MAX_ESCORT_SHIP_TYPES * sizeof(Etype));
    if (escortShipTypes == NULL) {
        printf("Memory allocation failed.\n");
        
        return;
    }
    Btype *battleshipTypes = malloc(MAX_BATTLESHIP_TYPES * sizeof(Btype));
    if (battleshipTypes == NULL) {
        printf("Memory allocation failed.\n");
       
        return;
    }
    // Process user choice
    int choice = getUserInput();

    switch (choice) {
        case 1:
            battleshipProperties(battleshipTypes);
            break;
        case 2:
            escortshipProperties(escortShipTypes);
            break;
        case 3:
            mainMenu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            setupMenu();
    }
    // Free allocated memory
    free(escortShipTypes);
    free(battleshipTypes);
}

// Function to display instructions menu
void instructionsMenu() {

//common instructions
    printf("\n=== Instructions Menu ===\n");
    printf("\nWelcome to the Naval Warfare Simulation Instructions!\n");
    printf("\nScenario Overview:\n");
    printf("In this simulation, you'll command a battleship engaging enemy escort ships protecting cargo vessels in the vast expanse of the ocean.\n");
    printf("Your objective is to strategically deploy your firepower to neutralize the enemy threat while minimizing damage to your own fleet.\n");
    printf("\nKey Features:\n");
    printf("1. Battleship: Represents the Allied forces'\n");
    printf("2. Escort Ships: Represent the Axis Powers' vessels tasked with protecting cargo ships.\n");
    printf("All the firings will follow parabolic paths influenced by gravity.\n");
    printf("\nGameplay Instructions:\n");
    printf("- Use your battleship's firepower to target and eliminate enemy escort ships.\n");
    printf("- Escort ships will attempt to defend their cargo vessels while engaging your battleship.\n");
    printf("The simulation will have 3 levels\n");
    printf("\nLevel 1: Basic Training\n");
    printf("\nLevel 2: Path of the Brave\n");
    printf("\nLevel 3: Advanced Warfare\n");
    printf("\nSelect the level you want to view the instructions of: \n");
    printf("Type 4 if you want to go back to the Main Menu:");
 
 //Ask the user to select which level's instruction they want to view  
    int choice = getUserInput();
    switch (choice) {
        case 1:
            instructionsMenuLevel1();
            break;
        case 2:
            instructionsMenuLevel2();
            break;
        case 3:
            instructionsMenuLevel3();
            break;
        case 4:
            mainMenu();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            instructionsMenu();
    }
}

// Function to display simulation statistics
void simulationStatistics(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile) {
    printf("\n=== Simulation Statistics ===\n");

    // Display the contents of the initial conditions file
    displayFileContents(initialConditionsFile);

    // Display the contents of the final conditions file
    displayFileContents(finalConditionsFile);

    // Display the contents of the hit escort ships file
    displayFileContents(hitEscortShipsFile);
}


// Function to display level 1 instructions
void instructionsMenuLevel1() {
    // Instructions for level 1
     printf("\nLevel 1: Boot Camp - Basic Training\n");
    printf("Welcome, Captain!\n");
    printf("Prepare to familiarize yourself with the basics of naval combat.\n");
    printf("In this training scenario, you will learn to navigate the naval battlefield and engage enemy ships.\n");
    printf("Your objective is to simulate a battle with the following simplified conditions:\n\n");
    printf("1. The gun on each ship can be reloaded and fired instantaneously.\n");
    printf("2. Escort ships can fire their guns only once.\n");
    printf("3. A single shell impact on a battleship or escort ship results in its destruction.\n");
    printf("4. Battleships can hit all ships within their attack range, while escort ships can hit battleships if within their attack range.\n");
    printf("5. The attack range of a battleship is represented by a circle, while escort ships have an annular attack range.\n");
    printf("\nGet ready to face the challenges of the open seas! \n");
    char choice;
     // Prompt user to return to instructions menu or exit the program
    printf("Enter 'M' to return to the instructions menu or 'E' to exit the program: ");
    scanf(" %c", &choice);
    if (choice == 'M' || choice == 'm') {
        instructionsMenu();
    } else if (choice == 'E' || choice == 'e') {
        exitProgram();
    } else {
        printf("Invalid choice. Returning to the main menu...\n");
        mainMenu();
    }
}

// Function to display level 2 instructions
void instructionsMenuLevel2() {
    printf("\nLevel 2: Path of the Brave\n");
    printf("Welcome, Captain!\n");
    printf("In this phase of your training, you will simulate battles with a moving battleship navigating a predefined path.\n");
    printf("Your objective is to master the art of maneuvering the battleship while engaging enemy ships.\n");
    printf("Here are the steps to complete Level 2:\n\n");
    printf("1. Navigate the battleship along a predefined path within the naval battlefield.\n");
    printf("2. Generate a specified number of points along the path to determine the battleship's trajectory.\n");
    printf("3. Simulate battleship movement and engagement with enemy ships at each point.\n");
    printf("4. Strategically plan your movements to outmaneuver enemy forces and achieve victory.\n");
    printf("\nPrepare to lead your battlegroup to victory,Captain!\n");
    
    // Prompt user to return to instructions menu or exit the program
    char choice;
    printf("Enter 'M' to return to the instructions menu or 'E' to exit the program: ");
    scanf(" %c", &choice);
    if (choice == 'M' || choice == 'm') {
        instructionsMenu();
    } else if (choice == 'E' || choice == 'e') {
        exitProgram();
    } else {
        printf("Invalid choice. Returning to the main menu...\n");
        mainMenu();
    }
}

// Function to display level 3 instructions
void instructionsMenuLevel3() {
printf("\nLevel 3: Advanced Warfare\n");
printf("Attention, Captain!\n");
printf("Your skills will be put to the ultimate test in this phase of the simulation.\n");
printf("Prepare to face more realistic scenarios where the battleship's endurance and strategic decisions play a crucial role.\n");
printf("Here's what you need to do in Level 3:\n\n");
printf("1. Engage enemy escort ships while considering their impact power on your battleship.\n");
printf("2. Assess the endurance of your battleship against multiple enemy attacks.\n");
printf("3. Plan your maneuvers and counterattacks strategically to survive and emerge victorious.\n");
printf("\nCaptain, your strategy will determine the outcome of this simulation. Good luck!\n");

// Prompt user to return to instructions menu or exit the program
char choice;
    printf("Enter 'M' to return to the instructions menu or 'E' to exit the program: ");
    scanf(" %c", &choice);
    if (choice == 'M' || choice == 'm') {
        instructionsMenu();
    } else if (choice == 'E' || choice == 'e') {
        exitProgram();
    } else {
        printf("Invalid choice. Returning to the main menu...\n");
        mainMenu();
    }
}

// Function to exit the program
void exitProgram() {
    printf("\nExiting program...\n");
    exit(0);
}

// Function to read and display the contents of a text file
void displayFileContents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    printf("\nContents of %s:\n", filename);
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Function to display battleship properties
void battleshipProperties(Btype battleshipTypes[]) {
     
    printf("\n=== Battleship Properties ===\n");
    for (int i = 0; i < MAX_BATTLESHIP_TYPES; ++i) {
        printf("Battleship %d:\n", i + 1);
        printf("Notation: %s\n", battleshipTypes[i].notation);
        printf("Name: %s\n", battleshipTypes[i].name);
        printf("Gun Name: %s\n\n", battleshipTypes[i].gunName);
    }
}

// Function to display escort ship properties
void escortshipProperties(Etype escortShipTypes[] ) {
    printf("\n=== Escort Ship Properties ===\n");
    for (int i = 0; i < MAX_ESCORT_SHIP_TYPES; ++i) {
        printf("Escort Ship %d:\n", i + 1);
        printf("Notation: %s\n", escortShipTypes[i].notation);
        printf("Name: %s\n", escortShipTypes[i].Name);
        printf("Gun Name: %s\n", escortShipTypes[i].gunName);
        printf("Impact Power: %.2f\n", escortShipTypes[i].impactPower);
        printf("Range: %d\n", escortShipTypes[i].range);
        printf("Min Angle: %d\n", escortShipTypes[i].minAngle);
        printf("Max Angle: %d\n", escortShipTypes[i].maxAngle);
        printf("Min Velocity: %d\n", escortShipTypes[i].minVelocity);
        printf("Max Velocity: %d\n", escortShipTypes[i].maxVelocity);
        printf("Velocity: %d\n", escortShipTypes[i].velocity);
        printf("Inner Radius: %.2f\n", escortShipTypes[i].innerRadius);
        printf("Outer Radius: %.2f\n", escortShipTypes[i].outerRadius);
        printf("Attack Range: %.2f\n\n", escortShipTypes[i].attackRange);
    }
}

// Function to implement mission logic
void mission() {
    srand(time(NULL)); // Seed the random number generator

    while (1) {
        printf("Select simulation Level:\n");
        printf("\nLevel 1: Basic Training\n");
        printf("\nLevel 2: Path of the Brave\n");
        printf("\nLevel 3: Advanced Warfare\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        int choice = getUserInput();

        switch (choice) {
            case 1:
                simulation1("initial_conditions_simulation1.txt", "final_conditions_simulation1.txt", "hit_escort_ships_simulation1.txt");
                break;
            case 2:
                simulation2("initial_conditions_simulation1.txt", "final_conditions_simulation1.txt", "hit_escort_ships_simulation1.txt");
                break;
            case 3:
                simulation3("initial_conditions_simulation1.txt", "final_conditions_simulation1.txt", "hit_escort_ships_simulation1.txt");
                break;
            case 4:
                printf("Exiting...\n");
                mainMenu();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

   
}

// Function to perform the simple version of the simulation
void simulation1(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile) {
    srand(time(NULL)); // Seed the random number generator

    // Declare variables
    int numShipsHit = 0;
    double t, B_attack; // Declare 't' and 'B_attack' as double
    char sankShipNotation; // Declare 'sankShipNotation' as char
    EscortShip E[MAX_ESCORT_SHIPS];

    Battleship *B = malloc(sizeof(Battleship));
    if (B == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    Etype *escortShipTypes = malloc(MAX_ESCORT_SHIP_TYPES * sizeof(Etype)); // Initialize escortShipTypes
    if (escortShipTypes == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    Btype *battleshipTypes = malloc(MAX_BATTLESHIP_TYPES * sizeof(Btype)); // Initialize battleshipTypes
    if (battleshipTypes == NULL) {
        printf("Memory allocation failed.\n");
        
        return;
    }

    printf("Enter the BattleVelocity: ");
    int BattleVelocity = getUserInput();

    // Initialize ship types
    setupSimulation(escortShipTypes, battleshipTypes, BattleVelocity);

    // Making the canvas
    int Dx, Dy;
    do {
        printf("Make a battlefield within the range of (0-100)\n");
        printf("Enter the x-coordinate of the upper-right corner: ");
        scanf("%d", &Dx);
        printf("Enter the y-coordinate of the upper-right corner: ");
        scanf("%d", &Dy);

        if (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE) {
            printf("Invalid coordinates. Please enter values within the range of (0-100).\n");
        }
    } while (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE);

    printf("Your battlefield is within the range of: (0, 0) and (%d, %d)\n", Dx, Dy);

    // Get number of escort ships from the user
    printf("Enter the number of escort ships you want in the simulation: ");
    int numShips = getUserInput();
    printf("Generating %d escort ships:\n", numShips);
    for (int i = 0; i < numShips; ++i) {
        E[i].x = rand() % Dx;
        E[i].y = rand() % Dy;
        E[i].type = *getRandomEscortShipType(escortShipTypes);
        printf("Escort Ship %d: Type - %c, Position - (%d, %d)\n", i + 1, E[i].type, E[i].x, E[i].y);
    }

    // Create the canvas
    char *canvas = malloc(Dx * Dy * sizeof(char));
    if (canvas == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    // Initialize the canvas with '~'
    for (int i = 0; i < Dx * Dy; ++i) {
        canvas[i] = '~';
    }

    // Display available battleship types and prompt user to select one
    printf("\nAvailable Battleship Types:\n");
    displayBattleshipTypes(battleshipTypes, MAX_BATTLESHIP_TYPES);
    int BattleshipIndex = selectBattleshipType(battleshipTypes, MAX_BATTLESHIP_TYPES);

    // Prompt user to input starting position for the selected battleship
    printf("\nEnter the starting position for your battleship (x y): \n (Note: should be between x:%d and y:%d)", Dx, Dy);
    placeBattleship(B, Dx, Dy);
    // Populate the canvas with escort ships and the battleship
    populateCanvas(E, numShips, Dx, Dy, canvas, escortShipTypes, B);

    // Display the canvas
    displayCanvas(canvas, Dx, Dy);
    // Save the intial conditions of the battlefeild to a text file
    saveInitialBattleshipConditions(B, Dx, Dy, initialConditionsFile, battleshipTypes, BattleshipIndex, E, numShips, escortShipTypes);

    // Calculate the attack ranges for each escort ship
    double escortShipAttackRanges[MAX_ESCORT_SHIPS];
    for (int i = 0; i < numShips; ++i) {
        escortShipAttackRanges[i] = escortShipTypes[i].attackRange;
    }
    // Calculating the attack range for battleship
    t = calculateTimeOfFlight(B, BattleVelocity);
    B_attack = calculateAttackRange(B, BattleVelocity, t);
   
    int battleshipSunk = determineOutcome(t, B_attack, B, E, numShips, escortShipTypes, &numShipsHit, &sankShipNotation, false);
    if (battleshipSunk) {
        printf("Your battleship has been sunk!\n");
        // Display the notation of the escort ship that sank the battleship
        printf("Notation of the escort ship that sank the battleship: %c\n", sankShipNotation);
    } else {
        printf("Your battleship survived!\n");
        // Display how many E ships get hit by B.
        printf("Total number of escort ships hit by the battleship: %d\n", numShipsHit);

        // Display how long it takes to end the battle.
        printf("Time to end the battle: %.2f seconds\n", t);

        // Save to a text file the detail of all Escortships that got hit by Battleships
        saveEscortShipDetails(E, numShips, hitEscortShipsFile, escortShipTypes, false);
    }
saveFinalConditions(canvas, Dx, Dy, finalConditionsFile, false, 0.0);
    // Free dynamically allocated memory
    free(B);
    free(escortShipTypes);
    free(battleshipTypes);
    free(canvas);
     
    
}

 void simulation2(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile) {
    srand(time(NULL)); // Seed the random number generator

    // Declare variables
    int numShipsHit = 0;
    double t, B_attack; // Declare 't' and 'B_attack' as double
    char sankShipNotation; // Declare 'sankShipNotation' as char
    EscortShip E[MAX_ESCORT_SHIPS];
    Battleship *B = malloc(sizeof(Battleship));
    if (B == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    Etype *escortShipTypes = malloc(MAX_ESCORT_SHIP_TYPES * sizeof(Etype)); // Initialize escortShipTypes
    if (escortShipTypes == NULL) {
        printf("Memory allocation failed.\n");
         return;
    }
    Btype *battleshipTypes = malloc(MAX_BATTLESHIP_TYPES * sizeof(Btype)); // Initialize battleshipTypes
    if (battleshipTypes == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter the BattleVelocity: ");
    int BattleVelocity = getUserInput();

    // Initialize ship types
    setupSimulation(escortShipTypes, battleshipTypes, BattleVelocity);

    // Making the canvas
    int Dx, Dy;
    do {
        printf("Make a battlefield within the range of (0-100)\n");
        printf("Enter the x-coordinate of the upper-right corner: ");
        scanf("%d", &Dx);
        printf("Enter the y-coordinate of the upper-right corner: ");
        scanf("%d", &Dy);

        if (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE) {
            printf("Invalid coordinates. Please enter values within the range of (0-100).\n");
        }
    } while (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE);

    printf("Your battlefield is within the range of: (0, 0) and (%d, %d)\n", Dx, Dy);

    // Get number of escort ships from the user
    printf("Enter the number of escort ships you want in the simulation: ");
    int numShips = getUserInput();
    printf("Generating %d escort ships:\n", numShips);
    for (int i = 0; i < numShips; ++i) {
        E[i].x = rand() % Dx;
        E[i].y = rand() % Dy;
        E[i].type = *getRandomEscortShipType(escortShipTypes);
        printf("Escort Ship %d: Type - %c, Position - (%d, %d)\n", i + 1, E[i].type, E[i].x, E[i].y);
    }

    // Create the canvas
    char *canvas = malloc(Dx * Dy * sizeof(char));
    if (canvas == NULL) {
        printf("Memory allocation failed.\n");
        
        return;
    }
    // Initialize the canvas with '~'
    for (int i = 0; i < Dx * Dy; ++i) {
        canvas[i] = '~';
    }

    // Display available battleship types and prompt user to select one
    printf("\nAvailable Battleship Types:\n");
    displayBattleshipTypes(battleshipTypes, MAX_BATTLESHIP_TYPES);
    int BattleshipIndex = selectBattleshipType(battleshipTypes, MAX_BATTLESHIP_TYPES);

    // Prompt user to input starting position for the selected battleship
    printf("\nEnter the starting position for your battleship (x y): \n (Note: should be between x:%d and y:%d)", Dx, Dy);
    placeBattleship(B, Dx, Dy);
    // Populate the canvas with escort ships and the battleship
    populateCanvas(E, numShips, Dx, Dy, canvas, escortShipTypes, B);

    // Display the canvas
    displayCanvas(canvas, Dx, Dy);

    // Set a limit for the maximum number of points
    int maxPoints = Dx * Dy;
    //Save intial battle conditions to a file
    saveInitialBattleshipConditions(B, Dx, Dy, initialConditionsFile, battleshipTypes, BattleshipIndex, E, numShips, escortShipTypes);

    // Generate k number of points and define the order of the points
    int k;
    do {
        printf("Enter the number of points you want to generate (up to %d): ", maxPoints);
        k = getUserInput();
        if (k < 1 || k > maxPoints) {
            printf("Invalid number of points. Please enter a value between 1 and %d.\n", maxPoints);
        }
    } while (k < 1 || k > maxPoints);

    Point path[k];
    generatePath(k, Dx, Dy, path);

    // Simulate battleship movement along the generated path
    simulateBattleshipMovement(path, k, canvas, Dx, Dy, B, E, numShips, escortShipTypes, &numShipsHit, &sankShipNotation, BattleVelocity, false);
    // Save final conditions of the battlefield to file
    saveFinalConditions(canvas, Dx, Dy, finalConditionsFile, false, 0.0);
// Save details of hit escort ships to file
    saveEscortShipDetails(E, numShips, hitEscortShipsFile, escortShipTypes, false);

    // Free dynamically allocated memory
    free(B);
    free(escortShipTypes);
    free(battleshipTypes);
    free(canvas);
    
}

void simulation3(const char *initialConditionsFile, const char *finalConditionsFile, const char *hitEscortShipsFile) {
    srand(time(NULL)); // Seed the random number generator

    // Declare variables
    int numShipsHit = 0;
    double t, B_attack;
    char sankShipNotation;
    EscortShip E[MAX_ESCORT_SHIPS];
    Battleship *B = malloc(sizeof(Battleship));
    if (B == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    Etype *escortShipTypes = malloc(MAX_ESCORT_SHIP_TYPES * sizeof(Etype));
    if (escortShipTypes == NULL) {
        printf("Memory allocation failed.\n");
        
        return;
    }
    Btype *battleshipTypes = malloc(MAX_BATTLESHIP_TYPES * sizeof(Btype));
    if (battleshipTypes == NULL) {
        printf("Memory allocation failed.\n");
       
        return;
    }

    printf("Enter the BattleVelocity: ");
    int BattleVelocity = getUserInput();

    // Initialize ship types
    setupSimulation(escortShipTypes, battleshipTypes, BattleVelocity);

    // Making the canvas
    int Dx, Dy;
    do {
        printf("Make a battlefield within the range of (0-100)\n");
        printf("Enter the x-coordinate of the upper-right corner: ");
        scanf("%d", &Dx);
        printf("Enter the y-coordinate of the upper-right corner: ");
        scanf("%d", &Dy);

        if (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE) {
            printf("Invalid coordinates. Please enter values within the range of (0-100).\n");
        }
    } while (Dx < 0 || Dx > MAX_SIZE || Dy < 0 || Dy > MAX_SIZE);

    printf("Your battlefield is within the range of: (0, 0) and (%d, %d)\n", Dx, Dy);

    // Get number of escort ships from the user
    printf("Enter the number of escort ships you want in the simulation: ");
    int numShips = getUserInput();
    printf("Generating %d escort ships:\n", numShips);
    for (int i = 0; i < numShips; ++i) {
        E[i].x = rand() % Dx;
        E[i].y = rand() % Dy;
        E[i].type = *getRandomEscortShipType(escortShipTypes);
        printf("Escort Ship %d: Type - %c, Position - (%d, %d)\n", i + 1, E[i].type, E[i].x, E[i].y);
    }

    // Create the canvas
    char *canvas = malloc(Dx * Dy * sizeof(char));
    if (canvas == NULL) {
        printf("Memory allocation failed.\n");
        
        return;
    }
    // Initialize the canvas with '~'
    for (int i = 0; i < Dx * Dy; ++i) {
        canvas[i] = '~';
    }

    // Display available battleship types and prompt user to select one
    printf("\nAvailable Battleship Types:\n");
    displayBattleshipTypes(battleshipTypes, MAX_BATTLESHIP_TYPES);
    int BattleshipIndex = selectBattleshipType(battleshipTypes, MAX_BATTLESHIP_TYPES);

    // Prompt user to input starting position for the selected battleship
    printf("\nEnter the starting position for your battleship (x y): \n (Note: should be between x:%d and y:%d)", Dx, Dy);
    placeBattleship(B, Dx, Dy);
    // Populate the canvas with escort ships and the battleship
    populateCanvas(E, numShips, Dx, Dy, canvas, escortShipTypes, B);

    // Display the canvas
    displayCanvas(canvas, Dx, Dy);
    // Save initial battleship conditions
    saveInitialBattleshipConditions(B, Dx, Dy, initialConditionsFile, battleshipTypes, BattleshipIndex, E, numShips, escortShipTypes);

    // Set a limit for the maximum number of points
    int maxPoints = Dx * Dy;

    // User prompt to choose mode
    printf("Choose mode:\n");
    printf("1. Attack from initial position\n");
    printf("2. Move across generated points\n");
    int mode= getUserInput();
        
    double cumulativeImpact = 0.0; // Declare cumulativeImpact
    Point *path; // Declare path as a pointer


    switch (mode) {
        case 1:
            // Calculate the attack ranges for each escort ship
            double escortShipAttackRanges[MAX_ESCORT_SHIPS];
            for (int i = 0; i < numShips; ++i) {
                escortShipAttackRanges[i] = escortShipTypes[i].attackRange;
            }
            // Calculating the attack range for battleship
            t = calculateTimeOfFlight(B, BattleVelocity);
            B_attack = calculateAttackRange(B, BattleVelocity, t);

            // Check if the battleship sank
            int battleshipSunk = determineOutcome(t, B_attack, B, E, numShips, escortShipTypes, &numShipsHit, &sankShipNotation, true);
            if (battleshipSunk) {
                printf("Your battleship has been sunk!\n");
                // Display the notation of the escort ship that sank the battleship
                printf("Notation of the escort ship that sank the battleship: %c\n", sankShipNotation);
            } else {
                printf("Your battleship survived!\n");

               // Display how many E ships get hit by B.
                printf("Total number of escort ships hit by the battleship: %d\n", numShipsHit);

                // Display how long it takes to end the battle.
                printf("Time to end the battle: %.2f seconds\n", t);

                // Save to a text file the detail of all Es that got hit by B
                

                saveFinalConditions(canvas, Dx, Dy, finalConditionsFile, true, cumulativeImpact);
                saveEscortShipDetails(E, numShips, hitEscortShipsFile, escortShipTypes, true); 
                      }
            break;

        case 2:
            // Generate k number of points and simulate battleship movement
            int k;
            do {
                printf("Enter the number of points you want to generate (up to %d): ", maxPoints);
                k = getUserInput();
                if (k < 1 || k > maxPoints) {
                    printf("Invalid number of points. Please enter a value between 1 and %d.\n", maxPoints);
                }
            } while (k < 1 || k > maxPoints);

            path = malloc(k * sizeof(Point)); // Allocate memory for path
        if (path == NULL) {
            printf("Memory allocation failed.\n");
            break;
        }
            generatePath(k, Dx, Dy, path);

            simulateBattleshipMovement(path, k, canvas, Dx, Dy, B, E, numShips, escortShipTypes, &numShipsHit, &sankShipNotation, BattleVelocity, true);

            // Save final conditions of the battlefield
           saveEscortShipDetails(E, numShips, hitEscortShipsFile, escortShipTypes, true); 
           saveFinalConditions(canvas, Dx, Dy, finalConditionsFile, true, cumulativeImpact);
        
            free(path);
            break;

        default:
            printf("Invalid mode selection.\n");
            break;
    }

    // Free dynamically allocated memory
    free(B);
    free(escortShipTypes);
    free(battleshipTypes);
    free(canvas);
}


void setupSimulation(Etype escortShipTypes[], Btype battleshipTypes[], int BattleVelocity) {
    // Initialize escort ship types
    escortShipTypes[0] = *initialCon1("A", "1936A-classDestroyer", "SK C/34 naval gun", 0.08, 20, BattleVelocity);
    escortShipTypes[1] = *initialCon1("B", "Gabbiano-class Corvette", "L/47 dual-purpose gun", 0.06, 30, BattleVelocity);
    escortShipTypes[2] = *initialCon1("C", "Matsu-class Destroyer", "Type 89 dual-purpose gun", 0.07, 25, BattleVelocity);
    escortShipTypes[3] = *initialCon1("D", "F-class Escort Ships", "SK C/32 naval gun", 0.05, 50, BattleVelocity);
    escortShipTypes[4] = *initialCon1("E", "Japanese Kaibōkan", "(4.7 inch) naval guns", 0.04, 70, BattleVelocity);

    // Initialize battleship types
    battleshipTypes[0] = *initialCon2("U", "USS Iowa (BB-61)", "50-caliber Mark 7 gun", BattleVelocity);
    battleshipTypes[1] = *initialCon2("M", "MS King George V", "(356mm) Mark VII gun", BattleVelocity);
    battleshipTypes[2] = *initialCon2("R", "Richelieu", "(15 inch) Mle 1935 gun", BattleVelocity);
    battleshipTypes[3] = *initialCon2("S", "Sovetsky Soyuz-class", "(16 inch) B-37 gun", BattleVelocity);
}

// Function to initialize Etype struct
Etype *initialCon1(char notation[], char Name[], char gunName[], double impactPower, int range, int BattleVelocity) {
    // Allocate memory for a single Etype struct
    Etype *E = malloc(sizeof(Etype));
    if (E == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Copy the values to the struct members
    strcpy(E->notation, notation);
    strcpy(E->Name, Name);
    strcpy(E->gunName, gunName);
    E->impactPower = impactPower;
    E->range = range;

    // Generate random min value within the angle range
   E->minAngle = ((rand() / (double)RAND_MAX) * (90 - range)) * PI / 180.0;

    // Generate the maxAngle between the minAngle and range
    E->maxAngle = E->minAngle + E->range;

    // Generate random value between 0 and 50 for minVelocity
    E->minVelocity = rand() % 50;

    // If notation is 'A', make maxVelocity 1.2 times of BattleVelocity. 
    if (strcmp(E->notation, "A") == 0) {
        E->maxVelocity = BattleVelocity * 1.2;
    }
    // Otherwise, generate a value between minVelocity and BattleVelocity for maxVelocity    
    else {   
        E->maxVelocity = E->minVelocity + rand() % (BattleVelocity - E->minVelocity + 1);
    }

    // Calculate the velocity of the escort ship randomly between maxVelocity and minVelocity
    E->velocity = E->minVelocity + rand() % (E->maxVelocity - E->minVelocity + 1);

    // Calculate the inner and outer radius
    E->innerRadius = calculateRadius(E->minAngle, E->velocity);
    E->outerRadius = calculateRadius(E->maxAngle, E->velocity);

    // Calculate attack range by subtracting outer radius from inner radius
    E->attackRange = (E->outerRadius - E->innerRadius)/100;

    return E;
}

// Function to initialize Btype struct
Btype *initialCon2(char notation[], char Name[], char gunName[], int BattleVelocity){
    // Allocate memory for a single Btype struct
    Btype *B = malloc(sizeof(Btype));
    if (B == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Copy the values to the struct members
    strcpy(B->notation, notation);
    strcpy(B->name, Name); 
    strcpy(B->gunName, gunName);
    B->BattleVelocity = BattleVelocity;
    // Generate random angle between 1 and 90    
    B->Angle = (rand() / (double)RAND_MAX) * (PI / 2); 
    return B;
}

// Func. to generate a random index within the range of available escort ship types
char *getRandomEscortShipType(Etype escortShipTypes[]) {
    int     randomIndex = rand() % MAX_ESCORT_SHIP_TYPES;
    return escortShipTypes[randomIndex].notation;
}

// Function to display available battleship types
void displayBattleshipTypes(Btype battleshipTypes[], int numBattleshipTypes) {
    for (int i = 0; i < numBattleshipTypes; ++i) {
       printf("%d. %s: %s\n \t \t \t gun type : %s\n", i + 1, battleshipTypes[i].notation, battleshipTypes[i].name, battleshipTypes[i].gunName);
    }
}

int selectBattleshipType(Btype battleshipTypes[], int numBattleshipTypes) {
    int choice;
    do {
        printf("\nSelect a battleship type by entering its number: ");
        choice = getUserInput();
        if (choice < 1 || choice > numBattleshipTypes) {
            printf("Invalid selection. Please choose a number between 1 and %d.\n", numBattleshipTypes);
        }
    } while (choice < 1 || choice > numBattleshipTypes);
    
    return choice;
}

// Function to place battleship on the canvas
void placeBattleship(Battleship *B, int Dx, int Dy) {
    do {
        scanf("%d %d", &B->x, &B->y);
        if (B->x < 0 || B->x >= Dx || B->y < 0 || B->y >= Dy) {
            printf("Invalid position. Please enter a position within the battlefield: ");
        }
    } while (B->x < 0 || B->x >= Dx || B->y < 0 || B->y >= Dy);
}

// Function to populate the canvas with escort ships and the battleship
void populateCanvas(EscortShip E[], int numShips, int Dx, int Dy, char *canvas, Etype escortShipTypes[], Battleship *B) {
    for (int i = 0; i < numShips; ++i) {
        int index = E[i].x * Dx + E[i].y;
        canvas[index] = E[i].type;
    }
    int index = B->x * Dx + B->y;
    canvas[index] = '*'; // Represent battleship with '*'
}

// Function to display the canvas with '~'
void displayCanvas(char *canvas, int Dx, int Dy) {
    printf("The battlefield:\n");
    for (int i = 0; i < Dx; ++i) {
        for (int j = 0; j < Dy; ++j) {
            if (canvas[i * Dx + j] == '\0') {
                printf("~ "); // Display '~' for empty cells
            } else {
                printf("%c ", canvas[i * Dx + j]);
            }
        }
        printf("\n");
    }
}
// Function to calculate the radius of the attack range
double calculateRadius(int angle, int velocity) {
    return (velocity * velocity * sin(2 * angle)) / GRAVITY;
}

// Function to calculate time of flight
double calculateTimeOfFlight(Battleship *B, double initialVelocity) {
    return (2 * initialVelocity * sin(B->Angle)) / GRAVITY;
}

// Function to calculate the attack range
double calculateAttackRange(Battleship *B, double initialVelocity, double t) {
    return initialVelocity * cos(B->Angle) * t/100;
}

// Function to determine the outcome of the battle
int determineOutcome(double t, double B_attack, Battleship *B, EscortShip E[], int numShips, Etype escortShipTypes[], int *numShipsHit, char *sankShipNotation, bool considerImpact) {
    *numShipsHit = 0;
    bool battleshipSunk = false;
    double cumulativeImpact = 0.0;

    // Check if battleship is within any escort ship's attack range
    for (int i = 0; i < numShips; ++i) {
        double distFromBattleship = sqrt(pow(B->x - E[i].x, 2) + pow(B->y - E[i].y, 2));
        if (distFromBattleship <= escortShipTypes[E[i].type - 'A'].attackRange) {
            if (considerImpact) {
                // Calculate the impact on the battleship from the current escort ship
                double impact = escortShipTypes[E[i].type - 'A'].impactPower;
                cumulativeImpact += impact;
                if (cumulativeImpact >= 1.0) {
                    printf("Your battleship has been sunk by Escort Ship %c!\n", E[i].type);
                    *sankShipNotation = E[i].type;
                    battleshipSunk = true;
                    break; // No need to check further if battleship is sunk
                }
            } else {
                // Original logic without impact consideration
                printf("Your battleship has been sunk by Escort Ship %c!\n", E[i].type);
                *sankShipNotation = E[i].type;
                battleshipSunk = true;
                break; // No need to check further if battleship is sunk
            }
        }
    }

    if (!battleshipSunk && !considerImpact) {
        // Check if any escort ship is within the battleship's attack range
        for (int i = 0; i < numShips; ++i) {
            double distFromBattleship = sqrt(pow(B->x - E[i].x, 2) + pow(B->y - E[i].y, 2));
            if (distFromBattleship <= B_attack) {
                printf("Escort Ship %d (%c) got hit!\n", i + 1, E[i].type);
                (*numShipsHit)++;
                // Save details of the hit escort ship to a text file
                saveEscortShipDetails(&E[i], 1, "hit_ships.txt", escortShipTypes, considerImpact);
            }
        }
        if (*numShipsHit > 0) {
            printf("Total number of escort ships hit by the battleship: %d\n", *numShipsHit);
            printf("Time to end the battle: %.2f seconds\n", t);
        } else {
            printf("No escort ships hit by the battleship.\n");
        }
    }

    if (battleshipSunk) {
        return 1;
    } else {
        return 0;
    }
}
// Function to save the details of escort ships that got hit by the battleship to a text file
void saveEscortShipDetails(EscortShip E[], int numShips, const char *filename, Etype escortShipTypes[], bool considerImpact) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    if (considerImpact) {
        // If considering impact, include impact power details
        for (int i = 0; i < numShips; ++i) {
            fprintf(file, "Escort Ship %d: Type - %c, Position - (%d, %d), Name - %s, Gun Name - %s, Impact Power - %.2f\n",
                    i + 1, E[i].type, E[i].x, E[i].y, escortShipTypes[E[i].type - 'A'].Name, 
                    escortShipTypes[E[i].type - 'A'].gunName, escortShipTypes[E[i].type - 'A'].impactPower);
        }
    } else {
        // If not considering impact, omit impact power details
        for (int i = 0; i < numShips; ++i) {
            fprintf(file, "Escort Ship %d: Type - %c, Position - (%d, %d), Name - %s, Gun Name - %s\n",
                    i + 1, E[i].type, E[i].x, E[i].y, escortShipTypes[E[i].type - 'A'].Name, 
                    escortShipTypes[E[i].type - 'A'].gunName);
        }
    }

    fclose(file);
}

// Function to free allocated memory
void freeMemory(Etype* escortShipTypes, Btype* battleshipTypes) {
    free(escortShipTypes);
    free(battleshipTypes);
}
//moves the battleship along and checks if it destroys and escortships or get destroyed by an escortship
void simulateBattleshipMovement(Point path[], int k, char *canvas, int Dx, int Dy, Battleship *B, EscortShip E[], int numShips, Etype *escortShipTypes, int *numShipsHit, char *sankShipNotation, int BattleVelocity, bool considerImpact) {
    int battleshipSunk = 0;
    double t = 0, B_attack = 0; // Initialize 't' and 'B_attack'
    double cumulativeImpact = 0.0; // Initialize cumulative impact

    for (int p = 0; p < k; ++p) {
        // Move battleship to the next point in the path
        B->x = path[p].x;
        B->y = path[p].y;

        if (considerImpact) {
            // Calculate the cumulative impact
          cumulativeImpact = calculateCumulativeImpact(B, E, numShips, escortShipTypes);

        }

        // Calculate the attack ranges for each escort ship
        double escortShipAttackRanges[MAX_ESCORT_SHIPS];
        for (int i = 0; i < numShips; ++i) {
            escortShipAttackRanges[i] = escortShipTypes[E[i].type - 'A'].attackRange;
        }

        // Calculate the time of flight and attack range for the battleship
        t = calculateTimeOfFlight(B, BattleVelocity);
        B_attack = calculateAttackRange(B, BattleVelocity, t);

        // Check if the battleship sank
        battleshipSunk = determineOutcome(t, B_attack, B, E, numShips, escortShipTypes, numShipsHit, sankShipNotation, considerImpact);
        if (battleshipSunk) {
            printf("Your battleship has been sunk at point (%d, %d)!\n", B->x, B->y);
            printf("Notation of the escort ship that sank the battleship: %c\n", *sankShipNotation);
            break;
        }

        // Save details of escort ships hit by the battleship
        
    }

    if (!battleshipSunk) {
        printf("Your battleship survived!\n");
        printf("Total number of escort ships hit by the battleship: %d\n", *numShipsHit);
        printf("Time to end the battle: %.2f seconds\n", t);

        if (considerImpact) {
            // Save final conditions with cumulative impact
            saveFinalConditions(canvas, Dx, Dy, "final_conditions.txt", true, cumulativeImpact);
        } else {
            // Save final conditions without considering impact
            saveFinalConditions(canvas, Dx, Dy, "final_conditions.txt", false, 0.0);
        }
    }
}

void generatePath(int k, int Dx, int Dy, Point path[]) {
    for (int i = 0; i < k; ++i) {
        // Generate random x and y coordinates for each point
        path[i].x = rand() % Dx; // Random x-coordinate
        path[i].y = rand() % Dy; // Random y-coordinate
    }
}


// function to save the intial conditions
void saveInitialBattleshipConditions(Battleship *B, int Dx, int Dy, const char *filename, Btype battleshipTypes[], int BattleshipIndex, EscortShip E[], int numShips, Etype escortShipTypes[]) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    fprintf(fp, "Initial Conditions of the Battlefield:\n");

    // Write battleship details
    fprintf(fp, "Battleship Details:\n");
    fprintf(fp, "Selected Battleship Type: %s\n", battleshipTypes[BattleshipIndex].name);
    fprintf(fp, "Selected Gun Name: %s\n", battleshipTypes[BattleshipIndex].gunName);
    fprintf(fp, "Selected Battle Velocity: %.2f\n", battleshipTypes[BattleshipIndex].BattleVelocity);
    fprintf(fp, "Selected Angle: %.2f\n", B->Angle);
    fprintf(fp, "Battleship Position: (%d, %d)\n", B->x, B->y);

    // Write initial conditions of escort ships
    fprintf(fp, "\nInitial Conditions of Escort Ships:\n");
    for (int i = 0; i < numShips; ++i) {
        fprintf(fp, "Escort Ship Type %c:\n", E[i].type);
        fprintf(fp, "Name: %s\n", escortShipTypes[E[i].type - 'A'].Name);
        fprintf(fp, "Gun Name: %s\n", escortShipTypes[E[i].type - 'A'].gunName);
        fprintf(fp, "Impact Power: %.2f\n", escortShipTypes[E[i].type - 'A'].impactPower);
        fprintf(fp, "Range: %d\n", escortShipTypes[E[i].type - 'A'].range);
        fprintf(fp, "Min Angle: %d\n", escortShipTypes[E[i].type - 'A'].minAngle);
        fprintf(fp, "Max Angle: %d\n", escortShipTypes[E[i].type - 'A'].maxAngle);
        fprintf(fp, "Min Velocity: %d\n", escortShipTypes[E[i].type - 'A'].minVelocity);
        fprintf(fp, "Max Velocity: %d\n", escortShipTypes[E[i].type - 'A'].maxVelocity);
        fprintf(fp, "Velocity: %d\n", escortShipTypes[E[i].type - 'A'].velocity);
    }

    fclose(fp);
}

// Function to save battlefield conditions to a text file
void saveFinalConditions(char *canvas, int Dx, int Dy, const char *filename, bool considerImpact, double cumulativeImpact) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    fprintf(file, "\nFinal Battlefield Conditions:\n");
    for (int i = 0; i < Dx; ++i) {
        for (int j = 0; j < Dy; ++j) {
            fprintf(file, "%c ", canvas[i * Dx + j]);
        }
        fprintf(file, "\n");
    }

    if (considerImpact) {
        fprintf(file, "\nCumulative Impact on Battleship: %.2f%%\n", cumulativeImpact * 100);
    }

    fclose(file);
}

double calculateCumulativeImpact(Battleship *B, EscortShip E[], int numShips, Etype escortShipTypes[]) {
    double cumulativeImpact = 0.0;

    for (int i = 0; i < numShips; ++i) {
        double distFromBattleship = sqrt(pow(B->x - E[i].x, 2) + pow(B->y - E[i].y, 2));
        if (distFromBattleship <= escortShipTypes[E[i].type - 'A'].attackRange) {
            cumulativeImpact += escortShipTypes[E[i].type - 'A'].impactPower;
        }
    }

    return cumulativeImpact;
}




 
