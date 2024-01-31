package models;

import android.os.Parcel;
import android.os.Parcelable;

import androidx.annotation.NonNull;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Date;

public class AnalogInput implements Parcelable {
    private int board_id;
    private String pin;
    private double measure_value1;
    private double measure_value2;
    private double measure_value3;
    private double measure_value4;
    private Timestamp timestamp;
    private String 	comment;
    private String timestampText;
    private long timeStampLong;

    public AnalogInput() {
    }

    public AnalogInput(int board_id, String pin, double measure_value1, double measure_value2, double measure_value3, double measure_value4, Timestamp timestamp, String comment) {
        this.board_id = board_id;
        this.pin = pin;
        this.measure_value1 = measure_value1;
        this.measure_value2 = measure_value2;
        this.measure_value3 = measure_value3;
        this.measure_value4 = measure_value4;
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

    public double getMeasure_value1() {
        return measure_value1;
    }

    public void setMeasure_value1(double measure_value1) {
        this.measure_value1 = measure_value1;
    }

    public double getMeasure_value2() {
        return measure_value2;
    }

    public void setMeasure_value2(double measure_value2) {
        this.measure_value2 = measure_value2;
    }

    public double getMeasure_value3() {
        return measure_value3;
    }

    public void setMeasure_value3(double measure_value3) {
        this.measure_value3 = measure_value3;
    }

    public double getMeasure_value4() {
        return measure_value4;
    }

    public void setMeasure_value4(double measure_value4) {
        this.measure_value4 = measure_value4;
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
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(@NonNull Parcel parcel, int i) {
        parcel.writeInt(board_id);
        parcel.writeString(pin);
        parcel.writeDouble(measure_value1);
        parcel.writeDouble(measure_value2);
        parcel.writeDouble(measure_value3);
        parcel.writeDouble(measure_value4);
        parcel.writeString(comment);
        Date date = new Date();
        date.setTime(timestamp.getTime());
        String formattedDate = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss").format(date);
        setTimestampText(formattedDate);
        parcel.writeString(timestampText);
        parcel.writeLong(timestamp.getTime());
    }

    protected AnalogInput(Parcel in) {
        board_id = in.readInt();
        pin = in.readString();
        measure_value1 = in.readDouble();
        measure_value2 = in.readDouble();
        measure_value3 = in.readDouble();
        measure_value4 = in.readDouble();
        comment = in.readString();
        timestampText = in.readString();
        timeStampLong = in.readLong();
    }

    public static final Creator<AnalogInput> CREATOR = new Creator<AnalogInput>() {
        @Override
        public AnalogInput createFromParcel(Parcel in) {
            return new AnalogInput(in);
        }

        @Override
        public AnalogInput[] newArray(int size) {
            return new AnalogInput[size];
        }
    };
}
