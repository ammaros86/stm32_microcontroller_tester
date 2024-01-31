package interfaces;

public interface RemoveLogListener {
    void onRemoveBoard(String response, int position);
    void onRemoveBoardFailed(Exception exception);
}
