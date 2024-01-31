package boardTestProcesses;

public class NewBoardTestHelper {

    private boolean isCompleteTest = false;
    private boolean isPowerPinsProcessVoltageDone = false;
    private boolean isGpioContinuityProcessDone = false;
    private boolean isCurrentMeasureProcessDone = false;
    private boolean isFlashProcessDone = false;
    private boolean isTemperatureProcessDone = false;
    private boolean isGpioInputProcessNoPullDone = false;
    private boolean isGpioInputProcessPullDownDone = false;
    private boolean isGpioInputProcessPullUpDone = false;
    private boolean isGpioOutputNoPullProcessDone = false;
    private boolean isGpioOutputPullDOwnProcessDone = false;
    private boolean isGpioOutputPullUpProcessDone = false;
    private boolean isAnalogInputProcessDone = false;
    private boolean isGpioShortCircuitProcessDone = false;


    public NewBoardTestHelper(){

    }

    public void setCompleteTestDone(){
        setCompleteTest(false);
        setGpioContinuityProcessDone(false);
        setCurrentMeasureProcessDone(false);
        setPowerPinsProcessVoltageDone(false);
        setFlashProcessDone(false);
        setTemperatureProcessDone(false);
        setGpioInputProcessNoPullDone(false);
        setGpioInputProcessPullDownDone(false);
        setGpioInputProcessPullUpDone(false);
        setGpioOutputNoPullProcessDone(false);
        setGpioOutputPullDOwnProcessDone(false);
        setGpioOutputPullUpProcessDone(false);
        setGpioShortCircuitProcessDone(false);
        setAnalogInputProcessDone(false);
    }

    public boolean isAllTestProcessesDone(){
        return  isGpioContinuityProcessDone() && isCurrentMeasureProcessDone() && isPowerPinsProcessVoltageDone() &&
                isFlashProcessDone() &&  isTemperatureProcessDone() && isGpioInputProcessNoPullDone()
                && isGpioInputProcessPullDownDone() && isGpioInputProcessPullUpDone() && isGpioOutputNoPullProcessDone() &&
                isGpioOutputPullDOwnProcessDone() && isGpioOutputPullUpProcessDone() && isGpioShortCircuitProcessDone() && isAnalogInputProcessDone();
    }

    public boolean isCompleteTest() {
        return isCompleteTest;
    }

    public void setCompleteTest(boolean completeTest) {
        isCompleteTest = completeTest;
    }

    public boolean isGpioContinuityProcessDone() {
        return isGpioContinuityProcessDone;
    }

    public void setGpioContinuityProcessDone(boolean gpioContinuityProcessDone) {
        isGpioContinuityProcessDone = gpioContinuityProcessDone;
    }
    public boolean isPowerPinsProcessVoltageDone() {
        return isPowerPinsProcessVoltageDone;
    }

    public void setPowerPinsProcessVoltageDone(boolean powerPinsProcessVoltageDone) {
        isPowerPinsProcessVoltageDone = powerPinsProcessVoltageDone;
    }

    public boolean isFlashProcessDone() {
        return isFlashProcessDone;
    }

    public void setFlashProcessDone(boolean flashProcessDone) {
        isFlashProcessDone = flashProcessDone;
    }

    public boolean isTemperatureProcessDone() {
        return isTemperatureProcessDone;
    }

    public void setTemperatureProcessDone(boolean temperatureProcessDone) {
        isTemperatureProcessDone = temperatureProcessDone;
    }

    public boolean isGpioInputProcessNoPullDone() {
        return isGpioInputProcessNoPullDone;
    }

    public void setGpioInputProcessNoPullDone(boolean gpioInputProcessNoPullDone) {
        isGpioInputProcessNoPullDone = gpioInputProcessNoPullDone;
    }

    public boolean isGpioInputProcessPullDownDone() {
        return isGpioInputProcessPullDownDone;
    }

    public void setGpioInputProcessPullDownDone(boolean gpioInputProcessPullDownDone) {
        isGpioInputProcessPullDownDone = gpioInputProcessPullDownDone;
    }

    public boolean isGpioInputProcessPullUpDone() {
        return isGpioInputProcessPullUpDone;
    }

    public void setGpioInputProcessPullUpDone(boolean gpioInputProcessPullUpDone) {
        isGpioInputProcessPullUpDone = gpioInputProcessPullUpDone;
    }

    public boolean isGpioOutputNoPullProcessDone() {
        return isGpioOutputNoPullProcessDone;
    }

    public void setGpioOutputNoPullProcessDone(boolean gpioOutputNoPullProcessDone) {
        isGpioOutputNoPullProcessDone = gpioOutputNoPullProcessDone;
    }

    public boolean isGpioOutputPullDOwnProcessDone() {
        return isGpioOutputPullDOwnProcessDone;
    }

    public void setGpioOutputPullDOwnProcessDone(boolean gpioOutputPullDOwnProcessDone) {
        isGpioOutputPullDOwnProcessDone = gpioOutputPullDOwnProcessDone;
    }

    public boolean isGpioOutputPullUpProcessDone() {
        return isGpioOutputPullUpProcessDone;
    }

    public void setGpioOutputPullUpProcessDone(boolean gpioOutputPullUpProcessDone) {
        isGpioOutputPullUpProcessDone = gpioOutputPullUpProcessDone;
    }
    public boolean isAnalogInputProcessDone() {
        return isAnalogInputProcessDone;
    }

    public void setAnalogInputProcessDone(boolean analogInputProcessDone) {
        isAnalogInputProcessDone = analogInputProcessDone;
    }
    public boolean isGpioShortCircuitProcessDone() {
        return isGpioShortCircuitProcessDone;
    }

    public void setGpioShortCircuitProcessDone(boolean gpioShortCircuitProcessDone) {
        isGpioShortCircuitProcessDone = gpioShortCircuitProcessDone;
    }
    public boolean isCurrentMeasureProcessDone() {
        return isCurrentMeasureProcessDone;
    }

    public void setCurrentMeasureProcessDone(boolean currentMeasureProcessDone) {
        isCurrentMeasureProcessDone = currentMeasureProcessDone;
    }
}

