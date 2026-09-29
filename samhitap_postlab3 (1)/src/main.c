#include <stdio.h>
#include <stdint.h>
#include "6190 2-1.h"

// Constants
#define SCREEN_ROWS 8
#define SCREEN_COLS 32
enum snake_direction { up, down, left, right };

// Setup for random value generator
#define RANDOM_VALUE_ADDR 0x600260B0 
#define RTC_CNTL_CLK_CONF_REG 0x60008070

void setupRandom(){
    int *rtc_cntl_clk_conf_rg = (int*) RTC_CNTL_CLK_CONF_REG;
    *rtc_cntl_clk_conf_rg |= (1 << 10);
}


// Snake struct definition
struct Snake {
    uint8_t body[SCREEN_COLS*SCREEN_ROWS]; // can get as large as entire screen in theory
    uint8_t direction;
    uint8_t length; 
};


void setup(){
    timerSetup();
    setupRandom();
    setupDisplay();
    eraseBuffer();
    int snake_buttons[5] = {BTNF, BTNU, BTND, BTNR, BTNL};
    for (int i = 0; i < 5; i++){
        pinSetup(snake_buttons[i], GPIO_INPUT);
    }
}

uint8_t getX(uint8_t location){
    /* Function that returns the x coordinate from an 8-bit value containing both x and y coordinates.
    Arguments:
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns:
        uint8_t representing the x-coordinate (0-31) encoded in the upper 5 bits of the argument.
    */

    // TO-DO: Implement getX()

    return (location >> 3);

}

uint8_t getY(uint8_t location){
    /* Function that returns the y coordinate from an 8-bit value containing both x and y coordinates.
    Arguments:
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns:
        uint8_t representing the y-coordinate (0-7) encoded in the lower 3 bits of the argument.
    */


    return (location & 0b00000111);

}


void setX(uint8_t *location, uint8_t new_x){
    /* Function that updates the upper 5 bits of the value stored at location to be equal to new_x.
    Arguments:
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_x: an unsigned integer representing the value (0-31) that we'd like to set for the x location.
    */

    // TO-DO: Implement setX()


    // Clear upper 5 bits (keep lower 3 bits)
    *location &= 0b00000111;   // 00000111

    // Insert new_x into upper 5 bits
    *location |= (new_x << 3);

}


void setY(uint8_t *location, uint8_t new_y){
    /* Function that updates the lower 3 bits of the value stored at location to be equal to new_y.
    Arguments:
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_y an unsigned integer representing the value (0-7) that we'd like to set for the y location.
    */

    // TO-DO: Implement setY()

    new_y = new_y % 8;   

    // Clear lower 3 bits (keep upper 5)
    *location &= 0xF8;   // 11111000

    // Insert new_y
    *location |= new_y;


}


void setPixel(uint8_t location, uint8_t val){
    /* Function that sets the value of an individual pixel (LED) on the display.
    Arguments:
        location: The location of the LED to be turned on/off on the LED array
        val: The binary (0 or 1) value indicating if the LED at the location should be off (0) or on (1).
    */

    // TO-DO: Implement setPixel()

    uint8_t x = getX(location);
    uint8_t y = getY(location);

    if (val) screen_buffer[y] |= (1 << x);
    else screen_buffer[y] &= ~(1 << x);
    

    

}


void drawBoard(struct Snake *snake, uint8_t food){
    /* Function to render the snake and food on the board on each given time-step.
    Arguments:
        struct Snake *snake: pointer to snake struct
        uint8_t food: location of food to render
    */

    // TODO: Implement drawBoard()

    eraseBuffer();
    
    for (int i = 0; i < snake->length; i++){
        setPixel(snake->body[i], 1);
    }

    // Draw food
    setPixel(food, 1);

    drawBuffer();

}



void updateSnake(struct Snake *snake){

    // Move body (back to front)
    for (int i = snake->length - 1; i > 0; i--){
        snake->body[i] = snake->body[i - 1];
    }

    // Get current head position
    uint8_t x = getX(snake->body[0]);
    uint8_t y = getY(snake->body[0]);

    // Update head position based on direction
    if (snake->direction == left){
        x = (x + 31) % 32;   // wrap around end at left
    }
    else if (snake->direction == right){
        x = (x + 1) % 32;    // wrap around end at right
    }
    else if (snake->direction == up){
        y = (y + 7) % 8;     // wrap around end at up
    }
    else if (snake->direction == down){
        y = (y + 1) % 8;     // wrap around end at down
    }

    // Set new head position
    setX(&snake->body[0], x);
    setY(&snake->body[0], y);

}


