package models;

public class BoardController {

    public BoardController() {
    }
    public BoardController(int turn_usb_port_on, int usb_port_state, int power_off, int power_state, int copy_logs_usb, int usb_media_state) {
        this.turn_usb_port_on = turn_usb_port_on;
        this.usb_port_state = usb_port_state;
        this.power_off = power_off;
        this.power_state = power_state;
        this.copy_logs_usb = copy_logs_usb;
        this.usb_media_state = usb_media_state;
    }

    public int getTurn_usb_port_on() {
        return turn_usb_port_on;
    }

    public void setTurn_usb_port_on(int turn_usb_port_on) {
        this.turn_usb_port_on = turn_usb_port_on;
    }

    public int getUsb_port_state() {
        return usb_port_state;
    }

    public void setUsb_port_state(int usb_port_state) {
        this.usb_port_state = usb_port_state;
    }

    public int getPower_off() {
        return power_off;
    }

    public void setPower_off(int power_off) {
        this.power_off = power_off;
    }

    public int getPower_state() {
        return power_state;
    }

    public void setPower_state(int power_state) {
        this.power_state = power_state;
    }

    public int getCopy_logs_usb() {
        return copy_logs_usb;
    }

    public void setCopy_logs_usb(int copy_logs_usb) {
        this.copy_logs_usb = copy_logs_usb;
    }

    public int getUsb_media_state() {
        return usb_media_state;
    }

    public void setUsb_media_state(int usb_media_state) {
        this.usb_media_state = usb_media_state;
    }

    private int turn_usb_port_on;
    private int usb_port_state;
    private int power_off;
    private int power_state;
    private int copy_logs_usb;
    private int usb_media_state;


    @Override
    public String toString() {
        return "BoardController{" +
                "turn_usb_port_on=" + turn_usb_port_on +
                ", usb_port_state=" + usb_port_state +
                ", power_off=" + power_off +
                ", power_state=" + power_state +
                ", copy_logs_usb=" + copy_logs_usb +
                ", usb_media_state=" + usb_media_state +
                '}';
    }
}
