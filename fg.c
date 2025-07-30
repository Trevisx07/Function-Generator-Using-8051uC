#include <reg51.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrins.h>
#define display_port P0      // Data pins connected to port 0 on microcontroller


sbit rs = P3^2;              // RS pin connected to pin 2 of port 3
sbit rw = P3^3;              // RW pin connected to pin 3 of port 3
sbit e =  P3^4;              // E pin connected to pin 4 of port 3

sbit C4 = P1^0;     // Connecting to Port 1
sbit C3 = P1^1;
sbit C2 = P1^2;
sbit C1 = P1^3;
sbit R4 = P1^4;
sbit R3 = P1^5;
sbit R2 = P1^6;
sbit R1 = P1^7;
sbit atoi_test = P3^5;
sfr DAC = 0x90; // Port P2 address

char freq_in[6];
long int freq_int = 0;
long unsigned int freq_square = 0;
long unsigned int freq_triangle = 0;
long unsigned int freq_sawtooth = 0;
long unsigned int freq_sine = 0;
char i = 0;

// Constants for waveform types
bit sine_flag =0;
bit square_flag =0;
bit triangle_flag =0;
bit sawtooth_flag =0;
int freq_selected = 0; // Flag indicating whether frequency is selected
int selected_waveform = 0; // Selected waveform type


void msdelay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 1275; j++);
}

void lcd_cmd(unsigned char command) {
    display_port = command;
    rs = 0;
    rw = 0;
    e = 1;
    msdelay(1);
    e = 0;
}


void clear_display_if_special() {
    if (C4 == 0) {  // Check if one of the special symbol keys is pressed
        lcd_cmd(0x80); // Clear screen
        msdelay(10);
    }
}
const unsigned char sine_table[50] = {
   128, 143, 159, 174, 189, 202, 215, 226,
235, 243, 249, 253, 255, 255, 253, 249,
243, 235, 226, 215, 202, 189, 174, 159,
143, 128, 112, 96, 81, 66, 53, 40,
29, 20, 12, 6, 2, 0, 0, 2,
6, 12, 20, 29, 40, 53, 66, 81,
96, 112
};
void generate_sine_wave(long int freq_sine) {
		
		/*//sine_flag =1;
    unsigned char index = 0;
    unsigned int period_us = 1000000 / freq_sine;
    unsigned int tmr_reload_value = 65536 - (unsigned int)((float)period_us * 11.0592 / 12.0);
		if (freq_sine <= 0) {
        // If frequency is not positive, exit the function
        return;
    }
    TMOD &= 0xF0;
    TMOD |= 0x01;

    TH0 = (unsigned char)(tmr_reload_value >> 8);
    TL0 = (unsigned char)tmr_reload_value;
    TR0 = 1;*/
		unsigned char index = 0; // Index for the sine lookup table
		unsigned char out_voltage=0;
    // Set up the output pin for the sine wave
    P2 = 0x00; // All pins are outputs

    // Set up Timer 0 for mode 1 (16-bit timer)
    TMOD = 0x01;

    // Set the timer reload value
    TH0 = TL0 = 65536 - (unsigned long int)((11059200UL / 12) * (1.0 / (4 * freq_sine)));
    while (1) {
        out_voltage = sine_table[index++];
        if (index >= 50) {
            index = 0;
        }
				P2 = out_voltage;
        nop();
    }
}


void generate_square_wave(long int freq_square) {
		unsigned int period_us;
    unsigned int tmr_reload_value;
		if (freq_square <= 0) {
        // If frequency is not positive, exit the function
        return;
    }
    // Calculate the period (in microseconds) for the desired frequency
    period_us = 1000000 / freq_square;
    // Calculate the timer reload value based on the system clock frequency (assuming 11.0592 MHz)
    tmr_reload_value = 65536 - (unsigned long int)((float)period_us * 11.0592 / 12.0);

    // Set up Timer 0 in Mode 1 (16-bit timer with auto-reload)
    TMOD &= 0xF0;  // Clear Timer 0 mode bits
    TMOD |= 0x01;  // Set Timer 0 to Mode 1

    // Set the initial value for Timer 0
    TH0 = (unsigned char)(tmr_reload_value >> 8);  // Load high byte
    TL0 = (unsigned char)tmr_reload_value;         // Load low byte

    // Start Timer 0
    TR0 = 1;
		
    // Main loop
    while (1) {
        // Check if Timer 0 overflowed
        if (TF0) {
            // Toggle Port 2 for DAC (assuming all pins are configured as outputs)
            P2 = ~P2;
            // Clear Timer 0 overflow flag
            TF0 = 0;
            // Reload Timer 0 with initial value
            TH0 = (unsigned char)(tmr_reload_value >> 8);  // Load high byte
            TL0 = (unsigned char)tmr_reload_value;         // Load low byte
        }
    }
}

