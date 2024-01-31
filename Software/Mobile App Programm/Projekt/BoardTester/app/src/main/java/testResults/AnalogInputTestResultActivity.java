package testResults;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;
import adapters.AnalogInputAdapter;

import java.util.ArrayList;

import helpers.Constants;
import models.AnalogInput;

public class AnalogInputTestResultActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private TextView analogInputTestDateTime, boardNameTextView;
    private ArrayList<AnalogInput> analogInputArrayList;

    private String boardName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_analog_input_test_result);
        analogInputArrayList = getIntent().getParcelableArrayListExtra(Constants.analogInputKey);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
        initialViews();
        initialAdapter();
        setGpioContinuityTestDateTime();
        setBoardName();
    }

    private void initialViews(){
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        recyclerView = findViewById(R.id.recyclerViewContinuity);
        analogInputTestDateTime = findViewById(R.id.analogInputTestTimeView);
        boardNameTextView = findViewById(R.id.testAnalogInputBoardID);
    }

    private void setGpioContinuityTestDateTime(){
        if(!analogInputArrayList.isEmpty()){
            AnalogInput analogInput = analogInputArrayList.get(0);
            analogInputTestDateTime.setText(analogInput.getTimestampText());
        }
    }

    private void initialAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        AnalogInputAdapter analogInputAdapter = new AnalogInputAdapter(this, analogInputArrayList);
        recyclerView.setAdapter(analogInputAdapter);
    }

    void setBoardName(){
        if(boardName != null)
            boardNameTextView.setText(boardName);
    }
}