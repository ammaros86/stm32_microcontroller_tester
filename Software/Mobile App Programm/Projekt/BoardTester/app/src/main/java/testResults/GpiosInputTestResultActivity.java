package testResults;

import static helpers.Constants.NotTestableGpioPins;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;

import java.util.ArrayList;

import adapters.GpioInputAdapter;
import helpers.Constants;
import models.GpioInput;

public class GpiosInputTestResultActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private ArrayList<GpioInput> boardMeasureInputNoPullList;
    private ArrayList<GpioInput> boardMeasureInputPullDownList;
    private ArrayList<GpioInput> boardMeasureInputPullUpList;
    private TextView gpioInputTestDateTime, boardNameTextView;
    private String boardName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gpios_input_test_result);
    }
    @Override
    protected void onResume() {
        super.onResume();
        boardMeasureInputNoPullList = getIntent().getParcelableArrayListExtra(Constants.gpioInputNoPullKey);
        boardMeasureInputPullDownList = getIntent().getParcelableArrayListExtra(Constants.gpioInputPullDownKey);
        boardMeasureInputPullUpList = getIntent().getParcelableArrayListExtra(Constants.gpioInputPullUpKey);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
        initialViews();
        initialAdapter();
        setGpioInputTestDateTime();
        setBoardName();
    }

    private void initialViews(){
        TextView notTestableGpioPins = findViewById(R.id.notTestedGpioInputPins);
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        notTestableGpioPins.setText(NotTestableGpioPins);
        recyclerView = findViewById(R.id.recyclerViewInput);
        gpioInputTestDateTime = findViewById(R.id.GpioInputTestTimeView);
        boardNameTextView = findViewById(R.id.testGpioInputBoardID);
    }

    private void setGpioInputTestDateTime(){
        if(!boardMeasureInputNoPullList.isEmpty()){
            GpioInput gpioInput = boardMeasureInputNoPullList.get(0);
            gpioInputTestDateTime.setText(gpioInput.getTimestampText());

        }
    }
    private void initialAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        GpioInputAdapter gpioInputAdapter = new GpioInputAdapter(this, boardMeasureInputNoPullList, boardMeasureInputPullDownList, boardMeasureInputPullUpList);
        recyclerView.setAdapter(gpioInputAdapter);
    }

    void setBoardName(){
        if(boardName != null)
            boardNameTextView.setText(boardName);
    }
}