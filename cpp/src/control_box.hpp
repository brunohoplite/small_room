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

        void doBlink(void);
        void doBreath(void);
        void prepareBreath(void);
        Mode stringToMode(const std::string& modeStr);

    public:
        ControlBox(SysfsPwm& ledStrip, int detectPin);
        void doMode(void);
        void setMode(const std::string& modeStr);
        void setBrightness(int newBrightness);
};
