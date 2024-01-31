package interfaces;

import java.util.ArrayList;

import models.*;

public interface TestProcessListener {

    //Temperature Process
    void onTemperatureDataLoaded(BoardTemperature boardsTemperature);
    void onFailedTemperatureDataLoad(Exception exception);

    //Current Measure Process
    void onCurrentConsumptionDataLoaded(BoardCurrentConsumption boardCurrentConsumption);
    void onFailedCurrentConsumptionDataLoad(Exception exception);

    // Flash Process
    void onFlashDataLoaded(BoardFlash boardFlash);
    void onFailedFlashDataLoad(Exception exception);

    //Power Process
    void onPowerPinsVoltageDataLoaded(ArrayList<PowerPinVoltage> powerPinsVoltageResult);
    void onFailedPowerPinsVoltageDataLoad(Exception exception);

    void onGpioContinuityDataLoaded(ArrayList<GpioContinuity> continuityArrayList);
    void onFailedGpioContinuityDataLoad(Exception exception);

    //Gpio Input Test Process
    void onGpioInputNoPullDataLoaded(ArrayList<GpioInput> gpioInputNoPullList);
    void onFailedGpioInputNoPullDataLoad(Exception exception);
    void onGpioInputPullDownDataLoaded(ArrayList<GpioInput> gpioInputPullDownArrayList);
    void onFailedGpioInputPullDownDataLoad(Exception exception);
    void onGpioInputPullUpDataLoaded(ArrayList<GpioInput> gpioInputPullUpArrayList);
    void onFailedGpioInputPullUpDataLoad(Exception exception);

    //GPIO Output Test Process
    void onGpioOutputNoPullDataLoaded(ArrayList<GpioOutput> gpioOutputNoPullArrayList);
    void onFailedGpioOutputNoPullDataLoad(Exception exception);
    void onGpioOutputPullDownDataLoaded(ArrayList<GpioOutput> gpioOutputPullDownArrayList);
    void onFailedGpioOutputPullDownDataLoad(Exception exception);
    void onGpioOutputPullUpDataLoaded(ArrayList<GpioOutput> gpioOutputPullUpArrayList);
    void onFailedGpioOutputPullUpDataLoad(Exception exception);

    // GPIO Short Circuit
    void onAnalogInputDataLoaded(ArrayList<AnalogInput> analogInputArrayList);
    void onFailedAnalogInputDataLoad(Exception exception);


    // GPIO Short Circuit
    void onGpioShortCircuitDataLoaded(ArrayList<GpioShortCircuit> gpioShortCircuitArray);
    void onFailedGpioShortCircuitDataLoad(Exception exception);

    //update App Controller
    void onFailedUpdateBoardCommands(Exception exception);
    void onFailedUpdateBoardController(Exception exception);

}
