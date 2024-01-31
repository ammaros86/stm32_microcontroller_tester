package models;

import java.io.Serializable;
import java.sql.Timestamp;

public class BoardFlash implements Serializable {
    private int board_id;
    private int upload_progrgamm;
    private int flash_test;
    private int flash_errors;
    private String comment;
    private Timestamp timestamp;

    public BoardFlash() {
    }

    public BoardFlash(int board_id, int upload_progrgamm, int flash_test, int flash_errors, String comment, Timestamp timestamp) {
        this.board_id = board_id;
        this.upload_progrgamm = upload_progrgamm;
        this.flash_test = flash_test;
        this.flash_errors = flash_errors;
        this.comment = comment;
        this.timestamp = timestamp;
    }

    public int getBoard_id() {
        return board_id;
    }

    public void setBoard_id(int board_id) {
        this.board_id = board_id;
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

    public int getUpload_progrgamm() {
        return upload_progrgamm;
    }

    public void setUpload_progrgamm(int upload_progrgamm) {
        this.upload_progrgamm = upload_progrgamm;
    }

    public int getFlash_test() {
        return flash_test;
    }

    public void setFlash_test(int flash_test) {
        this.flash_test = flash_test;
    }

    public int getFlash_errors() {
        return flash_errors;
    }

    public void setFlash_errors(int flash_errors) {
        this.flash_errors = flash_errors;
    }

}
