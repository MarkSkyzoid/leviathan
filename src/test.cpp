#include <iostream>

#include "leviathan.h"

// TEST CODE
enum class Button { Square, Triangle, Cross, Circle };
const char* ButtonNames[] = {
	"Square", "Triangle", "Cross", "Circle"
};

class EventButtonPressed : public leviathan::EventConcrete<EventButtonPressed> {
public:
	Button m_button;

	EventButtonPressed() = default;

	EventButtonPressed(Button button) : m_button(button) {}
};

void on_button_pressed_1(const leviathan::EventBase& event) {
	const EventButtonPressed& button_pressed = static_cast<const EventButtonPressed&>(event);
	if (button_pressed.m_button == Button::Square) {
		std::cout << "on_button_pressed_1: YOU HAVE PRESSED SQUARE!" << std::endl;
	}
	else {
		std::cout << "on_button_pressed_1: you have pressed the wrong button :(" << std::endl;
	}
}

void on_button_pressed_2(const leviathan::EventBase& event) {
	const EventButtonPressed& button_pressed = static_cast<const EventButtonPressed&>(event);
	std::cout << "on_button_pressed_2: you have pressed " << ButtonNames[static_cast<int>(button_pressed.m_button)] << std::endl;
}

class ButtonPressedListenerExample {
public:
	void on_event(const leviathan::EventBase& event) {
		std::cout << "on_event: The class event says that you are pressing a button!" << std::endl;
	}
};

class ButtonPressedCallableClassExample {
public:
	void operator()(const leviathan::EventBase& event) {
		std::cout << "The class event says that you are pressing a button! this is implemented overloading the operator ()" << std::endl;
	}
};


int main() {
	EventButtonPressed button_pressed{ Button::Triangle };
	std::cout << "EventButtonPressed ID: " << button_pressed.m_ID << std::endl;

	leviathan::EventBus event_bus;

	// Normal function delegates:
	event_bus.register_listener<EventButtonPressed>(on_button_pressed_1);
	event_bus.register_listener<EventButtonPressed>(on_button_pressed_2);

	// Member function delegates:
	// Make sure lifetime of object_listener is not shorter than the event call!
	ButtonPressedListenerExample object_listener;
	event_bus.register_listener<EventButtonPressed>(
		[&object_listener](const leviathan::EventBase& event) { object_listener.on_event(event); }
	);
	event_bus.register_listener<EventButtonPressed>(ButtonPressedCallableClassExample());

	event_bus.dispatch_event(button_pressed);
}