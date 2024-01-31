package adapters;

import static helpers.Constants.BoardNotConnectedValue;

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
import models.PowerPinVoltage;

public class PowerGndPinsAdapter extends RecyclerView.Adapter<PowerGndPinsAdapter.MainViewHolder> {
    private final ArrayList<PowerPinVoltage> powerPinsVoltageList;
    private final Drawable okImage, errorImage;
    private final BoardChecker boardChecker;

    public PowerGndPinsAdapter(Context context, ArrayList<PowerPinVoltage> powerPinsVoltageList) {
        this.powerPinsVoltageList = powerPinsVoltageList;
        okImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage =  AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
        boardChecker = new BoardChecker();
    }

    @NonNull
    @Override
    public PowerGndPinsAdapter.MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_power, parent, false);
        return new PowerGndPinsAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull PowerGndPinsAdapter.MainViewHolder holder, int position) {
        PowerPinVoltage powerPinVoltage = powerPinsVoltageList.get(position);
        if(powerPinVoltage != null){
            String pinNameText = powerPinVoltage.getPin();
            holder.pinName.setText(pinNameText);
            String voltage = Float.toString(powerPinVoltage.getVoltage());
            holder.pinVoltage.setText(voltage);
            if(boardChecker.isVinPin(powerPinVoltage.getPin()) && (boardChecker.boardConnectionTest(powerPinsVoltageList)) >= BoardNotConnectedValue){
                holder.pinStateImage.setImageDrawable(errorImage);
            }else {
                if(powerPinVoltage.getComment().equals("OK")){
                    holder.pinStateImage.setImageDrawable(okImage);
                }else{
                    holder.pinStateImage.setImageDrawable(errorImage);
                }
            }
        }else{
            holder.pinStateImage.setImageDrawable(errorImage);
        }
    }


    @Override
    public int getItemCount() {
        return powerPinsVoltageList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView pinName ;
        private final TextView  pinVoltage;
        private final ImageView pinStateImage;
        MainViewHolder(View view) {
            super(view);
            pinName = itemView.findViewById(R.id.titleView);
            pinVoltage = itemView.findViewById(R.id.voltageView);
            pinStateImage = itemView.findViewById(R.id.imagePowerState);
        }
    }
}
