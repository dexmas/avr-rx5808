
#include <avr/pgmspace.h>

#include "settings.h"

//Each screen has the following
// public entry method
// private draw method
// public update method
// private update draw method

class screens
{
    private: // helper functions for screens.
        uint8_t last_rssi;
        uint8_t best_rssi;
        uint8_t last_channel;
	
        void reset();
        void drawTitleBox(const char *title);

    public:
        screens();
        char begin(const char *call_sign);

        // MAIN MENU
        void mainMenu(uint8_t menu_id);

        // SEEK & MANUAL MODE
        void seekMode(uint8_t state); // seek and manual mode
        void updateSeekMode(uint8_t state, uint8_t channelIndex, uint8_t channel, uint8_t rssi, uint16_t channelFrequency, uint8_t rssi_seek_threshold, bool locked); // seek and manual mode

        // BAND SCAN
        void bandScanMode(uint8_t state);
        void updateBandScanMode(bool in_setup, uint8_t channel, uint8_t rssi, uint8_t channelName, uint16_t channelFrequency, uint16_t rssi_setup_min_a, uint16_t rssi_setup_max_a);

        // DIVERSITY
        void diversity(uint8_t diversity_mode);
        void updateDiversity(char active_receiver, uint8_t rssiA, uint8_t rssiB);

        // VOLTAGE MENU
#ifdef USE_VOLTAGE_MONITORING
        void voltage(uint8_t menu_id, int voltage_calibration, uint8_t warning_voltage, uint8_t critical_voltage);
        void updateVoltage(int voltage);
#endif

        // SETUP MENU
        void setupMenu(uint8_t menu_id,bool settings_beeps,bool settings_orderby_channel, const char *call_sign, char editing);

        // SAVE
        void save(uint8_t mode, uint8_t channelIndex, uint16_t channelFrequency, const char *call_sign);
};
