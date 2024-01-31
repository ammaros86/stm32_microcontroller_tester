package base;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.content.res.AppCompatResources;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;

import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;
import com.example.boardtester.R;

import java.util.ArrayList;
import java.util.HashMap;

import database.AppControllerHandler;
import database.BoardTesterDataBaseHandler;
import database.CommandHandler;
import interfaces.*;
import models.*;

public class BaseActivity extends AppCompatActivity implements AppCommandsListener , TestProcessListener, TestCommandsSetListener, ControlCommandSetListener, LogFilesListener, BoardManagerListener {

    private CommandHandler appCommandHandler;
    private AppControllerHandler appControllerHandler;
    public Drawable connectedImage;
    public Drawable errorConnectionImage;
    public BoardTesterDataBaseHandler boardTesterDataBaseHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        appCommandHandler = new CommandHandler(this, this);
        appControllerHandler = new AppControllerHandler(this, this);
        connectedImage = AppCompatResources.getDrawable(this, R.drawable.ic_baseline_circle_24_green);
        errorConnectionImage = AppCompatResources.getDrawable(this, R.drawable.ic_baseline_circle_24_red);
        RequestQueue myRequestQueue = Volley.newRequestQueue(this);
        boardTesterDataBaseHandler = new BoardTesterDataBaseHandler(this, this, this, this, this, myRequestQueue);

    }

    @Override
    public void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState) {

    }

    @Override
    public void onErrorLoadAppCommands(Exception exception) {

    }

    public void restartAppCommandHandler() {
        appCommandHandler.restartAppCommandHandler();
        appControllerHandler.restartAppControllerHandler();
    }

    public void stopCommandHandler() {
        appCommandHandler.stopCommandHandler();
    }

    public void startCommandHandler() {
        appCommandHandler.startCommandHandler();
    }

    public void startControllerHandler() {
        Log.d("DEBUGGER", "BaseActivity: startControllerHandler");
        appControllerHandler.startAppControllerHandler();
    }

    public void stopControllerHandler() {
        Log.d("DEBUGGER", "BaseActivity: stopControllerHandler");
        appControllerHandler.stopAppControllerHandler();
    }

    @Override
    public void onBoardControllerLoaded(BoardController boardController) {

    }

    @Override
    public void onFailedBoardController(Exception exception) {

    }

    @Override
    public void onBoardAdded(boolean isAdded) {

    }

    @Override
    public void onAddBoardError(Exception exception) {

    }

    @Override
    public void onRemoveBoard(boolean isRemoved, int position, int boardID) {

    }

    @Override
    public void onRemoveBoardError(Exception exception, int boardID) {

    }

    @Override
    public void onTryToSetTurnUsbOnCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetTurnBoardOffCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetCopyLogFilesCommand(boolean isSet) {

    }

    @Override
    public void onLogFilesLoaded(ArrayList<String> logFilesArrayList) {

    }

    @Override
    public void onLogFilesLoadError(Exception exception) {

    }

    @Override
    public void onTryToSetTemperatureCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetPowerGndPinsCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetGpioContinuityCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetFlashCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetGpioInputCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetGpioOutputCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetAnalogInputCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetGpioShortCircuitCommand(boolean isSet) {

    }

    @Override
    public void onTryToSetMeasureCurrentCommand(boolean isSet) {

    }

    @Override
    public void onTemperatureDataLoaded(BoardTemperature boardsTemperature) {

    }

    @Override
    public void onFailedTemperatureDataLoad(Exception exception) {

    }

    @Override
    public void onCurrentConsumptionDataLoaded(BoardCurrentConsumption boardCurrentConsumption) {

    }

    @Override
    public void onFailedCurrentConsumptionDataLoad(Exception exception) {

    }

    @Override
    public void onFlashDataLoaded(BoardFlash boardFlash) {

    }

    @Override
    public void onFailedFlashDataLoad(Exception exception) {

    }

    @Override
    public void onPowerPinsVoltageDataLoaded(ArrayList<PowerPinVoltage> powerPinsVoltageResult) {

    }

    @Override
    public void onFailedPowerPinsVoltageDataLoad(Exception exception) {

    }

    @Override
    public void onGpioContinuityDataLoaded(ArrayList<GpioContinuity> continuityArrayList) {

    }

    @Override
    public void onFailedGpioContinuityDataLoad(Exception exception) {

    }

    @Override
    public void onGpioInputNoPullDataLoaded(ArrayList<GpioInput> gpioInputNoPullList) {

    }

    @Override
    public void onFailedGpioInputNoPullDataLoad(Exception exception) {

    }

    @Override
    public void onGpioInputPullDownDataLoaded(ArrayList<GpioInput> gpioInputPullDownArrayList) {

    }

    @Override
    public void onFailedGpioInputPullDownDataLoad(Exception exception) {

    }

    @Override
    public void onGpioInputPullUpDataLoaded(ArrayList<GpioInput> gpioInputPullUpArrayList) {

    }

    @Override
    public void onFailedGpioInputPullUpDataLoad(Exception exception) {

    }

    @Override
    public void onGpioOutputNoPullDataLoaded(ArrayList<GpioOutput> gpioOutputNoPullArrayList) {

    }

    @Override
    public void onFailedGpioOutputNoPullDataLoad(Exception exception) {

    }

    @Override
    public void onGpioOutputPullDownDataLoaded(ArrayList<GpioOutput> gpioOutputPullDownArrayList) {

    }

    @Override
    public void onFailedGpioOutputPullDownDataLoad(Exception exception) {

    }

    @Override
    public void onGpioOutputPullUpDataLoaded(ArrayList<GpioOutput> gpioOutputPullUpArrayList) {

    }

    @Override
    public void onFailedGpioOutputPullUpDataLoad(Exception exception) {

    }

    @Override
    public void onAnalogInputDataLoaded(ArrayList<AnalogInput> analogInputArrayList) {

    }

    @Override
    public void onFailedAnalogInputDataLoad(Exception exception) {

    }

    @Override
    public void onGpioShortCircuitDataLoaded(ArrayList<GpioShortCircuit> gpioShortCircuitArray) {

    }

    @Override
    public void onFailedGpioShortCircuitDataLoad(Exception exception) {

    }

    @Override
    public void onFailedUpdateBoardCommands(Exception exception) {

    }

    @Override
    public void onFailedUpdateBoardController(Exception exception) {

    }
}