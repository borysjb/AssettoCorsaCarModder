#include <bits/stdc++.h>
#include <curses.h>
#include "../helper_files/constants.h"


class Engine {
    std::filesystem::path carpath;
    std::vector<double> settings = std::vector<double>(21, 0.0); // Vector to hold engine settings
    // Initialize with 20 zeros
    std::set<int> engineInts = {3, 4, 5, 6, 7, 8, 14, 19}; // Indices for integer settings

        // All engine parameters are now stored in the 'settings' vector.
        // Use engineSettingNames for mapping indices to names.

        public:
    Engine() {}
    Engine(const std::filesystem::path &carpath) {
        this->carpath = carpath;
        parseEngineFile();
    }
    
    void getAttributeList(std::map<std::string, double> &attributes) {
        // Loop starting at index 1
        for (size_t i = 1; i < sizeof(engineSettingNames)/sizeof(engineSettingNames[0]); i++) {
            attributes[engineSettingNames[i]] = settings[i];
        }
    }

    double getBoost(const int rpm) const {
        // Assuming boost is stored at index 11 in settings
        return settings[11] * std::min(1.0, pow((rpm/settings[14]), settings[15]));
    }

    private:

        void parseEngineFile() {
            std::string line;
            std::ifstream file(carpath / "engine.ini");
            if (!file.is_open()) {
                // Handle missing file gracefully.
                return;
            }
            while(getline(file, line)) {
                // Skip comments and empty lines
                if (line.empty() || line[0] == ';' || line[0] == '[') {
                    continue;
                }

                std::size_t found = line.find("=");
                if (found == std::string::npos) {
                    continue;
                }

                // Remove whitespace and comments after values
                std::string key = line.substr(0, found);
                std::string value = line.substr(found + 1);
                size_t commentPos = value.find(";");
                if (commentPos != std::string::npos) {
                    value = value.substr(0, commentPos);
                }
                
                // Trim whitespace
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                int setCount = sizeof(engineSets)/sizeof(engineSets[0]);
                for (int i = 0; i < setCount; i++) {
                    if (key == engineSets[i]) {
                        try {
                            settings[i+1] = std::stod(value);
                        } catch (const std::exception &e) {
                            // In case of conversion failure, leave default value.
                        }
                        break;
                    }
                }
            }
            file.close();
        }
        
    public:
    
