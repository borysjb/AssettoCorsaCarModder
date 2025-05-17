#include "menu.h"

class CarMenu : public Menu {
private:
    std::filesystem::path carpath;
    std::string carname;

public:
    CarMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win) 
        : Menu(win), carname(carname) {
        this->carpath = carpath;
        this->carpath /= "data";
        
        // First add directories
        for (const auto &entry : std::filesystem::directory_iterator(this->carpath)) {
            if (entry.is_directory()) {
                items.push_back(entry.path().filename().string() + "/");
            }
        }
        
        // Then add files
        for (const auto &entry : std::filesystem::directory_iterator(this->carpath)) {
            if (entry.is_regular_file()) {
                items.push_back(entry.path().filename().string());
            }
        }
    }

protected:
    void printHeader() override {
        wprintw(win, "\n     Car: %s\n", 
                carname.c_str());
        wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
        wprintw(win, "     ----------------------------------------\n");
    }
};