int generateFood(struct Snake *snake, uint8_t *food){
    /* Function to randomly generate food for the snake.
    Arguments: 
        struct Snake *snake: pointer to snake struct
        uint8_t *food: pointer to variable holding the food's location
    Returns:
        int: 0 if food does not conflict with the snake, 1 if it does.
    */

    int *random_value_ptr = (int*) RANDOM_VALUE_ADDR; // read this address to obtain a random integer value

    int random = *random_value_ptr;
    uint8_t foodnew = random % 256;

    for (int i = 0; i < snake->length; i++)
    {
        if (snake->body[i] == foodnew)
        {
            return 1;
        }
    }
    *food = foodnew;
    return 0;
    ////////////////////////////////////
    // TO-DO: Complete generateFood() // 
    ////////////////////////////////////



    return 0; // UPDATE
}

int snakeAteFood(uint8_t *snake_body, uint8_t food){
     /* Function to determine if the snake collided with (and ate) the food.
    Arguments: 
        uint8_t *snake_body: pointer to the snake body array
        uint8_t food: variable holding the food's location
    Returns:
        int: 0 if the snake did not eat the food, 1 if it did.
    */
    
    // TO-DO: Implement snakeAteFood()
    if (snake_body[0] == food) {
        return 1;
    }
    return 0;
}



uint8_t snakeCollisionCheck(struct Snake *snake){
    /* Function to determine if the snake collided with itself.
    Arguments: 
        struct Snake *snake: pointer to snake array
    Returns:
        int: 0 if the snake did not collide with itself, 1 if it did.
    */

    // TO-DO: Implement snakeCollisionCheck()
    uint8_t head = snake->body[0];
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i] == head) {
            return 1; // Collision detected
        }
    }
    return 0; // No collision

}



void app_main() {
    setup(); // System setup

    // Snake initialization
    struct Snake snake;
    // COMMENT OUT FOR PART 10
    // snake.body[0] = 0;
    // snake.direction = left;
    // snake.length = 1;
    pinSetup(BTNC, GPIO_INPUT); // Button for PAUSE


    // UNCOMMENT FOR PART 10
    for (int i=0; i<3; i++){
        setX(&snake.body[i], 5);
        setY(&snake.body[i], 3-i);
    }
    snake.length = 3;
    snake.direction = left;

    uint8_t food = 0; // food in top right corner of the game board

    while(generateFood(&snake, &food) != 0); // Generate food initially
    drawBoard(&snake, food);

    int paused = 0; // 0 = running, 1 = paused
    int gameOver = 0; // 0 = playing, 1 = dead

    while(1){       
        int button_pause_pressed = !pinRead(BTNC);

        if (button_pause_pressed) {
            paused = !paused;  // toggle pause state
            int start = millis();
            // simple debounce: wait 300ms to prevent multiple toggles
            while (millis() - start < 300); 
        } 
        int button_left_pressed = !pinRead(BTNL);
        int button_right_pressed = !pinRead(BTNR);
        int button_up_pressed = !pinRead(BTNU);
        int button_down_pressed = !pinRead(BTND);
        int button_reset_pressed = !pinRead(BTNF); // reset
        //////////////////////////////////////////////
        // INSERT BUTTON PRESS DETECTION LOGIC HERE //
        //////////////////////////////////////////////
        if (button_reset_pressed) {
            // Reset snake to initial state
            snake.length = 3;          // length 3
            snake.direction = left;     // moving left
            for (int i = 0; i < 3; i++){
                setX(&snake.body[i], 5);      // head at x = 5
                setY(&snake.body[i], 3 - i);  // y = 3,2,1
            }

            // Generate new food
            while(generateFood(&snake, &food) != 0);

            gameOver = 0;   // clear game over state
            paused = 0;     // resume game

            // Redraw board
            drawBoard(&snake, food);

            // Skip rest of loop this iteration
            continue;
        }

        if (!paused && !gameOver) {
            if (button_left_pressed && snake.direction != left){
                snake.direction = right;
                printf("BTNL\n");
            }
            else if (button_right_pressed && snake.direction != right){
                snake.direction = left;
                printf("BTNR");
            }
            else if (button_up_pressed && snake.direction != down){
                snake.direction = up;
                printf("BTNU");
            }
            else if (button_down_pressed && snake.direction != up){
                snake.direction = down;
                printf("BTND");
            }

            updateSnake(&snake);
            drawBoard(&snake, food);

            if (snakeAteFood(snake.body, food) == 1){
                //////////////////////////////
                // TO-DO: "Grow" snake      // 
                //////////////////////////////
                snake.length += 1;   
                // Generate food
                while (generateFood(&snake, &food) != 0); // generate food
            }

            if (snakeCollisionCheck(&snake) == 1){
                // Game ends
                eraseBuffer(); // Clear game board
                drawBuffer();
                gameOver = 1; // mark game as over
            }
        }

        // Code to limit frequency of loop so that gameplay is possible.
        // Put all game code above this point.
        int start = millis();
        while (millis() - start < 200); // One loop iteration every ~200ms
    }
}

