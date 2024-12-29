#include <chrono>


class Timer {
    private:
        std::chrono::milliseconds delay_;
        std::chrono::steady_clock::time_point start_;

    public:
        Timer(std::chrono::milliseconds delay) : delay_(delay), start_(std::chrono::steady_clock::now()) {}
        Timer() : delay_(std::chrono::milliseconds(1000)), start_(std::chrono::steady_clock::now()) {}

        bool hasElapsed(void) {
            const auto now = std::chrono::steady_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_);
            return elapsed >= delay_;
        }

        void reset(void) {
            start_ = std::chrono::steady_clock::now();
        }

        void setDelay(std::chrono::milliseconds newDelay) {
            delay_ = newDelay;
            reset();
        }
};