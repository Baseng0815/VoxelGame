#pragma once

#include "pch.h"

typedef unsigned int EventType;
typedef unsigned int CallbackId;
class Application;

struct Event {
	virtual EventType type();
	
	template<class T>
	T* get();
};

typedef std::function<void(Event *e)> Callback;

struct KeyEvent : public Event {
	static constexpr EventType TYPE = 0;

	Application* app;
	int key;
	int scancode;
	int action;
	int mods;

	EventType type() override final;
	KeyEvent(Application* app = nullptr, int key = 0, int scancode = 0, int action = 0, int mods = 0);
};

struct CursorEvent : public Event {
	static constexpr EventType TYPE = 1;

	Application* app;
	double x, y;
	double dx, dy;

	EventType type() override final;
	CursorEvent(Application* app = nullptr, double x = 0, double y = 0, double dx = 0, double dy = 0);
};

struct ScrollEvent : public Event {
	static constexpr EventType TYPE = 2;

	Application* app;
	double dx, dy;

	EventType type() override final;
	ScrollEvent(Application* app = nullptr, double dx = 0, double dy = 0);
};

struct FramebufferSizeEvent : public Event {
	static constexpr EventType TYPE = 3;

	Application* app;
	int width, height;

	EventType type() override final;
	FramebufferSizeEvent(Application* app = nullptr, int width = 0, int height = 0);
};
