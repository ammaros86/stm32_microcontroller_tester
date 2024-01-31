package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardGpioContinuityTestValuePosition;

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
import models.GpioContinuity;
import testResults.GpiosContinuityTestResultActivity;

public class ContinuityTestProcess extends ProgressBarHandler {
    private final Context context;
    private final CurrentProcessListener processListener;
    private final int boardID;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final BoardChecker boardChecker;
    private boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    protected final ArrayList<GpioContinuity> continuityArrayList;
    private final int textColor;
    private String boardName;

    public ContinuityTestProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
        this.context = context;
        this.processListener = processListener;
        this.boardID = boardID;
        this.boardName = boardName;
        this.boardTesterDataBaseHandler = boardTesterDataBaseHandler;
        this.newBoardTestViewsHelper = newBoardTestViewsHelper;
        continuityArrayList = new ArrayList<>();
        boardChecker = new BoardChecker();
        initialListener();
        textColor = ContextCompat.getColor(context, R.color.textColor);
    }

    public void initialListener(){
        newBoardTestViewsHelper.getRelativeLayoutContinuity().setOnClickListener(v ->{
            if(((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free) {
                startContinuityTestProcess(false);
            }
        });
        newBoardTestViewsHelper.getGpioContinuityInfo().setOnClickListener(v ->{
            Intent intent = new Intent(context, GpiosContinuityTestResultActivity.class);
            intent.putExtra(Constants.gpioContinuityKey, continuityArrayList);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startContinuityTestProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.ContinuityTestProcess);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            testValue = (1 << BoardGpioContinuityTestValuePosition);
            if(isCompleteTest){
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0, 0,0,1, 0,0,0,0,1 , errorsValue, testValue), CurrentTestProcess.ContinuityTestProcess);
            }else{
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0, 0,0,1, 0,0,0,0,0 , errorsValue, testValue), CurrentTestProcess.ContinuityTestProcess);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getContinuityProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getContinuityProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getGpioContinuityInfo().setVisibility(View.GONE);
        newBoardTestViewsHelper.getGpioContinuityTestTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getGpioContinuityTestTextView().setTextColor(textColor);
        newBoardTestViewsHelper.getArrowContinuity().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
    }

    protected void setGpioContinuityTestResult(ArrayList<GpioContinuity> gpioContinuityTestResult){
        continuityArrayList.clear();
        continuityArrayList.addAll(gpioContinuityTestResult);
        checkResultsReady();
    }

    private void checkResultsReady(){
        newBoardTestViewsHelper.getGpioContinuityInfo().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getContinuityProgressBar().setVisibility(View.GONE);
        int errors = boardChecker.getGpiosContinuityErrors(continuityArrayList);
        if(errors > 0){
            newBoardTestViewsHelper.getGpioContinuityTestTextView().setText("");
            newBoardTestViewsHelper.getArrowContinuity().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }else{
            newBoardTestViewsHelper.getArrowContinuity().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
            newBoardTestViewsHelper.getGpioContinuityTestTextView().setText(context.getString(R.string.ok));
            newBoardTestViewsHelper.getGpioContinuityTestTextView().setTextColor(textColor);
        }
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void onFailedGpioContinuityTest(Exception exception){
        String failedMessage = "Failed to test GPIOs Continuity cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getGpioContinuityTestTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowContinuity().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getContinuityProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command to Board Tester";
        newBoardTestViewsHelper.getGpioContinuityTestTextView().setText(failedMessage);
    }

    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
