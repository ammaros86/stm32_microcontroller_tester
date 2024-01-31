package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardGpioOutputTestValuePosition;

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
import models.GpioOutput;
import testResults.GpioOutputTestResultActivity;

class GpioOutputProcess extends ProgressBarHandler {
    private final Context context;
    private final CurrentProcessListener processListener;
    private final int boardID;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private int testValue ;
    private int errorsValue ;

    private final BoardChecker boardChecker;
    private boolean isProcessStart = false;
    private boolean noPullResultIsReady = false;
    private boolean pullDownResultIsReady = false;
    private boolean pullUpResultIsReady = false;
    protected final ArrayList<GpioOutput> gpioOutputNoPullArrayList;
    protected final ArrayList<GpioOutput> gpioOutputPullDownArrayList;
    protected final ArrayList<GpioOutput> gpioOutputPullUpArrayList;
    private final int textColor;
    private String boardName;
    public GpioOutputProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        gpioOutputNoPullArrayList = new ArrayList<>();
        gpioOutputPullDownArrayList = new ArrayList<>();
        gpioOutputPullUpArrayList = new ArrayList<>();
        boardChecker = new BoardChecker();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    public void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutGpioOutput().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free) {
                startGpioOutputProcess(false);
            }
        });
        newBoardTestViewsHelper.getGpioOutputInfo().setOnClickListener(v ->{
            Intent intent = new Intent(context, GpioOutputTestResultActivity.class);
            intent.putExtra(Constants.gpioOutputNoPullKey, gpioOutputNoPullArrayList);
            intent.putExtra(Constants.gpioOutputPullDownKey, gpioOutputPullDownArrayList);
            intent.putExtra(Constants.gpioOutputPullUpKey, gpioOutputPullUpArrayList);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startGpioOutputProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.GpioOutputTest);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardGpioOutputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0, 0,0, 1, 0,0, 0,1, errorsValue,  testValue), CurrentTestProcess.GpioOutputTest);
            }else{
                testValue = (1 << BoardGpioOutputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0,0,0, 0,0, 1, 0,0, 0,0, errorsValue,  testValue), CurrentTestProcess.GpioOutputTest);
            }

            startProgressBarHandler(newBoardTestViewsHelper.getGpioOutputTestProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getGpioOutputTestProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getGpioOutputInfo().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGpioOutputTestTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getGpioOutputTestTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowOutput().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setGpioOutputNoPullTestResult(ArrayList<GpioOutput> gpioOutputNoPullArray){
        gpioOutputNoPullArrayList.clear();
        gpioOutputNoPullArrayList.addAll(gpioOutputNoPullArray);
        noPullResultIsReady = true;
        checkResultsReady();
    }

    protected void setGpioOutputPullDownTestResult(ArrayList<GpioOutput> gpioOutputPullDownArray){
        gpioOutputPullDownArrayList.clear();
        gpioOutputPullDownArrayList.addAll(gpioOutputPullDownArray);
        pullDownResultIsReady = true;
        checkResultsReady();
    }

    protected void setGpioOutputPullUpTestResult(ArrayList<GpioOutput> gpioOutputPullUpArray){
        gpioOutputPullUpArrayList.clear();
        gpioOutputPullUpArrayList.addAll(gpioOutputPullUpArray);
        pullUpResultIsReady = true;
        checkResultsReady();
    }

    private void checkResultsReady(){
        if(noPullResultIsReady && pullDownResultIsReady && pullUpResultIsReady){
            newBoardTestViewsHelper.getGpioOutputInfo().setVisibility(View.VISIBLE);
            newBoardTestViewsHelper.getGpioOutputTestProgressBar().setVisibility(View.GONE);
            int errors = boardChecker.getGpioOutputTestResultsErrors(gpioOutputNoPullArrayList, gpioOutputPullDownArrayList, gpioOutputPullUpArrayList);
            if(errors > 0){
                newBoardTestViewsHelper.getGpioOutputTestTextView().setText("");
                newBoardTestViewsHelper.getArrowOutput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
            }else{
                newBoardTestViewsHelper.getGpioOutputTestTextView().setText(context.getString(R.string.ok));
                newBoardTestViewsHelper.getArrowOutput().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
            }
            processListener.setCurrentProcess(CurrentTestProcess.free);
            stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
            isProcessStart = false;
            noPullResultIsReady = false;
            pullDownResultIsReady = false;
            pullUpResultIsReady = false;
        }
    }

    protected void onFailedGpioOutputTest(Exception exception){
        String failedMessage = "Failed to test GPIO Output cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getGpioOutputTestTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowOutput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getGpioOutputTestProgressBar().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGpioOutputTestTextView().setTextColor(Color.RED);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
        noPullResultIsReady = false;
        pullDownResultIsReady = false;
        pullUpResultIsReady = false;
    }
    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getGpioOutputTestTextView().setText(failedMessage);
    }
    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
