package log;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.Toast;

import com.example.boardtester.R;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import adapters.LogFilesAdapter;
import interfaces.RemoveLogListener;

public class ListLogFiles extends AppCompatActivity implements RemoveLogListener {

    private RecyclerView recyclerView;
    private ArrayList<String> LogArrayList;
    private RemoveLogHandler removeLogHandler;
    private LogFilesAdapter logFilesAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_log_files);
        removeLogHandler = new RemoveLogHandler(this, this);
        LogArrayList = new ArrayList<>();
        Intent intent = getIntent();
        LogArrayList = intent.getStringArrayListExtra("logArrayList");
        initialViews();
        setLogFilesAdapter();
    }

    @Override
    protected void onResume() {
        super.onResume();

    }

    void initialViews(){
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        recyclerView = findViewById(R.id.recyclerViewLogFiles);
    }

    void setLogFilesAdapter(){
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false);
        recyclerView.setLayoutManager(layoutManager);
        if(!LogArrayList.isEmpty()){
            ArrayList<String> sortedLogFiles = sortLogFiles(LogArrayList);
            logFilesAdapter = new LogFilesAdapter(this, removeLogHandler, sortedLogFiles);
        }
        recyclerView.setAdapter(logFilesAdapter);
    }

    @Override
    public void onRemoveBoard(String response, int position) {

        if(response.equals("success")){
            Toast.makeText(this, "Log File removed", Toast.LENGTH_SHORT).show();
            logFilesAdapter.removeAt(position);

        }else{
            Toast.makeText(this, "Failed cause: " + response, Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onRemoveBoardFailed(Exception exception) {
        Toast.makeText(this, "Failed cause: " + exception.toString(), Toast.LENGTH_LONG).show();
    }


    public static ArrayList<String> sortLogFiles(ArrayList<String> fileNames) {
        SimpleDateFormat sdf = new SimpleDateFormat("dd_MM_yyyy_HH_mm_ss", Locale.getDefault());

        fileNames.sort((logFile1, logFile2) -> {
            try {
                String logFile1Text = logFile1.replace(".txt", "");
                int indexOfUnderScore1 = logFile1Text.indexOf("_");
                Date date1 = sdf.parse(logFile1Text.substring(indexOfUnderScore1 + 1, indexOfUnderScore1 + 19));

                String logFile2Text = logFile2.replace(".txt", "");
                int indexOfUnderScore2 = logFile2Text.indexOf("_");
                Date date2 = sdf.parse(logFile2Text.substring(indexOfUnderScore2 + 1, indexOfUnderScore2 + 19));

                return date2 != null ? date2.compareTo(date1) : 0;
            } catch (ParseException e) {
                e.printStackTrace();
                return 0;
            }
        });
        return fileNames;
    }
}