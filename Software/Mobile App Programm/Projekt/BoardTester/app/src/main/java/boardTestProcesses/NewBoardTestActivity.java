package boardTestProcesses;

import static helpers.Constants.BoardCompleteTestedValue;
import static helpers.Constants.ExceptionNoData;
import static helpers.Constants.ExceptionValueNull;
import static helpers.Constants.boardID_Default;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;

import com.example.boardtester.R;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Objects;

import base.BaseActivity;
import helpers.Constants;
import helpers.CurrentTestProcess;
import interfaces.CurrentProcessListener;
import models.AnalogInput;
import models.BoardCurrentConsumption;
import models.BoardFlash;
import models.BoardTemperature;
import models.BoardTestCommands;
import models.GpioContinuity;
import models.GpioInput;
import models.GpioOutput;
import models.GpioShortCircuit;
import models.PowerPinVoltage;

public class NewBoardTestActivity extends BaseActivity implements CurrentProcessListener {

    private CurrentTestProcess currentProcess;
    private TemperatureProcess temperatureProcess;
    private CurrentMeasureProcess currentMeasureProcess;
    private FlashProcess flashProcess;
    private PowerPinsTestProcess powerPinsTestProcess;
    private ContinuityTestProcess continuityTestProcess;
    private GpioInputTestProcess gpioInputTestProcess;
    private GpioOutputProcess gpioOutputTestProcess;
    private AnalogInputTestProcess analogInputTestProcess;
    private GpioShorCircuitProcess gpioShorCircuitProcess;
    private int boardID;
    private String boardName;
    private NewBoardTestViewsHelper newBoardTestViewsHelper;
    private NewBoardTestHelper newBoardTestHelper;
    private int currentTestValue = 0;
    private boolean isPowerPinsCommandSet, isContinuityCommandSet, isTemperatureCommandSet,  isMeasureCurrentCommandSet, isFlashCommandSet, isGpioInputTestCommandSet, isOutputCommandSet, isAnalogInputCommandSet, isGpioShortCommandSet;
    int waitCounterCompleteTest = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_board_test);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
        boardID = getIntent().getIntExtra(Constants.boardID, boardID_Default);

        newBoardTestViewsHelper = new NewBoardTestViewsHelper(this);
        newBoardTestHelper = new NewBoardTestHelper();
        newBoardTestViewsHelper.getBoardIDTextView().setText(boardName);
        initialProcess();
        getAndPostLastTestsResults();
        initialCompleteTest();
    }

    @Override
    protected void onStop() {
        super.onStop();
        startCommandHandler();
    }

    private void initialProcess(){
        currentProcess = CurrentTestProcess.free;
        continuityTestProcess = new ContinuityTestProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        currentMeasureProcess = new CurrentMeasureProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        temperatureProcess = new TemperatureProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        flashProcess = new FlashProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        powerPinsTestProcess = new PowerPinsTestProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        gpioInputTestProcess = new GpioInputTestProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        gpioOutputTestProcess = new GpioOutputProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        analogInputTestProcess = new AnalogInputTestProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
        gpioShorCircuitProcess = new GpioShorCircuitProcess(this, boardID, boardName, this, boardTesterDataBaseHandler, newBoardTestViewsHelper);
    }

    private void getAndPostLastTestsResults(){
        boardTesterDataBaseHandler.getBoardsGpioContinuityTestResult(boardID);
        boardTesterDataBaseHandler.getCurrentConsumption(boardID);
        boardTesterDataBaseHandler.getBoardsPowersPinsVoltage(boardID);
        boardTesterDataBaseHandler.getBoardsFlash(boardID);
        boardTesterDataBaseHandler.getBoardsTemperature(boardID);
        boardTesterDataBaseHandler.getBoardsGpioInputNoPullTestResult(boardID);
        boardTesterDataBaseHandler.getBoardsGpioInputPullDownTestResult(boardID);
        boardTesterDataBaseHandler.getBoardsGpioInputPullUpTestResult(boardID);
        boardTesterDataBaseHandler.getBoardsGpioOutputNoPullTestResult(boardID);
        boardTesterDataBaseHandler.getBoardsGpioOutputPullDownTestResult(boardID);
        boardTesterDataBaseHandler.getBoardsGpioOutputPullUpTestResult(boardID);
        boardTesterDataBaseHandler.getGpioShortCircuitTestResult(boardID);
        boardTesterDataBaseHandler.getAnalogInputTestResult(boardID);

    }

    private void initialCompleteTest(){
        newBoardTestViewsHelper.getRelativeCompleteTest().setOnClickListener(view -> {
            if(!newBoardTestHelper.isCompleteTest() && getCurrentProcess() == CurrentTestProcess.free){
                newBoardTestViewsHelper.getArrowCompleteTest().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
                newBoardTestHelper.setCompleteTest(true);
                continuityTestProcess.startContinuityTestProcess(true);
                newBoardTestViewsHelper.setCompleteInitialViews();
                int color = ContextCompat.getColor(this, R.color.textColor);
                newBoardTestViewsHelper.getCompleteTestText().setTextColor(color);
                newBoardTestViewsHelper.getCompleteTestText().setText(getString(R.string.wait_message));
            }
        });
    }

    protected void checkCompleteTestisDone(){
        if(currentTestValue == BoardCompleteTestedValue && !newBoardTestHelper.isCompleteTest()){
            newBoardTestViewsHelper.getArrowCompleteTest().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }else{
            newBoardTestViewsHelper.getArrowCompleteTest().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
        }
    }

    protected void checkIsCompleteTest(){
        if(newBoardTestHelper.isCompleteTest()) {
            if (newBoardTestHelper.isAllTestProcessesDone()) {
                checkCompleteTestisDone();
                newBoardTestHelper.setCompleteTestDone();
            }
            if ((newBoardTestHelper.isGpioContinuityProcessDone()) && (!newBoardTestHelper.isPowerPinsProcessVoltageDone())) {
                powerPinsTestProcess.startPowerPinsTestProcess(true);
            }
            if (newBoardTestHelper.isPowerPinsProcessVoltageDone() && !newBoardTestHelper.isFlashProcessDone()) {
                flashProcess.startFlashProcess(true);
            }
            if (newBoardTestHelper.isFlashProcessDone() && !newBoardTestHelper.isCurrentMeasureProcessDone()) {
                currentMeasureProcess.startCurrentMeasureProcess(true);
            }
            if ((newBoardTestHelper.isCurrentMeasureProcessDone()) && (!newBoardTestHelper.isTemperatureProcessDone())) {
                temperatureProcess.startTemperatureMeasureProcess(true);
            }
            if (newBoardTestHelper.isTemperatureProcessDone() && !(newBoardTestHelper.isGpioInputProcessNoPullDone() &&
                    newBoardTestHelper.isGpioInputProcessPullDownDone() && newBoardTestHelper.isGpioInputProcessPullUpDone())) {
                gpioInputTestProcess.startGpioInputProcess(true);
            }
            if ((newBoardTestHelper.isGpioInputProcessNoPullDone() && newBoardTestHelper.isGpioInputProcessPullDownDone() && newBoardTestHelper.isGpioInputProcessPullUpDone())
                    && !(newBoardTestHelper.isGpioOutputNoPullProcessDone() && newBoardTestHelper.isGpioOutputPullDOwnProcessDone() && newBoardTestHelper.isGpioOutputPullUpProcessDone())) {
                gpioOutputTestProcess.startGpioOutputProcess(true);
            }
            if ((newBoardTestHelper.isGpioOutputPullUpProcessDone() && newBoardTestHelper.isGpioOutputNoPullProcessDone() && newBoardTestHelper.isGpioOutputPullDOwnProcessDone())
                    && !(newBoardTestHelper.isGpioShortCircuitProcessDone())) {
                gpioShorCircuitProcess.startGpioShortCircuitProcess(true);
            }
            if (newBoardTestHelper.isGpioShortCircuitProcessDone() && (!newBoardTestHelper.isAnalogInputProcessDone())) {
                analogInputTestProcess.startAnalogInputProcess(true);
            }
            if (newBoardTestHelper.isAnalogInputProcessDone()) {
//                newBoardTestHelper.setCompleteTest(false);
                newBoardTestHelper.setCompleteTestDone();
            }
        }
    }

    @Override
    public void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState) {
        Log.d("LOGGERME", "onSuccessLoadedAppCommands");
        if(!appControllerState.isEmpty()){
            if(appControllerState.get(boardID) != null){
                checkCurrentProcess(appControllerState.get(boardID));
                if(appControllerState.get(boardID) != null){
                    updateTestValueAllProcesses(Objects.requireNonNull(appControllerState.get(boardID)).getTest_value());
                    int errors = Objects.requireNonNull(appControllerState.get(boardID)).getErrors();
                    Log.d("LOGGERME", "waitCounterCompleteTest: " + waitCounterCompleteTest);
                    if((Objects.requireNonNull(appControllerState.get(boardID)).getTest_value() == BoardCompleteTestedValue)){
                        if(errors > 0){
                            Log.d("LOGGERME", "errors > 0");
                            newBoardTestViewsHelper.getCompleteTestText().setTextColor(Color.RED);
                            String errorsText = Integer.toString(errors) + " Errors";
                            newBoardTestViewsHelper.getCompleteTestText().setText(errorsText);
                            newBoardTestViewsHelper.getArrowCompleteTest().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
                        }else{
                            Log.d("LOGGERME", "errors <= 0");

                            newBoardTestViewsHelper.getArrowCompleteTest().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
                            int color = ContextCompat.getColor(this, R.color.textColor);
                            newBoardTestViewsHelper.getCompleteTestText().setTextColor(color);
                            String errorsText = "Microcontroller OK";
                            newBoardTestViewsHelper.getCompleteTestText().setText(errorsText);
                        }
                    }else{
                        newBoardTestViewsHelper.getCompleteTestText().setText("");
                        waitCounterCompleteTest++;
                        if(waitCounterCompleteTest > 5){
                            waitCounterCompleteTest = 5;
                        }
                    }
                }
            }
        }
    }

    @Override
    public void onTryToSetGpioContinuityCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isContinuityCommandSet = true;
        }else{
            isContinuityCommandSet = false;
            continuityTestProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetPowerGndPinsCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isPowerPinsCommandSet = true;
        }else{
            isPowerPinsCommandSet = false;
            powerPinsTestProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetTemperatureCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isTemperatureCommandSet = true;
        }else{
            isTemperatureCommandSet = false;
            temperatureProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetMeasureCurrentCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isMeasureCurrentCommandSet = true;
        }else{
            isMeasureCurrentCommandSet = false;
            currentMeasureProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetFlashCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isFlashCommandSet = true;
        }else{
            isFlashCommandSet = false;
            flashProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetGpioInputCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isGpioInputTestCommandSet = true;
        }else{
            isGpioInputTestCommandSet = false;
            gpioInputTestProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetGpioOutputCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isOutputCommandSet = true;
        }else{
            isOutputCommandSet = false;
            gpioOutputTestProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetAnalogInputCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isAnalogInputCommandSet = true;
        }else{
            isAnalogInputCommandSet = false;
            analogInputTestProcess.setSendTestCommandFailed();
        }
    }

    @Override
    public void onTryToSetGpioShortCircuitCommand(boolean isSet) {
        startCommandHandler();
        if(isSet){
            isGpioShortCommandSet = true;
        }else{
            isGpioShortCommandSet = false;
            gpioShorCircuitProcess.setSendTestCommandFailed();
        }
    }
    public void checkCurrentProcess(BoardTestCommands boardTestController){
        if(isContinuityCommandSet && (currentProcess == CurrentTestProcess.ContinuityTestProcess) && (boardTestController.getGpio_continuity() == 0)){
            boardTesterDataBaseHandler.getBoardsGpioContinuityTestResult(boardID);
            isContinuityCommandSet = false;
        } if(isMeasureCurrentCommandSet && (currentProcess == CurrentTestProcess.CurrentMeasureProcess) && (boardTestController.getMeasure_current() == 0)){
            boardTesterDataBaseHandler.getCurrentConsumption(boardID);
            isMeasureCurrentCommandSet = false;
        }else if(isPowerPinsCommandSet && (currentProcess == CurrentTestProcess.PowerPinsProcess) && (boardTestController.getPower_gnd_test() == 0)){
            boardTesterDataBaseHandler.getBoardsPowersPinsVoltage(boardID);
            isPowerPinsCommandSet = false;
        }else if(isTemperatureCommandSet && (currentProcess == CurrentTestProcess.TemperatureProcess) && (boardTestController.getMeasure_temperature() == 0)){
            boardTesterDataBaseHandler.getBoardsTemperature(boardID);
            isTemperatureCommandSet = false;
        }else if(isFlashCommandSet && (currentProcess == CurrentTestProcess.FlashProcess) && (boardTestController.getFlash_board() == 0)){
            boardTesterDataBaseHandler.getBoardsFlash(boardID);
            isFlashCommandSet = false;
        }else if(isGpioInputTestCommandSet && (currentProcess == CurrentTestProcess.GpioInputTest) && (boardTestController.getGpio_input_test() == 0)){
            boardTesterDataBaseHandler.getBoardsGpioInputNoPullTestResult(boardID);
            boardTesterDataBaseHandler.getBoardsGpioInputPullDownTestResult(boardID);
            boardTesterDataBaseHandler.getBoardsGpioInputPullUpTestResult(boardID);
            isGpioInputTestCommandSet = false;
        }else if(isOutputCommandSet && (currentProcess == CurrentTestProcess.GpioOutputTest) && (boardTestController.getGpio_output_test() == 0)){
            boardTesterDataBaseHandler.getBoardsGpioOutputNoPullTestResult(boardID);
            boardTesterDataBaseHandler.getBoardsGpioOutputPullDownTestResult(boardID);
            boardTesterDataBaseHandler.getBoardsGpioOutputPullUpTestResult(boardID);
            isOutputCommandSet = false;
        }else if(isAnalogInputCommandSet && (currentProcess == CurrentTestProcess.AnalogInputTest) && (boardTestController.getAnalog_input_test() == 0)){
            boardTesterDataBaseHandler.getAnalogInputTestResult(boardID);
            isAnalogInputCommandSet = false;
        }else if(isGpioShortCommandSet && (currentProcess == CurrentTestProcess.GpioShortCircuitTest) && (boardTestController.getGpio_short_circuit_test() == 0)){
            boardTesterDataBaseHandler.getGpioShortCircuitTestResult(boardID);
            isGpioShortCommandSet = false;
        }
    }

    protected void updateTestValueAllProcesses(int testValue){
        currentTestValue = testValue;
        currentMeasureProcess.updateTestValue(testValue);
        temperatureProcess.updateTestValue(testValue);
        flashProcess.updateTestValue(testValue);
        powerPinsTestProcess.updateTestValue(testValue);
        continuityTestProcess.updateTestValue(testValue);
        gpioInputTestProcess.updateTestValue(testValue);
        gpioOutputTestProcess.updateTestValue(testValue);
        analogInputTestProcess.updateTestValue(testValue);
        gpioShorCircuitProcess.updateTestValue(testValue);
        checkCompleteTestisDone();
    }

    // POWER PINS
    @Override
    public void onPowerPinsVoltageDataLoaded(ArrayList<PowerPinVoltage> powerPinsVoltageResult) {
        powerPinsTestProcess.setPowerPinsVoltage(powerPinsVoltageResult);
        if(newBoardTestHelper.isCompleteTest()){
            newBoardTestHelper.setPowerPinsProcessVoltageDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedPowerPinsVoltageDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            powerPinsTestProcess.onTestPowerPinsVoltageFailedResponse(exception);
        }
        if(newBoardTestHelper.isCompleteTest()){
            newBoardTestHelper.setPowerPinsProcessVoltageDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioContinuityDataLoaded(ArrayList<GpioContinuity> continuityArrayList) {
        continuityTestProcess.setGpioContinuityTestResult(continuityArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioContinuityProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioContinuityDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            continuityTestProcess.onFailedGpioContinuityTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioContinuityProcessDone(true);
            checkIsCompleteTest();
        }
    }


    //FLASH Process
    @Override
    public void onFlashDataLoaded(BoardFlash boardFlash) {
        flashProcess.setFlashData(boardFlash);
        if(newBoardTestHelper.isCompleteTest()){
            newBoardTestHelper.setFlashProcessDone(true);
            checkIsCompleteTest();
        }

    }

    @Override
    public void onFailedFlashDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            flashProcess.onFlashFailedResponse(exception);
        }
        if(newBoardTestHelper.isCompleteTest()){
            newBoardTestHelper.setFlashProcessDone(true);
            checkIsCompleteTest();
        }
    }

    //Temperature Process
    @Override
    public void onTemperatureDataLoaded(BoardTemperature boardTemperature) {
        temperatureProcess.setTemperatureData(boardTemperature);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setTemperatureProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedTemperatureDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)){
            temperatureProcess.onReadTemperatureFailedResponse(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setTemperatureProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onCurrentConsumptionDataLoaded(BoardCurrentConsumption boardCurrentConsumption) {
        currentMeasureProcess.setCurrentMeasureData(boardCurrentConsumption);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setCurrentMeasureProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedCurrentConsumptionDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)){
            currentMeasureProcess.onReadCurrentFailedResponse(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setCurrentMeasureProcessDone(true);
            checkIsCompleteTest();
        }
    }

    // GPIO INPUT
    @Override
    public void onGpioInputNoPullDataLoaded(ArrayList<GpioInput> gpioInputNoPullList) {
        gpioInputTestProcess.setGpioInputNoPullTestResult(gpioInputNoPullList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessNoPullDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioInputNoPullDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioInputTestProcess.onFailedGpioInputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessNoPullDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioInputPullDownDataLoaded(ArrayList<GpioInput> gpioInputPullDownArrayList) {
        gpioInputTestProcess.setGpioInputPullDownTestResult(gpioInputPullDownArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessPullDownDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioInputPullDownDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioInputTestProcess.onFailedGpioInputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessPullDownDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioInputPullUpDataLoaded(ArrayList<GpioInput> gpioInputPullUpArrayList) {
        gpioInputTestProcess.setGpioInputPullUpTestResult(gpioInputPullUpArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessPullUpDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioInputPullUpDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioInputTestProcess.onFailedGpioInputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioInputProcessPullUpDone(true);
            checkIsCompleteTest();
        }
    }

    //GPIO OUTPUT
    @Override
    public void onGpioOutputNoPullDataLoaded(ArrayList<GpioOutput> gpioOutputNoPullArrayList) {
        gpioOutputTestProcess.setGpioOutputNoPullTestResult(gpioOutputNoPullArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioOutputNoPullProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioOutputNoPullDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioOutputTestProcess.onFailedGpioOutputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioOutputNoPullProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioOutputPullDownDataLoaded(ArrayList<GpioOutput> gpioOutputPullDownArrayList) {
        gpioOutputTestProcess.setGpioOutputPullDownTestResult(gpioOutputPullDownArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioOutputPullDOwnProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioOutputPullDownDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioOutputTestProcess.onFailedGpioOutputTest(exception);
        }
        if(newBoardTestHelper.isCompleteTest()){
            newBoardTestHelper.setGpioOutputPullDOwnProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioOutputPullUpDataLoaded(ArrayList<GpioOutput> gpioOutputPullUpArrayList) {
        gpioOutputTestProcess.setGpioOutputPullUpTestResult(gpioOutputPullUpArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioOutputPullUpProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioOutputPullUpDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioOutputTestProcess.onFailedGpioOutputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioOutputPullUpProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onAnalogInputDataLoaded(ArrayList<AnalogInput> analogInputArrayList) {
        analogInputTestProcess.setAnalogInputTestResult(analogInputArrayList);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setAnalogInputProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedAnalogInputDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            analogInputTestProcess.onFailedAnalogInputTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setAnalogInputProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onGpioShortCircuitDataLoaded(ArrayList<GpioShortCircuit> gpioShortCircuitArray) {
        gpioShorCircuitProcess.setGpioShortCircuitTestResult(gpioShortCircuitArray);
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioShortCircuitProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void onFailedGpioShortCircuitDataLoad(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData) && !exception.toString().contains(ExceptionValueNull)) {
            gpioShorCircuitProcess.onFailedGpioShortCircuitTest(exception);
        }
        if (newBoardTestHelper.isCompleteTest()) {
            newBoardTestHelper.setGpioShortCircuitProcessDone(true);
            checkIsCompleteTest();
        }
    }

    @Override
    public void setCurrentProcess(CurrentTestProcess currentProcess) {
        this.currentProcess = currentProcess;
    }

    public CurrentTestProcess getCurrentProcess(){
        return currentProcess;
    }

}