package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardAnalogInputTestValuePosition;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.boardtester.R;

import java.util.ArrayList;

import database.BoardTesterDataBaseHandler;
import helpers.BoardChecker;
import helpers.Constants;
import helpers.CurrentTestProcess;
import interfaces.CurrentProcessListener;
import models.AnalogInput;
import models.BoardTestCommands;
import testResults.AnalogInputTestResultActivity;

public class AnalogInputTestProcess  extends ProgressBarHandler {
    private final Context context;
    private final CurrentProcessListener processListener;
    private final int boardID;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final BoardChecker boardChecker;
    private boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    protected final ArrayList<AnalogInput> analogInputArrayList;
    private final int textColor;
    private String boardName;

    public AnalogInputTestProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        analogInputArrayList = new ArrayList<>();
        boardChecker = new BoardChecker();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    public void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutAnalogInput().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free) {
                startAnalogInputProcess(false);
            }
        });
        newBoardTestViewsHelper.getAnalogInputInfo().setOnClickListener(v ->{
            Intent intent = new Intent(context, AnalogInputTestResultActivity.class);
            intent.putExtra(Constants.analogInputKey, analogInputArrayList);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startAnalogInputProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.AnalogInputTest);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardAnalogInputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0, 0, 0,0,0, 1,0,1,errorsValue,  testValue), CurrentTestProcess.AnalogInputTest);
            }else{
                testValue = (1 << BoardAnalogInputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0, 0, 0,0,0, 1,0,0,errorsValue,  testValue), CurrentTestProcess.AnalogInputTest);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getAnalogInputTestProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getAnalogInputTestProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getAnalogInputInfo().setVisibility(View.GONE);
        newBoardTestViewsHelper.getAnalogInputTestTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getAnalogInputTestTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowAnalogInput().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setAnalogInputTestResult(ArrayList<AnalogInput> analogInputArray){
        analogInputArrayList.clear();
        analogInputArrayList.addAll(analogInputArray);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity)context).getCurrentProcess());

        newBoardTestViewsHelper.getAnalogInputInfo().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getAnalogInputTestProgressBar().setVisibility(View.GONE);
        int errors = boardChecker.getAnalogInputErrors(analogInputArrayList);
        if(errors > 0){
            newBoardTestViewsHelper.getAnalogInputTestTextView().setText("");
            newBoardTestViewsHelper.getArrowAnalogInput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }else{
            newBoardTestViewsHelper.getAnalogInputTestTextView().setText(context.getString(R.string.ok));
            newBoardTestViewsHelper.getAnalogInputTestTextView().setTextColor(textColor);
            newBoardTestViewsHelper.getArrowAnalogInput().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }
        isProcessStart = false;
    }

    protected void onFailedAnalogInputTest(Exception exception){
        String failedMessage = "Failed to test Analog Input cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getAnalogInputTestTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowAnalogInput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getAnalogInputTestTextView().setTextColor(Color.RED);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getAnalogInputTestProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getAnalogInputTestTextView().setText(failedMessage);
    }

    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}

