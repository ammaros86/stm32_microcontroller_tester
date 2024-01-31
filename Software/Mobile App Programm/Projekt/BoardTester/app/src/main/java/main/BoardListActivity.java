package main;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.boardtester.R;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import adapters.BoardsListAdapter;
import base.BaseActivity;
import database.AddRemoveBoardHandler;
import models.BoardTestCommands;

public class BoardListActivity extends BaseActivity {

    private RecyclerView recyclerView;
    private BoardsListAdapter boardsAdapter;
    private ArrayList<BoardTestCommands> boardList;
    private TextView boardListText;
    private BottomNavigationView bottomNavigationView;
    private AddRemoveBoardHandler boardsHandler;
    private boolean adapterInitialised = false;
    private Button addNewBoardButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_board_list);
        boardsHandler = new AddRemoveBoardHandler(this, this);
    }
    @Override
    protected void onResume() {
        super.onResume();
        adapterInitialised = false;
        boardList = new ArrayList<>();
        initialViews();
        initialMenuListener();
    }

    @Override
    protected void onStop() {
        super.onStop();
        boardsHandler.stopRequestQueue();
    }

    void initialViews(){
        boardListText = findViewById(R.id.textBoardList);
        recyclerView = findViewById(R.id.recyclerView);
        bottomNavigationView=findViewById(R.id.bottomNavigationView);
        addNewBoardButton = findViewById(R.id.addNewBoardButton);
        if(bottomNavigationView != null){
            bottomNavigationView.setSelectedItemId(R.id.boardsList);
        }
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(linearLayoutManager);
        linearLayoutManager.setReverseLayout(true);
        linearLayoutManager.setStackFromEnd(true);
        recyclerView.removeAllViews();
    }

    private void setNewBoardButton(){
        addNewBoardButton.setVisibility(View.VISIBLE);
        addNewBoardButton.setOnClickListener(view -> {
            finish();
            startActivity(new Intent(getApplicationContext(), NewBoardActivity.class));
            overridePendingTransition(0,0);
        });
    }

    void initAdapter(HashMap<Integer, BoardTestCommands> appControllerState){
        if(!adapterInitialised){
            appControllerState.forEach((key, value) -> boardList.add(value));
            boardList.sort(Comparator.comparing(BoardTestCommands::getTimestamp));
            boardsAdapter = new BoardsListAdapter(this, boardsHandler, boardList);
            recyclerView.setAdapter(boardsAdapter);
            adapterInitialised = true;
        }
    }

    @Override
    public void onErrorLoadAppCommands(Exception exception) {
        if(!exception.toString().contains("No Data")){
            recyclerView.setVisibility(View.GONE);
        }else{
            boardListText.setText(getString(R.string.boards_list_empty));
            boardListText.setTextColor(Color.BLACK);
            recyclerView.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState) {
        if (appControllerState.isEmpty()) {
            boardListText.setText(getString(R.string.boards_list_empty));
            setNewBoardButton();
        }else{
            boardListText.setText(getString(R.string.boards_list));
            recyclerView.setVisibility(View.VISIBLE);
            boardListText.setTextColor(Color.BLACK);
            addNewBoardButton.setVisibility(View.GONE);
            initAdapter(appControllerState);
        }
    }
    @Override
    public void onRemoveBoard(boolean isRemoved, int position, int boardID) {
        if(isRemoved){
            boardsAdapter.removeAt(position);
            Toast.makeText(this, "Delete Board" + boardID + " success", Toast.LENGTH_LONG).show();
        }else{
            Toast.makeText(this, "Delete Board " + boardID + " failed", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onRemoveBoardError(Exception exception, int boardID) {
        Toast.makeText(this, "Delete Board " + boardID + " failed cause: " + exception.toString(), Toast.LENGTH_LONG).show();
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