package models;

import java.sql.Timestamp;

public class BoardTestCommands {
    private int isUpdate;
    private int board_id;
    private String boardName;
    private int measure_temperature;
    private int measure_current;
    private int flash_board;
    private int power_gnd_test;
    private int gpio_continuity;
    private int gpio_output_test;
    private int gpio_input_test;
    private int analog_input_test;
    private int gpio_short_circuit_test;
    private int complete_test;
    private int errors;
    private int test_value;
    private Timestamp timestamp;

    public BoardTestCommands() {

    }

    public BoardTestCommands(int isUpdate, int board_id, String boardName, int measure_temperature, int measure_current, int flash_board, int power_gnd_test, int continuityTest, int gpio_output_test, int gpio_input_test, int analog_input_test, int gpio_short_circuit_test, int complete_test, int errors, int test_value, Timestamp timestamp) {
        this.isUpdate = isUpdate;
        this.board_id = board_id;
        this.boardName = boardName;
        this.measure_temperature = measure_temperature;
        this.measure_current = measure_current;
        this.flash_board = flash_board;
        this.power_gnd_test = power_gnd_test;
        this.gpio_continuity = gpio_continuity;
        this.gpio_output_test = gpio_output_test;
        this.gpio_input_test = gpio_input_test;
        this.analog_input_test = analog_input_test;
        this.gpio_short_circuit_test = gpio_short_circuit_test;
        this.complete_test = complete_test;
        this.errors = errors;
        this.test_value = test_value;
        this.timestamp = timestamp;
    }

    public BoardTestCommands(int isUpdate, int board_id,  int measure_temperature, int measure_current, int flash_board, int power_gnd_test, int continuityTest, int gpio_output_test, int gpio_input_test, int analog_input_test, int gpio_short_circuit_test, int complete_test, int errors, int test_value, Timestamp timestamp) {
        this.isUpdate = isUpdate;
        this.board_id = board_id;
        this.measure_temperature = measure_temperature;
        this.measure_current = measure_current;
        this.flash_board = flash_board;
        this.power_gnd_test = power_gnd_test;
        this.gpio_continuity = gpio_continuity;
        this.gpio_output_test = gpio_output_test;
        this.gpio_input_test = gpio_input_test;
        this.analog_input_test = analog_input_test;
        this.gpio_short_circuit_test = gpio_short_circuit_test;
        this.complete_test = complete_test;
        this.errors = errors;
        this.test_value = test_value;
        this.timestamp = timestamp;
    }
    public BoardTestCommands(String boardName) {
        this.boardName = boardName;
    }

    public BoardTestCommands(int isUpdate, int board_id, String boardName, int measure_temperature, int measure_current, int flash_board, int power_gnd_test, int gpio_continuity, int gpio_output_test, int gpio_input_test,  int analog_input_test, int gpio_short_circuit_test, int complete_test, int errors, int test_value) {
        this.isUpdate = isUpdate;
        this.board_id = board_id;
        this.boardName = boardName;
        this.measure_temperature = measure_temperature;
        this.measure_current = measure_current;
        this.flash_board = flash_board;
        this.power_gnd_test = power_gnd_test;
        this.gpio_continuity = gpio_continuity;
        this.gpio_output_test = gpio_output_test;
        this.gpio_input_test = gpio_input_test;
        this.analog_input_test = analog_input_test;
        this.gpio_short_circuit_test = gpio_short_circuit_test;
        this.complete_test = complete_test;
        this.errors = errors;
        this.test_value = test_value;
    }

    public BoardTestCommands(int isUpdate, int board_id, int measure_temperature, int measure_current, int flash_board, int power_gnd_test, int gpio_continuity, int gpio_output_test, int gpio_input_test,  int analog_input_test, int gpio_short_circuit_test, int complete_test, int errors, int test_value) {
        this.isUpdate = isUpdate;
        this.board_id = board_id;
        this.measure_temperature = measure_temperature;
        this.measure_current = measure_current;
        this.flash_board = flash_board;
        this.power_gnd_test = power_gnd_test;
        this.gpio_continuity = gpio_continuity;
        this.gpio_output_test = gpio_output_test;
        this.gpio_input_test = gpio_input_test;
        this.analog_input_test = analog_input_test;
        this.gpio_short_circuit_test = gpio_short_circuit_test;
        this.complete_test = complete_test;
        this.errors = errors;
        this.test_value = test_value;
    }

    public int getIsUpdate() {
        return isUpdate;
    }

    public void setIsUpdate(int isUpdate) {
        this.isUpdate = isUpdate;
    }

    public int getBoard_id() {
        return board_id;
    }

    public void setBoard_id(int board_id) {
        this.board_id = board_id;
    }

    public String getBoardName() {
        return boardName;
    }

    public void setBoardName(String boardName) {
        this.boardName = boardName;
    }
    public int getMeasure_temperature() {
        return measure_temperature;
    }

    public void setMeasure_temperature(int measure_temperature) {
        this.measure_temperature = measure_temperature;
    }

    public int getMeasure_current() {
        return measure_current;
    }

    public void setMeasure_current(int measure_current) {
        this.measure_current = measure_current;
    }
    public int getGpio_continuity() {
        return gpio_continuity;
    }

    public void setGpio_continuity(int gpio_continuity) {
        this.gpio_continuity = gpio_continuity;
    }
    public int getFlash_board() {
        return flash_board;
    }

    public void setFlash_board(int flash_board) {
        this.flash_board = flash_board;
    }

    public int getPower_gnd_test() {
        return power_gnd_test;
    }

    public void setPower_gnd_test(int power_gnd_test) {
        this.power_gnd_test = power_gnd_test;
    }

    public int getGpio_output_test() {
        return gpio_output_test;
    }

    public void setGpio_output_test(int gpio_output_test) {
        this.gpio_output_test = gpio_output_test;
    }
    public int getGpio_input_test() {
        return gpio_input_test;
    }
    public void setGpio_input_test(int gpio_input_test) {
        this.gpio_input_test = gpio_input_test;
    }
    public int getAnalog_input_test() {
        return analog_input_test;
    }

    public void setAnalog_input_test(int analog_input_test) {
        this.analog_input_test = analog_input_test;
    }
    public int getGpio_short_circuit_test() {
        return gpio_short_circuit_test;
    }

    public void setGpio_short_circuit_test(int gpio_short_circuit_test) {
        this.gpio_short_circuit_test = gpio_short_circuit_test;
    }
    public Timestamp getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Timestamp timestamp) {
        this.timestamp = timestamp;
    }

    public int getTest_value() {
        return test_value;
    }

    public void setTest_value(int test_value) {
        this.test_value = test_value;
    }

    public int getComplete_test() {
        return complete_test;
    }

    public void setComplete_test(int complete_test) {
        this.complete_test = complete_test;
    }

    public int getErrors() {
        return errors;
    }

    public void setErrors(int errors) {
        this.errors = errors;
    }
    @Override
    public String toString() {
        return "BoardTestController{" +
                "isUpdate=" + isUpdate +
                ", board_id='" + board_id + '\'' +
                ", measure_temperature=" + measure_temperature +
                ", flash_board=" + flash_board +
                ", power_gnd_test=" + power_gnd_test +
                ", gpio_continuity=" + gpio_continuity +
                ", gpio_output_test=" + gpio_output_test +
                ", gpio_input_test=" + gpio_input_test +
                ", gpio_short_circuit_test=" + gpio_short_circuit_test +
                ", complete_test=" + complete_test +
                ", timestamp=" + timestamp +
                ", test_value=" + test_value +
                '}';
    }
}
