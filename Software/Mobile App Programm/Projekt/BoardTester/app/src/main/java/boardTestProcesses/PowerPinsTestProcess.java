package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardPowerGndPinsTestValuePosition;

import android.content.Context;
import android.content.Intent;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.boardtester.R;

import java.util.ArrayList;

import database.BoardTesterDataBaseHandler;
import helpers.BoardChecker;
import helpers.Constants;
import helpers.CurrentTestProcess;
import interfaces.CurrentProcessListener;
import models.BoardTestCommands;
import models.PowerPinVoltage;
import testResults.PowerGndPinsTestResultActivity;

class PowerPinsTestProcess extends ProgressBarHandler {

    final private Context context;
    final private int boardID;
    final private CurrentProcessListener processListener;
    final private BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    protected boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    protected final ArrayList<PowerPinVoltage> powerPinsVoltageData;
    final private BoardChecker boardChecker;
    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final int textColor;
    private String boardName;
    public PowerPinsTestProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        boardChecker = new BoardChecker();
        powerPinsVoltageData = new ArrayList<>();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    private void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutPowerPins().setOnClickListener(view -> startPowerPinsTestProcess(false));
    }

    protected void startPowerPinsTestProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.PowerPinsProcess);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardPowerGndPinsTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0,0, 1,0, 0,0,0,0,1, errorsValue,  testValue), CurrentTestProcess.PowerPinsProcess);
            }else{
                testValue =  (1 << BoardPowerGndPinsTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0,0, 1,0, 0,0,0,0,0, errorsValue,  testValue), CurrentTestProcess.PowerPinsProcess);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getPowerPinsProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getBoardConnectionRelative().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGndConnectionRelative().setVisibility(View.GONE);
        newBoardTestViewsHelper.getPowerPinsRelative().setVisibility(View.GONE);
        newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.GONE);
        newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getPowerPinsTextView().setText(R.string.wait_message);
        newBoardTestViewsHelper.getPowerPinsTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
        newBoardTestViewsHelper.getPowerPinsProgressBar().setVisibility(View.VISIBLE);
    }

    protected void setPowerPinsVoltage(ArrayList<PowerPinVoltage> powerPinsVoltageResult){
        powerPinsVoltageData.clear();
        this.powerPinsVoltageData.addAll(powerPinsVoltageResult);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity)context).getCurrentProcess());
        newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getPowerPinsProgressBar().setVisibility(View.GONE);
        int errors = boardChecker.getPowerGndPinsErrors(powerPinsVoltageResult);
        if(errors > 0){
            newBoardTestViewsHelper.getPowerPinsTextView().setText("");
            newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }else{
            newBoardTestViewsHelper.getPowerPinsTextView().setText(context.getString(R.string.ok));
            newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }
        setPowerInformationListener();
        isProcessStart = false;
    }

    protected void setPowerInformationListener(){
        newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getPowerPinsInformation().setOnClickListener(v ->{
            Intent intent = new Intent(context, PowerGndPinsTestResultActivity.class);
            intent.putExtra(Constants.powerPinsVoltageKey, powerPinsVoltageData);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getPowerPinsTextView().setText(failedMessage);
    }

    protected void onTestPowerPinsVoltageFailedResponse(Exception exception){
        String failedMessage = "Test Failed cause: ";
        if(exception!= null){
            failedMessage +=  exception.getMessage();
        }
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getPowerPinsTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getPowerPinsProgressBar().setVisibility(View.GONE);
        setBoardConnected(-2);
        isProcessStart = false;
    }


    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }

    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }

    protected void checkBoardConnection(ArrayList<PowerPinVoltage> powerPinsVoltageData){
        if(!powerPinsVoltageData.isEmpty()){
            int boardConnectionErrors = boardChecker.boardConnectionTest(powerPinsVoltageData);
            if(boardConnectionErrors >= 15){
                setBoardConnected(0);
                newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
                newBoardTestViewsHelper.getPowerPinsTextView().setText(R.string.test_failed);
                newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
                newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.VISIBLE);
            }else if(boardConnectionErrors == 0){
                setBoardConnected(1);
                newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
                newBoardTestViewsHelper.getPowerPinsTextView().setText(R.string.tested);
                newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
                newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.VISIBLE);
            }else{
                setBoardConnected(-1);
                newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
                newBoardTestViewsHelper.getPowerPinsTextView().setText(R.string.tested);
                newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
                newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.VISIBLE);
            }
        }else{
            setBoardConnected(-2);
            newBoardTestViewsHelper.getPowerPinsTextView().setVisibility(View.VISIBLE);
            newBoardTestViewsHelper.getPowerPinsTextView().setText(R.string.test_failed);
            newBoardTestViewsHelper.getArrowPower().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
            newBoardTestViewsHelper.getPowerPinsInformation().setVisibility(View.VISIBLE);
        }
    }

    public void setBoardConnected(int connectedValue){
        newBoardTestViewsHelper.getBoardConnectionRelative().setVisibility(View.VISIBLE);
        if(connectedValue == 1){
            newBoardTestViewsHelper.getBoardConnectionImage().setImageDrawable(newBoardTestViewsHelper.getOkImage());
            newBoardTestViewsHelper.getBoardConnectionText().setText(R.string.board_connected);
        }else if(connectedValue == -1){
            newBoardTestViewsHelper.getBoardConnectionImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getBoardConnectionText().setText(R.string.board_not_correctly_connected_errors);
        }else if(connectedValue == 0){
            newBoardTestViewsHelper.getBoardConnectionImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getBoardConnectionText().setText(R.string.no_board_connected);
        }else{
            newBoardTestViewsHelper.getBoardConnectionImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getBoardConnectionText().setText(R.string.test_failed);
            newBoardTestViewsHelper.getBoardConnectionText().setText(R.string.no_test_result);
        }
    }

    protected void checkGndOk(ArrayList<PowerPinVoltage> powerPinsVoltageData){
        int errors = 0;
        if(!powerPinsVoltageData.isEmpty()){
            for(int i=0; i< powerPinsVoltageData.size(); i++){
                PowerPinVoltage powerPinVoltage = powerPinsVoltageData.get(i);
                if(boardChecker.isGndPin(powerPinVoltage.getPin())){
                    if(!powerPinVoltage.getComment().equals("OK")){
                        errors ++;
                    }
                }
            }
            if(errors == 0){
                setGndTestResult(1);
            }else{
                setGndTestResult(0);
            }
        }else{
            setGndTestResult(-1);
        }
    }

    public void setGndTestResult(int gndConnectionValue){
        newBoardTestViewsHelper.getGndConnectionRelative().setVisibility(View.VISIBLE);
        if(gndConnectionValue == 1){
            newBoardTestViewsHelper.getGndConnectionImage().setImageDrawable(newBoardTestViewsHelper.getOkImage());
            newBoardTestViewsHelper.getGndConnectionTextView().setText(R.string.gnd_connection_ok);
        }else if(gndConnectionValue == 0) {
            newBoardTestViewsHelper.getGndConnectionImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getGndConnectionTextView().setText(R.string.gnd_connection_error);
        }else{
            newBoardTestViewsHelper.getGndConnectionImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getGndConnectionTextView().setText(R.string.test_failed);
        }
    }

    protected void checkPowerPins(ArrayList<PowerPinVoltage> powerPinsVoltageData){
        int errors = 0;
        if(!powerPinsVoltageData.isEmpty()){
            for(int i=0; i< powerPinsVoltageData.size(); i++){
                if(boardChecker.is3VoltageTypePowerPin(powerPinsVoltageData.get(i).getPin())){
                    if(!powerPinsVoltageData.get(i).getComment().equals("OK")){
                        errors++;
                    }
                }
                if(boardChecker.is5VoltageTypePowerPin(powerPinsVoltageData.get(i).getPin())){
                    if(!powerPinsVoltageData.get(i).getComment().equals("OK")){
                        errors++;
                    }
                }
            }
            if(errors == 0){
                setPowerPinsTestResult(1);
            }else{
                setPowerPinsTestResult(0);
            }
        }else{
            setPowerPinsTestResult(-1);
        }
    }

    public void setPowerPinsTestResult(int powerPinsResultValue){
        newBoardTestViewsHelper.getPowerPinsRelative().setVisibility(View.VISIBLE);
        if(powerPinsResultValue == 1){
            newBoardTestViewsHelper.getPowerPinsImage().setImageDrawable(newBoardTestViewsHelper.getOkImage());
            newBoardTestViewsHelper.getPowerPinsTestResult().setText(R.string.power_pins_ok);
        }else if(powerPinsResultValue == 0){
            newBoardTestViewsHelper.getPowerPinsImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getPowerPinsTestResult().setText(R.string.power_pins_error);
        }else{
            newBoardTestViewsHelper.getPowerPinsImage().setImageDrawable(newBoardTestViewsHelper.getErrorImage());
            newBoardTestViewsHelper.getPowerPinsTestResult().setText(R.string.test_failed);
        }
    }
}