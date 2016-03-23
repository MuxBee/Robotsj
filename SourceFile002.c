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


bool rijden(int snelheid, bool moving, bool kruispunt, int richting)
{
	int remming = 10;
	string command;
//	bool kruispunt = 0;
	if(moving && !kruispunt)
	{
		if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] != 1) //Rechtdoor Rijden
		{
			if(SensorValue[lineLight] < 50)
			{
				gas(snelheid,snelheid);
			}
		}
		else if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] == 1) //Bocht naar rechts.
		{
			snelheid = 20;
			for(int i = snelheid; i > 0; i -= remming)
			{
				if(SensorValue[lineLight] >= 50)
				{
					remming = 15;
				}
				gas(i, snelheid);
			}
		}
		else if(SensorValue[lineColorLeft] == 1 && SensorValue[lineColorRight] != 1) // Bocht naar links.
		{
			snelheid = 20;
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
			moving = true;
			gas(10,10);
		}
	}
	if(moving && kruispunt)
	{

		if(richting == 1)
		{
			snelheid = 20;
			if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] != 1) //Rechtdoor Rijden
			{
				if(SensorValue[lineLight] < 50)
					{
						gas(snelheid,snelheid);
					}
			}

			if(SensorValue[lineColorLeft] == 1 && SensorValue[lineColorRight] == 1) // Linksaf
			{

				for(int i = snelheid; i > 0; i -= remming)
				{
					if(SensorValue[lineLight] >= 50)
					{
						remming = 5;
					}
					gas(i, snelheid);
				}
				kruispunt = false;
				richting = 0;

				moving = true;
			}

		}

		if(richting == 2)
		{
			snelheid = 30;
			if(SensorValue[lineColorLeft] != 1 && SensorValue[lineColorRight] != 1) //Rechtdoor Rijden
			{
				if(SensorValue[lineLight] < 50)
					{
						gas(snelheid,snelheid);
					}
			}

			if(SensorValue[lineColorLeft] == 1 && SensorValue[lineColorRight] == 1)
			{

				while(SensorValue[lineColorLeft] == 1 && SensorValue[lineColorRight] == 1)
				{
						gas(0,100);
						wait1Msec(13);
				}

				kruispunt = false;
				richting = 0;
				moving = true;
				moving = rijden(snelheid,moving,kruispunt,richting);
			}
		}

	}

	return moving;
}

task main()
{
	bool moving;
	int richting = 0;
	bool kruispunt = false;
	bool startE = false;
	string command;

	while(true)
	{
		int x;

		int snelheid = x;
		command = bluetooth();

		if(command == "DOWN")
		{
			remmen(snelheid);
			moving = false;
		}
		if(command == "UP" && !startE)
		{
			moving = true;
			x = 100;
		}
		if(command == "FIRE")
		{
			schieten(2);
		}

		if(command == "LEFT")
		{
			kruispunt = true;
			richting = 1;
		}
		if(command == "C")
		{
			kruispunt = false;
			richting = 0;
			snelheid = -25;
		}
	  moving = rijden(snelheid,moving,kruispunt,richting);
	}
}
