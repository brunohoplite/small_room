#include "presence_detector.hpp"
#include "sysfs_pwm.hpp"


#define STEPS 100
#define TIME_STEP (0.010)
#define MAX_DT (100)


class ControlBox : public PresenceDetector {
    public:
        enum class Mode {
            DETECT,
            DIM,
            BREATH,
            BLINK,
        };

    private:
        bool toggle_;
        Mode currentMode_;
        int brightnessDutyCycle_;
        int stepIndex_;
        int increasingDt_[STEPS];
        int decreasingDt_[STEPS];

        // enum class PollingPeriod {
        //     DETECT = 100,
        //     DIM = 250,
        //     BREATH = 10,
        //     BLINK = 1000,
        // };

        void doBlink(void);
        void doBreath(void);
        void prepareBreath(void);

    public:
        ControlBox(SysfsPwm& ledStrip, int detectPin);
        void doMode(void);
        void setMode(const Mode newMode);
        void setBrightness(int newBrightness);
};
