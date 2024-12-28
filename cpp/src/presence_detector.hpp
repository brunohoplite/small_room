#include "timer.hpp"

class PresenceDetector {
    private:
        Timer timer_; // Gets instantiated when PresenceDetector is instantiated (default constructor is called)
        int detectPin_;
        typedef enum {
            IDLE,
            DETECTED,
            WAIT,
        } State;
        State currentState;
        bool isDetectPinAsserted(void);

    public:
        PresenceDetector(int detectPin);
        ~PresenceDetector();
        void poll(void);
        void reset(void);
};