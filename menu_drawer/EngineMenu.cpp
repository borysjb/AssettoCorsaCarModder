#include "menu.h"
#include "../car_files/Engine.cpp"

class EngineMenu : public Menu {
    private:
        std::filesystem::path carpath;
        std::string carname;
        Engine engine; // Assuming Engine is a class that handles engine data

    public:

        EngineMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win)
            : Menu(win) {
            this->carpath = carpath;
            engine = Engine(carpath);
        }
    protected:
        
        void printHeader() override {
            wprintw(win, "\n     Engine settings of: %s\n", 
                    carname.c_str());
            wprintw(win, "     Press UP/DOWN to navigate\n");
            //engine.printDebug(win);
            wprintw(win, "     -----------------------%d %d %d %d\n", getmaxx(win), getmaxy(win), maxVisible, highlight);
        }
        
        void printItems() override {
            std::map<std::string, std::string> attributes;
            engine.getAttributeList(attributes);
            int ctr = 0;
            for (const auto &[k,v]:attributes) {
                if (ctr == highlight) {
                    mvwprintw(win, ctr - start + 4, 1, "--> ");
                } 
                mvwprintw(win, ctr - start + 4, 5, "%d) %s: %s",ctr, k.c_str(), v.c_str());
                
                ++ctr;
                if (ctr >= std::min(start + maxVisible, (int)items.size())) {
                    break;
                }
            }
            printFooter("this is fubar");
        }
};