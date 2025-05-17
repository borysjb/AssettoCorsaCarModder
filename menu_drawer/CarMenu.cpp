#include "menu.h"
#include "EngineMenu.cpp" // Include the EngineMenu class

class CarMenu : public Menu {
private:
    std::filesystem::path carpath;
    std::string carname;

public:
    CarMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win) 
        : Menu(win) {
        this->carpath = carpath;
        this->carpath /= "data";
        
        items.push_back("General Settings");
        items.push_back("Drivetrain/Engine Settings");
        items.push_back("Tire Settings");
        items.push_back("Suspension Settings");
        items.push_back("Aerodynamics Settings");
        items.push_back("Brakes Settings");
        items.push_back("Electronics Settings");
        items.push_back("Misc");
        items.push_back("Exit");
    }

protected:
    void printHeader() override {
        wprintw(win, "\n     Car: %s\n", 
                carname.c_str());
        wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
        wprintw(win, "     ----------------------------------------\n");
    }

    void handleInput(int ch) override {
        if (ch == 10) { // Enter key
            enterSubMenu();
        } else {
            Menu::handleInput(ch);
        }
    }

    void enterSubMenu() {
        try {
            switch (highlight) {
                case 0: // General Settings
                    // GeneralSettings generalSettings(carpath, carname, win);
                    break;
                case 1: // Engine Settings
                    enterEngineMenu();
                    break;
                case 2: // Tire Settings
                    // TireSettings tireSettings(carpath, carname, win);
                    break;
                case 3: // Suspension Settings
                    // SuspensionSettings suspensionSettings(carpath, carname, win);
                    break;
                case 4: // Aerodynamics Settings
                    // AerodynamicsSettings aerodynamicsSettings(carpath, carname, win);
                    break;
                case 5: // Brakes Settings
                    // BrakesSettings brakesSettings(carpath, carname, win);
                    break;
                case 6: // Electronics Settings
                    // ElectronicsSettings electronicsSettings(carpath, carname, win);
                    break;
                case 7: // Misc
                    // Misc misc(carpath, carname, win);
                    break;
                case 8: // Exit
                    exit = true;
                    break;
            }
        } catch (const std::exception &e) {
            printFooter(e.what());
            wrefresh(win);
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
        }
    }

    void enterEngineMenu() {
        try {
            EngineMenu engineMenu(carpath, carname, win);
            engineMenu.draw();
        } catch (const std::exception &e) {
            printFooter(e.what());
            wrefresh(win);
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
        }
    }
};