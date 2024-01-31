package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardGpioInputTestValuePosition;

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
import models.GpioInput;
import testResults.GpiosInputTestResultActivity;

class GpioInputTestProcess extends ProgressBarHandler {
    private final Context context;
    private final CurrentProcessListener processListener;
    private final int boardID;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final BoardChecker boardChecker;
    private boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    private boolean noPullResultIsReady = false;
    private boolean pullDownResultIsReady = false;
    private boolean pullUpResultIsReady = false;
    protected final ArrayList<GpioInput> gpioInputNoPullArrayList;
    protected final ArrayList<GpioInput> gpioInputPullDownArrayList;
    protected final ArrayList<GpioInput> gpioInputPullUpArrayList;
    private final int textColor;
    private String boardName;

    public GpioInputTestProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        gpioInputNoPullArrayList = new ArrayList<>();
        gpioInputPullDownArrayList = new ArrayList<>();
        gpioInputPullUpArrayList = new ArrayList<>();
        boardChecker = new BoardChecker();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    public void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutGpioInput().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free) {
                startGpioInputProcess(false);
            }
        });
        newBoardTestViewsHelper.getGpioInputInfo().setOnClickListener(v ->{
            Intent intent = new Intent(context, GpiosInputTestResultActivity.class);
            intent.putExtra(Constants.gpioInputNoPullKey, gpioInputNoPullArrayList);
            intent.putExtra(Constants.gpioInputPullDownKey, gpioInputPullDownArrayList);
            intent.putExtra(Constants.gpioInputPullUpKey, gpioInputPullUpArrayList);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startGpioInputProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.GpioInputTest);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardGpioInputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0, 0,0, 0,0,1,0,0,1, errorsValue,  testValue), CurrentTestProcess.GpioInputTest);
            }else{
                testValue = testValue | (1 << BoardGpioInputTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0, 0,0, 0,0,1,0,0,0, errorsValue,  testValue), CurrentTestProcess.GpioInputTest);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getGpioInputTestProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getGpioInputTestProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getGpioInputInfo().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGpioInputTestTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getGpioInputTestTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowInput().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setGpioInputNoPullTestResult(ArrayList<GpioInput> gpioInputNoPullList){
        gpioInputNoPullArrayList.clear();
        gpioInputNoPullArrayList.addAll(gpioInputNoPullList);
        noPullResultIsReady = true;
        checkResultsReady();
    }

    protected void setGpioInputPullDownTestResult(ArrayList<GpioInput> gpioInputPullDownList){
        gpioInputPullDownArrayList.clear();
        gpioInputPullDownArrayList.addAll(gpioInputPullDownList);
        pullDownResultIsReady = true;
        checkResultsReady();
    }

    protected void setGpioInputPullUpTestResult(ArrayList<GpioInput> gpioInputPullUpList){
        gpioInputPullUpArrayList.clear();
        gpioInputPullUpArrayList.addAll(gpioInputPullUpList);
        pullUpResultIsReady = true;
        checkResultsReady();
    }

    private void checkResultsReady(){
        if(noPullResultIsReady && pullDownResultIsReady && pullUpResultIsReady){
            newBoardTestViewsHelper.getGpioInputInfo().setVisibility(View.VISIBLE);
            newBoardTestViewsHelper.getGpioInputTestProgressBar().setVisibility(View.GONE);
            int errors = boardChecker.getGpioInputTestErrors(gpioInputNoPullArrayList, gpioInputPullDownArrayList, gpioInputPullUpArrayList);
            if(errors > 0){
                newBoardTestViewsHelper.getGpioInputTestTextView().setText("");
                newBoardTestViewsHelper.getArrowInput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
            }else{
                newBoardTestViewsHelper.getGpioInputTestTextView().setText(context.getString(R.string.ok));
                newBoardTestViewsHelper.getArrowInput().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
                newBoardTestViewsHelper.getGpioInputTestTextView().setTextColor(textColor);
            }
            processListener.setCurrentProcess(CurrentTestProcess.free);
            stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
            isProcessStart = false;
            noPullResultIsReady = false;
            pullDownResultIsReady = false;
            pullUpResultIsReady = false;
        }
    }

    protected void onFailedGpioInputTest(Exception exception){
        String failedMessage = "Failed to flash board cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getGpioInputTestTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowInput().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getGpioInputTestProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
        noPullResultIsReady = false;
        pullDownResultIsReady = false;
        pullUpResultIsReady = false;
    }
    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getGpioInputTestTextView().setText(failedMessage);
    }

    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
