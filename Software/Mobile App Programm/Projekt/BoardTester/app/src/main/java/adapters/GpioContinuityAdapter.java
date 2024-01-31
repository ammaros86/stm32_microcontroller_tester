package adapters;

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

import models.GpioContinuity;

public class GpioContinuityAdapter extends RecyclerView.Adapter<GpioContinuityAdapter.MainViewHolder> {

    private final Drawable okImage, errorImage, notUsedInputImage;
    private final ArrayList<GpioContinuity> gpioContinuityArrayList;

    public GpioContinuityAdapter(Context context, ArrayList<GpioContinuity> gpioContinuityArrayList) {
        this.gpioContinuityArrayList = gpioContinuityArrayList;
        okImage = AppCompatResources.getDrawable(context, R.drawable.ic_baseline_check_24);
        errorImage = AppCompatResources.getDrawable(context, R.drawable.ic_baseline_close_24);
        notUsedInputImage = AppCompatResources.getDrawable(context, R.drawable.ic_baseline_remove_24);
    }

    @NonNull
    @Override
    public MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_gpio_continuity, parent, false);
        return new  MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MainViewHolder holder, int position) {
        GpioContinuity gpioContinuity = gpioContinuityArrayList.get(position);
        String gpioText = gpioContinuity.getPin();
        String comment;
        holder.pin.setText(gpioText);
        if(gpioText.length() == 3){
            comment = "  " + gpioContinuity.getComment();
        }else{
            comment = gpioContinuity.getComment();
        }
        Log.d("MYDEBUGGER", "gpioText: " + gpioText + " length: "  + gpioText.length());

        if(comment.contains("GPIO Standard Open")){
            holder.comment.setText(comment);
            holder.image.setImageDrawable(notUsedInputImage);
        }else if(comment.contains("OK")){
            holder.comment.setText(comment);
            holder.image.setImageDrawable(okImage);
        }else {
            holder.comment.setText(comment);
            holder.image.setImageDrawable(errorImage);
        }
    }

    @Override
    public int getItemCount() {
        return gpioContinuityArrayList.size();
    }

    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView pin;
        private final TextView comment;
        private final ImageView image;

        MainViewHolder(View view) {
            super(view);
            pin = itemView.findViewById(R.id.continuity_pin);
            comment = itemView.findViewById(R.id.continuity_comment);
            image = itemView.findViewById(R.id.continuity_image);

        }
    }
}

