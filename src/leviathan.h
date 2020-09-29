// Leviathan is a lightweight C++ Event System, mainly thought for game development
// MIT License
// Copyright(c) 2020 Marco Vallario
//
// 
// TODOs:
// - Check if we can remove EventBase and concrete as we always have access to EvenID using the T
// - Find a typesafe way to send events to listners. Maybe check std::any and std::any_cast?

#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

namespace leviathan {
	// Provides an unique ID for events eg.:
	// int32_t button_pressed_event_id = EventID::get<ButtonPressedEvent>();
	struct EventID {
		using IDType = int32_t;

		template <typename T>
		static IDType get() {
			static IDType id = counter++;
			return id;
		}

	private:
		static IDType counter;
	};
	EventID::IDType EventID::counter = 0;

	class EventBase {
	public:
		int m_ID;

		EventBase(int ID) : m_ID(ID) {}
	};

	template <typename T>
	class EventConcrete : public EventBase {
	public:
		EventConcrete() : EventBase(EventID::get<T>()) {}
	};

	class EventBus {
	public:
		using FunctorType = std::function<void(const EventBase&)>;
		using FunctorVectorType = std::vector<FunctorType>;

		template <typename EventType>
		void register_listener(const FunctorType& functor) {
			auto event_id = EventID::get<EventType>();
			m_listeners_map[event_id].push_back(functor);
		}

		template <typename T>
		void dispatch_event(const T& event) {
			auto event_id = event.m_ID;
			FunctorVectorType& listeners = m_listeners_map[(event_id)];
			for (const auto& listener : listeners) {
				listener(event);
			}
		}

	private:
		std::unordered_map<EventID::IDType, FunctorVectorType> m_listeners_map;
	};
}