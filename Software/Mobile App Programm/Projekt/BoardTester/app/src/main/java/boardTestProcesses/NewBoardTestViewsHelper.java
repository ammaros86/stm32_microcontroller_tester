package boardTestProcesses;

import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.appcompat.content.res.AppCompatResources;

import com.example.boardtester.R;

public class NewBoardTestViewsHelper {
    private final Activity mActivity;
    private ImageView boardConnectionImage;
    private ImageView gndConnectionImage;
    private ImageView powerPinsImage;

    private ImageView arrowCurrent;
    private ImageView arrowTemperature;
    private ImageView arrowFlash;
    private ImageView arrowPower;
    private ImageView arrowContinuity;
    private ImageView arrowOutput;
    private ImageView arrowInput;
    private ImageView arrowAnalogInput;
    private ImageView arrowShortCircuit;
    private ImageView arrowCompleteTest;


    private RelativeLayout relativeLayoutCurrent;
    private RelativeLayout relativeLayoutTemperature;
    private RelativeLayout relativeLayoutFlash;
    private RelativeLayout relativeLayoutPowerPins;
    private RelativeLayout relativeLayoutContinuity;
    private RelativeLayout relativeLayoutGpioOutput;
    private RelativeLayout relativeLayoutGpioInput;
    private RelativeLayout relativeLayoutAnalogInput;
    private RelativeLayout relativeLayoutGpioShortCircuit;
    private RelativeLayout relativeCompleteTest;
    private RelativeLayout flashInformation;
    private RelativeLayout powerPinsInformation;
    private RelativeLayout gpioContinuityInfo;
    private RelativeLayout  currentMeasureInfo;
    private RelativeLayout gpioOutputInfo;
    private RelativeLayout analogInputInfo;
    private RelativeLayout gpioShortCircuitInfo;
    private RelativeLayout gpioInputInfo;
    private RelativeLayout boardConnectionRelative;
    private RelativeLayout gndConnectionRelative;
    private RelativeLayout powerPinsRelative;

    private TextView boardIDTextView;
    private TextView temperatureTextView;

    private TextView currentTextView;
    private TextView currentMeasureInfoTextView;

    private TextView flashTextView;
    private TextView powerPinsTextView;
    private TextView powerPinsTestResult;
    private TextView gpioContinuityTestTextView;
    private TextView gpioOutputTestTextView;
    private TextView gpioInputTestTextView;
    private TextView analogInputTestTextView;
    private TextView gpioShortCircuitTestTextView;
    private TextView completeTestText;
    private TextView boardConnectionText;
    private TextView gndConnectionTextView;
    private ProgressBar temperatureProgressBar;

    private ProgressBar currentMeasureProgressBar;
    private ProgressBar flashProgressBar;
    private ProgressBar powerPinsProgressBar;
    private ProgressBar continuityProgressBar;
    private ProgressBar gpioOutputTestProgressBar;
    private ProgressBar gpioInputTestProgressBar;
    private ProgressBar analogInputTestProgressBar;
    private ProgressBar gpioShortCircuitBar;

    private Drawable greyArrow, redArrow, greenArrow, okImage, errorImage;

