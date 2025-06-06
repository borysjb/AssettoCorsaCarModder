#include <bits/stdc++.h>
#include <curses.h>
#include "../helper_files/lutLoader.cpp"
#include "../helper_files/constants.h"


class Engine {
    std::filesystem::path carpath;
    std::string torqueFile;
    std::map<int, int> torqueCurve;
    //std::vector<double> settings;

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
        /*int ctr=0;
        for(std::string a:engineSettingNames) {
            attributes[a] = std::to_string(settings[ctr]);
            ++ctr;
        }*/
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
            std::ifstream file(carpath / "engine.ini");  // Add "data" subfolder


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
                /*int ctr;
                for (std::string a:engineSets) {
                    if (key==a) {
                        settings[ctr] = std::stod(value);
                    }
                    ++ctr;
                }*/

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
                for (int i = 0 ; i < 11; ++i) {
                    if(key == engineDoubles[i]) {
                        switch (i) {
                            case 0: altitudeSens = std::stod(value); break;
                            case 1: inertia = std::stod(value); break;
                            case 2: turboLagDown = std::stod(value); break;
                            case 3: turboLagUp = std::stod(value); break;
                            case 4: boost = std::stod(value); break;
                            case 5: wastegate = std::stod(value); break;
                            case 6: maxDisplayBoost = std::stod(value); break;
                            case 7: BoostGamma = std::stod(value); break;
                            case 8: turboThreshold = std::stod(value); break;
                            case 9: turboDamage = std::stod(value); break;
                            case 10: RPMDamage = std::stod(value); break;
                            default: break;
                        }
                    }
                }
            }
            file.close();
        }
        
    public:
    
        void writeToEngineFile() {
            std::filesystem::path filepath = carpath / "engine.ini";
            std::ofstream file;
            file.open(filepath);
            file << "[HEADER]\nVERSION=1\nPOWER_CURVE=power.lut			; power curve file\nCOAST_CURVE=FROM_COAST_REF 		; coast curve. can define 3 different options (coast reference, coast values for mathematical curve, coast curve file)\n\n[ENGINE_DATA]\nALTITUDE_SENSITIVITY=" << altitudeSens <<"		; sensitivity to altitude\nINERTIA="
            <<inertia<<"					; engine inertia\nMINIMUM="<<idleRPM<<"					; engine idle rpm\nLIMITER="<<redlineRPM<<"					; engine rev limiter. 0 no limiter\nLIMITER_HZ="<<limiterHZ<<
            "\n\n[COAST_REF]\nRPM="<<coastReference<<"						; rev number reference\nTORQUE="<<coastTorque<<"						; engine braking torque value in Nm at rev number reference\nNON_LINEARITY="<<
            coastNonlinearity<<"				; coast engine brake from ZERO to TORQUE value at rpm with linear (0) to fully exponential (1)\n\n[COAST_DATA]\nCOAST0=0						; \nCOAST1=0\nCOAST=0.0000015\n\n[COAST_CURVE]\nFILENAME=coast.lut				; coasting curve file\n\n[TURBO_0]\nLAG_DN="<<
            turboLagDown<<" 				; Interpolation lag used slowing down the turbo\nLAG_UP="<<turboLagUp<<"\nMAX_BOOST="<<boost<<" \nWASTEGATE="<<wastegate<<" \nDISPLAY_MAX_BOOST="<<maxDisplayBoost<<"		; Value used by display apps\nREFERENCE_RPM="<<maxBoostRPM<<"			; The reference rpm where the turbo reaches maximum boost (at max gas pedal). \nGAMMA="<<
            BoostGamma<<"\nCOCKPIT_ADJUSTABLE="<<boostAdjustable<<"\n\n[DAMAGE]\nTURBO_BOOST_THRESHOLD="<<turboThreshold<<"\nTURBO_DAMAGE_K="<<turboDamage<<"			; amount of damage per second per (boost - threshold)\nRPM_THRESHOLD="<<RPMThreshold<<"			; RPM at which the engine starts to take damage\nRPM_DAMAGE_K="<<RPMDamage<<"			; amount of damage per second per (rpm-threshold)\n";
        }

        // Add debug method to verify parsing
        void printDebug(WINDOW *win) {
            wprintw(win, "\nEngine Debug Info:");
            wprintw(win, "\nRedline: %d", redlineRPM);
            wprintw(win, "\nIdle: %d", idleRPM);
            wprintw(win, "\nBoost: %.2f", boost);
            wprintw(win, "\nInertia: %.3f", inertia);
        }

        void modValue(int item) {
            std::string itemname = engineSettingNames[item];
            if (item == 0) {
                return;
            }
        }
};
