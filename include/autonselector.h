#include "string"
#include <string>
#include "map"
#include "vex_controller.h"
#include "vector"

using std::string;
using std::map;


class AutonPage {
    public:
        map<string, void (*)()> autons = {};
        void addAuton(string name, void (*auton)()){autons[name] = auton;}
};

class AutonSelector {
    private:
        // list that holds pages
        std::vector<AutonPage> pages = {};
        void displayMenu();
        int selectedIdx = 0;
        int selectedPage = 0;
        vex::controller* controller;
    
    public:
        void addAuton(string name, void (*auton)());
        void addPage(AutonPage page);
        void (*runMenu())();
        AutonSelector(vex::controller* controller){this->controller = controller;}
};