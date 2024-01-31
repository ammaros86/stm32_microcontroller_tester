package adapters;

import static helpers.Constants.GPIO_OUTPUT_OK;
import static helpers.Constants.GpioOutputSize;
import static helpers.Constants.outputMaxLowVoltageDefault;
import static helpers.Constants.outputMinHighVoltageDefault;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.content.res.AppCompatResources;
import androidx.recyclerview.widget.RecyclerView;

import com.example.boardtester.R;

import java.util.ArrayList;
import java.util.Locale;

import helpers.BoardChecker;
import models.GpioOutput;

public class GpioOutputAdapter extends RecyclerView.Adapter<GpioOutputAdapter.MainViewHolder> {

    final Context context;
    final Drawable okImage, errorImage, notUsedInputImage;
    final ArrayList<GpioOutput> gpioOutputNoPullArrayList, gpioOutputPullDownArrayList, gpioOutputPullUpArrayList;
    private final BoardChecker boardChecker;

    public GpioOutputAdapter(Context context, ArrayList<GpioOutput> gpioOutputNoPullArrayList, ArrayList<GpioOutput> gpioOutputPullDownArrayList, ArrayList<GpioOutput> gpioOutputPullUpArrayList) {
        this.context = context;
        this.gpioOutputNoPullArrayList = gpioOutputNoPullArrayList;
        this.gpioOutputPullDownArrayList = gpioOutputPullDownArrayList;
        this.gpioOutputPullUpArrayList = gpioOutputPullUpArrayList;
        okImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
        notUsedInputImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_remove_24);
        boardChecker = new BoardChecker();
    }

    @NonNull
    @Override
    public GpioOutputAdapter.MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_output, parent, false);
        return new GpioOutputAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull GpioOutputAdapter.MainViewHolder holder, int position) {
        GpioOutput boardOutputGpio = gpioOutputNoPullArrayList.get(position);
        holder.outputTitleTextView.setText(String.valueOf(boardOutputGpio.getPin()));
        initialListener(holder, position);
        if (boardChecker.isGpioNotTestable(boardOutputGpio.getPin())){
            setNotUsedGpioOutput(holder);
        }else if(boardChecker.isGpioOutputConnectedToPushButton(boardOutputGpio)) {
            if(boardOutputGpio.getComment().equals(GPIO_OUTPUT_OK)){
                holder.outputNoPullHigh.setImageDrawable(okImage);
                holder.outputNoPullLow.setImageDrawable(okImage);
            }else{
                holder.outputNoPullHigh.setImageDrawable(errorImage);
                holder.outputNoPullLow.setImageDrawable(errorImage);
            }
            holder.outputPullDownHigh.setImageDrawable(notUsedInputImage);
            holder.outputPullDownLow.setImageDrawable(notUsedInputImage);
            holder.outputPullUpHigh.setImageDrawable(notUsedInputImage);
            holder.outputPullUpLow.setImageDrawable(notUsedInputImage);
        }else{
            checkNoPullOutputGpio(holder, boardOutputGpio);
            checkPullDownOutputGpio(holder, boardOutputGpio);
            checkPullUpOutputGpio(holder, boardOutputGpio);
        }
    }

    private void initialListener(GpioOutputAdapter.MainViewHolder holder, int position){
        if(gpioOutputNoPullArrayList.get(position) != null){
            GpioOutput gpioOutputNoPullResult = gpioOutputNoPullArrayList.get(position);
            holder.outputNoPullLow.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputNoPullResult.getPin() + "\nMeasured Low Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputNoPullResult.getLow_voltage()),  Toast.LENGTH_SHORT).show());
            holder.outputNoPullHigh.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputNoPullResult.getPin() + "\nMeasured High Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputNoPullResult.getHigh_voltage()) , Toast.LENGTH_SHORT).show());
        }

        if(gpioOutputPullDownArrayList.get(position) != null){
            GpioOutput gpioOutputPullDownResult = gpioOutputPullDownArrayList.get(position);
            holder.outputPullDownLow.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputPullDownResult.getPin() + " \nMeasured Low Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputPullDownResult.getLow_voltage()) , Toast.LENGTH_SHORT).show());
            holder.outputPullDownHigh.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputPullDownResult.getPin() + "\nMeasured High Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputPullDownResult.getHigh_voltage()) , Toast.LENGTH_SHORT).show());
        }

        if(gpioOutputPullUpArrayList.get(position) != null){
            GpioOutput gpioOutputPullUpResult = gpioOutputPullUpArrayList.get(position);
            holder.outputPullUpLow.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputPullUpResult.getPin() + "\nMeasured Low Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputPullUpResult.getLow_voltage()) , Toast.LENGTH_SHORT).show());
            holder.outputPullUpHigh.setOnClickListener(view -> Toast.makeText(context, "GPIO: " + gpioOutputPullUpResult.getPin() + "\nMeasured High Voltage:  " + String.format(Locale.US, "%.2f", gpioOutputPullUpResult.getHigh_voltage()) , Toast.LENGTH_SHORT).show());
        }


    }

    private void setNotUsedGpioOutput(GpioOutputAdapter.MainViewHolder holder){
        holder.outputNoPullHigh.setImageDrawable(notUsedInputImage);
        holder.outputNoPullLow.setImageDrawable(notUsedInputImage);
        holder.outputPullDownHigh.setImageDrawable(notUsedInputImage);
        holder.outputPullDownLow.setImageDrawable(notUsedInputImage);
        holder.outputPullUpHigh.setImageDrawable(notUsedInputImage);
        holder.outputPullUpLow.setImageDrawable(notUsedInputImage);
    }

    private void checkNoPullOutputGpio(GpioOutputAdapter.MainViewHolder holder, GpioOutput gpioOutput){
        if(gpioOutputNoPullArrayList.isEmpty()){
            holder.outputNoPullHigh.setImageDrawable(errorImage);
            holder.outputNoPullLow.setImageDrawable(errorImage);
        }else{
            String pin = gpioOutput.getPin();
            GpioOutput gpioOutputLowSpeed = boardChecker.getLowSpeedTest(pin, gpioOutputNoPullArrayList);
            GpioOutput gpioOutputMediumSpeed = boardChecker.getMediumSpeedTest(pin, gpioOutputNoPullArrayList);
            GpioOutput gpioOutputHighSpeed = boardChecker.getHighSpeedTest(pin, gpioOutputNoPullArrayList);

            if((gpioOutputLowSpeed != null) && (gpioOutputMediumSpeed != null) && (gpioOutputHighSpeed != null)){
                if(boardChecker.isGpioOutputOk(gpioOutputLowSpeed, gpioOutputMediumSpeed, gpioOutputHighSpeed)){
                    holder.outputNoPullHigh.setImageDrawable(okImage);
                    holder.outputNoPullLow.setImageDrawable(okImage);
                }else{
                    if(gpioOutputLowSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputMediumSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputHighSpeed.getLow_voltage() > outputMaxLowVoltageDefault){
                        holder.outputNoPullLow.setImageDrawable(errorImage);
                    }else{
                        holder.outputNoPullLow.setImageDrawable(okImage);
                    }
                    if(gpioOutputLowSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputMediumSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputHighSpeed.getHigh_voltage() < outputMinHighVoltageDefault){
                        holder.outputNoPullHigh.setImageDrawable(errorImage);
                    }else{
                        holder.outputNoPullHigh.setImageDrawable(okImage);
                    }
                }
            }else{
                holder.outputNoPullHigh.setImageDrawable(errorImage);
                holder.outputNoPullLow.setImageDrawable(errorImage);
            }
        }
    }

    private void checkPullDownOutputGpio(GpioOutputAdapter.MainViewHolder holder, GpioOutput gpioOutput){
        if(gpioOutputPullDownArrayList.isEmpty()){
            holder.outputPullDownHigh.setImageDrawable(errorImage);
            holder.outputPullDownLow.setImageDrawable(errorImage);
        }else{
            String pin = gpioOutput.getPin();
            GpioOutput gpioOutputLowSpeed = boardChecker.getLowSpeedTest(pin, gpioOutputPullDownArrayList);
            GpioOutput gpioOutputMediumSpeed = boardChecker.getMediumSpeedTest(pin, gpioOutputPullDownArrayList);
            GpioOutput gpioOutputHighSpeed = boardChecker.getHighSpeedTest(pin, gpioOutputPullDownArrayList);
            if((gpioOutputLowSpeed != null) && (gpioOutputMediumSpeed != null) && (gpioOutputHighSpeed != null)){
                if(boardChecker.isGpioOutputOk(gpioOutputLowSpeed, gpioOutputMediumSpeed, gpioOutputHighSpeed)){
                    holder.outputPullDownHigh.setImageDrawable(okImage);
                    holder.outputPullDownLow.setImageDrawable(okImage);
                }else{
                    if(gpioOutputLowSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputMediumSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputHighSpeed.getLow_voltage() > outputMaxLowVoltageDefault){
                        holder.outputPullDownLow.setImageDrawable(errorImage);
                    }else{
                        holder.outputPullDownLow.setImageDrawable(okImage);
                    }
                    if(gpioOutputLowSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputMediumSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputHighSpeed.getHigh_voltage() < outputMinHighVoltageDefault){
                        holder.outputPullDownHigh.setImageDrawable(errorImage);
                    }else{
                        holder.outputPullDownHigh.setImageDrawable(okImage);
                    }
                }
            }else{
                holder.outputPullDownHigh.setImageDrawable(errorImage);
                holder.outputPullDownLow.setImageDrawable(errorImage);
            }
        }
    }

    private void checkPullUpOutputGpio(GpioOutputAdapter.MainViewHolder holder, GpioOutput gpioOutput){
        if(gpioOutputPullUpArrayList.isEmpty()){
            holder.outputPullUpHigh.setImageDrawable(errorImage);
            holder.outputPullUpLow.setImageDrawable(errorImage);
        }else{
            String pin = gpioOutput.getPin();
            GpioOutput gpioOutputLowSpeed = boardChecker.getLowSpeedTest(pin, gpioOutputPullUpArrayList);
            GpioOutput gpioOutputMediumSpeed = boardChecker.getMediumSpeedTest(pin, gpioOutputPullUpArrayList);
            GpioOutput gpioOutputHighSpeed = boardChecker.getHighSpeedTest(pin, gpioOutputPullUpArrayList);
            if((gpioOutputLowSpeed != null) && (gpioOutputMediumSpeed != null) && (gpioOutputHighSpeed != null)){
                if(boardChecker.isGpioOutputOk(gpioOutputLowSpeed, gpioOutputMediumSpeed, gpioOutputHighSpeed)){
                    holder.outputPullUpHigh.setImageDrawable(okImage);
                    holder.outputPullUpLow.setImageDrawable(okImage);
                }else{
                    if(gpioOutputLowSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputMediumSpeed.getLow_voltage() > outputMaxLowVoltageDefault || gpioOutputHighSpeed.getLow_voltage() > outputMaxLowVoltageDefault){
                        holder.outputPullUpLow.setImageDrawable(errorImage);
                    }else{
                        holder.outputPullUpLow.setImageDrawable(okImage);
                    }
                    if(gpioOutputLowSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputMediumSpeed.getHigh_voltage() < outputMinHighVoltageDefault || gpioOutputHighSpeed.getHigh_voltage() < outputMinHighVoltageDefault){
                        holder.outputPullUpHigh.setImageDrawable(errorImage);
                    }else{
                        holder.outputPullUpHigh.setImageDrawable(okImage);
                    }
                }
            }else{
                holder.outputPullUpHigh.setImageDrawable(errorImage);
                holder.outputPullUpLow.setImageDrawable(errorImage);
            }
        }
    }

    @Override
    public int getItemCount() {
        return GpioOutputSize;
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView outputTitleTextView;
        private final ImageView outputNoPullHigh;
        private final ImageView outputNoPullLow;
        private final ImageView outputPullDownHigh;
        private final ImageView outputPullDownLow;
        private final ImageView outputPullUpHigh;
        private final ImageView outputPullUpLow;

        MainViewHolder(View view) {
            super(view);
            outputTitleTextView = itemView.findViewById(R.id.pinTitleOutput);
            outputNoPullHigh = itemView.findViewById(R.id.outputNoPullHigh);
            outputNoPullLow = itemView.findViewById(R.id.outputNoPullLow);
            outputPullDownHigh = itemView.findViewById(R.id.outputPullDownHigh);
            outputPullDownLow = itemView.findViewById(R.id.outputPullDownLow);
            outputPullUpHigh = itemView.findViewById(R.id.outputPullUpHigh);
            outputPullUpLow = itemView.findViewById(R.id.outputPullUpLow);
        }
    }
}