        void writeToEngineFile() {
            // Assuming settings vector indices:
            // [1] ALTITUDE_SENSITIVITY, [2] INERTIA, [3] MINIMUM (idleRPM), [4] LIMITER (redlineRPM),
            // [5] LIMITER_HZ, [6] COAST_REF RPM, [7] COAST_REF TORQUE, [8] COAST_REF NON_LINEARITY,
            // [9] TURBO LAG_DN, [10] TURBO LAG_UP, [11] MAX_BOOST (boost), [12] WASTEGATE,
            // [13] DISPLAY_MAX_BOOST, [14] REFERENCE_RPM (maxBoostRPM), [15] GAMMA (BoostGamma),
            // [16] COCKPIT_ADJUSTABLE, [17] TURBO_BOOST_THRESHOLD, [18] TURBO_DAMAGE_K,
            // [19] RPM_THRESHOLD, [20] RPM_DAMAGE_K
            std::filesystem::path filepath = carpath / "engine.ini";
            std::ofstream file(filepath);
            if (!file.is_open()) {
            return;
            }
            
            file << "[HEADER]\n"
             << "VERSION=1\n"
             << "POWER_CURVE=power.lut			; power curve file\n"
             << "COAST_CURVE=FROM_COAST_REF 		; coast curve. can define 3 different options (coast reference, coast values for mathematical curve, coast curve file)\n\n"
             << "[ENGINE_DATA]\n"
             << "ALTITUDE_SENSITIVITY=" << settings[1] << "		; sensitivity to altitude\n"
             << "INERTIA=" << settings[2] << "					; engine inertia\n"
             << "MINIMUM=" << static_cast<int>(settings[3]) << "					; engine idle rpm\n"
             << "LIMITER=" << static_cast<int>(settings[4]) << "					; engine rev limiter. 0 no limiter\n"
             << "LIMITER_HZ=" << static_cast<int>(settings[5]) << "\n\n"
             << "[COAST_REF]\n"
             << "RPM=" << static_cast<int>(settings[6]) << "						; rev number reference\n"
             << "TORQUE=" << static_cast<int>(settings[7]) << "						; engine braking torque value in Nm at rev number reference\n"
             << "NON_LINEARITY=" << settings[8] << "				; coast engine brake from ZERO to TORQUE value at rpm with linear (0) to fully exponential (1)\n\n"
             << "[COAST_DATA]\n"
             << "COAST0=0						; \n"
             << "COAST1=0\n"
             << "COAST=0.0000015\n\n"
             << "[COAST_CURVE]\n"
             << "FILENAME=coast.lut				; coasting curve file\n\n"
             << "[TURBO_0]\n"
             << "LAG_DN=" << settings[9] << " 				; Interpolation lag used slowing down the turbo\n"
             << "LAG_UP=" << settings[10] << "\n"
             << "MAX_BOOST=" << settings[11] << " \n"
             << "WASTEGATE=" << settings[12] << " \n"
             << "DISPLAY_MAX_BOOST=" << settings[13] << "		; Value used by display apps\n"
             << "REFERENCE_RPM=" << static_cast<int>(settings[14]) << "			; The reference rpm where the turbo reaches maximum boost (at max gas pedal).\n"
             << "GAMMA=" << settings[15] << "\n"
             << "COCKPIT_ADJUSTABLE=" << static_cast<int>(settings[16]) << "\n\n"
             << "[DAMAGE]\n"
             << "TURBO_BOOST_THRESHOLD=" << settings[17] << "\n"
             << "TURBO_DAMAGE_K=" << settings[18] << "			; amount of damage per second per (boost - threshold)\n"
             << "RPM_THRESHOLD=" << static_cast<int>(settings[19]) << "			; RPM at which the engine starts to take damage\n"
             << "RPM_DAMAGE_K=" << settings[20] << "			; amount of damage per second per (rpm-threshold)\n";
            
            file.close();
        }


        void modValue(int item) {
            std::string itemname = engineSettingNames[item];

            // Create a new window for the menu (height, width, start_y, start_x)
            WINDOW* win = newwin(10, 75, 5, 5);
            box(win, 0, 0);
            // Display the prompt and current value
            mvwprintw(win, 1, 2, "Modify setting for %s", itemname.c_str());
            mvwprintw(win, 2, 2, "Current value: %g", settings[item]);
            mvwprintw(win, 3, 2, "Enter new value (or 'c' to cancel): ");
            wrefresh(win);

            // Get input from the user
            char input[10];
            wgetnstr(win, input, 10);
            std::string strInput(input);

            if (strInput == "c" || strInput == "C") {
                mvwprintw(win, 6, 2, "Modification cancelled.");
                wrefresh(win);
                getch();  // Wait for a key press before closing the window
                delwin(win);
                return;
            }

            try {
                double newValue = std::stod(strInput);
                if (engineInts.contains(item) && newValue != static_cast<int>(newValue)) {
                    settings[item] = static_cast<int>(newValue); // Round to nearest integer if setting is an integer
                    mvwprintw(win, 6, 2, "Warning: Setting %s is an integer, rounding to %g", itemname.c_str(), settings[item]);
                } else if (item == 16 && newValue != 0 && newValue != 1) {
                    mvwprintw(win, 6, 2, "Warning: Setting %s is a boolean, enter value 0 or 1", itemname.c_str());
                } else {
                    settings[item] = newValue;
                    mvwprintw(win, 6, 2, "%s updated to %g", itemname.c_str(), settings[item]);
                }
                
            } catch (const std::exception &e) {
                mvwprintw(win, 5, 2, "Invalid input. Setting not modified.");
            }
            wrefresh(win);
            getch();
            delwin(win);
        }
        
};
