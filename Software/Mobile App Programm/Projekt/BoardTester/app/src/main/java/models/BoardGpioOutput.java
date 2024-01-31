package models;

import java.io.Serializable;
import java.sql.Timestamp;

class BoardGpioOutput  implements Serializable {
    private int board_id;
    private String pin;
    private  double noPullHigh;
    private  double noPullLow;
    private  double pullDownHigh;
    private  double pullDownLow;
    private  double pullUpHigh;
    private  double pullUpLow;
    private String commentNoPull;
    private String commentPullDown;
    private String commentPullUp;
    private Timestamp time;

    public BoardGpioOutput() {
    }

    public BoardGpioOutput(int board_id, String pinNumber, double noPullHigh, double noPullLow, double pullDownHigh, double pullDownLow, double pullUpHigh, double pullUpLow, String commentNoPull, String commentPullDown, String commentPullUp, Timestamp time) {
        this.board_id = board_id;
        this.pin = pinNumber;
        this.noPullHigh = noPullHigh;
        this.noPullLow = noPullLow;
        this.pullDownHigh = pullDownHigh;
        this.pullDownLow = pullDownLow;
        this.pullUpHigh = pullUpHigh;
        this.pullUpLow = pullUpLow;
        this.commentNoPull = commentNoPull;
        this.commentPullDown = commentPullDown;
        this.commentPullUp = commentPullUp;
        this.time = time;
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

    public double getNoPullHigh() {
        return noPullHigh;
    }

    public void setNoPullHigh(double noPullHigh) {
        this.noPullHigh = noPullHigh;
    }

    public double getNoPullLow() {
        return noPullLow;
    }

    public void setNoPullLow(double noPullLow) {
        this.noPullLow = noPullLow;
    }

    public double getPullDownHigh() {
        return pullDownHigh;
    }

    public void setPullDownHigh(double pullDownHigh) {
        this.pullDownHigh = pullDownHigh;
    }

    public double getPullDownLow() {
        return pullDownLow;
    }

    public void setPullDownLow(double pullDownLow) {
        this.pullDownLow = pullDownLow;
    }

    public double getPullUpHigh() {
        return pullUpHigh;
    }

    public void setPullUpHigh(double pullUpHigh) {
        this.pullUpHigh = pullUpHigh;
    }

    public double getPullUpLow() {
        return pullUpLow;
    }

    public void setPullUpLow(double pullUpLow) {
        this.pullUpLow = pullUpLow;
    }

    public String getCommentNoPull() {
        return commentNoPull;
    }

    public void setCommentNoPull(String commentNoPull) {
        this.commentNoPull = commentNoPull;
    }

    public String getCommentPullDown() {
        return commentPullDown;
    }

    public void setCommentPullDown(String commentPullDown) {
        this.commentPullDown = commentPullDown;
    }

    public String getCommentPullUp() {
        return commentPullUp;
    }

    public void setCommentPullUp(String commentPullUp) {
        this.commentPullUp = commentPullUp;
    }

    public Timestamp getTime() {
        return time;
    }

    public void setTime(Timestamp time) {
        this.time = time;
    }
}

