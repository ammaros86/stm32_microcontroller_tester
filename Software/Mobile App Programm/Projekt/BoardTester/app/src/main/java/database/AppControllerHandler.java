package database;

import static helpers.Constants.ControllerTag;
import static helpers.Constants.ExceptionNoData;
import static helpers.Constants.ReadAppControllerDelay;
import static helpers.Constants.UrlPrefix;
import static helpers.Constants.urlReadBoardController;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.StringWriter;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import helpers.SharePreferenceHelper;
import interfaces.AppCommandsListener;
import models.BoardController;

public class AppControllerHandler {
    private final ExecutorService executor;
    private final Handler handler;
    private Gson gson;
    private Runnable runnable;
    private final AppCommandsListener appControllerListener;
    private final RequestQueue requestQueue;
    private final Context context;

    private boolean isHandlerStart ;
    public AppControllerHandler(Context context, AppCommandsListener listener) {
        this.context = context;
        this.appControllerListener = listener;
        isHandlerStart = false;
        handler = new Handler(Looper.getMainLooper());
        executor = Executors.newFixedThreadPool(1);
        initHandler();
        runHandler();
        requestQueue = Volley.newRequestQueue(context);
    }

    public void initHandler(){
        String dateFormat = "yyyy-MM-dd HH:mm:ss";
        gson = new GsonBuilder()
                .setDateFormat(dateFormat)
                .create();
    }

    protected void runHandler() {
        isHandlerStart = true;
        executor.execute(runnable = new Runnable() {
            @Override
            public void run() {
                handler.postDelayed(this, ReadAppControllerDelay);
                handler.post(() -> getAppController());
            }
        });
    }

    public void getAppController() {
        Log.d("MMMMY", "Main: getAppController");
        StringWriter urlBoardController = new StringWriter();
        urlBoardController.append(UrlPrefix);
        urlBoardController.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardController.append(urlReadBoardController);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardController.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                BoardController boardController = gson.fromJson(responseJson.toString(), BoardController.class);
                if(boardController != null){
                    appControllerListener.onBoardControllerLoaded(boardController);
                }else{
                    appControllerListener.onFailedBoardController(new Exception(ExceptionNoData));
                }
                Log.d("MMMMY", "Main: responseJson: " +responseJson.toString());

                requestQueue.cancelAll(ControllerTag);
            } catch (JSONException e) {
                e.printStackTrace();
                requestQueue.cancelAll(ControllerTag);
                appControllerListener.onFailedBoardController(e);
            }

        }, appControllerListener::onFailedBoardController);
        stringRequest.setTag(ControllerTag);
        requestQueue.add(stringRequest);
    }

    public void restartAppControllerHandler(){
        requestQueue.stop();
        stopAppControllerHandler();
        requestQueue.start();
        runHandler();
    }

    public void startAppControllerHandler(){
        if(!isHandlerStart){
            requestQueue.start();
            runHandler();
            isHandlerStart = true;
        }
    }
    public void stopAppControllerHandler(){
        if(runnable != null){
            handler.removeCallbacks(runnable);
            isHandlerStart = false;
        }
    }
}
