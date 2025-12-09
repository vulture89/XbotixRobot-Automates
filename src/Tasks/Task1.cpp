#include "Tasks/Task1.h"
#include "Display.h"
#include "PID.h"
#include "MotorControls.h"
#include "Sensors/Color.h"
#include "Sensors/Ultrasonic.h"
#include "PoluluQTR/QTR.h"
#include "ArmManager/ArmManager.h"

// Task 1 State Machine
enum Task1State {
    T1_NAVIGATE_TO_FIRST_CUBE,
    T1_APPROACH_FIRST_CUBE,
    T1_COLLECT_FIRST_CUBE,
    T1_STORE_FIRST_CUBE,
    T1_NAVIGATE_TO_SECOND_CUBE,
    T1_APPROACH_SECOND_CUBE,
    T1_COLLECT_SECOND_CUBE,
    T1_STORE_SECOND_CUBE,
    T1_NAVIGATE_TO_CHAMBER,
    T1_ENTER_CHAMBER,
    T1_PLACE_CRIMSON,
    T1_PLACE_AZURE,
    T1_EXIT_CHAMBER,
    T1_COMPLETE
};

enum IntersectionType {
    NO_INTERSECTION,
    LEFT_TURN,
    RIGHT_TURN,
    T_JUNCTION,
    CROSS
};

// Global variables for Task 1
Task1State currentState = T1_NAVIGATE_TO_FIRST_CUBE;
bool hasCrimsonStone = false;
bool hasAzureStone = false;
String firstCubeColor = "";
String secondCubeColor = "";
unsigned long stateStartTime = 0;
int cubesCollected = 0;

// Helper function to detect cube using color sensor and ultrasonic
bool detectCube() {
    if (isObstacleFront(20)) {  // 20cm initial detection threshold
        delay(50);
        String color = detectColor();
        if (color == "Red" || color == "Blue") {
            return true;
        }
    }
    return false;
}

// Helper function to approach cube very close
void approachCube() {
    displayMsg("Approaching cube");
    
    // Move forward slowly until very close (5cm)
    while (!isObstacleFront(5)) {
        forward(80, 80);
        delay(50);
    }
    
    stop(200);
    displayMsg("Cube reached");
}

// Helper function to collect cube
void collectCube() {
    displayMsg("Collecting cube...");
    
    // Detect color
    String cubeColor = detectColor();
    displayMsg("Detected: " + cubeColor);
    
    // Ensure arm is in retrieval position (rotated to normal)
    armManager.rotateToNormal();
    delay(500);
    
    // Open hand and lower arm
    armManager.openHand();
    delay(500);
    armManager.liftTo(0);  // Lower to ground level
    delay(1000);
    
    // Move forward slowly to grab cube
    forward(80, 80);
    delay(600);
    stop(100);
    
    // Close hand to grab
    armManager.closeHand();
    delay(1000);
    
    // Lift up
    armManager.liftTo(10);  // Lift 10cm
    delay(1000);
    
    // Update collection status
    if (cubeColor == "Red") {
        hasCrimsonStone = true;
        if (firstCubeColor == "") {
            firstCubeColor = "Red";
        } else {
            secondCubeColor = "Red";
        }
    } else if (cubeColor == "Blue") {
        hasAzureStone = true;
        if (firstCubeColor == "") {
            firstCubeColor = "Blue";
        } else {
            secondCubeColor = "Blue";
        }
    }
    
    cubesCollected++;
    displayMsg("Cubes: " + String(cubesCollected));
    
    // Back up
    backward(100, 100);
    delay(800);
    stop(100);
}

// Helper function to store cube in dropbox
void storeCube() {
    displayMsg("Storing cube");
    
    // Rotate arm to dropbox position (90 degrees)
    armManager.rotateToAngle(90, 90);
    delay(1000);
    
    // Lower slightly for drop
    armManager.liftTo(5);
    delay(500);
    
    // Open hand to release
    armManager.openHand();
    delay(800);
    
    // Rotate back to normal position for next retrieval
    armManager.rotateToNormal();
    delay(1000);
    
    // Lift to safe travel height
    armManager.liftTo(10);
    delay(500);
    
    displayMsg("Cube stored");
}

