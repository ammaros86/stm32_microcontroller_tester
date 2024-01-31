package main;

import static helpers.Constants.ExceptionNoData;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.boardtester.R;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.HashMap;

import base.BaseActivity;
import database.AddRemoveBoardHandler;
import models.BoardTestCommands;

public class NewBoardActivity extends BaseActivity {

    private EditText editTextView;
    private Button postButton;
    private String textString;
    private ProgressBar progressBar;
    private Handler handler;
    private Runnable runnableProgressBar;
    private int currentProgressInt;
    private BottomNavigationView bottomNavigationView;
    private AddRemoveBoardHandler boardsHandler;
    private TextView addingNewBoardText;
    private ImageView connectionImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_board);
        boardsHandler = new AddRemoveBoardHandler(this, this);
    }
    @Override
    protected void onResume() {
        super.onResume();
        initialViews();
        initialListener();
        handler = new Handler(Looper.getMainLooper());
        if(bottomNavigationView != null){
            bottomNavigationView.setSelectedItemId(R.id.newBoardMenu);
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        boardsHandler.stopRequestQueue();
    }

    protected void initialViews(){
        editTextView =  findViewById(R.id.newBoardEditText);
        postButton =  findViewById(R.id.postNewBoard);
        progressBar = findViewById(R.id.progressBarNewBoard);
        bottomNavigationView=findViewById(R.id.bottomNavigationView);
        addingNewBoardText = findViewById(R.id.addingBoardTextView);
        connectionImage = findViewById(R.id.connectionImageNewBoard);
    }

    protected void initialListener(){
        initialMenuListener();
        postButton.setOnClickListener(v -> {
            textString = editTextView.getText().toString().trim();
            if (TextUtils.isEmpty(textString)) {
                Toast.makeText(this, "Invalid Board-ID", Toast.LENGTH_LONG).show();
            } else {
                waitResponse(textString);
                startProgressBarHandler();
                boardsHandler.addNewBoard(new BoardTestCommands(textString));
            }
        });
    }

    protected void waitResponse(String boardID){
        String text = "Adding new Board " + boardID;
        progressBar.setVisibility(View.VISIBLE);
        addingNewBoardText.setVisibility(View.VISIBLE);
        addingNewBoardText.setText(text);
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

    protected void responseHandler(boolean isBoardAdded){
        addingNewBoardText.setVisibility(View.VISIBLE);
        stopProgressBarHandler();
        showViewsAfterResponse();
        if(isBoardAdded){
            editTextView.setText("");
            String text = "Board-ID: " + textString +" successful added to Server";
            addingNewBoardText.setText(text);
            addingNewBoardText.setTextColor(Color.BLACK);
            Toast.makeText(this, text, Toast.LENGTH_LONG).show();
            textString = "";
        }else {
            String text = "Failed to add Board-ID: " + textString;
            addingNewBoardText.setText(text);
            addingNewBoardText.setTextColor(Color.RED);
            Toast.makeText(this, text, Toast.LENGTH_LONG).show();
        }
    }

    protected void stopProgressBarHandler(){
        if(runnableProgressBar != null)
            handler.removeCallbacks(runnableProgressBar);
    }

    protected void showViewsAfterResponse(){
        progressBar.setVisibility(View.GONE);
        postButton.setVisibility(View.VISIBLE);
        editTextView.setVisibility(View.VISIBLE);
    }
    @Override
    public void onBoardAdded(boolean isAdded){
        responseHandler(isAdded);
    }

    @Override
    public void onAddBoardError(Exception exception) {
        responseHandler(false);
    }

    @Override
    public void onErrorLoadAppCommands(Exception exception) {
        if(!exception.toString().contains(ExceptionNoData)){
            connectionImage.setImageDrawable(errorConnectionImage);
        }else{
            connectionImage.setImageDrawable(connectedImage);
        }
    }

    @Override
    public void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState) {
        connectionImage.setImageDrawable(connectedImage);
    }

    void initialMenuListener(){
        bottomNavigationView.setOnItemSelectedListener(item -> {
            int id = item.getItemId();
            if(id == R.id.home){
                finish();
                return true;
            }else if(id == R.id.newBoardMenu){
                return true;
            }else if(id == R.id.boardsList){
                finish();
                startActivity(new Intent(getApplicationContext(), BoardListActivity.class));
                overridePendingTransition(0,0);
                return true;
            }else if(id == R.id.newServer){
                finish();
                startActivity(new Intent(getApplicationContext(), NewServerConnectionActivity.class));
                overridePendingTransition(0,0);
                return true;
            }
            return true;
        });
    }
}