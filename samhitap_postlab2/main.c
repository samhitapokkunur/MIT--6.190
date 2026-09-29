#include <stdio.h>
#include "6190.h"
#include <string.h>
#include "ascii.h"

void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array
}


int messageLength(char* message){
    //////////////////////////////////////////
    // TO-DO: Complete messageLength()      //
    //////////////////////////////////////////

    char *ptr = message;
    while (*ptr != '\0') {
        ptr++;
    }
    return ptr - message;



}

void fillScreenBuffer(char *message, int total_offset) {
    int message_length = strlen(message);
    
    // clear screen buffer
    for (int row = 0; row < 8; row++) {
        screen_buffer[row] = 0;
    }
    
    // for each column choose which character and bit to display
    for (int col = 0; col < 32; col++) {

        int absolute_position = total_offset + col;
        int char_idx = absolute_position / 8;
        int bit_idx = absolute_position % 8;
        
        // check if we are within message limit
        if (char_idx >= message_length) break;
        
        char current_char = message[char_idx];
        
        // each row, extract bit from char
        for (int row = 0; row < 8; row++) {
            uint8_t char_pattern = ascii[(int)current_char][row];
            
            // 7 leftmost, 0 rightmost
            int bit_value = (char_pattern >> (7 - bit_idx)) & 1;
            
            if (bit_value) {
                // bit 31 leftmost column, 0 rightmost column
                screen_buffer[row] |= (1U << (31 - col));
            }
        }
    }
} 


void app_main(){
    setup();

    char message[] = "HELLO HAVE A GOOD DAY";
    int len = messageLength(message);  // TASK 1: CALCULATE MESSAGE LENGTH
    int offset = 0;
    while(1){
        if (offset == len * 8){  
            offset = 0; // Reset if we have reached the end of the message
        } else {
            eraseBuffer();  // Clear screen buffer
            fillScreenBuffer(message, offset); // TASK 2: FILL SCREEN BUFFER WITH CORRECT DATA
            drawBuffer();   // Transmit screen buffer data to screen
            offset++;        // Move forward
        }

        int start = millis();            // Get "start" time stamp
        while(millis() - start < 100);   // Wait until (current time stamp - "start") >= 100ms   
    }
}
