package interfaces;

import java.util.HashMap;

import models.BoardController;
import models.BoardTestCommands;

public interface AppCommandsListener {
    void onSuccessLoadedAppCommands(HashMap<Integer, BoardTestCommands> appControllerState);
    void onErrorLoadAppCommands(Exception exception);

    void onBoardControllerLoaded(BoardController boardController);
    void onFailedBoardController(Exception exception);
}
