#include "vex.h"
#include "vex_competition.h"
#include "vex_units.h"

void AutonSelector::addAuton(string name, void (*auton)()) {
    // find the first page that has less than 3 autons and if none do make a new page
    bool added = false;
    for (auto &page : pages) {
        if (page.autons.size() < 3) {
            page.addAuton(name, auton);
            added = true;
            break;
        }
    }

    if (!added) {
        AutonPage page;
        page.addAuton(name, auton);
        pages.push_back(page);
    }
}

void AutonSelector::displayMenu(){
    // Display the menu on the controller
    controller->Screen.clearScreen();
    controller->Screen.setCursor(1, 1);

    // Display the autons on the controller
    int i = 0;
    for (auto auton : pages[selectedPage].autons) {
        if (i == selectedIdx) {
            controller->Screen.print(">");
        } else {
            controller->Screen.print(" ");
        }
        controller->Screen.print(auton.first.c_str());
        controller->Screen.newLine();
        i++;
    }
}

void (*AutonSelector::runMenu())() {
    string selectedAuton = "";
    while (selectedAuton == ""){
        displayMenu();
        // Wait for a button press
        while (!controller->ButtonUp.pressing() && !controller->ButtonDown.pressing() && !controller->ButtonA.pressing()
                && !controller->ButtonB.pressing()) {
            wait(20, msec);
        }

        if (controller->ButtonUp.pressing()){
            if (selectedIdx > 0) {
                selectedIdx--;
            } else {
                // go up a page if it exists
                if (selectedPage > 0) {
                    selectedPage--;
                    selectedIdx = pages[selectedPage].autons.size() - 1;
                }
            }
        } else if (controller->ButtonDown.pressing()){
            if (selectedIdx < pages[selectedPage].autons.size() - 1) {
                selectedIdx++;
            } else {
                // go down a page if it exists
                if (selectedPage < pages.size() - 1) {
                    selectedPage++;
                    selectedIdx = 0;
                }
            }
        } else if (controller->ButtonA.pressing()){
            // Select the auton
            int i = 0;
            for (auto auton : pages[selectedPage].autons) {
                if (i == selectedIdx) {
                    selectedAuton = auton.first;
                    break;
                }
                i++;
            }
        } else if (controller->ButtonB.pressing()){
            controller->Screen.clearScreen();
            controller->rumble("...");
            // run the selected auton right away
            int i = 0;
            for (auto auton : pages[selectedPage].autons) {
                if (i == selectedIdx) {
                    auton.second();
                    break;
                }
                i++;
            }
        }
    }

    controller->Screen.clearScreen();
    controller->Screen.setCursor(1, 1);
    controller->Screen.print("Selected: ");
    controller->Screen.print(selectedAuton.c_str());
    // Return the selected auton
    return pages[selectedPage].autons[selectedAuton];
}