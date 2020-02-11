#pragma once

class Timer {
	bool clear = false;
	std::thread thr;

public:
	template<typename Function>
	void setTimeout(Function&& function, int delay) {
		this->clear = false;
		thr = std::thread([this, function = std::forward<Function>(function), delay]() {
			if (clear) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			if (clear) return;
			function();
		});
	}

	template<typename Function>
	void setInterval(Function&& function, int interval) {
		this->clear = false;
		thr = std::thread([this, function = std::forward<Function>(function), interval]() {
			while (true) {
				if (clear) return;
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
				if (clear) return;
				function();
			}
		});
	}

	void stop() {
		clear = true;
		if (thr.joinable())
			thr.join();
	}

	~Timer() {
		if (thr.joinable())
			thr.detach();
	}
};
