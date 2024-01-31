package boardTestProcesses;

import android.os.Handler;
import android.os.Looper;
import android.widget.ProgressBar;

import helpers.CurrentTestProcess;

public class ProgressBarHandler {
    private int currentProgressInt;
    private Runnable runnableProgressBar;
    private final Handler handler;
    public ProgressBarHandler(){
        handler = new Handler(Looper.getMainLooper());
    }

    void startProgressBarHandler(ProgressBar powerPinsProgressBar) {
        int progressStep = 10;
        int progressStart = 0;
        int progressMaxValue = 100;
        int delay = 500;
        currentProgressInt = progressStart;
        powerPinsProgressBar.setMax(progressMaxValue);
        powerPinsProgressBar.setProgress(progressStart);
        handler.postDelayed(runnableProgressBar = () -> {
            currentProgressInt +=progressStep;
            powerPinsProgressBar.setProgress(currentProgressInt);
            if(currentProgressInt == progressMaxValue){
                currentProgressInt = progressStart;
            }
            handler.postDelayed(runnableProgressBar, delay);
        }, delay);
    }

    protected void stopProgressBarHandler(CurrentTestProcess currentProcess){
        if(runnableProgressBar != null && (currentProcess == CurrentTestProcess.free)) {
            handler.removeCallbacks(runnableProgressBar);
            runnableProgressBar = null;
        }
    }

}
