package interfaces;

public interface BoardManagerListener {
    void onBoardAdded(boolean isAdded);
    void onAddBoardError(Exception exception);
    void onRemoveBoard(boolean isRemoved, int position, int boardID);
    void onRemoveBoardError(Exception exception, int boardID);
}
