#include <bits/stdc++.h>
#include <curses.h>
#include <panel.h>
#include <filesystem>

class CarMenu {
    int highlight = 0;
    int start = 0;
    bool exit = false;
    int maxVisible = 0;
    int maxy, maxx;
    std::filesystem::path carpath;
    std::string carname;
    std::vector<std::string> fileNames;
    WINDOW *win;
    
    public:
    CarMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win) {
        this->carpath = carpath;
        this->carname = carname;
        this->win = win;
        this->carpath /= "data";
        
        for (const auto &entry : std::filesystem::directory_iterator(this -> carpath)) {
            std::string fileName = entry.path().filename().string();
            if (entry.is_directory()) {
                fileName += "/"; // Append '/' for directories
                fileNames.push_back(fileName);
            }
            
        }
        for (const auto &entry : std::filesystem::directory_iterator(this -> carpath)) {
            if (entry.is_regular_file()) {
                fileNames.push_back(entry.path().filename().string());
            }
        }
        std::cerr << "Files in car dir: " << fileNames.size();
        getmaxyx(stdscr, maxy, maxx);
        maxVisible = maxy - 5; // Leave space for header
    }

    void draw() {
        while (!exit) {
            werase(win);
            printHeader();
            printFiles();
            wrefresh(win);
            
            int ch = wgetch(win);
            switch (ch) {
                case KEY_UP:
                    selectUp();
                    break;
                case KEY_DOWN:
                    selectDown();
                    break;
                case 10: // Enter key
                    // Handle file selection here
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
        if (highlight < fileNames.size() - 1) {
            highlight++;
            if (highlight >= start + maxVisible - 1 && start + maxVisible < fileNames.size()) {
                start++; // Scroll down
            }
        }
    }

    void printHeader() {
        wprintw(win, "\n     Car: %s - Files visible: %d, screen size: %d x %d\n", 
                carname.c_str(), maxVisible, maxx, maxy);
        wprintw(win, "     Press UP/DOWN to navigate, ENTER to select, ESC to exit.\n");
        wprintw(win, "     ----------------------------------------\n");
    }

    void printFiles() {
        for (int i = start; i < std::min(start + maxVisible, (int)fileNames.size()); ++i) {
            if (i == highlight) {
                mvwprintw(win, i - start + 4, 1, "--> %s", fileNames[i].c_str());
            } else {
                mvwprintw(win, i - start + 4, 5, "%s", fileNames[i].c_str());
            }
        }
    }
};