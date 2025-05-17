#include <bits/stdc++.h>
#include <curses.h>
#include <panel.h>
#include <filesystem>
#include "drawCarMenu.cpp" // Include the CarMenu class

class MainMenu {
    int highlight = 0;
    int start = 0; // Starting index for the visible window
    bool exit = false;
    int maxVisible = 0;
    int maxy, maxx;
    std::filesystem::path acpath;
    std::vector<std::string> carNames;
    WINDOW *win;
    
    public:

    MainMenu(const std::filesystem::path &acpath, WINDOW *win) {
        this->acpath = acpath;
        std::filesystem::path path = acpath;
        this -> acpath /= "content";
        this -> acpath /= "cars";
        for (const auto &entry : std::filesystem::directory_iterator(this -> acpath)) {
            if (entry.is_directory()) {
                std::string carName = entry.path().filename().string();
                carNames.push_back(carName);
            }
        }
        this->win = win;
        getmaxyx(stdscr, maxy, maxx); // Get the size of the window
        maxVisible = maxy - 5; // Leave space for header and footer
    }

    void draw() {
        while (!exit) {

            werase(win); // Clear the window

            printHeader(); // Print the header
            printCars(); // Print the car names

            wrefresh(win); // Refresh the window to show the changes
            
            int ch = wgetch(win); // Use wgetch to capture input for the specific window
            //CarMenu carmenu(acpath / carNames[0], carNames[0]); // Declare and initialize outside the switch
            switch (ch) {
                case KEY_UP:
                    selectUp();
                    break;
                case KEY_DOWN:
                    selectDown();
                    break;
                case 10: // Enter key
                    enterCarMenu(); // Call the function to enter the car menu
                    break;
                case 27: // Escape key
                    exit = true;
                    break;
                default:
                    break;
            }
        }
    }

    void selectUp() {
        if (highlight > 0) {
            highlight--;
            if (highlight < start + 1 && start > 0) {
                start--; // Scroll up
            }
        }
    }

    void selectDown() {
        if (highlight < carNames.size() - 1) {
            highlight++;
            if (highlight >= start + maxVisible - 1 && start + maxVisible < carNames.size()) {
                start++; // Scroll down
            }
        }
    }

    void printHeader() {
        werase(win); // Clear the window
        wprintw(win, "\n     Visible cars - %d, screen size: %d x %d\n", maxVisible, maxx, maxy);
        wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
        wprintw(win, "     ----------------------------------------\n");
    }

    void printCars() {
        for (int i = start; i < std::min(start + maxVisible, (int)carNames.size()); ++i) {
                if (i == highlight) {
                    mvwprintw(win, i - start + 4, 1, "--> %s", carNames[i].c_str());
                } else {
                    mvwprintw(win, i - start + 4, 5, "%s", carNames[i].c_str());
                }
            }
    }

    void printFooter(std::string message) {
        wprintw(win, "\n     ----------------------------------------\n");
        wprintw(win, "     %s \n", message.c_str());
    }

    void enterCarMenu() {
        try {
            CarMenu carmenu(acpath / carNames[highlight], carNames[highlight], win);
            carmenu.draw();
        } catch (const std::exception &e) {
            printFooter(e.what());
            wrefresh(win);
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
        }
    }

};
