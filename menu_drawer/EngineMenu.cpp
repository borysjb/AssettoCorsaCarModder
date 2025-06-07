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
            this->carname = carname;
            this->carpath = carpath;
            engine = Engine(carpath);
           
            engine.getAttributeList(attributes);
            for (const auto &[k,v]:attributes) {
                items.push_back(k);
            }
        }
    protected:
        
        void printHeader() override {
            wprintw(win, "\n     Engine settings of: %s\n", carname.c_str());
            wprintw(win, "     Press UP/DOWN to navigate, press S to save changes \n");
            //engine.printDebug(win);
            wprintw(win, "     ----------------------- %d\n", engine.getTestValue());
        }
        
        void printItems() override {
            for (int i = start; i < std::min(start + maxVisible, (int)items.size()); ++i) {
                if (i == highlight) {
                    mvwprintw(win, i - start + 4, 1, "-->");
                }
                mvwprintw(win, i - start + 4, 5, "%s: %s", engineSettingNames[i].c_str(), attributes[engineSettingNames[i]].c_str());
            }
            printFooter(engineExplanations[engineSettingNames[highlight]]);
        }

        void handleInput(int ch) override {
            switch(ch) {
                case 's':
                    confirmSave();
                    break;
                case 10: //Enter key
                    engine.modValue(highlight);
                default:
                    Menu::handleInput(ch);
            }
        }

        void confirmSave() {
            wclear(win);
            mvwprintw(win, (maxy/2)-1, (maxx/2) - 37, 
                "Are you sure you want to save (THIS WILL OVERRIDE THE PREVIOUS FILES)? (y/n)");
            wrefresh(win);
            int ch = wgetch(win);
            if (ch == 'y' || ch == 'Y' || ch == 10) {
                engine.writeToEngineFile();
                return;
            } else if (ch == 'n' || ch == 'N' || ch == 27) {
                return;
            } else {
                return confirmSave(); // Recursive call for invalid input
            }
        }
};