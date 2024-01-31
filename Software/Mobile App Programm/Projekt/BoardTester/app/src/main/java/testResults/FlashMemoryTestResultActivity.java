package testResults;

import static helpers.Constants.FlashTestDone;
import static helpers.Constants.UploadTestProgramSuccess;
import static helpers.Constants.flashKey;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.content.res.AppCompatResources;

import android.annotation.SuppressLint;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.boardtester.R;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

import helpers.Constants;
import models.BoardFlash;

public class FlashMemoryTestResultActivity extends AppCompatActivity {

    private TextView flashInfoText;
    private TextView boardNameTextView, flashTimeTextView;
    private BoardFlash currentBoardFlashInfo;
    private Drawable okImage;
    private Drawable errorImage;
    private ImageView flashProgramStateImage;
    private ImageView flashErrorsImage;
    private TextView flashProgramStateText;
    private TextView flashErrorsText;

    private TextView flashInformationTitle;
    private String boardName;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_flash_memory_test_result);
        currentBoardFlashInfo = (BoardFlash)getIntent().getSerializableExtra(flashKey);
        boardName = getIntent().getStringExtra(Constants.BOARD_NAME_KEY);
    }

    @Override
    protected void onResume() {
        super.onResume();
        okImage =  AppCompatResources.getDrawable(this, R.drawable.ic_baseline_check_24);
        errorImage =  AppCompatResources.getDrawable(this, R.drawable.ic_baseline_close_24);
        initialViews();
        checkFlashState();
    }

    private void initialViews(){
        flashInfoText = findViewById(R.id.flashInfoTextView);
        boardNameTextView = findViewById(R.id.testFlashBoardID);
        flashTimeTextView = findViewById(R.id.flashTimeView);
        flashProgramStateImage = findViewById(R.id.uploadProgramImage);
        flashErrorsImage = findViewById(R.id.errorsImage);
        flashProgramStateText = findViewById(R.id.uploadProgramState);
        flashErrorsText = findViewById(R.id.flashMemoryErrors);
        flashInformationTitle = findViewById(R.id.flashInformationTitle);
        ImageButton backButton = findViewById(R.id.backButton);
        backButton.setOnClickListener(v -> onBackPressed());
    }

    private void checkFlashState(){
        setBoardName();
        if(currentBoardFlashInfo.getUpload_progrgamm() == UploadTestProgramSuccess){
            flashProgramStateImage.setImageDrawable(okImage);
            String message = "Ok";
            flashProgramStateText.setText(message);
            setFlashResultData();
        }else{
            flashProgramStateImage.setImageDrawable(errorImage);
            String message = "failed";
            flashProgramStateText.setText(message);
        }
        if(currentBoardFlashInfo.getFlash_test() == FlashTestDone){
            String message;
            if(currentBoardFlashInfo.getFlash_errors() == 0){
                message = "0";
                flashErrorsImage.setImageDrawable(okImage);
            }else{
                message = Integer.toString(currentBoardFlashInfo.getFlash_errors());
                flashErrorsImage.setImageDrawable(errorImage);
            }
            flashErrorsText.setText(message);
        }else{
            String message = "failed";
            flashErrorsImage.setImageDrawable(errorImage);
            flashErrorsText.setText(message);
        }
    }

    private void setFlashResultData(){
        Timestamp timestamp = currentBoardFlashInfo.getTimestamp();
        flashInformationTitle.setVisibility(View.VISIBLE);
        flashInfoText.setText(getFlashInformation());
        Date date = new Date();
        date.setTime(timestamp.getTime());
        @SuppressLint("SimpleDateFormat") String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        flashTimeTextView.setText(formattedDate);
    }

    private String getFlashInformation(){
        String flashError = "Flash Read Information Error";
        String textFlash = currentBoardFlashInfo.getComment();
        if(textFlash != null){
            return textFlash;
        }else{
            return flashError;
        }
    }

    void setBoardName(){
        if(boardName != null)
            boardNameTextView.setText(boardName);
    }

}