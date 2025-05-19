#include "menu.h"
#include "../car_files/Engine.cpp"

class EngineMenu : public Menu {
    private:
        std::filesystem::path carpath;
        std::string carname;
        Engine engine; // Assuming Engine is a class that handles engine data
        std::map<std::string, std::string> attributes;

    public:

        EngineMenu(const std::filesystem::path &carpath, const std::string &carname, WINDOW *win)
            : Menu(win) {
            try{
            this->carname = carname;
            this->carpath = carpath;
            engine = Engine(carpath);

            engine.getAttributeList(attributes);
            for (const auto &[k,v]:attributes) {
                items.push_back(k);
            }
            } catch (const std::exception &e) {
                printFooter(e.what());
                wrefresh(win);
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Pause for 2 seconds
            }
        }
    protected:
        
        void printHeader() override {
            wprintw(win, "\n     Engine settings of: %s\n", carname.c_str());
            wprintw(win, "     Press UP/DOWN to navigate\n");
            //engine.printDebug(win);
            wprintw(win, "     -----------------------\n");
        }
        
        void printItems() override {
            int ctr = 0;
            for (const auto &i : engineSettingNames) {
                if (ctr == highlight) {
                    mvwprintw(win, ctr - start + 4, 1, "--> ");
                } 
                mvwprintw(win, ctr - start + 4, 5, "%s: %s",ctr, i.c_str(), attributes[i].c_str());
                
                ++ctr;
                if (ctr >= std::min(start + maxVisible, (int)attributes.size())) {
                    break;
                }
            }
        }
};