void generate_triangular_wave(long int freq_triangle) {
		unsigned int period_us;
    unsigned int tmr_reload_value;
    unsigned int count = 0; // initialize the counter to 0
    bit ascending = 1; // flag to indicate ascending or descending
		if (freq_triangle <= 0) {
        // If frequency is not positive, exit the function
        return;
    }
		// Set the desired frequency of the triangular wave
//#define FREQ 1000 // Hz

// Set the reference voltage for the triangular wave
#define VREF 5.0 // volts

// Calculate the step size
#define STEP_SIZE (255 / VREF)

// The timer reload value
//#define TMR_RELOAD (65536 - (11059200 / 12) * (1 / freq_triangle))
		// Calculate the period (in microseconds) for the desired frequency
    period_us = 1000000 / freq_triangle;
    // Calculate the timer reload value based on the system clock frequency (assuming 11.0592 MHz)
    tmr_reload_value = 65536 - (unsigned long int)((float)period_us * 11.0592 / 12.0)* (1 / freq_triangle);
    // Set up the output pin for the triangular wave
    P2 = 0x00; // all pins are outputs

    // Set up timer 0 for mode 1 (16-bit timer)
    TMOD = 0x01;

    // Set the timer reload value
    TH0 = (tmr_reload_value >> 8) & 0xFF;
    TL0 = tmr_reload_value & 0xFF;
		
    // Main loop
    while (1)
    {
        // Generate triangular waveform
        if (count == 0) {
            ascending = 1; // Start ascending
        } else if (count == 255) {
            ascending = 0; // Start descending
        }

        if (ascending) {
            count++;
        } else {
            count--;
        }

        // Output triangular wave value
        P2 = count;

        // Add a small delay to control the frequency
        // Adjust the delay as needed to achieve the desired frequency
        nop ();
				nop ();
				
    }
}


void generate_sawtooth_wave(long int freq_sawtooth) {
		unsigned int period_us;
    unsigned int tmr_reload_value;
		unsigned char count = 0; // initialize the counter to 0
		bit ascending = 1; // flag to indicate ascending or descending
		// Calculate the period (in microseconds) for the desired frequency
    period_us = 1000000 / freq_sawtooth;
    // Calculate the timer reload value based on the system clock frequency (assuming 11.0592 MHz)
    tmr_reload_value = 65536 - (unsigned long int)((float)period_us * 11.0592 / 12.0)*(1 / freq_sawtooth);
    
    
		if (freq_sawtooth <= 0) {
        // If frequency is not positive, exit the function
        return;
    }
		// Set the desired frequency of the triangular wave
//#define FREQ 1000 // Hz

// Set the reference voltage for the triangular wave
#define VREF 5.0 // volts

// Calculate the step size
#define STEP_SIZE (255 / VREF)

    P2 = 0x00; // all pins are outputs

    // Set up timer 0 for mode 1 (16-bit timer)
    TMOD = 0x01;

    // Set the timer reload value
    TH0 = (tmr_reload_value >> 8) & 0xFF;
    TL0 = tmr_reload_value & 0xFF;
		
    // Main loop
    while (freq_selected)
    {
        // Generate triangular waveform
        if (count == 0) {
            ascending = 1; // Start ascending
        } else if (count == 255) {
            ascending = 0; // Start descending
        }

        if (ascending) {
            count++;
        } else {
            count=0;
        }

        // Output triangular wave value
        P2 = count;

        // Add a small delay to control the frequency
        // Adjust the delay as needed to achieve the desired frequency
        nop ();
    }
}
// Function to handle 'e' press
void handle_e_press() {
    freq_int = atoi(freq_in); // Convert frequency input to integer
    freq_selected = 1; // Set flag indicating frequency is selected
        
            // Frequency is selected, wait for 'e' press to generate waveform
            if (sine_flag) {
                generate_sine_wave(freq_int);
            } else if (square_flag) {
                generate_square_wave(freq_int);
            } else if (triangle_flag) {
                generate_triangular_wave(freq_int);
            } else if (sawtooth_flag) {
                generate_sawtooth_wave(freq_int);
            } else {
                nop();
            }
       
}

