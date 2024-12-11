// //hoverButton.c

// #include "headers/hoverButton.h"
// #include "headers/structs.h"

// void handleSelectionBarHover(double xpos, double ypos, int width, int height) {
//     // Calculate button positions
//     float sbWidth = width * 0.2f;
//     float padding = 20.0f;
//     float buttonHeight = 100.0f;
//     float yPos = height - buttonHeight - padding;

//     // Adjust yPos because window coordinates start from top-left
//     ypos = height - ypos;

//     // Check if Circle Button was clicked
//     if (ypos >= yPos && ypos <= yPos + buttonHeight) {
//         selectedShapeType = SHAPE_CIRCLE;
//     }

//     // Check if Square Button was clicked
//     yPos -= (buttonHeight + padding);
//     if (ypos >= yPos && ypos <= yPos + buttonHeight) {
//         selectedShapeType = SHAPE_SQUARE;
//     }
// }

// void handlePauseButtonClick(double xpos, double ypos, int width, int height) {
//     int topBarHeight = 100; 
//     int buttonSize = 60; 
//     float padding = 20.0f;

//     ypos = height - ypos;

//     if (ypos >= height - topBarHeight && ypos <= height - topBarHeight + buttonSize) {
//         if (xpos >= width - buttonSize - padding && xpos <= width - padding) {

//         }
//     }
// }