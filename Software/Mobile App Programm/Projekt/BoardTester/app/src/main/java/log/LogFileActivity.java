package log;

import static helpers.BoardChecker.convertLogFileName;
import static helpers.Constants.UrlPrefix;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.ImageButton;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.boardtester.R;

import java.io.StringWriter;

import helpers.SharePreferenceHelper;

public class LogFileActivity extends AppCompatActivity {

    private TextView logFileTextView, logFileNameTextView;
    String logFileName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_log_file);
        logFileName = getIntent().getStringExtra("LOG_FILE_KEY");
    }

    @Override
    protected void onResume() {
        super.onResume();
        initialViews();
        String errorFileName = "Log-File Name Error";
        if(logFileName != null){
            String logFileNameText = convertLogFileName(logFileName.replace(".txt", ""));
            logFileNameTextView.setText(logFileNameText);
            getLogFile();
        }else{
            logFileNameTextView.setText(errorFileName);
        }
    }

    void initialViews(){
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
        logFileTextView = findViewById(R.id.logFileText);
        logFileNameTextView = findViewById(R.id.logFileName);

    }

    void getLogFile(){
        RequestQueue queue = Volley.newRequestQueue(this);
        StringWriter urlText = new StringWriter();
        urlText.append(UrlPrefix);
        urlText.append(SharePreferenceHelper.getServerUrl(this));
        urlText.append("/PhpFiles/Log/readLogFile.php?filename=");
        urlText.append(logFileName);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, urlText.toString(),
                response -> {
                    logFileTextView.setText(response.toString());
                }, error -> {
            Log.d("DEBUGGER", "LogFileActivity getLogFile error: " +error.toString());
        });
        queue.add(stringRequest);
    }
}