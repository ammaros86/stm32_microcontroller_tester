package helpers;

import static helpers.BoardTestState.BoardCompleteTested;
import static helpers.BoardTestState.BoardNotTested;
import static helpers.BoardTestState.BoardTestUncompleted;
import static helpers.Constants.*;

import android.content.Context;

import java.util.ArrayList;

import models.*;

public class BoardChecker {

    public BoardTestState isBoardTested(BoardTestCommands boardTestController) {
        if (boardTestController.getTest_value() == BoardCompleteTestedValue) {
            return BoardCompleteTested;
        } else if ((boardTestController.getTest_value() < BoardCompleteTestedValue) && (boardTestController.getTest_value() > BoardNotTestedValue)) {
            return BoardTestUncompleted;
        } else {
            return BoardNotTested;
        }
    }

    public boolean isGpioNotTestable(String pin) {
        return ((pin.contains(PA2)) || (pin.contains(PA3))
                || (pin.contains(PF0)) || (pin.contains(PC14))
                || (pin.contains(PC15) || (pin.contains(PA13)) || (pin.contains(PA14))));
    }

    public boolean isGpioConnectedToPushButton(String gpioPin) {
        return gpioPin.contains(PC13);
    }

    public boolean isGpioOutputConnectedToPushButton(GpioOutput gpioOutput) {
        return (gpioOutput.getPin().contains(PC13));
    }

    //CHECKED
    public int boardConnectionTest(ArrayList<PowerPinVoltage> powerPinsVoltageData) {
        int errors = 0;
        for (int i = 0; i < powerPinsVoltageData.size(); i++) {
            PowerPinVoltage powerPinVoltage = powerPinsVoltageData.get(i);
            String comment = powerPinVoltage.getComment();
            if (!comment.equals("OK")) {
                errors++;
            }
        }
        return errors;
    }

    //CHECKED
    boolean isPowerPinNotConnected(Context context, double measuredVoltage) {
        return ((measuredVoltage <= SharePreferenceHelper.getGNDMaxVoltage(context)) && (measuredVoltage >= SharePreferenceHelper.getGNDMinVoltage(context)));
    }

    //CHECKED
    boolean isGndPinNotConnected(Context context, double measuredVoltage) {
        return ((measuredVoltage <= SharePreferenceHelper.get5VMaxVoltage(context)) && (measuredVoltage >= SharePreferenceHelper.get5VMinVoltage(context)));
    }

    //CHECKED
    public boolean checkPowerPin(Context context, String pin, double measuredVoltage) {
        if (is3VoltageTypePowerPin(pin)) {
            return is3VPinOk(context, measuredVoltage);
        } else if (is5VoltageTypePowerPin(pin)) {
            return is5VPinOk(context, measuredVoltage);
        } else if (isE5VTypePowerPin(pin)) {
            return isE5VOk(context, measuredVoltage);
        } else {
            return isGndOk(context, measuredVoltage);
        }
    }

    //CHECKED
    public boolean isE5VOk(Context context, double measuredVoltage) {
        return ((measuredVoltage < SharePreferenceHelper.getE5VMaxVoltage(context)) && (measuredVoltage > SharePreferenceHelper.getE5VMinVoltage(context)));
    }

    //CHECKED
    public boolean isGndOk(Context context, double measuredVoltage) {
        return ((measuredVoltage <= SharePreferenceHelper.getGNDMaxVoltage(context)) && (measuredVoltage >= SharePreferenceHelper.getGNDMinVoltage(context)));
    }

    //CHECKED
    public boolean is5VPinOk(Context context, double measuredVoltage) {
        return ((measuredVoltage < SharePreferenceHelper.get5VMaxVoltage(context)) && (measuredVoltage > SharePreferenceHelper.get5VMinVoltage(context)));
    }

    //CHECKED
    public boolean is3VPinOk(Context context, double measuredVoltage) {
        return (((measuredVoltage < SharePreferenceHelper.get3VMaxVoltage(context)) && (measuredVoltage > SharePreferenceHelper.get3VMinVoltage(context))));
    }

    //CHECKED
    public boolean isVinPin(String pin) {
        return (pin.equals(vinPin));
    }

    //CHECKED
    public boolean isGndPin(String pin) {
        return (pin.equals(gnd1Pin)) || (pin.equals(aGndPin)) ||
                (pin.equals(gnd2Pin)) || (pin.equals(gnd3Pin)) ||
                (pin.equals(gnd4Pin) || (pin.equals(gnd5Pin)) || (pin.equals(gnd6Pin)));
    }

    //CHECKED
    public boolean is3VoltageTypePowerPin(String pin) {
        return (pin.equals(aVddPin)) || (pin.equals(vddPin)) ||
                (pin.equals(vBatPin)) || (pin.equals(iOrefPin)) ||
                (pin.equals(p3VPin));
    }

    //CHECKED
    public boolean is5VoltageTypePowerPin(String pin) {
        return (pin.equals(pU5VPin)) || (pin.equals(p5VPin));
    }

    //CHECKED
    private boolean isE5VTypePowerPin(String pin) {
        return (pin.equals(e5VPin));
    }

