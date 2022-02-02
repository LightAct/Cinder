#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/Event.h"

namespace cinder { namespace app {

class CI_API FocusEvent : public Event {
public:
	FocusEvent(WindowRef win, bool focusGained)
		: Event(win), mFocusGained(focusGained)
	{}

	bool getFocusGained() { return mFocusGained; }

private:
	bool mFocusGained;
};

} }