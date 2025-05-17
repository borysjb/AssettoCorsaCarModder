#pragma once
#include <bits/stdc++.h>
#include <curses.h>
#include <panel.h>

class Menu {
    protected:
        int highlight = 0;
        int start = 0; // Starting index for the visible window
        bool exit = false;
        int maxVisible = 0;
        int maxy, maxx;
        std::vector<std::string> items;
        WINDOW *win;
    
    public:
        Menu(WINDOW *win) : win(win) {
            getmaxyx(stdscr, maxy, maxx); // Get the size of the window
            maxVisible = maxy - 5; // Leave space for header and footer
        }

        virtual void draw() {
            while (!exit) {
                werase(win); // Clear the window
                printHeader(); // Print the header
                printItems(); // Print the items
                wrefresh(win); // Refresh the window to show the changes

                int ch = wgetch(win); // Use wgetch to capture input for the specific window
                handleInput(ch); // Handle input
            }
        }
    
    protected:
        virtual void printHeader() =0;

        virtual void handleInput(int ch) {
            switch (ch) {
                case KEY_UP:
                    selectUp();
                    break;
                case KEY_DOWN:
                    selectDown();
                    break;
                case 27: // Escape key
                    if (confirmExit()) {
                        exit = true;
                    }
                    break;
                default:
                    printFooter("Input not recognized, key nr %d");
                    break;
            }
        }

        void printFooter(std::string message) {
            wprintw(win, "\n     ----------------------------------------\n");
            wprintw(win, "     %s \n", message.c_str());
        }

        void printItems() {
            for (int i = start; i < std::min(start + maxVisible, (int)items.size()); ++i) {
                if (i == highlight) {
                    mvwprintw(win, i - start + 4, 1, "--> %s", items[i].c_str());
                } else {
                    mvwprintw(win, i - start + 4, 5, "%s", items[i].c_str());
                }
            }
        }

        bool confirmExit() {
            wclear(win);
            mvwprintw(win, (maxy/2), (maxx/2 - 18), 
                "Are you sure you want to go exit? (y/n)");
            wrefresh(win);
            int ch = wgetch(win);
            if (ch == 'y' || ch == 'Y') {
                return true;
            } else if (ch == 'n' || ch == 'N') {
                return false;
            } else {
                return confirmExit(); // Recursive call for invalid input
            }
        }

        void selectUp() {
            if (highlight > 0) {
                highlight--;
            }
            if (highlight < start + 1 && start > 0) {
                start--;
            }
        }

        void selectDown() {
            if (highlight < items.size() - 1) {
                highlight++;
            }
            if (highlight >= start + maxVisible - 1 && start + maxVisible < items.size()) {
                start++;
            }
        }
    
};