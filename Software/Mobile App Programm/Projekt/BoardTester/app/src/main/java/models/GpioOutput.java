package models;

import android.os.Parcel;
import android.os.Parcelable;

import androidx.annotation.NonNull;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class GpioOutput implements Parcelable {
    private int board_id;
    private String pin;
    private  double high_voltage;
    private  double low_voltage;
    private String comment;
    private int speed;
    private Timestamp timestamp;
    private String timestampText;
    private long timeStampLong;

    public GpioOutput(int board_id, String pin, double high_voltage, double low_voltage, String comment, int speed, Timestamp timestamp) {
        this.board_id = board_id;
        this.pin = pin;
        this.high_voltage = high_voltage;
        this.low_voltage = low_voltage;
        this.comment = comment;
        this.speed = speed;
        this.timestamp = timestamp;
    }


    public GpioOutput() {
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

    public double getHigh_voltage() {
        return high_voltage;
    }

    public void setHigh_voltage(double high_voltage) {
        this.high_voltage = high_voltage;
    }

    public double getLow_voltage() {
        return low_voltage;
    }

    public void setLow_voltage(double low_voltage) {
        this.low_voltage = low_voltage;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
    }
    public int getSpeed() {
        return speed;
    }

    public void setSpeed(int speed) {
        this.speed = speed;
    }

    public Timestamp getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Timestamp timestamp) {
        this.timestamp = timestamp;
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
        return "GpioOutput{" +
                "board_id='" + board_id + '\'' +
                ", pin='" + pin + '\'' +
                ", highVoltage=" + high_voltage+
                ", lowVoltage=" + low_voltage +
                ", comment='" + comment + '\'' +
                ", timestamp=" + timestamp +
                ", speed='" + speed + '\'' +
                '}';
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeInt(board_id);
        parcel.writeString(pin);
        parcel.writeDouble(high_voltage);
        parcel.writeDouble(low_voltage);
        parcel.writeString(comment);
        parcel.writeInt(speed);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected GpioOutput(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        high_voltage = in.readDouble();
        low_voltage = in.readDouble();
        comment = in.readString();
        speed = in.readInt();
        timestampText  = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Parcelable.Creator<GpioOutput> CREATOR = new Parcelable.Creator<GpioOutput>() {
        @Override
        public GpioOutput createFromParcel(Parcel in) {
            return new GpioOutput(in);
        }

        @Override
        public GpioOutput[] newArray(int size) {
            return new GpioOutput[size];
        }
    };
}
