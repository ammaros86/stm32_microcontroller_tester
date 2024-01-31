package main;

import static helpers.Constants.*;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.boardtester.R;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.ArrayList;

import base.BaseActivity;
import helpers.CurrentControlCommand;
import log.ListLogFiles;
import models.BoardController;

public class MainActivity extends BaseActivity {

    private BottomNavigationView bottomNavigationView;
    private TextView connectionStateTextView, logFilesCounterTextView;
    private ImageView connectionStateImageView;
    private int currentProgressInt;
    private Runnable runnableProgressBar;
    private Handler handler;
    private boolean isUsbPortOn;
    private TextView copyLogsStateTextView, usbPortStateTextView, boardOffStateTextView;
    private ProgressBar progressBar, copyLogsProgressBar, usbOnProgressBar, boardOffProgressBar;
    private BoardController boardControllerState;
    private RelativeLayout relativeLayoutLog, copyLogsRelativeLayout, usbPortRelativeLayout, boardOffRelative;
    private CurrentControlCommand currentControllCommand;
    private boolean isControllCommandTurnUsbSet = false;
    private boolean isControllCommandTurnBoardOffSet = false;
    private boolean isControllCommandCopyLogFiles = false;
    private boolean isConnected = false;
    private boolean isCopyLogsProcess = false;
    private ArrayList<String> logFilesArrayList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        isUsbPortOn = false;
    }

    @Override
    protected void onResume() {
        super.onResume();
        logFilesArrayList = new ArrayList<>();
        currentControllCommand = CurrentControlCommand.free;
        handler = new Handler(Looper.getMainLooper());
        initialViews();
        initialListener();
        initialMenuListener();
        if(bottomNavigationView != null){
            bottomNavigationView.setSelectedItemId(R.id.home);
        }
        copyLogsStateTextView.setText("");
        usbPortStateTextView.setText("");
        boardOffStateTextView.setText("");
        boardControllerState = new BoardController();
    }

    @Override
    protected void onStop() {
        super.onStop();
        isCopyLogsProcess = false;
    }

    private void initialViews(){
        connectionStateTextView = findViewById(R.id.connectionState);
        progressBar = findViewById(R.id.progressBarMain);
        connectionStateImageView = findViewById(R.id.connectionImage);
        bottomNavigationView=findViewById(R.id.bottomNavigationView);
        bottomNavigationView.setSelectedItemId(R.id.home);
        progressBar.setVisibility(View.VISIBLE);
        copyLogsRelativeLayout = findViewById(R.id.realtiveCopyLogs);
        usbPortRelativeLayout = findViewById(R.id.relativeUsbPort);
        boardOffRelative = findViewById(R.id.relativePowerOff);
        copyLogsStateTextView = findViewById(R.id.copyStateText);
        usbPortStateTextView = findViewById(R.id.usbPortStateText);
        boardOffStateTextView = findViewById(R.id.powerOffState);
        copyLogsProgressBar = findViewById(R.id.progressBarCopy);
        usbOnProgressBar = findViewById(R.id.progressBarUsb);
        boardOffProgressBar = findViewById(R.id.progressBarPowerOff);
        logFilesCounterTextView = findViewById(R.id.logFilesCounter);
        relativeLayoutLog = findViewById(R.id.relativeLogFiles);
        connectionStateImageView.setVisibility(View.GONE);
        connectionStateTextView.setVisibility(View.GONE);
    }

    private void initialListener(){
        copyLogsRelativeLayout.setOnClickListener(view ->{
            if(isConnected){
                isCopyLogsProcess = true;
                currentControllCommand = CurrentControlCommand.copyLogFiles;
                startCopyLogsProcess();
            }else{
                Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
            }
        });

        usbPortRelativeLayout.setOnClickListener(view -> {
            if(isConnected){
                currentControllCommand = CurrentControlCommand.turnUsbPortOn;
                if(!isUsbPortOn){
                    usbPortStateTextView.setText("");
                    startUsbOnProcess();
                }else{
                    Toast.makeText(this, "USB-Port current state: ON", Toast.LENGTH_SHORT).show();
                }
            }else{
                Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
            }
        });

        boardOffRelative.setOnClickListener(view ->{
            if(isConnected){
                currentControllCommand = CurrentControlCommand.turnBoardOff;
                startPowerOffProcess();
            }else{
                Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
            }
        } );

        relativeLayoutLog.setOnClickListener(view -> {
            if(isConnected) {
                Intent intent = new Intent(this, ListLogFiles.class);
                intent.putStringArrayListExtra("logArrayList", logFilesArrayList);
//                overridePendingTransition(0, 0);
                startActivity(intent);
            }else{
                Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
            }
        });
    }

    void startCopyLogsProcess(){
        startProgressBarHandler(copyLogsProgressBar);
        stopControllerHandler();
        copyLogsProgressBar.setVisibility(View.VISIBLE);
        copyLogsStateTextView.setText(WAIT_TEXT);
        boardControllerState.setCopy_logs_usb(1);
        boardTesterDataBaseHandler.updateBoardController(boardControllerState, CurrentControlCommand.copyLogFiles);
    }

    void startUsbOnProcess(){
        startProgressBarHandler(usbOnProgressBar);
        stopControllerHandler();
        usbOnProgressBar.setVisibility(View.VISIBLE);
        usbPortStateTextView.setText(WAIT_TEXT);
        boardControllerState.setTurn_usb_port_on(1);
        boardTesterDataBaseHandler.updateBoardController(boardControllerState, CurrentControlCommand.turnUsbPortOn);
    }

    void startPowerOffProcess(){
        startProgressBarHandler(boardOffProgressBar);
        stopControllerHandler();
        boardOffProgressBar.setVisibility(View.VISIBLE);
        boardOffStateTextView.setText(WAIT_TEXT);
        boardControllerState.setPower_off(1);
        boardTesterDataBaseHandler.updateBoardController(boardControllerState, CurrentControlCommand.turnBoardOff);
    }

    @Override
    public void onBoardControllerLoaded(BoardController boardController) {
        Log.d("DEBUGGER", "Main: onBoardControllerLoaded: " +boardController.toString());
        boardControllerState = boardController;
        checkConnection(true);
        progressBar.setVisibility(View.GONE);
        if(currentControllCommand == CurrentControlCommand.copyLogFiles && isControllCommandCopyLogFiles && isCopyLogFilesDone(boardController)){
            finishCopyLogsProcess(boardController, false);
            stopProgressBarHandler(currentControllCommand);
        }else if(currentControllCommand == CurrentControlCommand.turnUsbPortOn && isControllCommandTurnUsbSet && isTurnUsbPortDone(boardController)){
            finishUsbPortOnProcess(boardController, false);
            stopProgressBarHandler(currentControllCommand);
        }else if(currentControllCommand == CurrentControlCommand.turnBoardOff &&  isControllCommandTurnBoardOffSet && isTurnBoardOffDone(boardController)){
            finishPowerOffProcess(boardController, false);
            stopProgressBarHandler(currentControllCommand);
        }else{
            if(currentControllCommand == CurrentControlCommand.free && isConnected){
                setUsbOnResult(boardController);
                setPowerOffResult(boardController);
                setCopyLogResult(boardController);
                currentControllCommand = CurrentControlCommand.free;
                copyLogsProgressBar.setVisibility(View.GONE);
                usbOnProgressBar.setVisibility(View.GONE);
                boardOffProgressBar.setVisibility(View.GONE);
                boardTesterDataBaseHandler.getLogFilesList();
            }
        }
    }

    boolean isTurnUsbPortDone(BoardController boardController){
        return boardController.getTurn_usb_port_on() != 1;
    }

    boolean isTurnBoardOffDone(BoardController boardController){
        return boardController.getPower_off() != 1 && (boardController.getPower_state()== 0);
    }

    boolean isCopyLogFilesDone(BoardController boardController){
        return boardController.getCopy_logs_usb() != 1;
    }

    void finishCopyLogsProcess(BoardController boardController, boolean isFailed){
        currentControllCommand = CurrentControlCommand.free;
        copyLogsProgressBar.setVisibility(View.GONE);
        if(isFailed){
            copyLogsStateTextView.setText(COMMUNICATION_FAILURE);
        }else{
            setCopyLogResult(boardController);
        }
    }

    void setCopyLogResult(BoardController boardController){
        if(isCopyLogsProcess){
            if(boardController.getUsb_media_state() == 1){
                copyLogsStateTextView.setText(EXPORT_SUCCESS);
            }else if(boardController.getUsb_media_state() == -1){
                copyLogsStateTextView.setText(USB_MEDIA_ERROR);
            }else if(boardController.getUsb_media_state() == -2){
                String failedCopy = "Copy Log Files Failed";
                copyLogsStateTextView.setText(failedCopy);
            }
        }else{
            copyLogsStateTextView.setText("");
        }
    }

    void finishUsbPortOnProcess(BoardController boardController, boolean isFailed){
        currentControllCommand = CurrentControlCommand.free;
        usbOnProgressBar.setVisibility(View.GONE);
        if(isFailed){
            usbPortStateTextView.setText(COMMUNICATION_FAILURE);
        }else{
            setUsbOnResult(boardController);
        }
    }

    void setUsbOnResult(BoardController boardController){
        if(boardController.getUsb_port_state() == 1){
            isUsbPortOn = true;
            usbPortStateTextView.setText(USB_PORT_STATE_ON);
        }else if(boardController.getUsb_port_state() == 0){
            isUsbPortOn = false;
            usbPortStateTextView.setText(USB_PORT_STATE_OFF);
        }else{
            usbPortStateTextView.setText(FAILED_USB_PORT);
        }
    }

    void finishPowerOffProcess(BoardController boardController, boolean isFailed){
        currentControllCommand = CurrentControlCommand.free;
        boardOffProgressBar.setVisibility(View.GONE);
        if(isFailed){
            boardOffStateTextView.setText(COMMUNICATION_FAILURE);
        }else{
            setPowerOffResult(boardController);
        }
    }

    void setPowerOffResult(BoardController boardController){
        if(boardController.getPower_state() == 1){
            boardOffStateTextView.setText(POWER_STATE_ON);
        }else if(boardController.getPower_state() == 0){
            boardOffStateTextView.setText(POWER_STATE_OFF);
        }else if(boardController.getPower_state() == -1) {
            String powerOffFailed = "Failed to Turn OFF";
            boardOffStateTextView.setText(powerOffFailed);
        }else{
            boardOffStateTextView.setText("");
        }
    }

    private void checkConnection(boolean connectionState){
        Log.d("MMMMY", "checkConnection :. " +connectionState);
        connectionStateImageView.setVisibility(View.VISIBLE);
        connectionStateTextView.setVisibility(View.VISIBLE);
        if(connectionState && (boardControllerState.getPower_state() == 1)){
            isConnected = true;
            connectionStateImageView.setImageDrawable(connectedImage);
            connectionStateTextView.setText(R.string.connected);
        }else {
            isConnected = false;
            connectionStateImageView.setImageDrawable(errorConnectionImage);
            connectionStateTextView.setText(R.string.not_connected);
            usbPortStateTextView.setText("");
            copyLogsStateTextView.setText("");
            usbPortStateTextView.setText("");
            boardOffStateTextView.setText("");
            logFilesCounterTextView.setText("");
        }
    }

    private void initialMenuListener(){
        bottomNavigationView.setOnItemSelectedListener(item -> {
            int id = item.getItemId();
            if(id == R.id.home){
                return true;
            }else if(id == R.id.newBoardMenu){
                if(isConnected){
                    startActivity(new Intent(getApplicationContext(), NewBoardActivity.class));
                    overridePendingTransition(0,0);
                    return true;
                }else{
                    Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
                    return false;
                }
            }else if(id == R.id.boardsList){
                if(isConnected){
                    startActivity(new Intent(getApplicationContext(), BoardListActivity.class));
                    overridePendingTransition(0,0);
                    return true;
                }else{
                    Toast.makeText(this, "NOT CONNECTED", Toast.LENGTH_SHORT).show();
                    return false;
                }
            }else if(id == R.id.newServer){
                startActivity(new Intent(getApplicationContext(), NewServerConnectionActivity.class));
                overridePendingTransition(0,0);
                return true;
            }
            return true;
        });
    }

    void startProgressBarHandler(ProgressBar progressBar) {
        int progressStep = 10;
        int progressStart = 0;
        int progressMaxValue = 100;
        int delay = 200;
        currentProgressInt = progressStart;
        progressBar.setMax(progressMaxValue);
        progressBar.setProgress(progressStart);
        handler.postDelayed(runnableProgressBar = () -> {
            currentProgressInt +=progressStep;
            progressBar.setProgress(currentProgressInt);
            if(currentProgressInt == progressMaxValue){
                currentProgressInt = progressStart;
            }
            handler.postDelayed(runnableProgressBar, delay);
        }, delay);
    }

    protected void stopProgressBarHandler(CurrentControlCommand controllerCommand){
        if(runnableProgressBar != null && (controllerCommand == CurrentControlCommand.free)) {
            handler.removeCallbacks(runnableProgressBar);
            runnableProgressBar = null;
        }
    }

    @Override
    public void onLogFilesLoaded(ArrayList<String> logFilesArray) {
        logFilesArrayList = logFilesArray;
        String logFileCounterText = "               "+ logFilesArrayList.size();
        logFilesCounterTextView.setText(logFileCounterText);
    }

    @Override
    public void onLogFilesLoadError(Exception exception) {
        logFilesCounterTextView.setText("");
    }


    @Override
    public void onFailedBoardController(Exception exception) {
        if(exception.toString().contains("com.android.volley.TimeoutError") || exception.toString().contains("Failed to connect")){
            checkConnection(false);
            progressBar.setVisibility(View.GONE);
        }
        if(currentControllCommand == CurrentControlCommand.copyLogFiles){
            finishCopyLogsProcess(null, true);
        }else if(currentControllCommand == CurrentControlCommand.turnUsbPortOn){
            finishUsbPortOnProcess(null, true);
        }else if(currentControllCommand == CurrentControlCommand.turnBoardOff){
            finishPowerOffProcess(null, true);
        }
    }

    @Override
    public void onTryToSetTurnUsbOnCommand(boolean isSet) {
        isControllCommandTurnUsbSet = isSet;
        startControllerHandler();
    }

    @Override
    public void onTryToSetTurnBoardOffCommand(boolean isSet) {
        isControllCommandTurnBoardOffSet = isSet;
        startControllerHandler();
    }

    @Override
    public void onTryToSetCopyLogFilesCommand(boolean isSet) {
        isControllCommandCopyLogFiles = isSet;
        startControllerHandler();
    }
}