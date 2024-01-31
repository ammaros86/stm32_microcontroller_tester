package boardTestProcesses;

import static helpers.Constants.BoardCurrentTestValuePosition;

import android.content.Context;
import android.graphics.Color;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.boardtester.R;

import database.BoardTesterDataBaseHandler;
import helpers.CurrentTestProcess;
import interfaces.CurrentProcessListener;
import models.BoardCurrentConsumption;
import models.BoardTestCommands;

public class CurrentMeasureProcess extends ProgressBarHandler{
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

    public CurrentMeasureProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    private void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutCurrent().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
                startCurrentMeasureProcess(false);
            }
        });
    }

    protected void startCurrentMeasureProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            newBoardTestViewsHelper.getCurrentMeasureInfo().setVisibility(View.GONE);
            processListener.setCurrentProcess(CurrentTestProcess.CurrentMeasureProcess);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardCurrentTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 1,0,0,0,0, 0 ,0,0, 1,errorsValue,  testValue), CurrentTestProcess.CurrentMeasureProcess);
            }else{
                testValue = (1 << BoardCurrentTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 1,0,0,0,0, 0 ,0,0, 0,errorsValue,  testValue), CurrentTestProcess.CurrentMeasureProcess);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getCurrentMeasureProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getCurrentMeasureProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getCurrentTextView().setText(R.string.wait_message);
        newBoardTestViewsHelper.getCurrentTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowCurrent().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setCurrentMeasureData(BoardCurrentConsumption boardCurrentConsumption){
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getCurrentMeasureProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        float measuredCurrent = boardCurrentConsumption.getCurrent();
        if(measuredCurrent < 1 ){
            newBoardTestViewsHelper.getCurrentMeasureInfo().setVisibility(View.VISIBLE);
            String measurementFailed = "Measurement failed";
            newBoardTestViewsHelper.getCurrentTextView().setText(R.string.fail);
            newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setText(measurementFailed);
            newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setTextColor(Color.RED);
            newBoardTestViewsHelper.getArrowCurrent().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }else if(measuredCurrent < 63 ) {
            String currentOk = "Current Consumption OK";
            String currentText = measuredCurrent + " mA";
            newBoardTestViewsHelper.getCurrentTextView().setText(currentText);
            newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setText(currentOk);
            newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setTextColor(textColor);
            newBoardTestViewsHelper.getCurrentTextView().setTextColor(textColor);
            newBoardTestViewsHelper.getArrowCurrent().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }else {
            String measurementFailed = "High Current Consumption detected";
            String currentText = measuredCurrent + " mA";
            newBoardTestViewsHelper.getCurrentMeasureInfo().setVisibility(View.VISIBLE);
            newBoardTestViewsHelper.getCurrentTextView().setText(currentText);
            newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setText(measurementFailed);
            newBoardTestViewsHelper.getCurrentTextView().setTextColor(Color.RED);
            newBoardTestViewsHelper.getArrowCurrent().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }
        isProcessStart = false;
    }

    protected void onReadCurrentFailedResponse(Exception exception){
        String failedMessage = "Failed to read Current board cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getCurrentTextView().setText(R.string.failed);
        newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowCurrent().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getCurrentMeasureProgressBar().setVisibility(View.GONE);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity)context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getCurrentTextView().setText(R.string.failed);
        newBoardTestViewsHelper.getCurrentMeasureInfoTextView().setText(failedMessage);
    }
    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }

    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
