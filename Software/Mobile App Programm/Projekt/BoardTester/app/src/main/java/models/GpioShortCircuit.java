package models;

import android.os.Parcel;
import android.os.Parcelable;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class GpioShortCircuit implements Parcelable {

    private int board_id;
    private String pin;



    private int short_circuits_count;
    private String comment;
    private Timestamp timestamp;
    private String timestampText;
    private long timeStampLong;

    public GpioShortCircuit() {
    }
    public GpioShortCircuit(int board_id, String pin, int short_circuits_count, String comment, Timestamp timestamp) {
        this.board_id = board_id;
        this.pin = pin;
        this.short_circuits_count = short_circuits_count;
        this.comment = comment;
        this.timestamp = timestamp;
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

    public int getShort_circuits_count() {
        return short_circuits_count;
    }

    public void setShort_circuits_count(int short_circuits_count) {
        this.short_circuits_count = short_circuits_count;
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

    @Override
    public int describeContents() {
        return 0;
    }
    public long getTimeStampLong() {
        return timeStampLong;
    }

    public void setTimeStampLong(long timeStampLong) {
        this.timeStampLong = timeStampLong;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeInt(board_id);
        parcel.writeString(pin);
        parcel.writeInt(short_circuits_count);
        parcel.writeString(comment);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected GpioShortCircuit(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        short_circuits_count = in.readInt();
        comment = in.readString();
        timestampText  = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Parcelable.Creator<GpioShortCircuit> CREATOR = new Parcelable.Creator<GpioShortCircuit>() {
        @Override
        public GpioShortCircuit createFromParcel(Parcel in) {
            return new GpioShortCircuit(in);
        }

        @Override
        public GpioShortCircuit[] newArray(int size) {
            return new GpioShortCircuit[size];
        }
    };

    @Override
    public String toString() {
        return "GpioShortCircuit{" +
                "board_id='" + board_id + '\'' +
                ", pin='" + pin + '\'' +
                ", short_circuits_count=" + short_circuits_count +
                ", comment='" + comment + '\'' +
                ", timestamp=" + timestamp +
                '}';
    }
}
