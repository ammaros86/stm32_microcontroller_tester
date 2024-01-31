package models;

import androidx.annotation.NonNull;

import java.sql.Timestamp;

public class BoardTemperature {
    private int board_id;
    private float temperature;
    private Timestamp timestamp;

    public BoardTemperature() {
    }

    public BoardTemperature(int board_id, float temperature, Timestamp timestamp, int readingTemp) {
        this.board_id = board_id;
        this.temperature = temperature;
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

    public float getTemperature() {
        return temperature;
    }

    public void setTemperature(float temperature) {
        this.temperature = temperature;
    }


    @NonNull
    @Override
    public String toString() {
        return "BoardTemperature{" +
                "board_id='" + board_id + '\'' +
                ", temperature=" + temperature +
                ", timestamp=" + timestamp +
                '}';
    }
}