    public boolean isGpioOutputOk(GpioOutput gpioOutputLowSpeed, GpioOutput gpioOutputMediumSpeed, GpioOutput gpioOutputHighSpeed) {
        return (gpioOutputLowSpeed.getComment().equals(GPIO_OUTPUT_OK)) && (gpioOutputMediumSpeed.getComment().equals(GPIO_OUTPUT_OK)) && (gpioOutputHighSpeed.getComment().equals(GPIO_OUTPUT_OK));
    }


    public GpioOutput getLowSpeedTest(String pin, ArrayList<GpioOutput> arrayList) {
        GpioOutput gpioOutputLowSpeed = null;
        for (int i = 0; i < arrayList.size(); i++) {
            GpioOutput gpioOutput = arrayList.get(i);
            if ((pin.equals(gpioOutput.getPin())) && (gpioOutput.getSpeed() == GpioOutputTestSpeedLow)) {
                gpioOutputLowSpeed = gpioOutput;
                break;
            }
        }
        return gpioOutputLowSpeed;
    }


    public GpioOutput getMediumSpeedTest(String pin, ArrayList<GpioOutput> arrayList) {
        GpioOutput gpioOutputMediumSpeed = null;
        for (int i = 0; i < arrayList.size(); i++) {
            GpioOutput gpioOutput = arrayList.get(i);
            if ((pin.equals(gpioOutput.getPin())) && (gpioOutput.getSpeed() == GpioOutputTestSpeedMedium)) {
                gpioOutputMediumSpeed = gpioOutput;
                break;
            }
        }
        return gpioOutputMediumSpeed;
    }


    public GpioOutput getHighSpeedTest(String pin, ArrayList<GpioOutput> arrayList) {
        GpioOutput gpioOutputHighSpeed = null;
        for (int i = 0; i < arrayList.size(); i++) {
            GpioOutput gpioOutput = arrayList.get(i);
            if ((pin.equals(gpioOutput.getPin())) && (gpioOutput.getSpeed() == GpioOutputTestSpeedHigh)) {
                gpioOutputHighSpeed = gpioOutput;
                break;
            }
        }
        return gpioOutputHighSpeed;
    }


    public boolean hasOutputNoPullError(ArrayList<GpioOutput> gpioOutputNoPullArrayList) {
        boolean isError = false;
        for (int i = 0; i < gpioOutputNoPullArrayList.size(); i++) {
            GpioOutput boardOutputGpio = gpioOutputNoPullArrayList.get(i);
            if (!isGpioNotTestable(boardOutputGpio.getPin())) {
                if (isGpioOutputConnectedToPushButton(boardOutputGpio)) {
                    if (!boardOutputGpio.getComment().equals(GPIO_OUTPUT_OK)) {
                        isError = true;
                        break;
                    }
                } else {
                    GpioOutput gpioOutputLowSpeed = getLowSpeedTest(boardOutputGpio.getPin(), gpioOutputNoPullArrayList);
                    GpioOutput gpioOutputMediumSpeed = getMediumSpeedTest(boardOutputGpio.getPin(), gpioOutputNoPullArrayList);
                    GpioOutput gpioOutputHighSpeed = getHighSpeedTest(boardOutputGpio.getPin(), gpioOutputNoPullArrayList);
                    if ((gpioOutputLowSpeed != null) && (gpioOutputMediumSpeed != null) && (gpioOutputHighSpeed != null)) {
                        if (!isGpioOutputOk(gpioOutputLowSpeed, gpioOutputMediumSpeed, gpioOutputHighSpeed)) {
                            isError = true;
                            break;
                        }
                    } else {
                        isError = true;
                        break;
                    }
                }
            }
        }
        return isError;
    }

    public boolean hasOutputPullDownUpError(ArrayList<GpioOutput> gpioOutputArrayList) {
        boolean isError = false;
        for (int i = 0; i < gpioOutputArrayList.size(); i++) {
            GpioOutput boardOutputGpio = gpioOutputArrayList.get(i);
            if (!isGpioNotTestable(boardOutputGpio.getPin())) {
                if (!isGpioOutputConnectedToPushButton(boardOutputGpio)) {
                    GpioOutput gpioOutputLowSpeed = getLowSpeedTest(boardOutputGpio.getPin(), gpioOutputArrayList);
                    GpioOutput gpioOutputMediumSpeed = getMediumSpeedTest(boardOutputGpio.getPin(), gpioOutputArrayList);
                    GpioOutput gpioOutputHighSpeed = getHighSpeedTest(boardOutputGpio.getPin(), gpioOutputArrayList);
                    if ((gpioOutputLowSpeed != null) && (gpioOutputMediumSpeed != null) && (gpioOutputHighSpeed != null)) {
                        if (!isGpioOutputOk(gpioOutputLowSpeed, gpioOutputMediumSpeed, gpioOutputHighSpeed)) {
                            isError = true;
                            break;
                        }
                    } else {
                        isError = true;
                        break;
                    }
                }
            }
        }
        return isError;
    }

