package testResults;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;

import com.example.boardtester.R;

import java.util.ArrayList;

import adapters.GpioContinuityAdapter;
import helpers.Constants;
import models.GpioContinuity;

public class GpiosContinuityTestResultActivity extends AppCompatActivity {
    private RecyclerView recyclerView;
    private TextView gpioContinuityTestDateTime, boardNameTextView;
    private ArrayList<GpioContinuity> gpioContinuityArrayList;
    private String boardName;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gpios_continuity_test_result);
        gpioContinuityArrayList = getIntent().getParcelableArrayListExtra(Constants.gpioContinuityKey);
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
        gpioContinuityTestDateTime = findViewById(R.id.GpioContinuityTestTimeView);
        boardNameTextView = findViewById(R.id.boardNameView);
    }

    private void setGpioContinuityTestDateTime(){
        if(!gpioContinuityArrayList.isEmpty()){
            GpioContinuity gpioContinuity = gpioContinuityArrayList.get(0);
            gpioContinuityTestDateTime.setText(gpioContinuity.getTimestampText());
        }
    }

    private void initialAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        GpioContinuityAdapter gpioInputAdapter = new GpioContinuityAdapter(this, gpioContinuityArrayList);
        recyclerView.setAdapter(gpioInputAdapter);
    }
    void setBoardName(){
        if(!gpioContinuityArrayList.isEmpty()){
            if(boardName != null)
                boardNameTextView.setText(boardName);
        }
    }
}