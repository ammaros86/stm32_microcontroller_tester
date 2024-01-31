package adapters;

import static helpers.Constants.GpioInputHighOk;
import static helpers.Constants.GpioInputLowOk;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.content.res.AppCompatResources;
import androidx.recyclerview.widget.RecyclerView;

import com.example.boardtester.R;

import java.util.ArrayList;

import helpers.BoardChecker;
import models.GpioInput;

public class GpioInputAdapter extends RecyclerView.Adapter<GpioInputAdapter.MainViewHolder> {

    private final Drawable okImage, errorImage, notUsedInputImage;
    private final BoardChecker boardChecker;
    private final ArrayList<GpioInput> boardMeasureInputNoPullList, boardMeasureInputPullDownList, boardMeasureInputPullUpList;

    public GpioInputAdapter(Context context, ArrayList<GpioInput> boardMeasureInputNoPull, ArrayList<GpioInput> boardMeasureInputPullDown, ArrayList<GpioInput> boardMeasureInputPullUp) {
        this.boardMeasureInputNoPullList = boardMeasureInputNoPull;
        this.boardMeasureInputPullDownList = boardMeasureInputPullDown;
        this.boardMeasureInputPullUpList = boardMeasureInputPullUp;
        okImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
        notUsedInputImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_remove_24);
        boardChecker = new BoardChecker();
    }

    @NonNull
    @Override
    public GpioInputAdapter.MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_input, parent, false);
        return new GpioInputAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MainViewHolder holder, int position) {
        GpioInput boardInputGpio = boardMeasureInputNoPullList.get(position);
        holder.inputTitleTextView.setText(String.valueOf(boardInputGpio.getPin()));
        if (boardChecker.isGpioNotTestable(boardInputGpio.getPin())) {
            setNotUsedGpioInput(holder);
        }else{
            checkNoPullInputGpio(holder, position);
            checkPullDownInputGpio(holder, position);
            checkPullUpInputGpio(holder, position);
        }
    }

    private void setNotUsedGpioInput(MainViewHolder holder){
        holder.inputNoPullHigh.setImageDrawable(notUsedInputImage);
        holder.inputNoPullLow.setImageDrawable(notUsedInputImage);
        holder.inputPullDownHigh.setImageDrawable(notUsedInputImage);
        holder.inputPullDownLow.setImageDrawable(notUsedInputImage);
        holder.inputPullUpHigh.setImageDrawable(notUsedInputImage);
        holder.inputPullUpLow.setImageDrawable(notUsedInputImage);
    }

    private void checkNoPullInputGpio(MainViewHolder holder, int position){
        GpioInput gpioInput = boardMeasureInputNoPullList.get(position);
        if(gpioInput.getHigh_state() == GpioInputHighOk){
            holder.inputNoPullHigh.setImageDrawable(okImage);
        }else{
            holder.inputNoPullHigh.setImageDrawable(errorImage);
        }
        if(gpioInput.getLow_state() == GpioInputLowOk){
            holder.inputNoPullLow.setImageDrawable(okImage);
        }else{
            holder.inputNoPullLow.setImageDrawable(errorImage);
        }
    }

    private void checkPullDownInputGpio(MainViewHolder holder, int position){
        GpioInput gpioInput = boardMeasureInputPullDownList.get(position);
        if(boardChecker.isGpioConnectedToPushButton(gpioInput.getPin())){
            holder.inputPullDownHigh.setImageDrawable(notUsedInputImage);
            holder.inputPullDownLow.setImageDrawable(notUsedInputImage);
        }else{
            if(gpioInput.getHigh_state() == GpioInputHighOk){
                holder.inputPullDownHigh.setImageDrawable(okImage);
            }else{
                holder.inputPullDownHigh.setImageDrawable(errorImage);
            }
            if(gpioInput.getLow_state() == GpioInputLowOk){
                holder.inputPullDownLow.setImageDrawable(okImage);
            }else{
                holder.inputPullDownLow.setImageDrawable(errorImage);
            }
        }
    }

    private void checkPullUpInputGpio(MainViewHolder holder, int position){
        GpioInput gpioInput = boardMeasureInputPullUpList.get(position);
        if(boardChecker.isGpioConnectedToPushButton(gpioInput.getPin())){
            holder.inputPullUpHigh.setImageDrawable(notUsedInputImage);
            holder.inputPullUpLow.setImageDrawable(notUsedInputImage);
        }else{
            if(gpioInput.getHigh_state() == GpioInputHighOk){
                holder.inputPullUpHigh.setImageDrawable(okImage);
            }else{
                holder.inputPullUpHigh.setImageDrawable(errorImage);
            }
            if(gpioInput.getLow_state() == GpioInputLowOk){
                holder.inputPullUpLow.setImageDrawable(okImage);
            }else{
                holder.inputPullUpLow.setImageDrawable(errorImage);
            }
        }
    }

    @Override
    public int getItemCount() {
        return boardMeasureInputNoPullList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView inputTitleTextView;
        private final ImageView inputNoPullHigh;
        private final ImageView inputNoPullLow;
        private final ImageView inputPullDownHigh;
        private final ImageView inputPullDownLow;
        private final ImageView inputPullUpHigh;
        private final ImageView inputPullUpLow;

        MainViewHolder(View view) {
            super(view);
            inputTitleTextView = itemView.findViewById(R.id.pinTitleInput);
            inputNoPullHigh = itemView.findViewById(R.id.inputNoPullHigh);
            inputNoPullLow = itemView.findViewById(R.id.inputNoPullLow);
            inputPullDownHigh = itemView.findViewById(R.id.inputPullDownHigh);
            inputPullDownLow = itemView.findViewById(R.id.inputPullDownLow);
            inputPullUpHigh = itemView.findViewById(R.id.inputPullUpHigh);
            inputPullUpLow = itemView.findViewById(R.id.inputPullUpLow);
        }
    }
}
