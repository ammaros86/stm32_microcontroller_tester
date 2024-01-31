package database;

import static helpers.Constants.*;


import android.content.Context;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import helpers.*;
import interfaces.*;
import models.*;


public class BoardTesterDataBaseHandler {
    private Gson gson;
    private final TestProcessListener boardTesterDatabaseListener;
    private final TestCommandsSetListener commandsSetListener;
    private final LogFilesListener logFilesListener;
    private final ControlCommandSetListener controlCommandSetListener;

    private final RequestQueue myRequestQueue;
    private final Context context;

    public BoardTesterDataBaseHandler(Context context, TestProcessListener listener, TestCommandsSetListener commandsSetListener, ControlCommandSetListener controlCommandSetListener, LogFilesListener logFilesListener, RequestQueue MyRequestQueue) {
        this.context = context;
        this.boardTesterDatabaseListener = listener;
        this.commandsSetListener = commandsSetListener;
        this.controlCommandSetListener = controlCommandSetListener;
        this.logFilesListener = logFilesListener;
        this.myRequestQueue = MyRequestQueue;
        initHandler();
    }

    public void initHandler(){
        gson = new GsonBuilder()
                .setDateFormat(DateFormat)
                .create();
    }

    public void updateBoardController(BoardController controller, CurrentControlCommand currentControlCommand){
        StringWriter urlBoardController = new StringWriter();
        urlBoardController.append(UrlPrefix);
        urlBoardController.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardController.append(urlBoardControllerSuffix);
        StringRequest MyStringRequest = new StringRequest(Request.Method.POST, urlBoardController.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                checkBoardControllerSet(responseJson, currentControlCommand);
            } catch (JSONException e) {
                e.printStackTrace();
                boardTesterDatabaseListener.onFailedUpdateBoardController(e);
                checkBoardControllerSet(new JSONObject(), currentControlCommand);
            }
        }, boardTesterDatabaseListener::onFailedUpdateBoardController){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put("copy_logs_usb", String.valueOf(controller.getCopy_logs_usb()));
                MyData.put("turn_usb_port_on", String.valueOf(controller.getTurn_usb_port_on()));
                MyData.put("power_off", String.valueOf(controller.getPower_off()));
                return MyData;
            }
        };
        myRequestQueue.add(MyStringRequest);
    }

    void checkBoardControllerSet(JSONObject responseJson, CurrentControlCommand currentControlCommand){
        String done = "done";
        String value = "";
        try {
            value = responseJson.getString("message");
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }
        switch (currentControlCommand){
            case turnUsbPortOn:
                controlCommandSetListener.onTryToSetTurnUsbOnCommand(value.equals(done));
                break;
            case copyLogFiles:
                controlCommandSetListener.onTryToSetCopyLogFilesCommand(value.equals(done));
                break;
            case turnBoardOff:
                controlCommandSetListener.onTryToSetTurnBoardOffCommand(value.equals(done));
                break;
        }
    }

    public void updateBoardTestCommand(BoardTestCommands command, CurrentTestProcess currentTestProcessCommand){
        StringWriter urlBoardController = new StringWriter();
        urlBoardController.append(UrlPrefix);
        urlBoardController.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardController.append(urlBoardCommandSuffix);
        StringRequest MyStringRequest = new StringRequest(Request.Method.POST, urlBoardController.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                checkCommandSet(responseJson, currentTestProcessCommand);
            } catch (JSONException e) {
                e.printStackTrace();
                boardTesterDatabaseListener.onFailedUpdateBoardCommands(e);
                checkCommandSet(new JSONObject(), currentTestProcessCommand);
            }
        }, boardTesterDatabaseListener::onFailedUpdateBoardCommands
        ){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put("isUpdate", String.valueOf(command.getIsUpdate()));
                MyData.put("board_id", String.valueOf(command.getBoard_id()));
                MyData.put("measure_temperature", String.valueOf(command.getMeasure_temperature()));
                MyData.put("measure_current", String.valueOf(command.getMeasure_current()));
                MyData.put("flash_board", String.valueOf(command.getFlash_board()));
                MyData.put("gpio_continuity", String.valueOf(command.getGpio_continuity()));
                MyData.put("power_gnd_test", String.valueOf(command.getPower_gnd_test()));
                MyData.put("gpio_output_test", String.valueOf(command.getGpio_output_test()));
                MyData.put("gpio_input_test", String.valueOf(command.getGpio_input_test()));
                MyData.put("analog_input_test", String.valueOf(command.getAnalog_input_test()));
                MyData.put("gpio_short_circuit_test", String.valueOf(command.getGpio_short_circuit_test()));
                MyData.put("complete_test", String.valueOf(command.getComplete_test()));
                MyData.put("test_value", String.valueOf(command.getTest_value()));
                MyData.put("errors", String.valueOf(command.getErrors()));
                return MyData;
            }
        };
        myRequestQueue.add(MyStringRequest);
    }

    public void checkCommandSet(JSONObject responseJson, CurrentTestProcess currentProcess){
        String done = "done";
        String value = "";
        try {
            value = responseJson.getString("success");
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }
        switch (currentProcess){
            case FlashProcess:
                commandsSetListener.onTryToSetFlashCommand(value.equals(done));
                break;
            case TemperatureProcess:
                commandsSetListener.onTryToSetTemperatureCommand(value.equals(done));
                break;
            case CurrentMeasureProcess:
                commandsSetListener.onTryToSetMeasureCurrentCommand(value.equals(done));
                break;
            case PowerPinsProcess:
                commandsSetListener.onTryToSetPowerGndPinsCommand(value.equals(done));
                break;
            case ContinuityTestProcess:
                commandsSetListener.onTryToSetGpioContinuityCommand(value.equals(done));
                break;
            case GpioInputTest:
                commandsSetListener.onTryToSetGpioInputCommand(value.equals(done));
                break;
            case GpioOutputTest:
                commandsSetListener.onTryToSetGpioOutputCommand(value.equals(done));
                break;
            case AnalogInputTest:
                commandsSetListener.onTryToSetAnalogInputCommand(value.equals(done));
                break;
            case GpioShortCircuitTest:
                commandsSetListener.onTryToSetGpioShortCircuitCommand(value.equals(done));
                break;
        }
    }

    public void getLogFilesList(){
        StringWriter urlLogFiles = new StringWriter();
        urlLogFiles.append(UrlPrefix);
        urlLogFiles.append(SharePreferenceHelper.getServerUrl(context));
        urlLogFiles.append(urlLogFilesSuffix);
        ArrayList<String> logFilesArrayList = new ArrayList<>();
        StringRequest stringRequest = new StringRequest(Request.Method.GET, urlLogFiles.toString(),
                response -> {
                    try {
                        JSONArray allFilesArray = new JSONArray(response);
                        for(int i = 0; i < allFilesArray.length(); i++) {
                            String fileName = allFilesArray.getString(i);
                            logFilesArrayList.add(fileName);
                        }
                        logFilesListener.onLogFilesLoaded(logFilesArrayList);
                        myRequestQueue.cancelAll(LogFilesTag);
                    }catch (JSONException e) {
                        e.printStackTrace();
                        logFilesListener.onLogFilesLoadError(e);
                        myRequestQueue.cancelAll(LogFilesTag);
                    }
                }, error -> {
            logFilesListener.onLogFilesLoadError(error);
            myRequestQueue.cancelAll(LogFilesTag);
        });
        stringRequest.setTag(LogFilesTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsTemperature(int boardID){
        StringWriter urlBoardTemperature = new StringWriter();
        urlBoardTemperature.append(UrlPrefix);
        urlBoardTemperature.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardTemperature.append(urlBoardTemperatureSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardTemperature.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                BoardTemperature boardTemperature = gson.fromJson(responseJson.toString(), BoardTemperature.class);
                if(boardTemperature != null){
                    boardTesterDatabaseListener.onTemperatureDataLoaded(boardTemperature);
                }else{
                    boardTesterDatabaseListener.onFailedTemperatureDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(TemperatureTag);
            } catch (JSONException e) {
                e.printStackTrace();
                myRequestQueue.cancelAll(TemperatureTag);
                boardTesterDatabaseListener.onFailedTemperatureDataLoad(e);
            }
        }, error -> {
            error.printStackTrace();
            boardTesterDatabaseListener.onFailedTemperatureDataLoad(error);
            myRequestQueue.cancelAll(TemperatureTag);
        }) {
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(TemperatureTag);
        myRequestQueue.add(stringRequest);
    }

    public void getCurrentConsumption(int boardID){
        StringWriter urlBoardCurrentConsumption = new StringWriter();
        urlBoardCurrentConsumption.append(UrlPrefix);
        urlBoardCurrentConsumption.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardCurrentConsumption.append(urlCurrentConsumptionSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardCurrentConsumption.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                BoardCurrentConsumption boardCurrentConsumption = gson.fromJson(responseJson.toString(), BoardCurrentConsumption.class);
                if(boardCurrentConsumption != null){
                    boardTesterDatabaseListener.onCurrentConsumptionDataLoaded(boardCurrentConsumption);
                }else{
                    boardTesterDatabaseListener.onFailedCurrentConsumptionDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(CurrentMeasureTag);
            } catch (JSONException e) {
                e.printStackTrace();
                myRequestQueue.cancelAll(CurrentMeasureTag);
                boardTesterDatabaseListener.onFailedCurrentConsumptionDataLoad(e);
            }
        }, error -> {
            error.printStackTrace();
            boardTesterDatabaseListener.onFailedCurrentConsumptionDataLoad(error);
            myRequestQueue.cancelAll(CurrentMeasureTag);
        }) {
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(CurrentMeasureTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsFlash(int boardID){
        StringWriter urlBoardFlash = new StringWriter();
        urlBoardFlash.append(UrlPrefix);
        urlBoardFlash.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardFlash.append(urlBoardFlashSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardFlash.toString(), response -> {
            try {
                JSONObject responseJson = new JSONObject(response);
                BoardFlash boardFlash = gson.fromJson(responseJson.toString(), BoardFlash.class);
                if(boardFlash != null){
                    boardTesterDatabaseListener.onFlashDataLoaded(boardFlash);
                }else{
                    boardTesterDatabaseListener.onFailedFlashDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(FlashTag);
            } catch (JSONException e) {
                e.printStackTrace();
                boardTesterDatabaseListener.onFailedFlashDataLoad(e);
                myRequestQueue.cancelAll(FlashTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedFlashDataLoad(error);
            myRequestQueue.cancelAll(FlashTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(FlashTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsPowersPinsVoltage(int boardID){
        ArrayList<PowerPinVoltage> boardPowerPinsVoltage = new ArrayList<>() ;
        StringWriter urlBoardConnectivity = new StringWriter();
        urlBoardConnectivity.append(UrlPrefix);
        urlBoardConnectivity.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardConnectivity.append(urlBoardPowerVoltageSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardConnectivity.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    PowerPinVoltage boardPowerPins = gson.fromJson(responseJson.toString(), PowerPinVoltage.class);
                    boardPowerPinsVoltage.add(boardPowerPins);
                }
                if(!boardPowerPinsVoltage.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onPowerPinsVoltageDataLoaded(boardPowerPinsVoltage);
                }else{
                    boardTesterDatabaseListener.onFailedPowerPinsVoltageDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(PowerPinsVoltageTag);

            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedPowerPinsVoltageDataLoad(e);
                e.printStackTrace();
                myRequestQueue.cancelAll(PowerPinsVoltageTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedPowerPinsVoltageDataLoad(error);
            myRequestQueue.cancelAll(PowerPinsVoltageTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(PowerPinsVoltageTag);
        myRequestQueue.add(stringRequest);

    }

    public void getBoardsGpioContinuityTestResult(int boardID){
        ArrayList<GpioContinuity> continuityArrayList = new ArrayList<>() ;
        StringWriter urlBoardGpioContinuity = new StringWriter();
        urlBoardGpioContinuity.append(UrlPrefix);
        urlBoardGpioContinuity.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioContinuity.append(urlBoardGpioContinuitySuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioContinuity.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                continuityArrayList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    GpioContinuity gpioContinuity = gson.fromJson(responseJson.toString(), GpioContinuity.class);
                    continuityArrayList.add(gpioContinuity);
                }
                if(!continuityArrayList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioContinuityDataLoaded(continuityArrayList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioContinuityDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioContinuityTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioContinuityDataLoad(e);
                myRequestQueue.cancelAll(GpioContinuityTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioContinuityDataLoad(error);
            myRequestQueue.cancelAll(GpioContinuityTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));

                return MyData;
            }
        };
        stringRequest.setTag(GpioContinuityTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioOutputNoPullTestResult(int boardID){
        ArrayList<GpioOutput> boGpioOutputNoPullsList = new ArrayList<>() ;
        StringWriter urlBoardGpioOutput = new StringWriter();
        urlBoardGpioOutput.append(UrlPrefix);
        urlBoardGpioOutput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioOutput.append(urlBoardGpioOutputNoPullSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioOutput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    GpioOutput gpioOutput = gson.fromJson(responseJson.toString(), GpioOutput.class);
                    boGpioOutputNoPullsList.add(gpioOutput);
                }

                if(!boGpioOutputNoPullsList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioOutputNoPullDataLoaded(boGpioOutputNoPullsList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioOutputNoPullDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioOutputNoPullTag);

            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioOutputNoPullDataLoad(e);
                e.printStackTrace();
                myRequestQueue.cancelAll(GpioOutputNoPullTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioOutputNoPullDataLoad(error);
            myRequestQueue.cancelAll(GpioOutputNoPullTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioOutputNoPullTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioOutputPullDownTestResult(int boardID){
        ArrayList<GpioOutput> boGpioOutputPullDownsList = new ArrayList<>() ;
        StringWriter urlBoardGpioOutput = new StringWriter();
        urlBoardGpioOutput.append(UrlPrefix);
        urlBoardGpioOutput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioOutput.append(urlBoardGpioOutputPullDownSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioOutput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                boGpioOutputPullDownsList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    GpioOutput gpioOutput = gson.fromJson(responseJson.toString(), GpioOutput.class);
                    boGpioOutputPullDownsList.add(gpioOutput);
                }
                if(!boGpioOutputPullDownsList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioOutputPullDownDataLoaded(boGpioOutputPullDownsList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioOutputPullDownDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioOutputPullDownTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioOutputPullDownDataLoad(e);
                e.printStackTrace();
                myRequestQueue.cancelAll(GpioOutputPullDownTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioOutputPullDownDataLoad(error);
            myRequestQueue.cancelAll(GpioOutputPullDownTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioOutputPullDownTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioOutputPullUpTestResult(int boardID){
        ArrayList<GpioOutput> boGpioOutputPullUpsList = new ArrayList<>() ;
        StringWriter urlBoardGpioOutput = new StringWriter();
        urlBoardGpioOutput.append(UrlPrefix);
        urlBoardGpioOutput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioOutput.append(urlBoardGpioOutputPullUpSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioOutput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                boGpioOutputPullUpsList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    GpioOutput gpioOutput = gson.fromJson(responseJson.toString(), GpioOutput.class);
                    boGpioOutputPullUpsList.add(gpioOutput);
                }
                if(!boGpioOutputPullUpsList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioOutputPullUpDataLoaded(boGpioOutputPullUpsList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioOutputPullUpDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioOutputPullUpTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioOutputPullUpDataLoad(e);
                e.printStackTrace();
                myRequestQueue.cancelAll(GpioOutputPullUpTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioOutputPullUpDataLoad(error);
            myRequestQueue.cancelAll(GpioOutputPullUpTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioOutputPullUpTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioInputNoPullTestResult(int boardID){
        ArrayList<GpioInput> boardMeasureInputNoPullList = new ArrayList<>() ;
        StringWriter urlBoardGpioInput = new StringWriter();
        urlBoardGpioInput.append(UrlPrefix);
        urlBoardGpioInput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioInput.append(urlBoardGpioInputNoPullSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioInput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                boardMeasureInputNoPullList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    JSONObject responseJson = jsonarray.getJSONObject(i);
                    GpioInput gpioInput = gson.fromJson(responseJson.toString(), GpioInput.class);
                    boardMeasureInputNoPullList.add(gpioInput);
                }
                if(!boardMeasureInputNoPullList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioInputNoPullDataLoaded(boardMeasureInputNoPullList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioInputNoPullDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioInputNoPullTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioInputNoPullDataLoad(e);
                myRequestQueue.cancelAll(GpioInputNoPullTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioInputNoPullDataLoad(error);
            myRequestQueue.cancelAll(GpioInputNoPullTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioInputNoPullTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioInputPullDownTestResult(int boardID){
        ArrayList<GpioInput> boardMeasureInputPullDownList = new ArrayList<>() ;
        StringWriter urlBoardGpioInput = new StringWriter();
        urlBoardGpioInput.append(UrlPrefix);
        urlBoardGpioInput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioInput.append(urlBoardGpioInputPullDownSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioInput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                boardMeasureInputPullDownList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    boardMeasureInputPullDownList.add(gson.fromJson(jsonarray.getJSONObject(i).toString(), GpioInput.class));
                }
                if(!boardMeasureInputPullDownList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioInputPullDownDataLoaded(boardMeasureInputPullDownList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioInputPullDownDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioInputPullDownTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioInputPullDownDataLoad(e);
                myRequestQueue.cancelAll(GpioInputPullDownTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioInputPullDownDataLoad(error);
            myRequestQueue.cancelAll(GpioInputPullDownTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioInputPullDownTag);
        myRequestQueue.add(stringRequest);
    }

    public void getBoardsGpioInputPullUpTestResult(int boardID){
        ArrayList<GpioInput> boardMeasureInputPullUpList = new ArrayList<>() ;
        StringWriter urlBoardGpioInput = new StringWriter();
        urlBoardGpioInput.append(UrlPrefix);
        urlBoardGpioInput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioInput.append(urlBoardGpioInputPullUpSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioInput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                boardMeasureInputPullUpList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    boardMeasureInputPullUpList.add(gson.fromJson(jsonarray.getJSONObject(i).toString(), GpioInput.class));
                }
                if(!boardMeasureInputPullUpList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioInputPullUpDataLoaded(boardMeasureInputPullUpList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioInputPullUpDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioInputPullUpTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioInputPullUpDataLoad(e);
                myRequestQueue.cancelAll(GpioInputPullUpTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioInputPullUpDataLoad(error);
            myRequestQueue.cancelAll(GpioInputPullUpTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioInputPullUpTag);
        myRequestQueue.add(stringRequest);
    }

    public void getAnalogInputTestResult(int boardID){
        ArrayList<AnalogInput> analogInputArrayList = new ArrayList<>() ;
        StringWriter urlBoardAnalogInput = new StringWriter();
        urlBoardAnalogInput.append(UrlPrefix);
        urlBoardAnalogInput.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardAnalogInput.append(urlBoardAnalogInputSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardAnalogInput.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                analogInputArrayList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    analogInputArrayList.add(gson.fromJson(jsonarray.getJSONObject(i).toString(), AnalogInput.class));
                }
                if(!analogInputArrayList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onAnalogInputDataLoaded(analogInputArrayList);
                }else{
                    boardTesterDatabaseListener.onFailedAnalogInputDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(AnalogInputTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedAnalogInputDataLoad(e);
                myRequestQueue.cancelAll(AnalogInputTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedAnalogInputDataLoad(error);
            myRequestQueue.cancelAll(AnalogInputTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(AnalogInputTag);
        myRequestQueue.add(stringRequest);
    }


    public void getGpioShortCircuitTestResult(int boardID){
        ArrayList<GpioShortCircuit> gpioShortCircuitArrayList = new ArrayList<>() ;
        StringWriter urlBoardGpioShortCircuits = new StringWriter();
        urlBoardGpioShortCircuits.append(UrlPrefix);
        urlBoardGpioShortCircuits.append(SharePreferenceHelper.getServerUrl(context));
        urlBoardGpioShortCircuits.append(urlBoardGpioShortCircuitSuffix);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, urlBoardGpioShortCircuits.toString(), response -> {
            try {
                JSONArray jsonarray = new JSONArray(response);
                gpioShortCircuitArrayList.clear();
                for(int i=0; i < jsonarray.length(); i++) {
                    gpioShortCircuitArrayList.add(gson.fromJson(jsonarray.getJSONObject(i).toString(), GpioShortCircuit.class));
                }
                if(!gpioShortCircuitArrayList.isEmpty()){
                    if (boardTesterDatabaseListener != null)
                        boardTesterDatabaseListener.onGpioShortCircuitDataLoaded(gpioShortCircuitArrayList);
                }else{
                    boardTesterDatabaseListener.onFailedGpioShortCircuitDataLoad(new Exception(ExceptionNoData));
                }
                myRequestQueue.cancelAll(GpioShortCircuitTag);
            } catch (JSONException e) {
                boardTesterDatabaseListener.onFailedGpioShortCircuitDataLoad(e);
                myRequestQueue.cancelAll(GpioShortCircuitTag);
            }
        }, error -> {
            boardTesterDatabaseListener.onFailedGpioShortCircuitDataLoad(error);
            myRequestQueue.cancelAll(GpioShortCircuitTag);
        }){
            protected Map<String, String> getParams() {
                Map<String, String> MyData = new HashMap<>();
                MyData.put(BoardIdKey, String.valueOf(boardID));
                return MyData;
            }
        };
        stringRequest.setTag(GpioShortCircuitTag);
        myRequestQueue.add(stringRequest);
    }
}
