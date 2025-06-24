#include "menu.h"
#include "CarMenu.cpp" // Include the CarMenu class

class MainMenu : public Menu {
    public:
        MainMenu(const std::filesystem::path &acpath, WINDOW *win) : Menu(win) {
            this->carpath = acpath;
            
            for (const auto &entry : std::filesystem::directory_iterator(this->carpath)) {
                if (entry.is_directory()) {
                    items.push_back(entry.path().filename().string());
                }
            }
        }

    protected:

        void printHeader() override {
            wprintw(win, "\n     Select the car to modify\n");
            wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
            wprintw(win, "     ----------------------------------------\n");
        }

    void enter() {
        try {
            std::filesystem::path checkpath = carpath / items[highlight] / "data" / "engine.ini";
            std::ifstream file(checkpath);  //check a mandatory file to throw no data folder as early as possible
             if(!file.good()) {
                throw std::runtime_error("No data folder");
            }
            CarMenu carmenu(carpath / items[highlight], items[highlight], win);
            carmenu.draw();
        } catch (const std::exception &e) {
            printFooter(e.what());
            wrefresh(win);
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 2 seconds
        }
    }

};