// Helper function to detect intersection type
IntersectionType detectIntersection() {
    readMainQTR();
    readEdgeQTR();
    
    // Count active sensors
    int mainCount = 0;
    int leftEdge = edgeQTRDval[0];   // Left edge sensor
    int rightEdge = edgeQTRDval[1];  // Right edge sensor
    
    for (int i = 0; i < MAIN_SENSOR_COUNT; i++) {
        if (mainQTRDval[i]) mainCount++;
    }
    
    // Wide line detected = some kind of intersection
    if (mainCount >= 5) {
        // Analyze edge sensors to determine type
        if (leftEdge && rightEdge) {
            displayMsg("Cross detected");
            return CROSS;
        } else if (leftEdge && !rightEdge) {
            displayMsg("Left turn detected");
            return LEFT_TURN;
        } else if (!leftEdge && rightEdge) {
            displayMsg("Right turn detected");
            return RIGHT_TURN;
        } else {
            displayMsg("T-junction detected");
            return T_JUNCTION;
        }
    }
    
    return NO_INTERSECTION;
}

// Helper function to turn right at intersection
void turnRight() {
    displayMsg("Executing right turn");
    
    // Step 1: Move forward until right tyre sensor aligns with the turn road
    bool tyreSensorsAligned = false;
    
    while (!tyreSensorsAligned) {
        forward(100, 100);
        delay(50);
        
        readTyreQTR();
        
        // Check if right tyre sensor detects the turn road
        if (tyreQTRDval[1]) {  // Right tyre sensor
            tyreSensorsAligned = true;
        }
    }
    
    stop(200);
    displayMsg("Tyres aligned for right");
    
    // Step 2: Start turning clockwise
    unsigned long turnStartTime = millis();
    bool turnComplete = false;
    
    while (!turnComplete && (millis() - turnStartTime < 3000)) {  // 3 sec timeout
        clockwise(120, 120);
        delay(50);
        
        // Read sensors to check if turn is complete
        readMainQTR();
        readTyreQTR();
        
        // Check if we've turned onto the new line
        // The center main sensors should detect the line
        int centerCount = 0;
        for (int i = 3; i < 6; i++) {  // Center 3 sensors
            if (mainQTRDval[i]) centerCount++;
        }
        
        // AND the tyre sensors should no longer be on the intersection
        bool tyreSensorsOff = !tyreQTRDval[0] && !tyreQTRDval[1];
        
        if (centerCount >= 2 && tyreSensorsOff) {
            turnComplete = true;
        }
    }
    
    stop(200);
    displayMsg("Right turn complete");
}

// Helper function to turn left at intersection
void turnLeft() {
    displayMsg("Executing left turn");
    
    // Step 1: Move forward until left tyre sensor aligns with the turn road
    bool tyreSensorsAligned = false;
    
    while (!tyreSensorsAligned) {
        forward(100, 100);
        delay(50);
        
        readTyreQTR();
        
        // Check if left tyre sensor detects the turn road
        if (tyreQTRDval[0]) {  // Left tyre sensor
            tyreSensorsAligned = true;
        }
    }
    
    stop(200);
    displayMsg("Tyres aligned for left");
    
    // Step 2: Start turning anticlockwise
    unsigned long turnStartTime = millis();
    bool turnComplete = false;
    
    while (!turnComplete && (millis() - turnStartTime < 3000)) {  // 3 sec timeout
        anticlockwise(120, 120);
        delay(50);
        
        // Read sensors to check if turn is complete
        readMainQTR();
        readTyreQTR();
        
        // Check if we've turned onto the new line
        // The center main sensors should detect the line
        int centerCount = 0;
        for (int i = 3; i < 6; i++) {  // Center 3 sensors
            if (mainQTRDval[i]) centerCount++;
        }
        
        // AND the tyre sensors should no longer be on the intersection
        bool tyreSensorsOff = !tyreQTRDval[0] && !tyreQTRDval[1];
        
        if (centerCount >= 2 && tyreSensorsOff) {
            turnComplete = true;
        }
    }
    
    stop(200);
    displayMsg("Left turn complete");
}

// Helper function to continue straight at intersection
void goStraight() {
    displayMsg("Going straight");
    
    // Just move forward through the intersection
    forward(120, 120);
    delay(500);
    
    stop(100);
}

