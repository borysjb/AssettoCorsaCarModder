#include <bits/stdc++.h>
#include <curses.h>

class Engine {
    std::filesystem::path carpath;
    std::string torqueFile;
    std::map<int, int> torqueCurve;
    
    
    double altitudeSens;
    double inertia;
    int idleRPM;
    int redlineRPM;
    int limiterHZ;

    int coastReference;
    int coastTorque;
    int coastNonlinearity;

    double turboLagUp;
    double turboLagDown;
    double boost;
    double wastegate;
    double maxDisplayBoost;
    double maxBoostRPM;
    double BoostGamma;
    bool boostAdjustable;

    double turboThreshold;
    double turboDamage;
    double RPMThreshold;
    double RPMDamage;

    public:
    Engine() {}
    Engine(const std::filesystem::path &carpath) {
        this->carpath = carpath;
        torqueFile = "power.lut";
        parseEngineFile();
    }

    std::map<int, int> getTorqueCurve() {
        return torqueCurve;
    }

    private:
        void parseTorqueFile() {
            std::string line;
            std::ifstream file(carpath / torqueFile);  // Add "data" subfolder
            if (!file.is_open()) {
                throw std::runtime_error("Could not open power.lut file");
            }

            while(getline(file, line)) {
                // Skip comments and empty lines
                if (line.empty() || line[0] == ';' || line[0] == '[') {
                    continue;
                }

                std::size_t found = line.find("|");
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

                // Parse the torque curve
                int rpm = std::stoi(key);
                int torque = std::stoi(value);
                torqueCurve[rpm] = torque;
            }
            file.close();
        }

        void parseEngineFile() {
            std::string line;
            std::ifstream file(carpath / "engine.ini");  // Add "data" subfolder
            if (!file.is_open()) {
                throw std::runtime_error("Could not open engine.ini file");
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


                if (key == "ALTITUDE_SENSITIVITY") {
                    altitudeSens = std::stod(value);
                } else if (key == "INERTIA") {
                    inertia = std::stod(value);
                } else if (key == "MINIMUM") {
                    idleRPM = std::stoi(value);
                } else if (key == "LIMITER") {
                    redlineRPM = std::stoi(value);
                } else if (key == "LIMITER_HZ") {
                    limiterHZ = std::stoi(value);
                } else if (key == "RPM") {
                    coastReference = std::stoi(value);
                } else if (key == "TORQUE") {
                    coastTorque = std::stoi(value);
                } else if (key == "NON_LINEARITY") {
                    coastNonlinearity = std::stoi(value);
                } else if (key == "LAG_DN") {
                    turboLagDown = std::stod(value);
                } else if (key == "LAG_UP") {
                    turboLagUp = std::stod(value);
                } else if (key == "MAX_BOOST") {
                    boost = std::stod(value);
                } else if (key == "WASTEGATE") {
                    wastegate = std::stod(value);
                } else if (key == "DISPLAY_MAX_BOOST") {
                    maxDisplayBoost = std::stod(value);
                } else if (key == "REFERENCE_RPM") {
                    maxBoostRPM = std::stod(value);
                } else if (key == "GAMMA") {
                    BoostGamma = std::stod(value);
                } else if (key == "COCKPIT_ADJUSTABLE") {
                    boostAdjustable = value == "1";
                } else if (key == "TURBO_BOOST_THRESHOLD") {
                    turboThreshold = std::stod(value);
                } else if (key == "TURBO_DAMAGE_K") {
                    turboDamage = std::stod(value);
                } else if (key == "RPM_THRESHOLD") {
                    RPMThreshold = std::stod(value);
                } else if (key == "RPM_DAMAGE_K") {
                    RPMDamage = std::stod(value);
                }
            }
            file.close();
        }


    public:
        // Add debug method to verify parsing
        void printDebug(WINDOW *win) {
            wprintw(win, "\nEngine Debug Info:");
            wprintw(win, "\nRedline: %d", redlineRPM);
            wprintw(win, "\nIdle: %d", idleRPM);
            wprintw(win, "\nBoost: %.2f", boost);
            wprintw(win, "\nInertia: %.3f", inertia);
        }
};