    public NewBoardTestViewsHelper(Activity activity) {
        mActivity = activity;
        initializeViews();
    }
    protected void initializeViews(){
        setBoardIDTextView(mActivity.findViewById(R.id.boardID));
        setRelativeLayoutPowerPins(mActivity.findViewById(R.id.relativeViewConnectivity));
        setRelativeLayoutFlash(mActivity.findViewById(R.id.relativeViewFlash));
        setRelativeLayoutTemperature(mActivity.findViewById(R.id.relativeViewTemperature));
        setRelativeLayoutContinuity(mActivity.findViewById(R.id.relativeViewContinuity));
        setRelativeLayoutGpioInput(mActivity.findViewById(R.id.relativeViewGpioInput));
        setRelativeLayoutGpioOutput(mActivity.findViewById(R.id.relativeViewGpioOutput));
        setRelativeLayoutAnalogInput(mActivity.findViewById(R.id.relativeViewAnalogInput));
        setRelativeLayoutGpioShortCircuit(mActivity.findViewById(R.id.relativeViewGpioShortCircuit));
        setRelativeLayoutCurrent(mActivity.findViewById(R.id.relativeViewCurrentMeasure));

        setArrowPower(mActivity.findViewById(R.id.backImageBoardPowerPins));
        setArrowFlash(mActivity.findViewById(R.id.backImageBoardFlash));
        setArrowTemperature(mActivity.findViewById(R.id.backImageBoardTemperatur));
        setArrowContinuity(mActivity.findViewById(R.id.backImageBoardGPIOContinuityTest));
        setArrowInput(mActivity.findViewById(R.id.backImageBoardGPIOInputTest));
        setArrowOutput(mActivity.findViewById(R.id.backImageBoardGPIOOutputTest));
        setArrowAnalogInput(mActivity.findViewById(R.id.backImageBoardAnalogInputTest));
        setArrowShortCircuit(mActivity.findViewById(R.id.backImageBoardGPIOShortCircuitTest));
        setArrowCurrent(mActivity.findViewById(R.id.backImageCurrentMeasureTest));


        setPowerPinsProgressBar(mActivity.findViewById(R.id.progressBarPowerPin));
        setTemperatureProgressBar(mActivity.findViewById(R.id.progressBarTemp));
        setFlashProgressBar(mActivity.findViewById(R.id.progressBar));
        setContinuityProgressBar(mActivity.findViewById(R.id.progressBarContinuity));
        setGpioInputTestProgressBar(mActivity.findViewById(R.id.progressBarInput));
        setGpioOutputTestProgressBar(mActivity.findViewById(R.id.progressBarOutput));
        setAnalogInputTestProgressBar(mActivity.findViewById(R.id.progressBarAnalogInput));
        setGpioShortCircuitBar(mActivity.findViewById(R.id.progressBarShortCircuit));
        setCurrentMeasureProgressBar(mActivity.findViewById(R.id.progressBarCurrentMeasure));

        setPowerPinsInformation(mActivity.findViewById(R.id.relativeViewPowerPinsInfo));
        setFlashInformation(mActivity.findViewById(R.id.relativeViewFlashInformation));
        setGpioInputInfo(mActivity.findViewById(R.id.relativeViewGpioInputMeasureInfo));
        setGpioOutputInfo(mActivity.findViewById(R.id.relativeViewGpioOutputMeasureInfo));
        setAnalogInputInfo(mActivity.findViewById(R.id.relativeViewAnalogInputMeasureInfo));
        setGpioShortCircuitInfo(mActivity.findViewById(R.id.relativeViewGpioShortCircuitResult));
        setGpioContinuityInfo(mActivity.findViewById(R.id.relativeViewContinuityInfo));
        setCurrentMeasureInfo(mActivity.findViewById(R.id.relativeViewCurrentInfo));

        setPowerPinsTextView(mActivity.findViewById(R.id.powerPinsTextTitle));
        setFlashTextView(mActivity.findViewById(R.id.flashState));
        setTemperatureTextView(mActivity.findViewById(R.id.temperature));
        setGpioContinuityTestTextView(mActivity.findViewById(R.id.gpioContinuityTestState));
        setGpioInputTestTextView(mActivity.findViewById(R.id.gpioInputTestState));
        setGpioOutputTestTextView(mActivity.findViewById(R.id.gpioOutputTestState));
        setAnalogInputTestTextView(mActivity.findViewById(R.id.analogInputTestState));
        setGpioShortCircuitTestTextView(mActivity.findViewById(R.id.gpioShortCircuitTestState));
        setCurrentTextView(mActivity.findViewById(R.id.currentMeasureTestState));
        setCurrentMeasureInfoTextView(mActivity.findViewById(R.id.currentResultText));
        setRelativeCompleteTest(mActivity.findViewById(R.id.relativeViewCompleteTest));
        setCompleteTestText(mActivity.findViewById(R.id.boardCompleteTestText));
        setBoardConnectionRelative(mActivity.findViewById(R.id.boardConnectionRelative));
        setGndConnectionRelative(mActivity.findViewById(R.id.gndTestRelative));
        setPowerPinsRelative(mActivity.findViewById(R.id.powerPinsTestRelative));
        setBoardConnectionImage(mActivity.findViewById(R.id.boardConnectionImage));
        setGndConnectionImage(mActivity.findViewById(R.id.gndTestImage));
        setPowerPinsImage(mActivity.findViewById(R.id.powerPinsImage));
        setBoardConnectionText(mActivity.findViewById(R.id.boardConnectionText));
        setGndConnectionTextView(mActivity.findViewById(R.id.gndTestText));
        setPowerPinsTestResult(mActivity.findViewById(R.id.powerPinsResultText));

        setArrowCompleteTest(mActivity.findViewById(R.id.backImageCompleteTest));

        setGreyArrow(AppCompatResources.getDrawable(mActivity, R.drawable.ic_keyboard_arrow_right_black_24dp));
        setRedArrow(AppCompatResources.getDrawable(mActivity, R.drawable.ic_keyboard_arrow_right_red_24dp));
        setGreenArrow(AppCompatResources.getDrawable(mActivity, R.drawable.ic_keyboard_arrow_right_green_24dp));
        setOkImage(AppCompatResources.getDrawable(mActivity, R.drawable.ic_baseline_check_24));
        setErrorImage(AppCompatResources.getDrawable(mActivity, R.drawable.ic_baseline_close_24));

        getGpioOutputInfo().setVisibility(View.GONE);
        getGpioInputInfo().setVisibility(View.GONE);
        getAnalogInputInfo().setVisibility(View.GONE);
        ImageButton backButton = mActivity.findViewById(R.id.backButtonTestActivity);
        backButton.setOnClickListener(v -> mActivity.onBackPressed());
    }

