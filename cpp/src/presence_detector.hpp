#include "timer.hpp"
#include "sysfs_pwm.hpp"

class PresenceDetector {
    private:
        SysfsPwm& ledStrip_;
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
        PresenceDetector(SysfsPwm& ledStrip, int detectPin)
            : ledStrip_(ledStrip), detectPin_(detectPin), currentState(IDLE) {}
        ~PresenceDetector();
        void poll(void);
        void reset(void);
};