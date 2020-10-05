/*
min voltage difference by 12bitDAC = 1.1mV
pins are DAC - sda,scl output goes to CE
      RE is grounded through buffer
      ADC i/p is A0
      specifications   can detect  -240 to +240 microA
*/
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#define ADCin A0 // current to voltage converter 

Adafruit_MCP4725 dac; // For setting up DAC

void setup(void) {
  Serial.begin(9600);
  dac.begin(0x60); // The address for DAC
  Serial.println("CLEARSHEET"); // clears starting at row 1
  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Sweep Rate,Volt in CE(V),Current in WE,Impedance(kohm) , "); //always write LABEL, so excel knows the next things will be the names of the columns instead of Acolumn you could write Time for instance)
  Serial.println("RESETTIMER"); //resets timer to 0  
}

void loop(void) {
    uint32_t dac_value = 0;
    double adcValueRead = 0;
    long double currentRead = 0;
    double sweep_rate_value ;
    int round_currentvalue;
    float v_low, v_high ,sweep_rate ;
    double dac_expected_output;
    double impedance;

 
  v_low =  -0.7000 ;
  v_high =  0.7000;
  //sweep_rate =  2 ;
  
  for(int iteration = 0; iteration < 3 ; iteration++)
   {
    if(iteration == 0 )sweep_rate_value = 100.000;// sweep rate 118 mV/s
    if(iteration == 1 )sweep_rate_value = 200.000;// sweep rate 236mV/s
     if(iteration == 2 )sweep_rate_value = 250.000;// sweep rate 59mV/s

    //sweep_rate_value = (sweep_rate*4096)/(1000*4.84);

    Serial.print("\n \n \nSweepRate(mV/s): ");
    Serial.print((sweep_rate_value*5.05*1000.000)/4096);
    Serial.print("\n  ");
    Serial.println("BEEP");

      
    for (dac_value = ((double)v_low+(double)1.0)*(double)1000.0/((double)1.2329);dac_value <((double)v_high+(double)1.0)*(double)1000/(double)1.2329 ;dac_value =(double) dac_value +(double) sweep_rate_value)
    {
      dac.setVoltage(dac_value, false);
      delay(20);
      dac_expected_output = ((double)5.05/(double)4096) * (double)dac_value;
      
      
      adcValueRead = analogRead(ADCin);
      currentRead = ((double)adcValueRead - (double)511)/(double)2.15714;
      round_currentvalue = round(currentRead);

      impedance = ((double)dac_expected_output - (double)1.0)/(double)round_currentvalue;

      
      /*Serial.print("\tCE Voltage given(V): ");
      Serial.print(dac_expected_output - 1.0,3);
        
      Serial.print("\tWE current(microA): ");      
      Serial.println(round_currentvalue); 
      */
      
      Serial.println( (String) "DATA," + (sweep_rate_value*5.05*1000)/4096 + "," + (double)((double)dac_expected_output - (double)1.0) + "," + round_currentvalue + "," + (double)impedance*(double)1000 + "," + " " /*+ "AUTOSCROLL_20"*/);

      delay(1000);
    }

    for (;dac_value >= (v_low+1.0)*1000/(1.2329); dac_value = dac_value - sweep_rate_value)
    {
      dac.setVoltage(dac_value, false);
      delay(20);
      dac_expected_output = (5.05/(double)4096.0) *(double) dac_value;
      
      
      adcValueRead = analogRead(ADCin);
      currentRead = ((double)adcValueRead - (double)511)/2.15714;
      round_currentvalue = round(currentRead);
      
      impedance = ((double)dac_expected_output -(double)1.0)/(double)round_currentvalue;

      //Serial.print(dac_value);
      /*
      Serial.print("\tCE Voltage given(V): ");
      Serial.print(dac_expected_output - 1.0,3);

      Serial.print("\tWE current(microA): ");      
      Serial.println(round_currentvalue);
      */
      Serial.println( (String) "DATA," + (sweep_rate_value*5.05*1000)/4096 + "," + (double)(dac_expected_output - 1.0) + "," + round_currentvalue + "," + (double)impedance*1000 + "," + " " /*+ "AUTOSCROLL_20"*/);
      
      delay(1000);
    }
   }
}
