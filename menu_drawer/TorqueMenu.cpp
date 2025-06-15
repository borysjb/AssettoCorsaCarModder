#include "menu.h"
#include "../helper_files/lutLoader.h"

class TorqueMenu : public Menu {
private:
    std::filesystem::path carpath;
    std::string carname;
    std::map<int, int> torqueCurve; // Map to hold the torque curve
    std::string torqueFile; // Name of the torque file
    int mode = 0; // Mode for different torque curve views
    std::string modes [3]= {"Raw File", "NA Power", "Full Power"}; // Modes for different views
    Engine engine;

public:
    TorqueMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win, const Engine &engine)
        : Menu(win), carpath(carpath), carname(carname) {
        torqueFile = "power.lut"; // Default torque file name
        this->engine = engine; // Store the engine object
        try {
            lutLoader(carpath / torqueFile, torqueCurve); // Load the torque curve from the file
        } catch(const std::exception &e) {
            // If LUT file fails to load, use a default entry to prevent crashing.
            torqueCurve[1000] = 0;
        }
        // Populate menu items from torqueCurve keys.
        for (const auto &[key, value] : torqueCurve) {
            items.push_back(std::to_string(key));
        }
    }

    protected:
    void printHeader() override {
        wprintw(win, "\n     Torque Curve Settings for: %s\n", carname.c_str());
        wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
        wprintw(win, "     press 1 for raw file, 2 for NA power (before turbo kicks in), 3 for full power\n");
        wprintw(win, "     ---- RPM  | Value ---------------------------- mode: %s\n", modes[mode].c_str());
    }

    void printItems() override {
        for (int i = start; i < std::min(start + maxVisible, (int)items.size()); ++i) {
            int rpm = std::stoi(items[i]);
            if (i == highlight) {
                mvwprintw(win, i - start + 4, 1, "-->");
            }
            switch (mode){
                case 0:
                    mvwprintw(win, i - start + 4, 5, "%s: %d", rpm, torqueCurve[rpm]);
                    break;
                case 1:
                    mvwprintw(win, i - start + 4, 5, "%s: %.2f", rpm, 1.0*torqueCurve[rpm]*rpm/7127);
                    break;
                case 2:
                    mvwprintw(win, i - start + 4, 5, "%s: %.2f", rpm, 1.0*torqueCurve[rpm]*rpm*engine.getBoost()/7127);
                    break;
                default:
                    break;
            }
        }
    }

    void changeMode(int newMode) {
        if (newMode >= 0 && newMode < 3) {
            mode = newMode;
        }
    }

    void handleInput(int ch) override {
        switch (ch) {
            case '1':
                changeMode(0);
                break;
            case '2':
                changeMode(1);
                break;
            case '3':
                changeMode(2);
                break;
            default:
                Menu::handleInput(ch);
        }
    }
};