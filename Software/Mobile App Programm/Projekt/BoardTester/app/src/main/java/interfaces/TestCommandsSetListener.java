package interfaces;

public interface TestCommandsSetListener {
    void onTryToSetTemperatureCommand(boolean isSet);
    void onTryToSetPowerGndPinsCommand(boolean isSet);
    void onTryToSetGpioContinuityCommand(boolean isSet);
    void onTryToSetFlashCommand(boolean isSet);
    void onTryToSetGpioInputCommand(boolean isSet);
    void onTryToSetGpioOutputCommand(boolean isSet);
    void onTryToSetAnalogInputCommand(boolean isSet);
    void onTryToSetGpioShortCircuitCommand(boolean isSet);
    void onTryToSetMeasureCurrentCommand(boolean isSet);
}