    public void setCompleteInitialViews(){
        getArrowCompleteTest().setImageDrawable(getGreyArrow());

        getArrowPower().setImageDrawable(getGreyArrow());
        getBoardConnectionRelative().setVisibility(View.GONE);
        getGndConnectionRelative().setVisibility(View.GONE);
        getPowerPinsRelative().setVisibility(View.GONE);
        getPowerPinsInformation().setVisibility(View.GONE);
        getPowerPinsTextView().setText("");

        getArrowFlash().setImageDrawable(getGreyArrow());
        getFlashInformation().setVisibility(View.GONE);
        getFlashTextView().setText("");

        getArrowTemperature().setImageDrawable(getGreyArrow());
        getTemperatureTextView().setText("");

        getArrowInput().setImageDrawable(getGreyArrow());
        getGpioInputInfo().setVisibility(View.GONE);
        getGpioInputTestTextView().setText("");

        getArrowOutput().setImageDrawable(getGreyArrow());
        getGpioOutputInfo().setVisibility(View.GONE);
        getGpioOutputTestTextView().setText("");

        getArrowAnalogInput().setImageDrawable(getGreyArrow());
        getAnalogInputInfo().setVisibility(View.GONE);
        getAnalogInputTestTextView().setText("");

        getArrowShortCircuit().setImageDrawable(getGreyArrow());
        getGpioShortCircuitInfo().setVisibility(View.GONE);
        getGpioShortCircuitTestTextView().setText("");

        getArrowCurrent().setImageDrawable(getGreyArrow());
        getCurrentMeasureInfo().setVisibility(View.GONE);
        getCurrentMeasureInfoTextView().setText("");
        getCurrentTextView().setText("");
    }

    public ImageView getArrowContinuity() {
        return arrowContinuity;
    }

    public void setArrowContinuity(ImageView arrowContinuity) {
        this.arrowContinuity = arrowContinuity;
    }

    public RelativeLayout getRelativeLayoutContinuity() {
        return relativeLayoutContinuity;
    }

    public void setRelativeLayoutContinuity(RelativeLayout relativeLayoutContinuity) {
        this.relativeLayoutContinuity = relativeLayoutContinuity;
    }

    public RelativeLayout getGpioContinuityInfo() {
        return gpioContinuityInfo;
    }

    public void setGpioContinuityInfo(RelativeLayout gpioContinuityInfo) {
        this.gpioContinuityInfo = gpioContinuityInfo;
    }
    public RelativeLayout getCurrentMeasureInfo() {
        return currentMeasureInfo;
    }

    public void setCurrentMeasureInfo(RelativeLayout currentMeasureInfo) {
        this.currentMeasureInfo = currentMeasureInfo;
    }
    public TextView getGpioContinuityTestTextView() {
        return gpioContinuityTestTextView;
    }

    public void setGpioContinuityTestTextView(TextView gpioContinuityTestTextView) {
        this.gpioContinuityTestTextView = gpioContinuityTestTextView;
    }

    public ProgressBar getContinuityProgressBar() {
        return continuityProgressBar;
    }

    public void setContinuityProgressBar(ProgressBar continuityProgressBar) {
        this.continuityProgressBar = continuityProgressBar;
    }
    public TextView getPowerPinsTestResult() {
        return powerPinsTestResult;
    }

