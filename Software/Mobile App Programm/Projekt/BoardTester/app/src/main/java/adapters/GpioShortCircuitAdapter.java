package adapters;

import static helpers.Constants.FailedTest;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.Log;
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
import models.GpioShortCircuit;

public class GpioShortCircuitAdapter extends RecyclerView.Adapter<GpioShortCircuitAdapter.MainViewHolder>{
    private final ArrayList<GpioShortCircuit> gpioShortCircuitArrayList;
    private final Drawable okImage, errorImage, notUsedImage;
    private final BoardChecker boardChecker;

    public GpioShortCircuitAdapter(Context context, ArrayList<GpioShortCircuit> gpioShortCircuitArrayList) {
        this.gpioShortCircuitArrayList = gpioShortCircuitArrayList;
        okImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
        notUsedImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_remove_24);
        boardChecker = new BoardChecker();
    }

    @NonNull
    @Override
    public  MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_gpio_short, parent, false);
        return new  MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MainViewHolder holder, int position) {
        GpioShortCircuit gpioShortCircuit = gpioShortCircuitArrayList.get(position);
        if(gpioShortCircuit != null) {
            String pinNameText = gpioShortCircuit.getPin();
            holder.pinName.setText(pinNameText);
            String shortNumbersText = Integer.toString(gpioShortCircuit.getShort_circuits_count());
            int shortNumbers = gpioShortCircuit.getShort_circuits_count();
            Log.d("MYDEBUGGER", "pinNameText: "+pinNameText + " , shortNumbers: " +shortNumbers + " comment" + gpioShortCircuit.getComment());
            if (boardChecker.isGpioNotTestable(pinNameText)) {
                holder.pinStateImage.setImageDrawable(notUsedImage);
                holder.shortPins.setText("-");
                holder.shortedPins.setText("-");
            }else{
                if (shortNumbers == 0){
                    holder.pinStateImage.setImageDrawable(okImage);
                    holder.shortPins.setText("0");
                    holder.shortedPins.setText("-");
                }else if(shortNumbers > 0){
                    holder.pinStateImage.setImageDrawable(errorImage);
                    holder.shortPins.setText(shortNumbersText);
                    holder.shortedPins.setText(gpioShortCircuit.getComment());
                }else {
                    holder.pinStateImage.setImageDrawable(errorImage);
                    holder.shortPins.setText("-");
                    holder.shortedPins.setText(FailedTest);
                }
            }
        }else{
            holder.pinStateImage.setImageDrawable(errorImage);
        }
    }

    @Override
    public int getItemCount() {
        return gpioShortCircuitArrayList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView pinName ;
        private final TextView shortPins;
        private final TextView shortedPins;

        private final ImageView pinStateImage;
        MainViewHolder(View view) {
            super(view);
            pinName = itemView.findViewById(R.id.gpioPin);
            shortPins = itemView.findViewById(R.id.shortPins);
            shortedPins = itemView.findViewById(R.id.shortedPins);
            pinStateImage = itemView.findViewById(R.id.imageShortState);
        }
    }
}