    public int getGpioOutputTestResultsErrors(ArrayList<GpioOutput> gpioOutputNoPullArrayList, ArrayList<GpioOutput> gpioOutputPullDownArrayList, ArrayList<GpioOutput> gpioOutputPullUpArrayList) {
        int errors = 0;
        for (int i = 0; i < gpioOutputNoPullArrayList.size(); i++) {
            GpioOutput gpioOutput = gpioOutputNoPullArrayList.get(i);
            if (!isGpioNotTestable(gpioOutput.getPin())) {
                if (!gpioOutput.getComment().equals("OK")) {
                    errors++;
                }
            }
        }
        for (int i = 0; i < gpioOutputPullDownArrayList.size(); i++) {
            GpioOutput gpioOutput = gpioOutputNoPullArrayList.get(i);
            if (!isGpioNotTestable(gpioOutput.getPin())) {
                if (!gpioOutput.getComment().equals("OK")) {
                    errors++;
                }
            }
        }
        for (int i = 0; i < gpioOutputPullUpArrayList.size(); i++) {
            GpioOutput gpioOutput = gpioOutputNoPullArrayList.get(i);
            if (!isGpioNotTestable(gpioOutput.getPin())) {
                if (!gpioOutput.getComment().equals("OK")) {
                    errors++;
                }
            }
        }
        return errors;
    }

    public int getGpioInputTestErrors(ArrayList<GpioInput> gpioInputNoPullList, ArrayList<GpioInput> gpioInputPullDownList, ArrayList<GpioInput> gpioInputPullUpList) {
        int errors = 0;
        for (int i = 0; i < gpioInputNoPullList.size(); i++) {
            GpioInput boardInputGpio = gpioInputNoPullList.get(i);
            if (!isGpioNotTestable(boardInputGpio.getPin())) {
                if (!boardInputGpio.getComment().equals("OK")) {
                    errors++;
                }
            }
        }
        for (int i = 0; i < gpioInputPullDownList.size(); i++) {
            GpioInput boardInputGpio = gpioInputPullDownList.get(i);
            if (!isGpioNotTestable(boardInputGpio.getPin())) {
                if (!isGpioConnectedToPushButton(boardInputGpio.getPin())) {
                    if (!boardInputGpio.getComment().equals("OK")) {
                        errors++;
                    }
                }
            }
        }
        for (int i = 0; i < gpioInputPullUpList.size(); i++) {
            GpioInput boardInputGpio = gpioInputPullUpList.get(i);
            if (!isGpioNotTestable(boardInputGpio.getPin())) {
                if (!isGpioConnectedToPushButton(boardInputGpio.getPin())) {
                    if (!boardInputGpio.getComment().equals("OK")) {
                        errors++;
                    }
                }
            }
        }
        return errors ;
    }

    public boolean hasGpioShortCircuit(ArrayList<GpioShortCircuit> gpioShortCircuitArrayList) {
        boolean hasError = false;
        for (int i = 0; i < gpioShortCircuitArrayList.size(); i++) {
            GpioShortCircuit gpioShortCircuit = gpioShortCircuitArrayList.get(i);
            if (!isGpioNotTestable(gpioShortCircuit.getPin())) {
                if (!isGpioConnectedToPushButton(gpioShortCircuit.getPin())) {
                    if (gpioShortCircuit.getShort_circuits_count() != 0) {
                        hasError = true;
                        break;
                    }
                }
            }
        }
        return hasError;
    }

    public int getGpiosContinuityErrors(ArrayList<GpioContinuity> continuityArrayList) {
        int errors = 0;
        for (int i = 0; i < continuityArrayList.size(); i++) {
            GpioContinuity gpioContinuity = continuityArrayList.get(i);
            if (gpioContinuity != null) {
                String comment = gpioContinuity.getComment();
                if ((!comment.contains("OK")) && (!comment.contains("GPIO Standard Open"))) {
                    errors++;
                }
            }
        }
        return errors;
    }

    public int getAnalogInputErrors(ArrayList<AnalogInput> analogInputArrayList) {
        int errors = 0;
        for (int i = 0; i < analogInputArrayList.size(); i++) {
            AnalogInput analogInput = analogInputArrayList.get(i);
            if (analogInput != null) {
                String comment = analogInput.getComment();
                if (!comment.contains("OK")) {
                    errors++;
                }
            }
        }
        return errors;
    }

    public int getPowerGndPinsErrors(ArrayList<PowerPinVoltage> powerPinVoltages) {
        int errors = 0;
        for (int i = 0; i < powerPinVoltages.size(); i++) {
            if (!powerPinVoltages.get(i).getComment().equals("OK")) {
                errors++;
            }
        }
        return errors;
    }

    public static String convertLogFileName(String logFileName) {
        String[] parts = logFileName.split("_");
        if (parts.length == 7) {
            String fileName = parts[0];
            String day = parts[1];
            String month = parts[2];
            String year = parts[3];
            String hour = parts[4];
            String minute = parts[5];
            String second = parts[6];
            return fileName + "  " + day + "/" + month + "/" + year + " " + hour + ":" + minute + ":" + second;
        }
        return logFileName;
    }
}
