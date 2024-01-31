package models;

import android.os.Parcel;
import android.os.Parcelable;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class GpioInput implements Parcelable {

    private int board_id;
    private String pin;
    private  int high_state;
    private  int low_state;
    private Timestamp timestamp;
    private String 	comment;
    private String timestampText;
    private long timeStampLong;

    public GpioInput() {
    }

    public GpioInput(int board_id, String pin, int high_state, int low_state, Timestamp timestamp, String comment) {
        this.board_id = board_id;
        this.pin = pin;
        this.high_state = high_state;
        this.low_state = low_state;
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

    public int getHigh_state() {
        return high_state;
    }

    public void setHigh_state(int high_state) {
        this.high_state = high_state;
    }

    public int getLow_state() {
        return low_state;
    }

    public void setLow_state(int low_state) {
        this.low_state = low_state;
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
                ", high=" + high_state +
                ", low=" + low_state +
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
        parcel.writeInt(high_state);
        parcel.writeInt(low_state);
        parcel.writeString(comment);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected GpioInput(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        high_state = in.readInt();
        low_state = in.readInt();
        comment = in.readString();
        timestampText  = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Creator<GpioInput> CREATOR = new Creator<GpioInput>() {
        @Override
        public GpioInput createFromParcel(Parcel in) {
            return new GpioInput(in);
        }

        @Override
        public GpioInput[] newArray(int size) {
            return new GpioInput[size];
        }
    };
}
