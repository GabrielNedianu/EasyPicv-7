#define BUFFER_SIZE 32

// LCD module connections
//

char buffer[BUFFER_SIZE];
int buffer_index = 0;
int current_menu_index = 0;

// Menu options
char *menu[] = {"Option 1", "Option 2", "Option 3", "Exit"};
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

// Utility Functions
void send_to_terminal(char *message);

void main() {
    char received_char;
    // Call the init methods and display the standard menu

    while (1) {
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

void clear_buffer() {
    buffer_index = 0;
    buffer[0] = '\0';
    send_to_terminal("Waiting for new command: ");
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
     // If the menu index is > 0, decrement it and display the new menu
     // Also you need to send to the terminal some infos
}

void scroll_menu_down() {
     // Similar to the menu_up
}

void select_menu_option() {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, "Selected:");
    Lcd_Out(2, 1, menu[current_menu_index]); // Display selected option
    send_to_terminal("Selected option: ");
    send_to_terminal(menu[current_menu_index]);
    send_to_terminal("\n\r");
    Delay_ms(2000); // Show selection for 2 seconds
    display_menu();
}

void execute_command(char *command) {
    // You have 3 commands, UP, DOWN and SELECT
    // Based on the received command, call one of them and also if command is not recognized send to the terminal a log "Unknown command"
}
