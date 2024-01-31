package models;

import android.os.Parcel;
import android.os.Parcelable;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class GpioContinuity implements Parcelable {
    private int board_id;
    private String pin;



    private double measure_value;
    private Timestamp timestamp;
    private String 	comment;
    private String timestampText;
    private long timeStampLong;

    public GpioContinuity() {
    }

    public GpioContinuity(int board_id, String pin, double measure_value, Timestamp timestamp, String comment) {
        this.board_id = board_id;
        this.pin = pin;
        this.measure_value = measure_value;
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

    public double getMeasure_value() {
        return measure_value;
    }

    public void setMeasure_value(double measure_value) {
        this.measure_value = measure_value;
    }

    public String getComment() {
        return comment;
    }

    public void setComment(String comment) {
        this.comment = comment;
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
    @Override
    public String toString() {
        return "GpioInput{" +
                "board_id='" + board_id + '\'' +
                ", pin='" + pin + '\'' +
                ", measure_voltage=" + measure_value +
                ", timestamp=" + timestamp +
                ", comment='" + comment + '\'' +
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
        parcel.writeDouble(measure_value);
        parcel.writeString(comment);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected GpioContinuity(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        measure_value = in.readDouble();
        comment = in.readString();
        timestampText  = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Parcelable.Creator<GpioContinuity> CREATOR = new Parcelable.Creator<GpioContinuity>() {
        @Override
        public GpioContinuity createFromParcel(Parcel in) {
            return new GpioContinuity(in);
        }

        @Override
        public GpioContinuity[] newArray(int size) {
            return new GpioContinuity[size];
        }
    };
}