// Helper function to navigate maze with intersection handling
void navigateMaze() {
    // Read QTR sensors
    readMainQTR();
    readEdgeQTR();
    
    // Check for dead end (all main sensors off line)
    bool allOff = true;
    for (int i = 0; i < MAIN_SENSOR_COUNT; i++) {
        if (mainQTRDval[i]) {
            allOff = false;
            break;
        }
    }
    
    if (allOff) {
        // Dead end detected - turn around 180 degrees
        displayMsg("Dead end!");
        stop(200);
        
        // Start turning clockwise 180 degrees
        unsigned long turnStartTime = millis();
        bool turnComplete = false;
        
        // First phase: turn until we lose the line completely
        bool lineLost = false;
        while (!lineLost && (millis() - turnStartTime < 2000)) {
            clockwise(150, 150);
            delay(50);
            
            readMainQTR();
            int onCount = 0;
            for (int i = 0; i < MAIN_SENSOR_COUNT; i++) {
                if (mainQTRDval[i]) onCount++;
            }
            
            if (onCount == 0) {
                lineLost = true;
            }
        }
        
        // Second phase: continue turning until we find the line again (180 complete)
        while (!turnComplete && (millis() - turnStartTime < 4000)) {
            clockwise(150, 150);
            delay(50);
            
            readMainQTR();
            
            // Check if center sensors detect line again
            int centerCount = 0;
            for (int i = 3; i < 6; i++) {
                if (mainQTRDval[i]) centerCount++;
            }
            
            if (centerCount >= 2) {
                turnComplete = true;
            }
        }
        
        stop(200);
        displayMsg("180 turn complete");
        
        return;
    }
    
    // Check for cube detection
    if (detectCube() && cubesCollected < 2) {
        stop(200);
        
        if (cubesCollected == 0) {
            currentState = T1_APPROACH_FIRST_CUBE;
        } else if (cubesCollected == 1) {
            currentState = T1_APPROACH_SECOND_CUBE;
        }
        return;
    }
    
    // Check for chamber entry (after both cubes collected)
    if (cubesCollected == 2) {
        int onCount = 0;
        for (int i = 0; i < MAIN_SENSOR_COUNT; i++) {
            if (mainQTRDval[i]) onCount++;
        }
        
        if (onCount >= 6) {  // Wide path = circle entry
            displayMsg("Chamber!");
            currentState = T1_ENTER_CHAMBER;
            return;
        }
    }
    
    // Check for intersection
    IntersectionType intersection = detectIntersection();
    
    if (intersection != NO_INTERSECTION) {
        stop(200);
        
        switch (intersection) {
            case LEFT_TURN:
                turnLeft();
                break;
                
            case RIGHT_TURN:
                turnRight();
                break;
                
            case T_JUNCTION:
                // Choose left or right based on strategy (prefer right for now)
                turnRight();
                break;
                
            case CROSS:
                // At cross, choose direction (prefer straight, then right, then left)
                goStraight();
                break;
                
            default:
                break;
        }
        
        return;
    }
    
    // Standard line following
    lineFollow(3500);  // Center setpoint for 8-sensor array
}

// Helper function to place cube in socket
void placeCubeInSocket(bool isCrimson) {
    displayMsg(isCrimson ? "Placing Crimson" : "Placing Azure");
    
    // Rotate arm to normal position for placement
    armManager.rotateToNormal();
    delay(800);
    
    // Lower to drop height
    armManager.liftTo(3);
    delay(800);
    
    // Open hand to release
    armManager.openHand();
    delay(800);
    
    // Move back
    backward(100, 100);
    delay(500);
    stop(100);
    
    // Reset arm
    armManager.liftTo(10);
    delay(500);
}

