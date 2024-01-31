package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardGpioShortCircuitTestValuePosition;

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
import models.BoardTestCommands;
import models.GpioShortCircuit;
import testResults.GpiosShortCircuitTestResultActivity;

public class GpioShorCircuitProcess extends ProgressBarHandler{

    private final Context context;
    private final int boardID;
    private final CurrentProcessListener processListener;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final BoardChecker boardChecker;
    protected final ArrayList<GpioShortCircuit> gpioShortCircuitArrayList;
    private final int textColor;
    private String boardName;
    public GpioShorCircuitProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        gpioShortCircuitArrayList = new ArrayList<>();
        boardChecker = new BoardChecker();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    public void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutGpioShortCircuit().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free) {
                startGpioShortCircuitProcess(false);
            }
        });
        newBoardTestViewsHelper.getGpioShortCircuitInfo().setOnClickListener(v ->{
            Intent intent = new Intent(context, GpiosShortCircuitTestResultActivity.class);
            intent.putExtra(Constants.gpioShortCircuitKey, gpioShortCircuitArrayList);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startGpioShortCircuitProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.GpioShortCircuitTest);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardGpioShortCircuitTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0,0,0, 0,0, 0,1,1, errorsValue, testValue), CurrentTestProcess.GpioShortCircuitTest);
            }else{
                testValue = (1 << BoardGpioShortCircuitTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0,0,0, 0,0, 0,1,0, errorsValue, testValue), CurrentTestProcess.GpioShortCircuitTest);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getGpioShortCircuitBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getGpioShortCircuitBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getGpioShortCircuitInfo().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowShortCircuit().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setGpioShortCircuitTestResult(ArrayList<GpioShortCircuit> gpioShortCircuitArray){
        this.gpioShortCircuitArrayList.clear();
        this.gpioShortCircuitArrayList.addAll(gpioShortCircuitArray);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity)context).getCurrentProcess());
        newBoardTestViewsHelper.getGpioShortCircuitInfo().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getGpioShortCircuitBar().setVisibility(View.GONE);
        if(boardChecker.hasGpioShortCircuit(gpioShortCircuitArray)){
            newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setText("");
            newBoardTestViewsHelper.getArrowShortCircuit().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }else{
            newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setText(context.getString(R.string.ok));
            newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setTextColor(textColor);
            newBoardTestViewsHelper.getArrowShortCircuit().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }
        isProcessStart = false;
    }

    protected void onFailedGpioShortCircuitTest(Exception exception){
        String failedMessage = "Failed to test GPIO short circuit cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowShortCircuit().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setTextColor(Color.RED);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getGpioShortCircuitBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getGpioShortCircuitTestTextView().setText(failedMessage);
    }

    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
