package testResults;

import static helpers.Constants.NotTestableGpioPins;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.util.Log;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;

import java.util.ArrayList;

import adapters.GpioOutputAdapter;
import helpers.Constants;
import models.GpioOutput;

public class GpioOutputTestResultActivity extends AppCompatActivity {

    private RecyclerView recyclerView;
    private ArrayList<GpioOutput> gpioOutputNoPullArrayList;
    private ArrayList<GpioOutput> gpioOutputPullDownArrayList;
    private ArrayList<GpioOutput> gpioOutputPullUpArrayList;
    private TextView gpioOutputTestDateTime, boardNameTextView;
    private String boardName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gpio_output_test_result);
        gpioOutputNoPullArrayList = getIntent().getParcelableArrayListExtra(Constants.gpioOutputNoPullKey);
        gpioOutputPullDownArrayList = getIntent().getParcelableArrayListExtra(Constants.gpioOutputPullDownKey);
        gpioOutputPullUpArrayList = getIntent().getParcelableArrayListExtra(Constants.gpioOutputPullUpKey);
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
        TextView notTesterGpioPins = findViewById(R.id.notTestableGpioOutputPins);
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        notTesterGpioPins.setText(NotTestableGpioPins);
        recyclerView = findViewById(R.id.recyclerViewOutput);
        gpioOutputTestDateTime = findViewById(R.id.GpioOutputTestTimeView);
        boardNameTextView = findViewById(R.id.testGpioOutputBoardID);
    }

    private void initialAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        GpioOutputAdapter gpioOutputAdapter = new GpioOutputAdapter(this, gpioOutputNoPullArrayList, gpioOutputPullDownArrayList, gpioOutputPullUpArrayList);
        recyclerView.setAdapter(gpioOutputAdapter);
    }

    private void setGpioOutputTestDateTime(){
        if(!gpioOutputNoPullArrayList.isEmpty()){
            GpioOutput gpioOutput = gpioOutputNoPullArrayList.get(0);
            Log.d("gpioOutput", "gpio: " +gpioOutput.toString());
            gpioOutputTestDateTime.setText(gpioOutput.getTimestampText());
        }
    }

    void setBoardName(){
        if(boardName != null)
            boardNameTextView.setText(boardName);
    }
}