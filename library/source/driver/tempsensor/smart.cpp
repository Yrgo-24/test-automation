/**
 * @brief Smart temperature sensor implementation details.
 */
#include <stdint.h>

#include "driver/adc/interface.h"
#include "driver/tempsensor/smart.h"
#include "ml/lin_reg/interface.h"
#include "utils/utils.h"

namespace driver
{
namespace tempsensor
{
// -----------------------------------------------------------------------------
Smart::Smart(uint8_t pin, adc::Interface& adc, ml::lin_reg::Interface& linReg) noexcept
    : myAdc{adc}
    , myLinReg{linReg}
    , myPin{pin}
{}

// -----------------------------------------------------------------------------
bool Smart::isInitialized() const noexcept
{
    // Return true if the ADC is initialized, the pin is valid, and the linear regression
    // model is trained.
    return myAdc.isInitialized() && myAdc.isChannelValid(myPin) && myLinReg.isTrained();
}

// -----------------------------------------------------------------------------
int16_t Smart::read() const noexcept
{
    // Predict the temperature based on the the input voltage if initialized.
    return isInitialized() ? 
        utils::round<int16_t>(myLinReg.predict(myAdc.inputVoltage(myPin))) : 0;
}
} // namespace tempsensor
} // namespace driver
