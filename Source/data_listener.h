#pragma once
#include <string>
#ifndef data_listener_h
#define data_listener_h

class data_listener {
	virtual void onDataReceived(const std::string& symbol) = 0;
};
#endif // !data_listener.h

