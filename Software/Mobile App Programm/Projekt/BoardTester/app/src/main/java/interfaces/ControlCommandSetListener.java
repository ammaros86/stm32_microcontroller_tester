package interfaces;

public interface ControlCommandSetListener {
    void onTryToSetTurnUsbOnCommand(boolean isSet);
    void onTryToSetTurnBoardOffCommand(boolean isSet);
    void onTryToSetCopyLogFilesCommand(boolean isSet);
}
