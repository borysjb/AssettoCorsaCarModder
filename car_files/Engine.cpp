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
    int maxBoostRPM;
    double BoostGamma;
    bool boostAdjustable;

    double turboThreshold;
    double turboDamage;
    int RPMThreshold;
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

    int getTestValue() {
        return redlineRPM;
    }
    
    void getAttributeList(std::map<std::string, std::string> &attributes) {
        attributes["Torque curve File"] = torqueFile;
        attributes["Altitude Sensitivity"] = std::to_string(altitudeSens);
        attributes["Inertia"] = std::to_string(inertia);
        attributes["Idle RPM"] = std::to_string(idleRPM);
        attributes["Redline RPM"] = std::to_string(redlineRPM);
        attributes["Limiter HZ"] = std::to_string(limiterHZ);
        attributes["Coast Reference RPM"] = std::to_string(coastReference);
        attributes["Coast Torque"] = std::to_string(coastTorque);
        attributes["Coast Nonlinearity"] = std::to_string(coastNonlinearity);
        attributes["Turbo Lag Spooling Up"] = std::to_string(turboLagUp);
        attributes["Turbo Lag Spooling Down"] = std::to_string(turboLagDown);
        attributes["Max Boost"] = std::to_string(boost);
        attributes["Wastegate Pressure"] = std::to_string(wastegate);
        attributes["Max Display Boost (for use in game UI)"] = std::to_string(maxDisplayBoost);
        attributes["RPM at which max boost is reached"] = std::to_string(maxBoostRPM);
        attributes["Boost Gamma"] = std::to_string(BoostGamma);
        attributes["Boost adjustable from cockpit?"] = boostAdjustable ? "Yes" : "No";
        attributes["Turbo damage threshold pressure"] = std::to_string(turboThreshold);
        attributes["Turbo damage factor"] = std::to_string(turboDamage);
        attributes["Engine damage threshold RPM"] = std::to_string(RPMThreshold);
        attributes["Overrev damage factor"] = std::to_string(RPMDamage);
    }

    private:

        void parseEngineFile() {
            std::string line;
            std::ifstream file;
            try{
                std::ifstream file(carpath / "engine.ini");  // Add "data" subfolder
            } catch (const std::exception &e) {
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

                for (int i = 0 ; i < 9; ++i) {
                    if (key == engineInts[i]) {
                        switch (i) {
                            case 0: idleRPM = std::stoi(value); break;
                            case 1: redlineRPM = std::stoi(value); break;
                            case 2: limiterHZ = std::stoi(value); break;
                            case 3: coastReference = std::stoi(value); break;
                            case 4: coastTorque = std::stoi(value); break;
                            case 5: coastNonlinearity = std::stoi(value); break;
                            case 6: boostAdjustable = value == "1"; break;
                            case 7: maxBoostRPM = std::stoi(value); break;
                            case 8: RPMThreshold = std::stoi(value); break;
                            default: break;
                        }
                    }
                }
                for (int i = 0 ; i < 10; ++i) {
                    if(key == engineDoubles[i]) {
                        switch (i) {
                            case 0: altitudeSens = std::stod(value); break;
                            case 1: inertia = std::stod(value); break;
                            case 2: turboLagDown = std::stod(value); break;
                            case 3: turboLagUp = std::stod(value); break;
                            case 4: wastegate = std::stod(value); break;
                            case 5: maxDisplayBoost = std::stod(value); break;
                            case 6: BoostGamma = std::stod(value); break;
                            case 7: turboThreshold = std::stod(value); break;
                            case 8: turboDamage = std::stod(value); break;
                            case 9: RPMDamage = std::stod(value); break;
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