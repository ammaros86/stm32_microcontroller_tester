package adapters;

import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.content.res.AppCompatResources;
import androidx.recyclerview.widget.RecyclerView;

import com.example.boardtester.R;

import java.util.ArrayList;
import java.util.Locale;

import models.AnalogInput;

public class AnalogInputAdapter extends RecyclerView.Adapter<AnalogInputAdapter.MainViewHolder> {
    private final Drawable okImage, errorImage;
    private final ArrayList<AnalogInput> analogInputArrayList;
    private final Context context;
    public AnalogInputAdapter(Context context, ArrayList<AnalogInput> analogInputArrayList) {
        this.context = context;
        this.analogInputArrayList = analogInputArrayList;
        okImage = AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage = AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
    }

    @NonNull
    @Override
    public MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_gpio_continuity, parent, false);
        return new AnalogInputAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MainViewHolder holder, int position) {
        AnalogInput analogInput = analogInputArrayList.get(position);
        setListener(holder, position);
        String analogPin = analogInput.getPin();
        holder.pin.setText(analogPin);
        if(analogInput.getComment().equals("OK")){
            if(analogPin.length() == 3){
                holder.comment.setText("  OK");
            }else{
                holder.comment.setText("OK");
            }
            holder.image.setImageDrawable(okImage);
        }else{
            if(analogPin.length() == 4){
                holder.comment.setText(" Error");
            }else{
                holder.comment.setText("Error");
            }
            holder.image.setImageDrawable(errorImage);
        }
    }

    private void setListener(AnalogInputAdapter.MainViewHolder holder, final int position){
        holder.analogResultLinearLayout.setOnClickListener(view -> {
            Dialog customDialog = new Dialog(context);
            customDialog.setContentView(R.layout.analoginputresult);
            TextView analogPinView = customDialog.findViewById(R.id.analogPin);
            TextView voltage1View = customDialog.findViewById(R.id.voltage1);
            TextView voltage2View = customDialog.findViewById(R.id.voltage2);
            TextView voltage3View = customDialog.findViewById(R.id.voltage3);
            TextView voltage4View = customDialog.findViewById(R.id.voltage4);
            AnalogInput analogInput = analogInputArrayList.get(position);
            double voltage1 = analogInput.getMeasure_value1();
            double voltage2 = analogInput.getMeasure_value2();
            double voltage3 = analogInput.getMeasure_value3();
            double voltage4 = analogInput.getMeasure_value4();
            String voltage1Text = String.format(Locale.US, "%.2f", voltage1) + " V";
            String voltage2Text = String.format(Locale.US, "%.2f", voltage2) + " V";
            String voltage3Text = String.format(Locale.US, "%.2f", voltage3) + " V";
            String voltage4Text = String.format(Locale.US, "%.2f", voltage4) + " V";
            String analogPinText = "Analog Pin: " + analogInput.getPin();
            analogPinView.setText(analogPinText);
            voltage1View.setText(voltage1Text);
            voltage2View.setText(voltage2Text);
            voltage3View.setText(voltage3Text);
            voltage4View.setText(voltage4Text);
            customDialog.show();
        });
    }


    @Override
    public int getItemCount() {
        return analogInputArrayList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView pin;
        private final TextView comment;
        private final ImageView image;
        private final LinearLayout analogResultLinearLayout;
        MainViewHolder(View view) {
            super(view);
            pin = itemView.findViewById(R.id.continuity_pin);
            comment = itemView.findViewById(R.id.continuity_comment);
            image = itemView.findViewById(R.id.continuity_image);
            analogResultLinearLayout = itemView.findViewById(R.id.analogResultLinearLayout);

        }
    }
}
