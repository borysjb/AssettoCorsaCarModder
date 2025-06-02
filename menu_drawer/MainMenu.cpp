#include "menu.h"
#include "CarMenu.cpp" // Include the CarMenu class

class MainMenu : public Menu {
    private:
        std::filesystem::path acpath;
    
    public:
        MainMenu(const std::filesystem::path &acpath, WINDOW *win) : Menu(win) {
            this->acpath = acpath;
            
            for (const auto &entry : std::filesystem::directory_iterator(this->acpath)) {
                if (entry.is_directory()) {
                    items.push_back(entry.path().filename().string());
                }
            }
        }

    protected:

        void handleInput(int ch) override {
            if (ch == 10) { // Enter key
                enterCarMenu();
            } else {
                Menu::handleInput(ch);
            }
        }

        void printHeader() override {
            wprintw(win, "\n     Select the car to modify\n");
            wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
            wprintw(win, "     ---------------------------------------- %s\n", acpath.string());
        }

    void enterCarMenu() {
        try {
            CarMenu carmenu(acpath / items[highlight], items[highlight], win);
            carmenu.draw();
        } catch (const std::exception &e) {
            printFooter(e.what());
            wrefresh(win);
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
        }
    }

};
