#include "menu.h"
#include "../helper_files/lutLoader.h"
//#include "../car_files/Engine.cpp" // Include the Engine class

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
    TorqueMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win, const Engine &eng)
        : Menu(win), carpath(carpath), carname(carname), engine(eng) {
        torqueFile = "power.lut"; // Default torque file name
        try {
            lutLoader(carpath / torqueFile, torqueCurve); // Load the torque curve from the file
        } catch(const std::exception &e) {
            // Log the error (could add logging here)
        }
        // Ensure torqueCurve is not empty to avoid crashes '
        if (torqueCurve.empty()) {
            torqueCurve[1000] = 0;
        }
        items.clear();
        for (const auto &[key, value] : torqueCurve) {
            items.push_back(std::to_string(key));
        }
        --maxVisible; // Adjust maxVisible to account for bigger header
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
                mvwprintw(win, i - start + 5, 1, "-->");
            }
            switch (mode){
                case 0:
                    mvwprintw(win, i - start + 5, 5, "%d: %d", rpm, torqueCurve[rpm]);
                    break;
                case 1:
                    mvwprintw(win, i - start + 5, 5, "%d: %.2f", rpm, 1.0*torqueCurve[rpm]*rpm/7127);
                    break;
                case 2:
                    mvwprintw(win, i - start + 5, 5, "%d: %.2f", rpm, 1.0*torqueCurve[rpm]*rpm*(1.0+engine.getBoost(rpm))/7127);
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
            case 10: // Enter key
                if (highlight < items.size()) {
                    int rpm = std::stoi(items[highlight]);
                    modValue(rpm); // Modify the value for the selected RPM
                }
                break;
            case 's' || 'S':
                try {
                    savePrompt();
                } catch (const std::exception &e) {
                    printFooter(e.what());
                    wrefresh(win);
                    std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 2 seconds
                }
                break;
            default:
                Menu::handleInput(ch);
        }
    }

    void modValue(int rpm) {

            // Create a new window for the menu (height, width, start_y, start_x)
            WINDOW* win = newwin(10, 75, 5, 5);
            box(win, 0, 0);
            // Display the prompt and current value
            mvwprintw(win, 1, 2, "Modify value for %d", rpm);
            mvwprintw(win, 2, 2, "Current value: %d", torqueCurve[rpm]);
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
                switch (mode) {
                    case 0:
                        torqueCurve[rpm] = static_cast<int>(std::round(newValue));
                        break;
                    case 1:
                        torqueCurve[rpm] = static_cast<int>(std::round(newValue * 7127 / rpm));
                        break;
                    case 2:
                        torqueCurve[rpm] = static_cast<int>(std::round(newValue * 7127 / (rpm * (1.0 + engine.getBoost(rpm)))));
                        break;
                    default:
                        break;
                }
                mvwprintw(win, 6, 2, "raw torque at %d updated to %d (with rounding since raw file has to have integers)", rpm, torqueCurve[rpm]);
            } catch (const std::exception &e) {
                mvwprintw(win, 5, 2, "Invalid input. Setting not modified.");
            }
            wrefresh(win);
            getch();
            delwin(win);
        }
    
    void savePrompt() {
            wclear(win);
            mvwprintw(win, (maxy/2)-1, (maxx/2) - 37, 
                "Are you sure you want to save (THIS WILL OVERRIDE THE PREVIOUS FILES)? (y/n)");
            wrefresh(win);
            int ch = wgetch(win);
            if (ch == 'y' || ch == 'Y' || ch == 10) {
                save();
                return;
            } else if (ch == 'n' || ch == 'N' || ch == 27) {
                return;
            } else {
                return savePrompt(); // Recursive call for invalid input
            }
        }

    void save() {
        std::ofstream file(carpath / torqueFile);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open torque file for writing");
        }
        for (const auto &[rpm, value] : torqueCurve) {
            file << rpm << " | " << value << "\n"; // Write each RPM and its corresponding value
        }
    }
};