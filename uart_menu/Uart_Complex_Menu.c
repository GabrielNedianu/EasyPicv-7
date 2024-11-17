#define BUFFER_SIZE 32

// LCD module connections

char buffer[BUFFER_SIZE];
int buffer_index = 0;
int selected_option = -1;  // -1 = No option selected
int current_menu_index = 0;

// Menu options
char *menu[] = {"Potentiometer", "Option 2", "Option 3", "Exit"};
int menu_size = 4;

// Function Prototypes
void init_UART();
void init_LCD();
void execute_command(char *command);
void clear_buffer();
void display_menu();
void scroll_menu_up();
void scroll_menu_down();
void select_menu_option();

// Option Handlers
void process_potentiometer();
void process_exit();
void process_not_implemented();

// Utility Functions
void send_to_terminal(char *message);

void main() {
    char received_char;

    init_UART();
    init_LCD();
                // Configure PORTA as input for ADC
    display_menu();

    while (1) {
        if (selected_option >= 0) {
            switch (selected_option) {
                case 0: process_potentiometer(); break;
                case 3: process_exit(); break;
                default: process_not_implemented(); break;
            }
            selected_option = -1; // Reset option after processing
            display_menu();
        }

        // Handle UART input
        if (UART1_Data_Ready()) {
            received_char = UART1_Read();

            // Echo received character
            UART1_Write(received_char);

            if (received_char == '\r' || buffer_index >= BUFFER_SIZE - 1) {
                buffer[buffer_index] = '\0'; // Null-terminate the string
                send_to_terminal("\n\rReceived command: ");
                send_to_terminal(buffer);
                send_to_terminal("\n\r");
                execute_command(buffer);    // Execute command
                clear_buffer();
            } else {
                buffer[buffer_index++] = received_char;
            }
        }
    }
}

void send_to_terminal(char *message) {
    UART1_Write_Text(message); // Send the message to the terminal
}

void clear_buffer() {
    buffer_index = 0;
    buffer[0] = '\0';
    send_to_terminal("\rWaiting for new command: ");
}

void display_menu() {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "<- ");
    Lcd_Out(1, 4, menu[current_menu_index]); // Display current option
    if (current_menu_index + 1 < menu_size) {
        Lcd_Out(2, 1, menu[current_menu_index + 1]); // Display next option
    } else {
        Lcd_Out(2, 1, " "); // Clear second row
    }
}

void scroll_menu_up() {
    if (current_menu_index > 0) {
        current_menu_index--;
        display_menu();
        send_to_terminal("Scrolling up...\n\r");
    } else {
        send_to_terminal("Already at the top!\n\r");
    }
}

void scroll_menu_down() {
    if (current_menu_index < menu_size - 1) {
        current_menu_index++;
        display_menu();
        send_to_terminal("Scrolling down...\n\r");
    } else {
        send_to_terminal("Already at the bottom!\n\r");
    }
}

void select_menu_option() {
    send_to_terminal("Selected option: ");
    send_to_terminal(menu[current_menu_index]);
    send_to_terminal("\n\r");

    selected_option = current_menu_index;
}

void execute_command(char *command) {
    if (strcmp(command, "UP") == 0) {
        scroll_menu_up();
    } else if (strcmp(command, "DOWN") == 0) {
        scroll_menu_down();
    } else if (strcmp(command, "SELECT") == 0) {
        select_menu_option();
    } else {
        send_to_terminal("Unknown command.\n\r");
        display_menu();
    }
}

void process_potentiometer() {
    unsigned int adc_value;
    char adc_str[7];

    while (1) {
        // Read potentiometer value from RA0, convert ADC value to string and trim left + right empty spaces

        // Clear display and write //Pot Value: {adc_str} //


        Delay_ms(300); // Refresh every 300 ms

        if (UART1_Data_Ready()) { // Exit potentiometer mode on UART input
            clear_buffer();
            break;
        }
    }
}

void process_exit() {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "Exiting...");
    Delay_ms(5000);
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "PRESS RESET BUTTON");
    Lcd_Out(2, 1, "TO START...");
    while (1); // Stop program execution
}

void process_not_implemented() {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "Feature Not");
    Lcd_Out(2, 1, "Implemented!");
    Delay_ms(3000);
}
