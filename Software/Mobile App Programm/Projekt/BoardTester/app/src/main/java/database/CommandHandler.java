package database;

import static helpers.Constants.AppControllerRequestTag;
import static helpers.Constants.ReadAppCommandDelay;
import static helpers.Constants.UrlPrefix;
import static helpers.Constants.urlBoardCommandStateSuffix;

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

import org.json.JSONArray;
import org.json.JSONException;

import java.io.StringWriter;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import helpers.SharePreferenceHelper;
import interfaces.AppCommandsListener;
import models.BoardTestCommands;

public class CommandHandler {
    private final ExecutorService executor;
    private final Handler handler;
    private Gson gson;
    private Runnable runnable;
    private HashMap<Integer, BoardTestCommands> appControllerState ;
    private final AppCommandsListener appControllerListener;
    private final RequestQueue requestQueue;
    private final Context context;

    public CommandHandler(Context context, AppCommandsListener listener) {
        this.context = context;
        this.appControllerListener = listener;
        handler = new Handler(Looper.getMainLooper());
        executor = Executors.newFixedThreadPool(1);
        initHandler();
        runHandler();
        requestQueue = Volley.newRequestQueue(context);
    }

    public void initHandler(){
        appControllerState = new HashMap<>();
        String dateFormat = "yyyy-MM-dd HH:mm:ss";
        gson = new GsonBuilder()
                .setDateFormat(dateFormat)
                .create();
    }

    public void stopCommandHandler(){
        Log.d("POWERPINTEST", "AppCommandHandler: stopCommandHandler");
        requestQueue.stop();
        if(runnable != null){
            handler.removeCallbacks(runnable);

        }
//        executor.shutdown();
    }

    public void startCommandHandler(){
        requestQueue.start();
        runHandler();
    }
    protected void runHandler() {
        executor.execute(runnable = new Runnable() {
            @Override
            public void run() {
                handler.postDelayed(this, ReadAppCommandDelay);
                handler.post(() -> getAppCommandsState());
            }
        });
    }

    public void getAppCommandsState() {
        StringWriter urlBoardControllerState = new StringWriter();
        urlBoardControllerState.append(UrlPrefix);
        urlBoardControllerState.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardControllerState.append(urlBoardCommandStateSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, urlBoardControllerState.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                appControllerState.clear();
                for (int i = 0; i < jsonarray.length(); i++) {
                    appControllerState.put(gson.fromJson(jsonarray.getJSONObject(i).toString(), BoardTestCommands.class).getBoard_id(), gson.fromJson(jsonarray.getJSONObject(i).toString(), BoardTestCommands.class));
                }
                appControllerListener.onSuccessLoadedAppCommands(appControllerState);
                requestQueue.cancelAll(AppControllerRequestTag);
            } catch (JSONException e) {
                e.printStackTrace();
                appControllerListener.onErrorLoadAppCommands(e);
                requestQueue.cancelAll(AppControllerRequestTag);
            }
        }, appControllerListener::onErrorLoadAppCommands);
        stringRequest.setTag(AppControllerRequestTag);
        requestQueue.add(stringRequest);
    }

    public void restartAppCommandHandler(){
//        requestQueue.stop();
//        stopCommandHandler();
//        requestQueue.start();
//        runHandler();
    }

}
