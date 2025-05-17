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
            wprintw(win, "\n     Car: %s\n", 
                    carname.c_str());
            wprintw(win, "     Press UP/DOWN to navigate\n");
            //engine.printDebug(win);
            wprintw(win, "     ----------------------------------------\n");
        }
        
        void printItems() override {
            for(auto &item : engine.getTorqueCurve()) {
                wprintw(win, "     %d RPM: %d Nm\n", item.first, item.second);
            }
        }
};