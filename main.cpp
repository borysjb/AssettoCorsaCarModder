#include <bits/stdc++.h>
#include <curses.h>
#include <panel.h>

#include "menu_drawer/MainMenu.cpp"

int main() {
    std::filesystem::path path;
    path = "content";
    path /= "cars";

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    WINDOW *win = newwin(maxy, maxx, 0, 0); // Create window that fits the terminal
    curs_set(0);
    scrollok(win, TRUE);
    keypad(win, TRUE);
    echo();
    MainMenu menu(path, win);
    menu.draw();

    endwin();

    return 0;
}