    public void setPowerPinsTestResult(TextView powerPinsTestResult) {
        this.powerPinsTestResult = powerPinsTestResult;
    }

    public ImageView getBoardConnectionImage() {
        return boardConnectionImage;
    }

    public void setBoardConnectionImage(ImageView boardConnectionImage) {
        this.boardConnectionImage = boardConnectionImage;
    }

    public ImageView getGndConnectionImage() {
        return gndConnectionImage;
    }

    public void setGndConnectionImage(ImageView gndConnectionImage) {
        this.gndConnectionImage = gndConnectionImage;
    }

    public ImageView getPowerPinsImage() {
        return powerPinsImage;
    }

    public void setPowerPinsImage(ImageView powerPinsImage) {
        this.powerPinsImage = powerPinsImage;
    }

    public ImageView getArrowCurrent() {
        return arrowCurrent;
    }

    public void setArrowCurrent(ImageView arrowCurrent) {
        this.arrowCurrent = arrowCurrent;
    }
    public ImageView getArrowTemperature() {
        return arrowTemperature;
    }

    public void setArrowTemperature(ImageView arrowTemperature) {
        this.arrowTemperature = arrowTemperature;
    }

    public ImageView getArrowFlash() {
        return arrowFlash;
    }

    public void setArrowFlash(ImageView arrowFlash) {
        this.arrowFlash = arrowFlash;
    }

    public ImageView getArrowPower() {
        return arrowPower;
    }

    public void setArrowPower(ImageView arrowPower) {
        this.arrowPower = arrowPower;
    }

    public ImageView getArrowOutput() {
        return arrowOutput;
    }

    public void setArrowOutput(ImageView arrowOutput) {
        this.arrowOutput = arrowOutput;
    }

    public ImageView getArrowInput() {
        return arrowInput;
    }

    public void setArrowInput(ImageView arrowInput) {
        this.arrowInput = arrowInput;
    }
    public ImageView getArrowShortCircuit() {
        return arrowShortCircuit;
    }

    public void setArrowShortCircuit(ImageView arrowShortCircuit) {
        this.arrowShortCircuit = arrowShortCircuit;
    }
    public RelativeLayout getRelativeLayoutCurrent() {
        return relativeLayoutCurrent;
    }

    public void setRelativeLayoutCurrent(RelativeLayout relativeLayoutCurrent) {
        this.relativeLayoutCurrent = relativeLayoutCurrent;
    }
    public RelativeLayout getRelativeLayoutTemperature() {
        return relativeLayoutTemperature;
    }

    public void setRelativeLayoutTemperature(RelativeLayout relativeLayoutTemperature) {
        this.relativeLayoutTemperature = relativeLayoutTemperature;
    }

    public RelativeLayout getRelativeLayoutFlash() {
        return relativeLayoutFlash;
    }

    public void setRelativeLayoutFlash(RelativeLayout relativeLayoutFlash) {
        this.relativeLayoutFlash = relativeLayoutFlash;
    }

    public RelativeLayout getRelativeLayoutPowerPins() {
        return relativeLayoutPowerPins;
    }

    public void setRelativeLayoutPowerPins(RelativeLayout relativeLayoutPowerPins) {
        this.relativeLayoutPowerPins = relativeLayoutPowerPins;
    }

    public RelativeLayout getRelativeLayoutGpioOutput() {
        return relativeLayoutGpioOutput;
    }

    public void setRelativeLayoutGpioOutput(RelativeLayout relativeLayoutGpioOutput) {
        this.relativeLayoutGpioOutput = relativeLayoutGpioOutput;
    }

    public RelativeLayout getRelativeLayoutGpioShortCircuit() {
        return relativeLayoutGpioShortCircuit;
    }

    public void setRelativeLayoutGpioShortCircuit(RelativeLayout relativeLayoutGpioShortCircuit) {
        this.relativeLayoutGpioShortCircuit = relativeLayoutGpioShortCircuit;
    }
    public RelativeLayout getRelativeLayoutGpioInput() {
        return relativeLayoutGpioInput;
    }

    public void setRelativeLayoutGpioInput(RelativeLayout relativeLayoutGpioInput) {
        this.relativeLayoutGpioInput = relativeLayoutGpioInput;
    }

    public RelativeLayout getRelativeCompleteTest() {
        return relativeCompleteTest;
    }

    public void setRelativeCompleteTest(RelativeLayout relativeCompleteTest) {
        this.relativeCompleteTest = relativeCompleteTest;
    }

