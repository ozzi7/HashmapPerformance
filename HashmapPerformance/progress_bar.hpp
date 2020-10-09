#ifndef PROGRESSBAR_PROGRESSBAR_HPP
#define PROGRESSBAR_PROGRESSBAR_HPP

#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>

#define max_macro(a, b) (((a) < (b)) ? (b) : (a))

class ProgressBar {
private:
	uint64_t ticks = 0;

	const uint64_t total_ticks;
	const unsigned int bar_width;
	const char complete_char = '=';
	const char incomplete_char = ' ';
	const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	std::mutex mtx;

public:
	ProgressBar(uint64_t total, unsigned int width, char complete, char incomplete) :
		total_ticks{ total }, bar_width{ width }, complete_char{ complete }, incomplete_char{ incomplete } {}

	ProgressBar(uint64_t total, unsigned int width) : total_ticks{ total }, bar_width{ width } {}

	void operator++() {
		mtx.lock();
		ticks++;
		mtx.unlock();
	}
	void add(uint64_t iters) {
		mtx.lock();
		ticks += iters;
		mtx.unlock();
	}

	void display()
	{
		double progress = (double)ticks / total_ticks;
		int pos = (int)(bar_width * progress);

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

		std::ostringstream ostrstream;
		ostrstream << "\r[";

		for (int i = 0; i < bar_width; ++i) {
			if (i < pos) ostrstream << complete_char;
			else if (i == pos) ostrstream << ">";
			else ostrstream << incomplete_char;
		}
		ostrstream << "] " << std::fixed << std::setprecision(2) << progress * 100.0 << "% " << max_macro(0, ticks / (double(time_elapsed) / 1000.0)) <<
			" iter/s " << double(time_elapsed) / 1000.0 << "s elapsed       ";
		std::cout << ostrstream.str();
		std::cout.flush();
	}

	void done()
	{
		mtx.lock();

		display();
		std::cout << std::endl;
		mtx.unlock();
	}
};

#endif //PROGRESSBAR_PROGRESSBAR_HPP