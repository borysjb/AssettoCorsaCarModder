#include <bits/stdc++.h>
#include <curses.h>
#include "../helper_files/lutLoader.cpp"
#include "../helper_files/constants.h"

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
        lutLoader(carpath / torqueFile, torqueCurve);
        parseEngineFile();
    }

    std::map<int, int> getTorqueCurve() {
        return torqueCurve;
    }
    
    void getAttributeList(std::map<std::string, std::string> &attributes) {
        attributes["Torque Curve"] = torqueFile;
        attributes["Altitude Sensitivity"] = std::to_string(altitudeSens);
        attributes["Inertia"] = std::to_string(inertia);
        attributes["Idle RPM"] = std::to_string(idleRPM);
        attributes["Redline RPM"] = std::to_string(redlineRPM);
        attributes["Limiter HZ"] = std::to_string(limiterHZ);
        attributes["Coast Reference"] = std::to_string(coastReference);
        attributes["Coast Torque"] = std::to_string(coastTorque);
        attributes["Coast Nonlinearity"] = std::to_string(coastNonlinearity);
        attributes["Turbo Lag Up"] = std::to_string(turboLagUp);
        attributes["Turbo Lag Down"] = std::to_string(turboLagDown);
        attributes["Boost"] = std::to_string(boost);
        attributes["Wastegate"] = std::to_string(wastegate);
        attributes["Max Display Boost"] = std::to_string(maxDisplayBoost);
        attributes["Max Boost RPM"] = std::to_string(maxBoostRPM);
        attributes["Boost Gamma"] = std::to_string(BoostGamma);
        attributes["Boost adjustable from cockpit"] = boostAdjustable ? "Yes" : "No";
        attributes["Turbo damage threshold"] = std::to_string(turboThreshold);
        attributes["Turbo damage factor"] = std::to_string(turboDamage);
        attributes["RPM damage threshold"] = std::to_string(RPMThreshold);
        attributes["RPM damage factor"] = std::to_string(RPMDamage);
    }

    private:

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

                for (int i = 0 ; i < 7; ++i) {
                    if (key == engineInts[i]) {
                        switch (i) {
                            case 0: idleRPM = std::stoi(value); break;
                            case 1: redlineRPM = std::stoi(value); break;
                            case 2: limiterHZ = std::stoi(value); break;
                            case 3: coastReference = std::stoi(value); break;
                            case 4: coastTorque = std::stoi(value); break;
                            case 5: coastNonlinearity = std::stoi(value); break;
                            case 6: boostAdjustable = value == "1"; break;
                            default: break;
                        }
                    }
                }
                for (int i = 0 ; i < 13; ++i) {
                    if(key == engineDoubles[i]) {
                        switch (i) {
                            case 0: altitudeSens = std::stod(value); break;
                            case 1: inertia = std::stod(value); break;
                            case 2: turboLagDown = std::stod(value); break;
                            case 3: turboLagUp = std::stod(value); break;
                            case 4: maxBoostRPM = std::stod(value); break;
                            case 5: wastegate = std::stod(value); break;
                            case 6: maxDisplayBoost = std::stod(value); break;
                            case 7: boost = std::stod(value); break;
                            case 8: BoostGamma = std::stod(value); break;
                            case 9: turboThreshold = std::stod(value); break;
                            case 10: turboDamage = std::stod(value); break;
                            case 11: RPMThreshold = std::stod(value); break;
                            case 12: RPMDamage = std::stod(value); break;
                            default: break;
                        }
                    }
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