    public RelativeLayout getFlashInformation() {
        return flashInformation;
    }

    public void setFlashInformation(RelativeLayout flashInformation) {
        this.flashInformation = flashInformation;
    }

    public RelativeLayout getPowerPinsInformation() {
        return powerPinsInformation;
    }

    public void setPowerPinsInformation(RelativeLayout powerPinsInformation) {
        this.powerPinsInformation = powerPinsInformation;
    }

    public RelativeLayout getGpioOutputInfo() {
        return gpioOutputInfo;
    }

    public void setGpioOutputInfo(RelativeLayout gpioOutputInfo) {
        this.gpioOutputInfo = gpioOutputInfo;
    }

    public RelativeLayout getGpioShortCircuitInfo() {
        return gpioShortCircuitInfo;
    }

    public void setGpioShortCircuitInfo(RelativeLayout gpioShortCircuitInfo) {
        this.gpioShortCircuitInfo = gpioShortCircuitInfo;
    }

    public RelativeLayout getGpioInputInfo() {
        return gpioInputInfo;
    }

    public void setGpioInputInfo(RelativeLayout gpioInputInfo) {
        this.gpioInputInfo = gpioInputInfo;
    }

    public RelativeLayout getBoardConnectionRelative() {
        return boardConnectionRelative;
    }

    public void setBoardConnectionRelative(RelativeLayout boardConnectionRelative) {
        this.boardConnectionRelative = boardConnectionRelative;
    }

    public RelativeLayout getGndConnectionRelative() {
        return gndConnectionRelative;
    }

    public void setGndConnectionRelative(RelativeLayout gndConnectionRelative) {
        this.gndConnectionRelative = gndConnectionRelative;
    }

    public RelativeLayout getPowerPinsRelative() {
        return powerPinsRelative;
    }

    public void setPowerPinsRelative(RelativeLayout powerPinsRelative) {
        this.powerPinsRelative = powerPinsRelative;
    }

    public TextView getBoardIDTextView() {
        return boardIDTextView;
    }

    public void setBoardIDTextView(TextView boardIDTextView) {
        this.boardIDTextView = boardIDTextView;
    }
    public TextView getCurrentTextView() {
        return currentTextView;
    }

    public void setCurrentTextView(TextView currentTextView) {
        this.currentTextView = currentTextView;
    }

    public TextView getCurrentMeasureInfoTextView() {
        return currentMeasureInfoTextView;
    }

    public void setCurrentMeasureInfoTextView(TextView currentMeasureInfoTextView) {
        this.currentMeasureInfoTextView = currentMeasureInfoTextView;
    }
    public TextView getTemperatureTextView() {
        return temperatureTextView;
    }

    public void setTemperatureTextView(TextView temperatureTextView) {
        this.temperatureTextView = temperatureTextView;
    }

    public TextView getFlashTextView() {
        return flashTextView;
    }

    public void setFlashTextView(TextView flashTextView) {
        this.flashTextView = flashTextView;
    }

    public TextView getPowerPinsTextView() {
        return powerPinsTextView;
    }

    public void setPowerPinsTextView(TextView powerPinsTextView) {
        this.powerPinsTextView = powerPinsTextView;
    }

    public TextView getGpioOutputTestTextView() {
        return gpioOutputTestTextView;
    }

    public void setGpioOutputTestTextView(TextView gpioOutputTestTextView) {
        this.gpioOutputTestTextView = gpioOutputTestTextView;
    }

    public TextView getGpioInputTestTextView() {
        return gpioInputTestTextView;
    }

    public void setGpioInputTestTextView(TextView gpioInputTestTextView) {
        this.gpioInputTestTextView = gpioInputTestTextView;
    }
    public TextView getGpioShortCircuitTestTextView() {
        return gpioShortCircuitTestTextView;
    }

    public void setGpioShortCircuitTestTextView(TextView gpioShortCircuitTestTextView) {
        this.gpioShortCircuitTestTextView = gpioShortCircuitTestTextView;
    }
    public TextView getCompleteTestText() {
        return completeTestText;
    }

    public void setCompleteTestText(TextView completeTestText) {
        this.completeTestText = completeTestText;
    }

    public TextView getBoardConnectionText() {
        return boardConnectionText;
    }

    public void setBoardConnectionText(TextView boardConnectionText) {
        this.boardConnectionText = boardConnectionText;
    }

