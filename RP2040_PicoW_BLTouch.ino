#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "hardware/gpio.h"

const uint PULSE_PIN = 9;  // Pin to output the pulse
const uint BLTOUCH_SENSOR_PIN = 6;  // Pin to read BLTouch sensor state

// Function to generate a pulse of specified width in microseconds
void generate_pulse(uint32_t pulse_width_us) {
    gpio_put(PULSE_PIN, 1);  // Set pin high
    busy_wait_us(pulse_width_us);  // Wait for specified microseconds
    gpio_put(PULSE_PIN, 0);  // Set pin low
}

void bltouch_deploy() {
    for (int x = 0; x < 4; x++) {
        generate_pulse(640);  // Generate the 640μs pulse
        sleep_ms(20);  // Wait for 20ms (50Hz refresh rate)
    }
}

void bltouch_retract() {
    for (int x = 0; x < 4; x++) {
        generate_pulse(1500);  // Generate the 1500μs pulse
        sleep_ms(20);  // Wait for 20ms (50Hz refresh rate)
    }
}

void bltouch_self_test() {
    for (int x = 0; x < 4; x++) {
        generate_pulse(1800);  // Generate the 1800μs pulse
        sleep_ms(20);  // Wait for 20ms (50Hz refresh rate)
    }
}

void bltouch_alarm_release() {
    for (int x = 0; x < 4; x++) {
        generate_pulse(2200);  // Generate the 2200μs pulse
        sleep_ms(20);  // Wait for 20ms (50Hz refresh rate)
    }
}

bool is_bltouch_triggered() {
    return gpio_get(BLTOUCH_SENSOR_PIN);  // BLTouch triggers when pin goes HIGH
}

void print_menu() {
    Serial.println("\nBLTouch Control Menu:");
    Serial.println("1: Deploy probe (640μs pulses)");
    Serial.println("2: Retract probe (1500μs pulses)");
    Serial.println("3: Self-test (1800μs pulses)");
    Serial.println("4: Alarm release (2200μs pulses)");
    Serial.println("5: Read sensor state");
    Serial.print("Enter choice: ");
}

void setup() {
    Serial.begin(115200);
    
    // Initialize GPIO
    gpio_init(PULSE_PIN);
    gpio_set_dir(PULSE_PIN, GPIO_OUT);
    
    gpio_init(BLTOUCH_SENSOR_PIN);
    gpio_set_dir(BLTOUCH_SENSOR_PIN, GPIO_IN);
    gpio_pull_up(BLTOUCH_SENSOR_PIN);

    Serial.println("BLTouch Control System Initialized");
    print_menu();
}

void loop() {
    if (Serial.available() > 0) {
        char choice = Serial.read();
        
        // Consume any additional characters (like newline)
        while (Serial.available()) Serial.read();
        
        Serial.print("You chose: ");
        Serial.println(choice);
        
        switch(choice) {
            case '1':
                Serial.println("Deploying probe...");
                bltouch_deploy();
                break;
            case '2':
                Serial.println("Retracting probe...");
                bltouch_retract();
                break;
            case '3':
                Serial.println("Performing self-test...");
                bltouch_self_test();
                break;
            case '4':
                Serial.println("Releasing alarm...");
                bltouch_alarm_release();
                break;
            case '5':
                Serial.print("Sensor state: ");
                Serial.println(is_bltouch_triggered() ? "Triggered" : "Not triggered");
                break;
            default:
                Serial.println("Invalid choice. Please try again.");
        }
        
        print_menu();
    }
}