package interfaces;

import java.util.ArrayList;

public interface LogFilesListener {
    void onLogFilesLoaded(ArrayList<String> logFilesArrayList);
    void onLogFilesLoadError(Exception exception);
}
