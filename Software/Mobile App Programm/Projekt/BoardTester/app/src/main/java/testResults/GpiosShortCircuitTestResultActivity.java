package testResults;

import static helpers.Constants.NotTestableGpioShort;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;

import java.util.ArrayList;

import adapters.GpioShortCircuitAdapter;
import helpers.Constants;
import models.GpioShortCircuit;

public class GpiosShortCircuitTestResultActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private ArrayList<GpioShortCircuit> gpioShortCircuitArrayList;
    private TextView gpioShortCircuitTestDateTime, boardNameTextView;
    private String boardName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gpios_short_circuit_test_result);
        gpioShortCircuitArrayList = getIntent().getParcelableArrayListExtra(Constants.gpioShortCircuitKey);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
    }

    @Override
    protected void onResume() {
        super.onResume();
        initialViews();
        initialAdapter();
        setGpioOutputTestDateTime();
        setBoardName();
    }

    protected void initialViews(){
        TextView notTesterGpioPins = findViewById(R.id.notTestableGpioShort);
        ImageButton backButton = findViewById(R.id.backButtonShort);
        backButton.setOnClickListener(v -> onBackPressed());
        notTesterGpioPins.setText(NotTestableGpioShort);
        recyclerView = findViewById(R.id.recyclerViewGpioShort);
        gpioShortCircuitTestDateTime = findViewById(R.id.GpioShortCircuitTestTimeView);
        boardNameTextView = findViewById(R.id.testGpiosShortBoardID);
    }

    private void initialAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        GpioShortCircuitAdapter gpioShortCircuitAdapter = new GpioShortCircuitAdapter(this, gpioShortCircuitArrayList);
        recyclerView.setAdapter(gpioShortCircuitAdapter);
    }

    private void setGpioOutputTestDateTime(){
        if(!gpioShortCircuitArrayList.isEmpty()){
            GpioShortCircuit gpioShortCircuit = gpioShortCircuitArrayList.get(0);
            gpioShortCircuitTestDateTime.setText(gpioShortCircuit.getTimestampText());
        }
    }
    void setBoardName(){
        if(boardName != null)
            boardNameTextView.setText(boardName);
    }
}