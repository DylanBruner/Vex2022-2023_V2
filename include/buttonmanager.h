// #include "map"
// #include "string"

// class ButtonMapper {
//     public:
//         ButtonMapper(vex::controller *controller) {this->controller = controller;}
//         void mapButton(vex::controller::button buttonName, void (*buttonFunction)()) {buttonMap[buttonName] = buttonFunction;}
//         void tick();

//     private:
//         vex::controller *controller;
//         std::map<vex::controller::button, void (*)()> buttonMap;
// };