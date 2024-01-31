package models;

import androidx.annotation.NonNull;

import java.sql.Timestamp;

public class BoardCurrentConsumption {
    private int board_id;
    private float  current;
    private Timestamp timestamp;

    public BoardCurrentConsumption() {
    }

    public BoardCurrentConsumption(int board_id, float current, Timestamp timestamp ) {
        this.board_id = board_id;
        this.current = current;
        this.timestamp = timestamp;
    }


    public Timestamp getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Timestamp timestamp) {
        this.timestamp = timestamp;
    }

    public int getBoard_id() {
        return board_id;
    }

    public void setBoard_id(int board_id) {
        this.board_id = board_id;
    }

    public float getCurrent() {
        return current;
    }

    public void setCurrent(float current) {
        this.current = current;
    }


    @NonNull
    @Override
    public String toString() {
        return "BoardTemperature{" +
                "board_id='" + board_id + '\'' +
                ", current=" + current +
                ", timestamp=" + timestamp +
                '}';
    }
}
