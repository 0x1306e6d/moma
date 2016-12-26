#include "common.h"
#include "printer.h"
#include "timer.h"
#include "flex.h"
#include "hc_sr04.h"
#include "button.h"
#include "bluetooth.h"

// flash load "C:\Users\USER\Desktop\monday\moma\Debug\moma.axf"
// flash load "C:\Users\USER\Desktop\monday\moma\flashclear.axf"

int main()
{
	SystemInit();
	PrinterInit();
	TimerInit();
	ButtonInit();

	BluetoothInit();
	FlexSensorInit();
	SonarSensorInit();

	StartFlexSensorCalibration();
	StartSonarSensorCalibration();

	while (true)
	{
		HandleFlexSensor();
		HandleSonarSensor();

//		DelayMillis(33);
	}
}
