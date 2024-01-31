package main;

import static helpers.Constants.ExceptionNoData;
import static helpers.Constants.newServerWaitText;
import static helpers.Constants.waitText;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.boardtester.R;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import base.BaseActivity;
import helpers.SharePreferenceHelper;
import models.BoardTestCommands;

public class NewServerConnectionActivity extends BaseActivity {
    private TextView editTextView;
    private TextView waitTextView;
    private TextView lastSavedAddressTextView;
    private Button postButton;
    private String serverIpText, lastSavedIpAddressText;
    private ProgressBar progressBar;
    private Handler handler;
    private Runnable runnableProgressBar;
    int currentProgressInt;
    private BottomNavigationView bottomNavigationView;
    boolean isNewConfiguration = false;
    private ImageView connectionImage;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_server_connection);
    }

    @Override
    protected void onResume() {
        super.onResume();
        lastSavedIpAddressText =  SharePreferenceHelper.getServerUrl(this);
        initialViews();
        initialListener();
        handler = new Handler(Looper.getMainLooper());
        initialMenuListener();
    }

    @Override
    protected void onStop() {
        super.onStop();
        stopProgressBarHandler();
    }

    protected void initialViews(){
        waitTextView =  findViewById(R.id.waitText);
        editTextView =  findViewById(R.id.ipAdressEditText);
        postButton =  findViewById(R.id.postIpAdress);
        progressBar = findViewById(R.id.progressBarNewServerBoard);
        connectionImage = findViewById(R.id.connectionImageNewServer);
        bottomNavigationView=findViewById(R.id.bottomNavigationView);
        bottomNavigationView.setSelectedItemId(R.id.newServer);
        lastSavedAddressTextView = findViewById(R.id.lastSavedIp);
        if(!lastSavedIpAddressText.equals("")){
            lastSavedAddressTextView.setVisibility(View.VISIBLE);
            lastSavedIpAddressText = "Last saved IP-Address: " + lastSavedIpAddressText;
            lastSavedAddressTextView.setText(lastSavedIpAddressText);
        }
    }

    protected void initialListener(){
        postButton.setOnClickListener(v -> {
            serverIpText = editTextView.getText().toString().trim();
            if (TextUtils.isEmpty(serverIpText)) {
                Toast.makeText(this, "Invalid IP-Address", Toast.LENGTH_LONG).show();
            }else {
                lastSavedAddressTextView.setVisibility(View.GONE);
                checkDataAndPost();
            }
        });
    }

    protected void checkDataAndPost(){
        String text = newServerWaitText+ serverIpText + waitText;
        waitTextView.setText(text);
        if(!isValidIPAddress(serverIpText)){
            Toast.makeText(this, serverIpText + ": Invalid IP-Address", Toast.LENGTH_LONG).show();
        }else{
            waitResponse();
            startProgressBarHandler();
            SharePreferenceHelper.removeUrl(this);
            SharePreferenceHelper.setServerUrl(this, serverIpText);
            isNewConfiguration = true;
            super.restartAppCommandHandler();
        }
    }

    public boolean isValidIPAddress(String ip) {
        String zeroTo255 = "(\\d{1,2}|([01])\\" + "d{2}|2[0-4]\\d|25[0-5])";
        String regex = zeroTo255 + "\\." + zeroTo255 + "\\." + zeroTo255 + "\\." + zeroTo255; // Compile the ReGex
        Pattern p = Pattern.compile(regex);
        if (ip == null) {
            return false;
        }
        Matcher m = p.matcher(ip);
        return m.matches();
    }

    protected void waitResponse(){
        waitTextView.setVisibility(View.VISIBLE);
        progressBar.setVisibility(View.VISIBLE);
        postButton.setVisibility(View.GONE);
        editTextView.setVisibility(View.GONE);
    }

    protected void startProgressBarHandler() {
        final int delay = 500; // 1000 milliseconds == 1 second
        progressBar.setMax(100);
        currentProgressInt = 0;
        handler.postDelayed(runnableProgressBar = () -> {
            currentProgressInt +=10;
            progressBar.setProgress(currentProgressInt);
            if(currentProgressInt == 100){
                currentProgressInt = 0;
            }
            handler.postDelayed(runnableProgressBar, delay);
        }, delay);
    }

    protected void stopProgressBarHandler(){
        if(runnableProgressBar != null)
            handler.removeCallbacks(runnableProgressBar);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        stopProgressBarHandler();
    }

    protected void failedResponse(){
        progressBar.setVisibility(View.GONE);
        waitTextView.setVisibility(View.GONE);
        postButton.setVisibility(View.VISIBLE);
        editTextView.setVisibility(View.VISIBLE);
    }

    protected void responseHandler(boolean isConnected) {
        if(isNewConfiguration){
            String text = "Connected to Server IP-Address " + serverIpText;
            if (isConnected) {
                stopProgressBarHandler();
                Toast.makeText(this, text, Toast.LENGTH_LONG).show();
                progressBar.setVisibility(View.GONE);
                lastSavedIpAddressText =  SharePreferenceHelper.getServerUrl(this);
                String textSuccess = newServerWaitText+ serverIpText + " \n Success";
                waitTextView.setText(textSuccess);
            }else{
                stopProgressBarHandler();
                failedResponse();
                text += " failed";
                Toast.makeText(this, text, Toast.LENGTH_LONG).show();
                progressBar.setVisibility(View.GONE);
            }
            isNewConfiguration = false;
        }
    }

    @Override
    public void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState) {
        super.onSuccessLoadedAppCommands(appControllerState);
        responseHandler(true);
        connectionImage.setImageDrawable(connectedImage);
    }

    @Override
    public void onErrorLoadAppCommands(Exception exception) {
        super.onErrorLoadAppCommands(exception);
        responseHandler(false);
        if(!exception.toString().contains(ExceptionNoData)){
            connectionImage.setImageDrawable(errorConnectionImage);
        }else{
            connectionImage.setImageDrawable(connectedImage);
        }
    }

    void initialMenuListener(){
        bottomNavigationView.setOnItemSelectedListener(item -> {
            int id = item.getItemId();
            if(id == R.id.home){
                finish();
                return true;
            }else if(id == R.id.newBoardMenu){
                finish();
                startActivity(new Intent(getApplicationContext(), NewBoardActivity.class));
                overridePendingTransition(0,0);
                return true;
            }else if(id == R.id.boardsList){
                finish();
                startActivity(new Intent(getApplicationContext(), BoardListActivity.class));
                overridePendingTransition(0,0);
                return true;
            }else if(id == R.id.newServer){
                return true;
            }
            return true;
        });
    }


}