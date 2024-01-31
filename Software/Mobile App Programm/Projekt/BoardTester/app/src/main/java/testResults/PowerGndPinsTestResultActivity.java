package testResults;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;

import java.util.ArrayList;

import adapters.PowerGndPinsAdapter;
import helpers.Constants;
import models.PowerPinVoltage;

public class PowerGndPinsTestResultActivity extends AppCompatActivity {
    private RecyclerView recyclerView;
    private ArrayList<PowerPinVoltage> powerPinsVoltageArray;
    private TextView nucleoBoardNameTextView;
    private TextView powerGndTestDate;
    private String boardName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_power_gnd_pins_test_result);
        powerPinsVoltageArray = getIntent().getParcelableArrayListExtra(Constants.powerPinsVoltageKey);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
    }

    @Override
    protected void onResume() {
        super.onResume();
        initialViews();
        initialAdapters();
        if(powerPinsVoltageArray.get(0) != null){
            powerGndTestDate.setText(powerPinsVoltageArray.get(0).getTimestampText());
            setBoardName();
        }
    }

    private void initialViews(){
        recyclerView = findViewById(R.id.recyclerViewPower);
        nucleoBoardNameTextView = findViewById(R.id.boardNameView);
        powerGndTestDate = findViewById(R.id.PowerGndTestTimeView);
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        LinearLayoutManager layoutManager1 = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager1);
    }

    private void initialAdapters(){
        PowerGndPinsAdapter powerPinsAdapter = new PowerGndPinsAdapter(this, powerPinsVoltageArray);
        recyclerView.setAdapter(powerPinsAdapter);
    }

    void setBoardName(){
        if(!powerPinsVoltageArray.isEmpty()){
            if(boardName != null)
                nucleoBoardNameTextView.setText(boardName);
        }
    }
}