void lcd_data(unsigned char disp_data) {
    display_port = disp_data;
    rs = 1;
    rw = 0;
    e = 1;
    msdelay(1);
    e = 0;
}

void lcd_init() {
    lcd_cmd(0x38);  // for using 2 lines and 5X7 matrix of LCD
    msdelay(10);
    lcd_cmd(0x0F);  // turn display ON, cursor blinking
    msdelay(10);
    lcd_cmd(0x01);  // clear screen
    msdelay(10);
    lcd_cmd(0x81);  // bring cursor to position 1 of line 1
    msdelay(10);
}
void row_finder1() {
    R1 = R2 = R3 = R4 = 1; // Set all rows to high
    C1 = 0;C2 = C3 = C4 = 1; // Ground column 1
		msdelay(20); // Wait for signals to settle
    
    if (R1 == 0) {
        //lcd_data('7');
        //freq_in[i++] = '7';
				lcd_data('1');
        freq_in[i++] = '1';
    }
    else if (R2 == 0) {
        lcd_data('4');
        freq_in[i++] = '4';
    }
    else if (R3 == 0) {
        //lcd_data('1');
        //freq_in[i++] = '1';
				lcd_data('7');
        freq_in[i++] = '7';
    }
    else if (R4 == 0) {
        
				atoi_test =0;
				//freq_int = atoi(freq_in); // Convert the string to an integer
				handle_e_press();
        lcd_data('e');
    }
}





void row_finder2() {
    R1 = R2 = R3 = R4 = 1;
    C2 = 0; C1 = C3 = C4 = 1;
		msdelay(20); // Wait for signals to settle
    if (R1 == 0) {
        //lcd_data('8');
        //freq_in[i++] = '8';
				lcd_data('2');
        freq_in[i++] = '2';
    }
    if (R2 == 0) {
        lcd_data('5');
        freq_in[i++] = '5';
    }
    if (R3 == 0) {
				lcd_data('8');
        freq_in[i++] = '8';
        //lcd_data('2');
        //freq_in[i++] = '2';
    }
    if (R4 == 0) {
        lcd_data('0');
        freq_in[i++] = '0';
    }
}

void row_finder3() {
    R1 = R2 = R3 = R4 = 1;
    C1 = C2 = C4 = 1;
    C3 = 0;  // Activate column 3
    msdelay(20);  // Wait for signals to settle

    if (R1 == 0) {
        lcd_data('3');
        freq_in[i++] = '3';
    }
    else if (R2 == 0) {
        lcd_data('6');
        freq_in[i++] = '6';
    }
    else if (R3 == 0) {
        lcd_data('9');
        freq_in[i++] = '9';
    }
    else if (R4 == 0) {
        msdelay(5);  // Adjust delay as needed
        lcd_cmd(0xC0); // Move cursor to the beginning of the second line
        msdelay(10);
        lcd_data('F');
        msdelay(15);
        lcd_data('r');
        msdelay(15);
        lcd_data('e');
        msdelay(15);
        lcd_data('q');
        lcd_data(' ');
        msdelay(15);
        lcd_data('{');
        msdelay(15);
        lcd_data('H');
        lcd_data('z');
        msdelay(15);
        lcd_data('}');
        lcd_data(' ');
    }
}

