#include "Arduino.h"
#include "DFRobot_ESP_EC.h"
#include <math.h>

#define EC_PIN 34

DFRobot_ESP_EC ec;

float voltage, ecValue, temperature = 25;

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(32);		//needed EEPROM.begin to store calibration k in eeprom
	ec.begin();				//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
}

float cond_to_ppt(float cond, float temp) 
{
	float sal;
	float r,ds;
	float ok=0;
	float a0,a1,a2,a3,a4,a5;
	float b0,b1,b2,b3,b4,b5;
	float c0,c1,c2,c3,c4;
	float r2;

	a0=0.008;
	a1=-0.1692;
	a2=25.3851;
	a3=14.0941;
	a4=-7.0261;
	a5=2.7081;

	b0=0.0005;
	b1=-0.0056;
	b2=-0.0066;
	b3=-0.0375;
	b4=0.0636;
	b5=-0.0144;

	c0=0.6766097;
	c1=0.0200564;
	c2=0.0001104259;
	c3=-0.00000069698;
	c4=0.0000000010031;

	if (temp < 0 || temp > 30)
	{
		Serial.print("Buiten het bereik. Vloeistof moet tussen de 0 en 30 graden zijn");
	}
	else
	{
		if (cond <= 0){
			Serial.print("Geen meting");
		}
		else
		{
			r=cond/42914;
			r/=(c0+temp*(c1+temp*(c2+temp*(c3+temp*c4))));

			r2=sqrt(r);
			ds=b0+r2*(b1+r2*(b2+r2*(b3+r2*(b4+r2*b5))));
			ds*=((temp-15.0)/(1.0+0.0162*(temp-15.0)));

			sal=a0+r2*(a1+r2*(a2+r2*(a3+r2*(a4+r2*a5))))+ds;

			if(sal > 2.0){
				Serial.print("Salinity under scale");
			}

			if(sal > 42.0){
				Serial.print("Salinity over scale");
			}

			else
			{
				sal = round(sal);
				Serial.print(sal,1);
			}
		}
	}

}

void loop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{

	// 	timepoint = millis();
	// 	voltage = analogRead(EC_PIN)/4095.0*3300;
	// 	Serial.print("voltage:");
	// 	Serial.println(voltage, 4);

	// 	//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
	// 	Serial.print("temperature:");
	// 	Serial.print(temperature, 1);
	// 	Serial.println("^C");

	// 	ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
	// 	Serial.print("EC:");
	// 	Serial.print(ecValue, 4);
	cond_to_ppt(14130, 25);
	}
	// ec.calibration(voltage, temperature); // calibration process by Serail CMD
}

// float readTemperature()
// {
// 	//add your code here to get the temperature from your temperature sensor
// }

