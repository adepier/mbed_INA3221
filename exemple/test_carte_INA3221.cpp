#include "mbed.h"
#include "INA3221.h"
//L432KC
//  #define I2C_SDA PA_10
// #define I2C_SCL PA_9  
//L452RE
#define I2C_SDA PB_7
#define I2C_SCL PB_6 
 I2C i2c(I2C_SDA, I2C_SCL);
 
INA3221 INA(i2c, 0x43);
DigitalOut myled(LED1);
 
int main() {
    uint8_t i=0; 
printf("ConfigurationRegister(RW):        0x%X\n",INA.ReadRegister(INA3221_REG_CONFIG));
printf("Channel-1ShuntVoltage(R):         0x%X\n",INA.ReadRegister(INA3221_REG_SHUNTVOLTAGE_1));
printf("Channel-1BusVoltage(R):           0x%X\n",INA.ReadRegister(INA3221_REG_BUSVOLTAGE_1));
printf("Channel-2ShuntVoltage(R):         0x%X\n",INA.ReadRegister(INA3221_REG_SHUNTVOLTAGE_2));
printf("Channel-2BusVoltage(R):           0x%X\n",INA.ReadRegister(INA3221_REG_BUSVOLTAGE_2));
printf("Channel-3ShuntVoltage(R):         0x%X\n",INA.ReadRegister(INA3221_REG_SHUNTVOLTAGE_3));
printf("Channel-3BusVoltage(R):           0x%X\n",INA.ReadRegister(INA3221_REG_BUSVOLTAGE_3));
printf("Channel-1CriticalAlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_CRITICAL_1));
printf("Channel-1Warning-AlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_WARNING_1));
printf("Channel-2CriticalAlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_CRITICAL_2));
printf("Channel-2Warning-AlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_WARNING_2));
printf("Channel-3CriticalAlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_CRITICAL_3));
printf("Channel-3Warning-AlertLimit(RW):  0x%X\n",INA.ReadRegister(INA3221_REG_WARNING_3));
printf("ShuntVoltageSum(R):               0x%X\n",INA.ReadRegister(INA3221_REG_SUM_SHUNTVOLTAGE));
printf("ShuntVoltageSumLimit(RW):         0x%X\n",INA.ReadRegister(INA3221_REG_SUM_LIMIT));
printf("Mask/Enable(RW):                  0x%X\n",INA.ReadRegister(INA3221_REG_MASK));
printf("Power-ValidUpperLimit(RW):        0x%X\n",INA.ReadRegister(INA3221_REG_POWERVALID_UPPER_LIMIT));
printf("Power-ValidLowerLimit(RW):        0x%X\n",INA.ReadRegister(INA3221_REG_POWERVALID_LOWER_LIMIT));
printf("ManufacturerID(R):                0x%X\n",INA.ReadRegister(INA3221_REG_MANUFACTURER_ID));
printf("DieID(R):                         0x%X\n",INA.ReadRegister(INA3221_REG_DIE_ID));

   if(INA.CheckConnection()==1){
        printf("Connection OK \n");
    }
    else{
        printf("Error no Connection \n");
        while(1);
    }
        
    INA.Rest();
    INA.SetMode(INA3221_MODE_SHUNT_BUS_CONTINUOUS);
    INA.SetPowerValidLowerLimitVoltage(4.0f);
    INA.SetPowerValidUpperLimitVoltage(5.0f); 
    INA.SetShuntConversionTime(INA3221_1_1_MS);                          
    INA.SetBusConversionTime(INA3221_1_1_MS);                            
    INA.SetAveragingMode(INA3221_AVERAGE_64);                                        
    
    for(i = 1;i<4;i++)
    {
        INA.EnableChannel(i); 
        INA.EnableChannelSummation(i);
        // The critical-alert feature monitors functions based on individual conversions of each shunt-voltage channel.
        INA.SetCurrentCriticalAlertLimit(i, 0.7 );  // Peak max. 0.7 A
        // The warning alert monitors the averaged value of each shunt-voltage channel.
        INA.SetCurrentWarningAlertLimit(i, 0.5 );  // Average max. 0.5 A
    }
    
    
    while(1) {
        while(INA.ConversionReady()==0);
        
        if(INA.GetPowerVaildAlertFlag()){ 
            printf("Bus Voltage OK\n");
        }
        else{
            printf("Bus Voltage under Lower Limit\n");
        }
        
        printf("Ch1: %f mA Ch2: %f mA Ch3: %f mA\n",INA.GetCurrent(1)*1000.0f,INA.GetCurrent(2)*1000.0f,INA.GetCurrent(3)*1000.0f);
        printf("Ch1: %f V Ch2: %f V Ch3: %f V\n\n",INA.GetBusVoltage(1),INA.GetBusVoltage(2),INA.GetBusVoltage(3));
    
        myled = !myled;
        ThisThread::sleep_for(chrono::milliseconds(200));
    }
}
 
 