void row_finder4() {
    R1 = R2 = R3 = R4 = 1;
    C1 = C2 = C3 = 1;
    C4 = 0; // Activate column 4
    msdelay(10); // Wait for signals to settle

    if (R1 == 0) {
        clear_display_if_special(); // Clear the display if a special symbol key is pressed
        lcd_data('S');
        msdelay(15);
        lcd_data('i');
        msdelay(15);
        lcd_data('n');
        msdelay(15);
        lcd_data('e');
        //generate_sine_wave(freq_int);
				sine_flag =1;
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');

    }
    else if (R2 == 0) {
        clear_display_if_special(); // Clear the display if a special symbol key is pressed
        lcd_data('S');
        msdelay(15);
        lcd_data('q');
        msdelay(15);
        lcd_data('u');
        msdelay(15);
        lcd_data('a');
        msdelay(15);
        lcd_data('r');
        msdelay(15);
        lcd_data('e');
				square_flag =1;
        //generate_square_wave(freq_int); // Generate square wave continuously
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
    }
    else if (R3 == 0) {
        clear_display_if_special(); // Clear the display if a special symbol key is pressed
        lcd_data('T');
        msdelay(15);
        lcd_data('r');
        msdelay(15);
        lcd_data('i');
        msdelay(15);
        lcd_data('a');
        msdelay(15);
        lcd_data('n');
        msdelay(15);
        lcd_data('g');
        msdelay(15);
        lcd_data('l');
        msdelay(15);
        lcd_data('e');
				//generate_triangular_wave(freq_int);
				triangle_flag =1;
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
    }
    else if (R4 == 0) {
        clear_display_if_special(); // Clear the display if a special symbol key is pressed
        lcd_data('S');
        msdelay(15);
        lcd_data('a');
        msdelay(15);
        lcd_data('w');
        msdelay(15);
        lcd_data('t');
        msdelay(15);
        lcd_data('o');
        msdelay(15);
        lcd_data('o');
        msdelay(15);
        lcd_data('t');
        msdelay(15);
        lcd_data('h');
				//generate_sawtooth_wave(freq_int);
				sawtooth_flag =1;
        lcd_data(' ');
        lcd_data(' ');
        lcd_data(' ');
    }

    
}

void scan_keypad() {
    // Ground all rows
    R1 = R2 = R3 = R4 = 0;
    // Set all columns as input
    C1 = C2 = C3 = C4 = 1;
		msdelay(15);
    if(C1==0)
				row_finder1();
    else if(C2==0)
        row_finder2();
    else if(C3==0)
        row_finder3();
    else if(C4==0)
        row_finder4();
}

void display_function_generator() {
    lcd_cmd(0x01); // Clear screen
    msdelay(10);
    
    lcd_cmd(0x80); // Set cursor to the beginning of the first line
    msdelay(10);
    lcd_data('F');
    msdelay(15);
    lcd_data('U');
    msdelay(15);
    lcd_data('N');
    msdelay(15);
    lcd_data('C');
    msdelay(15);
    lcd_data('T');
    msdelay(15);
    lcd_data('I');
    msdelay(15);
    lcd_data('O');
    msdelay(15);
    lcd_data('N');
    msdelay(15);

    lcd_cmd(0xC0); // Set cursor to the beginning of the second line
    msdelay(10);
    lcd_data('G');
    msdelay(15);
    lcd_data('E');
    msdelay(15);
    lcd_data('N');
    msdelay(15);
    lcd_data('E');
    msdelay(15);
    lcd_data('R');
    msdelay(15);
    lcd_data('A');
    msdelay(15);
    lcd_data('T');
    msdelay(15);
    lcd_data('O');
    msdelay(15);
    lcd_data('R');
    msdelay(15);
}

void clear_display() {
    lcd_cmd(0x01); // Clear screen
    msdelay(10);
}
void main() {
    unsigned char keypad_pressed = 0;

    // Initialize LCD
    lcd_init();
    
    // Display "FUNCTION GENERATOR" on reset
    display_function_generator();

    while (1) {
        // Check if any keypad button is pressed
        scan_keypad();
        if (keypad_pressed) {
            // Clear the display
            clear_display();
            // Reset the flag to indicate no button is pressed
            keypad_pressed = 0;
        }
    }
}
