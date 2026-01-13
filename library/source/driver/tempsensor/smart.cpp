//! @todo Add smart temperature sensor implementation details here!

#include <stdint.h>
//***********LOCAL HEADERS*********//
#include "driver/adc/interface.h"
#include "driver/tempsensor/smart.h"
#include "ml/lin_reg/interface.h"
#include "utils/utils.h"

namespace driver
{

namespace tempsensor
{

// -----------------------------------------------------------------------------
Smart::Smart(const uint8_t pin, adc::Interface& adc, ml::lin_reg::Interface& linReg) noexcept

    : myAdc{adc}
    , myLinReg{linReg}
    , myPin{pin}
{
    // Enable the ADC if the initialization succeeded.
    if (isInitialized()) { myAdc.setEnabled(true); }
}

// -----------------------------------------------------------------------------
bool Smart::isInitialized() const noexcept 
{ 
    // Return true if the temperature sensor pin is valid and the ADC is initialized.
    return myAdc.isChannelValid(myPin) && myAdc.isInitialized() && myLinReg.isTrained(); 
}

// -----------------------------------------------------------------------------
int16_t Smart::read() const noexcept
{
    // Return 0 if initialization failed.
    if (!isInitialized()) 
    { 
        // Calculate the inputvoltage with ADC
        const auto inputVoltage {myAdc.inputVoltage()};

        // Predict the temperature based on the input voltage.
        const double predictedTemp{myLinReg.predict(inputVoltage)};

        // Return the etmprature rounded to the nearest integer.
        return utils::round<int16_t>(predictedTemp);
        
    }
    // Return 0 is the temp snesor isn't initialized.
    return 0;
}
} // namespace tempsensor
} // namespace driver