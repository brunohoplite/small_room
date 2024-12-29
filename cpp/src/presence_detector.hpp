#include "timer.hpp"
#include "sysfs_pwm.hpp"

class PresenceDetector {
    protected:
        SysfsPwm& ledStrip_;
        void reset(void);

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
        PresenceDetector(SysfsPwm& ledStrip, int detectPin)
            : ledStrip_(ledStrip), detectPin_(detectPin) { reset(); }
        void poll(void);
};