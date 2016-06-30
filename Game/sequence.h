#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <assert.h>

template<class T> class Sequence
{
public:
	typedef void (T::*Process)(float);

	Sequence() :
		_time(0),
		_prev(nullptr),
		_now(nullptr),
		_next(nullptr)
	{}
	~Sequence() {}

	inline void run(T* obj, const float delta) 
	{
		if (!_now && !_next) return;
		if (_next) {
			_prev = _now;
			_now = _next;
			_next = nullptr;
			_time = 0.0f;
		}
		(obj->*_now)(delta);
		_time += delta;
	}

	inline void change(const Process sequence = nullptr)
	{
		if (!sequence && !_next) {
			assert(0);
		}
		_next = sequence;
		_time = 0.0f;
	}

	inline float getTime() const { return _time; }

private:
	float _time;
	Process _prev;
	Process _now;
	Process _next;

};

#endif