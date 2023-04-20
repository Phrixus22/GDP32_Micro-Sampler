#include <SPI.h>
#include <SD.h>

// Define analog input
#define ANALOG_IN_PIN A1
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;
File dataFile;
unsigned long StartTime;


void setup() {
  char recievedChar;
  Serial.begin(9600); //Start Serial Monitor to display current read value on Serial monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SD.begin(4);
  
  Serial.println("Data Logging Software v1.2");
  Serial.println("Data must be taken off after every test");
  dataFile = SD.open("TESTDATA.txt", FILE_WRITE);
  dataFile.println("Time,Voltage,Current");
  Serial.println("Enter 'Q' to finish collecting data.");
  StartTime = millis();
}

void loop() {
  char recievedChar;
  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A0);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;//Taking Average of Samples

  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset. If your arduino is working on different voltage than 
  //you must change the offset according to the input voltage)
  //0.100v(100mV) is rise in output voltage when 1A current flows at input
  AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.100;

  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);

  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0; 

  // Calculate voltage at divider input
  in_voltage = adc_voltage / (R2/(R1+R2)) ; 
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  // Print results to Serial Monitor to 2 decimal places
  dataFile.print(ElapsedTime);
  dataFile.print(",");

  dataFile.print(in_voltage, 4);
  dataFile.print(",");

  dataFile.println(AcsValueF, 4);//Print the read current on Serial monitor
  Serial.println(AcsValueF, 4);//Print the read current on Serial monitor
  if (Serial.available() > 0){
    recievedChar = Serial.read();
    if(recievedChar == 'Q'){
      dataFile.close();
      Serial.println("Data Collected.");
    }
  }
}