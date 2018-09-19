#pragma once

#include <Hardware/LEDs/MAX7219_Base.hpp>

class MAX7219 : public MAX7219_Base, public ExtendedIOElement {

  public:
    MAX7219(pin_t loadPin) : MAX7219_Base(loadPin), ExtendedIOElement(8 * 8) {}

    void begin() override {
        init();
    }

    void setLED(uint8_t col, uint8_t row, bool val) {
        if (val)
            buffer[row] |= 1 << col;
        else
            buffer[row] &= ~(1 << col);
        update();
    }

    bool getLED(uint8_t col, uint8_t row) { return buffer[row] & (1 << col); }

    /**
     * @brief   The pinMode function is not implemented because the mode is
     *          `OUTPUT` by definition.
     */
    void pinMode(UNUSED_PARAM pin_t pin, UNUSED_PARAM uint8_t mode) override
        __attribute__((deprecated)) {}

    /**
     * @brief   Set the state of a given output pin.
     * 
     * @param   pin
     *          The output pin to set.
     * @param   val
     *          The value to set the output to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, uint8_t val) override {
        setLED(pin % 8, pin / 8, val);
    }

    /**
     * @brief   Get the current state of a given output.
     * 
     * @param   pin
     *          The output to read the state from.
     * @return  0
     *          The state of the output is `LOW`.
     * @return  1
     *          The state of the output is `HIGH`.
     */
    int digitalRead(pin_t pin) override { return getLED(pin % 8, pin / 8); }

    /**
     * @brief   The analogRead function is deprecated because a MAX7219
     *          is always digital.
     * @param   pin
     *          The output to read the state from.
     * @return  0
     *          The state of the output is `LOW`.
     * @return  1023
     *          The state of the output is `HIGH`.
     */
    analog_t analogRead(pin_t pin) override __attribute__((deprecated)) {
        return 1023 * digitalRead(pin);
    }

    /**
     * @brief   The analogWrite function is not deprecated because a MAX7219
     *          is always digital.
     * @param   pin
     *          The output to set.
     * @param   val
     *          The value to set the output to. A value greater or equal to 0x80
     *          will set the output to a `HIGH` state, a value less than 0x80 
     *          will set the output to a `LOW` state.
     */
    void analogWrite(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80);
    }

    void update() override {
        for (uint8_t i = 0; i < 8; i++)
            sendRaw(i + 1, buffer[i]);
    }

  private:
    uint8_t buffer[8] = {}; // TODO: use BitArray
};
