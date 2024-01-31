package boardTestProcesses;

import static helpers.Constants.BOARD_NAME_KEY;
import static helpers.Constants.BoardFlashTestValuePosition;
import static helpers.Constants.FlashTestDone;
import static helpers.Constants.FlashTestFail;
import static helpers.Constants.UploadTestProgramSuccess;
import static helpers.Constants.flashKey;

import android.content.Context;
import android.content.Intent;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.boardtester.R;

import database.BoardTesterDataBaseHandler;
import helpers.CurrentTestProcess;
import interfaces.CurrentProcessListener;
import models.BoardFlash;
import models.BoardTestCommands;
import testResults.FlashMemoryTestResultActivity;

class FlashProcess extends ProgressBarHandler {
    private final Context context;
    private final int boardID;
    private final CurrentProcessListener processListener;
    private final BoardTesterDataBaseHandler boardTesterDataBaseHandler;
    private BoardFlash currentBoardFlashInfo;
    private boolean isProcessStart = false;
    private int testValue ;
    private int errorsValue ;

    private final NewBoardTestViewsHelper newBoardTestViewsHelper;
    private final int textColor;
    private String boardName;
    public FlashProcess(Context context, int boardID, String boardName, CurrentProcessListener processListener, BoardTesterDataBaseHandler boardTesterDataBaseHandler, NewBoardTestViewsHelper newBoardTestViewsHelper){
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
        newBoardTestViewsHelper.getRelativeLayoutFlash().setOnClickListener(v -> startFlashProcess(false));
        newBoardTestViewsHelper.getFlashInformation().setOnClickListener(v ->{
            Intent intent = new Intent(context, FlashMemoryTestResultActivity.class);
            intent.putExtra(flashKey, currentBoardFlashInfo);
            intent.putExtra(BOARD_NAME_KEY, boardName);
            context.startActivity(intent);
        });
    }

    protected void startFlashProcess(boolean isCompleteTest){
        if(!isProcessStart && ((NewBoardTestActivity)context).getCurrentProcess() == CurrentTestProcess.free){
            processListener.setCurrentProcess(CurrentTestProcess.FlashProcess);
            isProcessStart = true;
            ((NewBoardTestActivity)context).stopCommandHandler();
            setViewsProcessStart();
            if(isCompleteTest){
                testValue = testValue | (1 << BoardFlashTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0,1,0,0, 0 ,0,0,0, 1, errorsValue, testValue), CurrentTestProcess.FlashProcess);
            }else{
                testValue =  (1 << BoardFlashTestValuePosition);
                boardTesterDataBaseHandler.updateBoardTestCommand(new BoardTestCommands(1, boardID, 0, 0,1,0,0, 0 ,0,0,0, 0, errorsValue, testValue), CurrentTestProcess.FlashProcess);
            }
            startProgressBarHandler(newBoardTestViewsHelper.getFlashProgressBar());
        }
    }

    public void setViewsProcessStart(){
        newBoardTestViewsHelper.getArrowFlash().setImageDrawable(newBoardTestViewsHelper.getGreyArrow());
        newBoardTestViewsHelper.getFlashProgressBar().setVisibility(View.VISIBLE);
        newBoardTestViewsHelper.getFlashInformation().setVisibility(View.GONE);
        newBoardTestViewsHelper.getFlashTextView().setText(context.getString(R.string.wait_message));
        newBoardTestViewsHelper.getFlashTextView().setTextColor(textColor);
    }

    protected void setFlashData(BoardFlash boardFlash){
        currentBoardFlashInfo = boardFlash;
        processListener.setCurrentProcess(CurrentTestProcess.free);
        newBoardTestViewsHelper.getFlashProgressBar().setVisibility(View.GONE);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        newBoardTestViewsHelper.getFlashInformation().setVisibility(View.VISIBLE);
        if((boardFlash.getUpload_progrgamm() == UploadTestProgramSuccess) && (boardFlash.getFlash_test() == FlashTestDone)){
            newBoardTestViewsHelper.getFlashTextView().setText(context.getString(R.string.ok));
            newBoardTestViewsHelper.getFlashTextView().setTextColor(textColor);
            newBoardTestViewsHelper.getArrowFlash().setImageDrawable(newBoardTestViewsHelper.getGreenArrow());
        }else{
            String message = "";
            if((boardFlash.getUpload_progrgamm() == UploadTestProgramSuccess)){
                if((boardFlash.getFlash_test() == FlashTestFail)){
                    message =  "Flash Memory Errors";
                }
            }else{
                message =  "Upload Test Program fail";
                newBoardTestViewsHelper.getFlashInformation().setVisibility(View.GONE);
            }
            newBoardTestViewsHelper.getFlashTextView().setText(message);
            newBoardTestViewsHelper.getArrowFlash().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        }
        isProcessStart = false;
    }

    protected void onFlashFailedResponse(Exception exception){
        String failedMessage = "Failed to flash board cause: ";
        if(exception != null){
            failedMessage += exception.getMessage();
        }else{
            failedMessage += " unknown Exception";
        }
        newBoardTestViewsHelper.getFlashTextView().setText(failedMessage);
        newBoardTestViewsHelper.getArrowFlash().setImageDrawable(newBoardTestViewsHelper.getRedArrow());
        newBoardTestViewsHelper.getFlashProgressBar().setVisibility(View.GONE);
        processListener.setCurrentProcess(CurrentTestProcess.free);
        stopProgressBarHandler(((NewBoardTestActivity) context).getCurrentProcess());
        isProcessStart = false;
    }

    protected void setSendTestCommandFailed(){
        String failedMessage = "Failed to send Command";
        newBoardTestViewsHelper.getFlashTextView().setText(failedMessage);
    }
    protected void updateTestValue(int testValue){
        this.testValue = testValue;
    }
    protected void updateErrorsValue(int errors){
        this.errorsValue = errors;
    }
}