// Main Task 1 execution function
void executeTask1() {
    // Ensure arm manager is updated
    armManager.update();
    
    switch (currentState) {
        case T1_NAVIGATE_TO_FIRST_CUBE:
            if (millis() - stateStartTime < 100) return;
            stateStartTime = millis();
            
            displayMsg("Finding cube 1");
            setLineFollowPID('F');
            navigateMaze();
            break;
            
        case T1_APPROACH_FIRST_CUBE:
            approachCube();
            currentState = T1_COLLECT_FIRST_CUBE;
            break;
            
        case T1_COLLECT_FIRST_CUBE:
            collectCube();
            currentState = T1_STORE_FIRST_CUBE;
            break;
            
        case T1_STORE_FIRST_CUBE:
            storeCube();
            currentState = T1_NAVIGATE_TO_SECOND_CUBE;
            stateStartTime = millis();
            break;
            
        case T1_NAVIGATE_TO_SECOND_CUBE:
            if (millis() - stateStartTime < 100) return;
            stateStartTime = millis();
            
            displayMsg("Finding cube 2");
            navigateMaze();
            break;
            
        case T1_APPROACH_SECOND_CUBE:
            approachCube();
            currentState = T1_COLLECT_SECOND_CUBE;
            break;
            
        case T1_COLLECT_SECOND_CUBE:
            collectCube();
            currentState = T1_STORE_SECOND_CUBE;
            break;
            
        case T1_STORE_SECOND_CUBE:
            storeCube();
            currentState = T1_NAVIGATE_TO_CHAMBER;
            stateStartTime = millis();
            break;
            
        case T1_NAVIGATE_TO_CHAMBER:
            if (millis() - stateStartTime < 100) return;
            stateStartTime = millis();
            
            displayMsg("Finding chamber");
            navigateMaze();
            break;
            
        case T1_ENTER_CHAMBER:
            displayMsg("Entering chamber");
            stop(200);
            
            // Move forward into chamber center
            forward(150, 150);
            delay(1200);
            stop(200);
            
            // Check we have both cubes
            if (hasCrimsonStone && hasAzureStone) {
                currentState = T1_PLACE_CRIMSON;
            } else {
                taskManager.failCurrentTask();
                displayMsg("ERROR: Missing cubes!");
            }
            break;
            
        case T1_PLACE_CRIMSON:
            displayMsg("Crimson socket");
            
            // Turn left to Crimson socket (red - left side)
            anticlockwise(150, 150);
            delay(600);
            stop(200);
            
            // Move to socket
            forward(120, 120);
            delay(1000);
            stop(200);
            
            // Rotate arm to normal and place
            armManager.rotateToNormal();
            delay(500);
            placeCubeInSocket(true);
            hasCrimsonStone = false;
            
            // Return to center
            backward(120, 120);
            delay(1000);
            stop(200);
            
            currentState = T1_PLACE_AZURE;
            break;
            
        case T1_PLACE_AZURE:
            displayMsg("Azure socket");
            
            // Turn 180 to face Azure socket (blue - right side)
            clockwise(150, 150);
            delay(1200);
            stop(200);
            
            // Move to socket
            forward(120, 120);
            delay(1000);
            stop(200);
            
            // Rotate arm to normal and place
            armManager.rotateToNormal();
            delay(500);
            placeCubeInSocket(false);
            hasAzureStone = false;
            
            // Return to center
            backward(120, 120);
            delay(1000);
            stop(200);
            
            currentState = T1_EXIT_CHAMBER;
            break;
            
        case T1_EXIT_CHAMBER:
            displayMsg("Exiting");
            
            // Turn to face exit
            clockwise(150, 150);
            delay(600);
            stop(200);
            
            // Move forward to find line
            forward(120, 120);
            delay(800);
            
            // Check for line detection
            readMainQTR();
            int onCount = 0;
            for (int i = 0; i < MAIN_SENSOR_COUNT; i++) {
                if (mainQTRDval[i]) onCount++;
            }
            
            if (onCount >= 3) {
                currentState = T1_COMPLETE;
            }
            break;
            
        case T1_COMPLETE:
            displayMsg("Task 1 Complete!");
            stop(500);
            
            // Reset arm to safe position
            armManager.rotateToNormal();
            armManager.liftTo(10);
            armManager.openHand();
            
            // Complete task
            taskManager.completeCurrentTask();
            
            // Reset state
            currentState = T1_NAVIGATE_TO_FIRST_CUBE;
            hasCrimsonStone = false;
            hasAzureStone = false;
            cubesCollected = 0;
            firstCubeColor = "";
            secondCubeColor = "";
            break;
    }
}