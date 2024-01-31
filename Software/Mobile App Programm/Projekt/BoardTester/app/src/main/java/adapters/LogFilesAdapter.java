package adapters;

import static helpers.BoardChecker.convertLogFileName;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.boardtester.R;

import java.util.ArrayList;

import log.LogFileActivity;
import log.RemoveLogHandler;

public class LogFilesAdapter extends RecyclerView.Adapter<LogFilesAdapter.MainViewHolder> {

    private final ArrayList<String> logFilesArrayList;
    private final Context context;
    private final RemoveLogHandler removeLogHandler;

    public LogFilesAdapter(Context context, RemoveLogHandler removeLogHandler, ArrayList<String> logFilesArrayList) {
        this.context = context;
        this.logFilesArrayList = logFilesArrayList;
        this.removeLogHandler = removeLogHandler;
    }

    @NonNull
    @Override
    public LogFilesAdapter.MainViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.row_log_files, parent, false);
        return new LogFilesAdapter.MainViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull LogFilesAdapter.MainViewHolder holder, int position) {
        String logFileName = logFilesArrayList.get(position);
        if(logFileName != null){
            String logFileNameText = convertLogFileName(logFileName.replace(".txt", ""));
            holder.logFileNameTextView.setText(logFileNameText);
        }
        holder.logFileNameTextView.setOnClickListener(view -> {
            Intent intent = new Intent(context, LogFileActivity.class);
            intent.putExtra("LOG_FILE_KEY", logFileName);
            context.startActivity(intent);
        });
        holder.removeLogFileImageView.setOnClickListener(view -> removeLogHandler.removeLogFile(logFileName, position));

    }


    public void removeAt(int position) {
        logFilesArrayList.remove(position);
        notifyItemRemoved(position);
        notifyItemRangeChanged(position, logFilesArrayList.size());
    }

    @Override
    public int getItemCount() {
        return logFilesArrayList.size();
    }
    public static class MainViewHolder extends RecyclerView.ViewHolder {
        private final TextView logFileNameTextView;
        private final ImageView removeLogFileImageView;

        MainViewHolder(View view) {
            super(view);
            logFileNameTextView = itemView.findViewById(R.id.logFileNameView);
            removeLogFileImageView = itemView.findViewById(R.id.removeLogFile);

        }
    }

}
