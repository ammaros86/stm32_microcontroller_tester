package boardTestProcesses;

import static helpers.Constants.BoardTemperatureTestValuePosition;

import android.content.Context;
import android.graphics.Color;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.boardtester.R;

import database.BoardTesterDataBaseHandler;
import helpers.CurrentTestProcess;
import helpers.SharePreferenceHelper;
import interfaces.CurrentProcessListener;
import models.BoardTemperature;
import models.BoardTestCommands;

class TemperatureProcess extends ProgressBarHandler {
    private final Context context;
    private final int boardID;
    private final CurrentProcessListener processListener;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final int textColor;
    private String boardName;

    public TemperatureProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    private void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutTemperature().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
                startTemperatureMeasureProcess(false);
            }
        });
    }

    protected void startTemperatureMeasureProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.TemperatureProcess);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardTemperatureTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 1, 0,0,0,0,0, 0 ,0,0, 1,errorsValue,  testValue), CurrentTestProcess.TemperatureProcess);
            }else{
                testValue = (1 << BoardTemperatureTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 1, 0,0,0,0,0, 0 ,0,0, 0,errorsValue,  testValue), CurrentTestProcess.TemperatureProcess);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getTemperatureProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getTemperatureProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getTemperatureTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getTemperatureTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowTemperature().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setTemperatureData(BoardTemperature boardTemperature){
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getTemperatureProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        float temperature = boardTemperature.getTemperature();
        if(temperature == -1){
            String message = "Measurement failed";
            newBoardTestViewsHelper.getTemperatureTextView().setText(message);
        }else{
            String temperatureText = temperature + " °C";
            newBoardTestViewsHelper.getTemperatureTextView().setText(temperatureText);
        }
        if(temperature < SharePreferenceHelper.getMaxTemperature(context) && temperature > -1) {
            newBoardTestViewsHelper.getArrowTemperature().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
            newBoardTestViewsHelper.getTemperatureTextView().setTextColor(textColor);
        }else{
            newBoardTestViewsHelper.getArrowTemperature().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
            newBoardTestViewsHelper.getTemperatureTextView().setTextColor(Color.RED);
        }
        isProcessStart = false;
    }

    protected void onReadTemperatureFailedResponse(Exception exception){
        String failedMessage = "Measure failed cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getTemperatureTextView().setText(failedMessage);
        newBoardTestViewsHelper.getTemperatureTextView().setTextColor(Color.RED);
        newBoardTestViewsHelper.getArrowTemperature().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getTemperatureProgressBar().setVisibility(View.GONE);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity)context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command to Board Tester";
        newBoardTestViewsHelper.getTemperatureTextView().setText(failedMessage);
    }
    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
