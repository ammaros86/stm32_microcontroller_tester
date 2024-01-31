package models;

import android.os.Parcel;
import android.os.Parcelable;

import androidx.annotation.NonNull;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class PowerPinVoltage implements Parcelable {
    private int board_id;
    private String pin;
    private float voltage;
    private Timestamp timestamp;
    private String comment;
    private String timestampText;
    private long timeStampLong;

    public PowerPinVoltage() {

    }

    public PowerPinVoltage(int board_id, String pin, float voltage, Timestamp timestamp, String comment) {
        this.board_id = board_id;
        this.pin = pin;
        this.voltage = voltage;
        this.timestamp = timestamp;
        this.comment = comment;
    }

    public int getBoard_id() {
        return board_id;
    }

    public void setBoard_id(int board_id) {
        this.board_id = board_id;
    }

    public String getPin() {
        return pin;
    }

    public void setPin(String pin) {
        this.pin = pin;
    }

    public Timestamp getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Timestamp timestamp) {
        this.timestamp = timestamp;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }
    public float getVoltage() {
        return voltage;
    }

    public void setVoltage(float voltage) {
        this.voltage = voltage;
    }
    public String getTimestampText() {
        return timestampText;
    }

    public void setTimestampText(String timestampText) {
        this.timestampText = timestampText;
    }

    public long getTimeStampLong() {
        return timeStampLong;
    }

    public void setTimeStampLong(long timeStampLong) {
        this.timeStampLong = timeStampLong;
    }

    @NonNull
    @Override
    public String toString() {
        return "BoardPowerPins{" +
                "board_id='" + board_id + '\'' +
                ", pin=" + pin +
                ", voltage=" + voltage +
                ", timestamp=" + timestamp +
                ", comment=" + comment +
                '}';
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(@NonNull Parcel parcel, int i) {
        parcel.writeInt(board_id);
        parcel.writeString(pin);
        parcel.writeFloat(voltage);
        parcel.writeString(comment);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected PowerPinVoltage(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        voltage = in.readFloat();
        comment = in.readString();
        timestampText  = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Parcelable.Creator<PowerPinVoltage> CREATOR = new Parcelable.Creator<PowerPinVoltage>() {
        @Override
        public PowerPinVoltage createFromParcel(Parcel in) {
            return new PowerPinVoltage(in);
        }

        @Override
        public PowerPinVoltage[] newArray(int size) {
            return new PowerPinVoltage[size];
        }
    };
}
