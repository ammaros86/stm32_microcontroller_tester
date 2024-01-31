package log;

import static helpers.Constants.UrlPrefix;
import static helpers.Constants.removeLogTag;
import static helpers.Constants.urlRemoveLogSuffix;

import android.content.Context;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;

import helpers.SharePreferenceHelper;
import interfaces.RemoveLogListener;

public class RemoveLogHandler {

    private final RequestQueue requestQueue;
    private final Context context;
    private final RemoveLogListener removeLogListener;

    public RemoveLogHandler(Context context, RemoveLogListener listener) {
        this.context = context;
        this.removeLogListener = listener;
        requestQueue = Volley.newRequestQueue(context);
    }

    public void stopRequestQueue(){
        requestQueue.stop();
    }


    public void removeLogFile(String logFileName, int position){

        StringWriter urlRemoveBoard = new StringWriter();
        urlRemoveBoard.append(UrlPrefix);
        urlRemoveBoard.append(SharePreferenceHelper.getServerUrl(context));
        urlRemoveBoard.append(urlRemoveLogSuffix);
        Log.d("MYDEBUGGER", "removeLogFile logFileName:" +logFileName + " position:.. " + position );
        Log.d("MYDEBUGGER", "removeLogFile urlRemoveBoard:" +urlRemoveBoard.toString() );

        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlRemoveBoard.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                Log.d("MYDEBUGGER", "removeLogFile responseJson:" +responseJson.toString() );

                removeLogListener.onRemoveBoard(responseJson.get("response").toString(), position);
            } catch (JSONException e) {
                e.printStackTrace();
                removeLogListener.onRemoveBoardFailed(e);
                Log.d("MYDEBUGGER", "removeLogFile onRemoveBoardFailed:" +e.toString() );

                requestQueue.cancelAll(removeLogTag);
            }
        }, error -> {
            removeLogListener.onRemoveBoardFailed(error);
            Log.d("MYDEBUGGER", "removeLogFile onRemoveBoardFailed error:" +error.toString() );

            requestQueue.cancelAll(removeLogTag);
        }) {
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put("logfilename", logFileName);
                return MyData;
            }
        };
        stringRequest.setTag(removeLogTag);
        requestQueue.add(stringRequest);
    }

}
