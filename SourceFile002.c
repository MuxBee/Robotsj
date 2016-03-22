#pragma config(Sensor, S1,     lineLight,      sensorLightActive)
#pragma config(Sensor, S2,     lineColorRight, sensorColorNxtFULL)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Sensor, S4,     lineColorLeft,  sensorColorNxtFULL)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
const int maxSize = 20;
const int INBOX = 5;

void remmen(int snelheid)
{
	for(int i = snelheid; i > 0; i --)
	{
		motor[motorB] = i;
		motor[motorC] = i;
		wait1Msec(10);
	}
	motor[motorB] = 0;
	motor[motorC] = 0;
}
void gas(int rechterMotor, int linkerMotor)
{
	motor[motorB] = rechterMotor;
	motor[motorC] = linkerMotor;
}
void schieten(int rondjes)
{
	int graden = rondjes * 360;
	nMotorEncoder[motorA] = 0;
	while(nMotorEncoder[motorA] < graden)
	{
		motor[motorA] = 100;
	}
	motor[motorA] = 0;
}
char* bluetooth()
{
	TFileIOResult result;
  int size;
  ubyte buffer[maxSize];
  size = cCmdMessageGetSize(INBOX);
  string s = "";

  if (size > maxSize)
    size = maxSize;
  if (size > 0){
  	result = cCmdMessageRead(buffer, size, INBOX);
  	buffer[size] = '\0';
  	stringFromChars(s, (char *) buffer);
  }
  return s;
}

bool rijden(int snelheid, bool moving)
{
	int remming = 10;
	if(moving)
	{
		if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] != 1)
		{
			if(SensorValue[lineLight] < 50)
			{
				gas(snelheid,snelheid);
			}
		}
		else if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] == 1)
		{
			snelheid = 50;
			for(int i = snelheid; i > 0; i -= remming)
			{
				if(SensorValue[lineLight] >= 50)
				{
					remming = 15;
				}
				gas(i, snelheid);
			}
		}
		else if(SensorValue[lineColorLeft] == 1 && SensorValue[lineColorRight] != 1)
		{
			snelheid = 50;
			for(int i = snelheid; i > 0; i -= remming)
			{
				if(SensorValue[lineLight] >= 50)
				{
					remming = 20;
				}
				gas(snelheid, i);
			}
		}
		if(SensorValue[sonar] < 30)
		{
			remmen(snelheid);
			schieten(2);
			moving = false;
		}
	}
	return moving;
}

task main()
{
	bool moving;
	bool startE = false;
	string command;

	while(true)
	{
		int snelheid = 100;
		command = bluetooth();
		if(command == "A")
		{
			remmen(snelheid);
			moving = false;
		}
		if(command == "B" && !startE)
		{
			moving = true;
		}
		if(command == "C")
		{
			schieten(2);
		}
	  moving = rijden(snelheid,moving);
	}
}