    public TextView getGndConnectionTextView() {
        return gndConnectionTextView;
    }

    public void setGndConnectionTextView(TextView gndConnectionTextView) {
        this.gndConnectionTextView = gndConnectionTextView;
    }

    public ProgressBar getTemperatureProgressBar() {
        return temperatureProgressBar;
    }
    public ProgressBar getCurrentMeasureProgressBar() {
        return currentMeasureProgressBar;
    }

    public void setCurrentMeasureProgressBar(ProgressBar currentMeasureProgressBar) {
        this.currentMeasureProgressBar = currentMeasureProgressBar;
    }

    public void setTemperatureProgressBar(ProgressBar temperatureProgressBar) {
        this.temperatureProgressBar = temperatureProgressBar;
    }

    public ProgressBar getFlashProgressBar() {
        return flashProgressBar;
    }

    public void setFlashProgressBar(ProgressBar flashProgressBar) {
        this.flashProgressBar = flashProgressBar;
    }

    public ProgressBar getPowerPinsProgressBar() {
        return powerPinsProgressBar;
    }

    public void setPowerPinsProgressBar(ProgressBar powerPinsProgressBar) {
        this.powerPinsProgressBar = powerPinsProgressBar;
    }

    public ProgressBar getGpioOutputTestProgressBar() {
        return gpioOutputTestProgressBar;
    }

    public void setGpioOutputTestProgressBar(ProgressBar gpioOutputTestProgressBar) {
        this.gpioOutputTestProgressBar = gpioOutputTestProgressBar;
    }
    public ProgressBar getGpioShortCircuitBar() {
        return gpioShortCircuitBar;
    }

    public void setGpioShortCircuitBar(ProgressBar gpioShortCircuitBar) {
        this.gpioShortCircuitBar = gpioShortCircuitBar;
    }
    public ProgressBar getGpioInputTestProgressBar() {
        return gpioInputTestProgressBar;
    }

    public void setGpioInputTestProgressBar(ProgressBar gpioInputTestProgressBar) {
        this.gpioInputTestProgressBar = gpioInputTestProgressBar;
    }

    public ImageView getArrowCompleteTest() {
        return arrowCompleteTest;
    }

    public void setArrowCompleteTest(ImageView arrowCompleteTest) {
        this.arrowCompleteTest = arrowCompleteTest;
    }

    public Drawable getGreyArrow() {
        return greyArrow;
    }

    public void setGreyArrow(Drawable greyArrow) {
        this.greyArrow = greyArrow;
    }

    public Drawable getRedArrow() {
        return redArrow;
    }

    public void setRedArrow(Drawable redArrow) {
        this.redArrow = redArrow;
    }

    public Drawable getGreenArrow() {
        return greenArrow;
    }

    public void setGreenArrow(Drawable greenArrow) {
        this.greenArrow = greenArrow;
    }

    public Drawable getOkImage() {
        return okImage;
    }

    public void setOkImage(Drawable okImage) {
        this.okImage = okImage;
    }

    public Drawable getErrorImage() {
        return errorImage;
    }

    public void setErrorImage(Drawable errorImage) {
        this.errorImage = errorImage;
    }

    public RelativeLayout getRelativeLayoutAnalogInput() {
        return relativeLayoutAnalogInput;
    }

    public void setRelativeLayoutAnalogInput(RelativeLayout relativeLayoutAnalogInput) {
        this.relativeLayoutAnalogInput = relativeLayoutAnalogInput;
    }

    public RelativeLayout getAnalogInputInfo() {
        return analogInputInfo;
    }

    public void setAnalogInputInfo(RelativeLayout analogInputInfo) {
        this.analogInputInfo = analogInputInfo;
    }

    public TextView getAnalogInputTestTextView() {
        return analogInputTestTextView;
    }

    public void setAnalogInputTestTextView(TextView analogInputTestTextView) {
        this.analogInputTestTextView = analogInputTestTextView;
    }

    public ProgressBar getAnalogInputTestProgressBar() {
        return analogInputTestProgressBar;
    }

    public void setAnalogInputTestProgressBar(ProgressBar analogInputTestProgressBar) {
        this.analogInputTestProgressBar = analogInputTestProgressBar;
    }

    public ImageView getArrowAnalogInput() {
        return arrowAnalogInput;
    }

    public void setArrowAnalogInput(ImageView arrowAnalogInput) {
        this.arrowAnalogInput = arrowAnalogInput;
    }
}
