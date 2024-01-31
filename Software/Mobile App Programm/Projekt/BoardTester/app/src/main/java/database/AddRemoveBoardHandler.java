package database;

import static helpers.Constants.*;
import android.content.Context;
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
import interfaces.BoardManagerListener;
import models.BoardTestCommands;

public class AddRemoveBoardHandler {
    private final RequestQueue requestQueue;
    private final Context context;
    private final BoardManagerListener boardHandlerListener;

    public AddRemoveBoardHandler(Context context, BoardManagerListener listener) {
        this.context = context;
        this.boardHandlerListener = listener;
        requestQueue = Volley.newRequestQueue(context);
    }

    public void stopRequestQueue(){
        requestQueue.stop();
    }

    public void addNewBoard(BoardTestCommands controller){
        StringWriter urlBoardController = new StringWriter();
        urlBoardController.append(UrlPrefix);
        urlBoardController.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardController.append(urlAddNewBoardSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardController.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                String status = responseJson.getString(SuccessKey);
                boardHandlerListener.onBoardAdded(status.equals(BoardAdded));
                requestQueue.cancelAll(addBoardTag);
            } catch (JSONException e) {
                e.printStackTrace();
                boardHandlerListener.onAddBoardError(e);
                requestQueue.cancelAll(addBoardTag);
            }
        }, error -> {
            boardHandlerListener.onAddBoardError(error);
            requestQueue.cancelAll(addBoardTag);
        }) {
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put("boardName", controller.getBoardName());
                return MyData;
            }
        };
        stringRequest.setTag(addBoardTag);
        requestQueue.add(stringRequest);
    }

    public void removeBoard(int boardID, int position){
        StringWriter urlRemoveBoard = new StringWriter();
        urlRemoveBoard.append(UrlPrefix);
        urlRemoveBoard.append(SharePreferenceHelper.getServerUrl(context));
        urlRemoveBoard.append(urlRemoveBoardSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlRemoveBoard.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                checkBoardRemoved(responseJson, position, boardID);
            } catch (JSONException e) {
                e.printStackTrace();
                boardHandlerListener.onRemoveBoardError(e, boardID);
                requestQueue.cancelAll(removeBoardTag);
            }
        }, error -> {
            boardHandlerListener.onRemoveBoardError(error, boardID);
            requestQueue.cancelAll(removeBoardTag);
        }) {
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put("board_id", String.valueOf(boardID));

                return MyData;
            }
        };
        stringRequest.setTag(removeBoardTag);
        requestQueue.add(stringRequest);
    }
    //TODO add on remove current table

    void checkBoardRemoved(JSONObject responseJson, int position, int boardID) {
        try {
            int board_controller = Integer.parseInt(responseJson.getString(BoardCommandsTable));
            int board_flash = Integer.parseInt(responseJson.getString(BoardFlashTable));
            int board_temperature = Integer.parseInt(responseJson.getString(BoardTemperatureTable));

            int GpioInputNoPull = Integer.parseInt(responseJson.getString(BoardInputNoPullTable));
            int GpioInputPullDown = Integer.parseInt(responseJson.getString(BoardInputPullDownTable));
            int GpioInputPullUp = Integer.parseInt(responseJson.getString(BoardInputPullUpTable));

            int GpioOutputNoPull = Integer.parseInt(responseJson.getString(BoardOutputNoPullTable));
            int GpioOutputPullDown  = Integer.parseInt(responseJson.getString(BoardOutputPullDownTable));
            int GpioOutputPullUp = Integer.parseInt(responseJson.getString(BoardOutputPullUpTable));

            int short_circuit = Integer.parseInt(responseJson.getString(BoardGpioShortCircuitTable));
            int power_gnd_pins = Integer.parseInt(responseJson.getString(BoardPowerGndPinsTable));

            boardHandlerListener.onRemoveBoard((board_controller == 1) && (board_flash == 1) && (board_temperature == 1) &&
                    (GpioInputNoPull == 1) && (GpioInputPullDown == 1) && (GpioInputPullUp == 1) && (GpioOutputNoPull == 1) &&
                    (GpioOutputPullDown == 1 && (GpioOutputPullUp == 1)&& (short_circuit == 1)&& (power_gnd_pins == 1)), position, boardID);
        } catch (JSONException e) {
            boardHandlerListener.onRemoveBoardError(e, boardID);
        }
        requestQueue.cancelAll(removeBoardTag);
    }
}
