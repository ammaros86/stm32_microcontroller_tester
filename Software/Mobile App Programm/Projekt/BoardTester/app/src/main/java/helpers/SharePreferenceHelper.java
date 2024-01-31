package helpers;

import static android.content.Context.MODE_PRIVATE;

import static helpers.Constants.maxBoardTemperature;

import android.content.Context;
import android.content.SharedPreferences;

public class SharePreferenceHelper {

    public static double getMaxTemperature(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return pref.getFloat("MaxTemperature", maxBoardTemperature);
    }

    public static void setMaxTemperature(Context context, double temperature){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("MaxTemperature");
        sEditor.putFloat("MaxTemperature", (float)temperature);
        sEditor.apply();
    }

    public static String getServerUrl(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return pref.getString("serverUrl", "");
    }

    public static void setServerUrl(Context context, String url){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("serverUrl");
        sEditor.putString("serverUrl", url);
        sEditor.apply();
    }

    public static void removeUrl(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("serverUrl");
        sEditor.apply();
    }

    public static double get3VMaxVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("3vMaxVoltage", "3.6" ));
    }

    public static void set3VMaxVoltage(Context context, double maxVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("3vMaxVoltage");
        sEditor.putString("3vMaxVoltage", Double.toString(maxVoltage));
        sEditor.apply();
    }
    public static double get3VMinVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("3vMinVoltage", "2.7" ));
    }

    public static void set3VMinVoltage(Context context, double minVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("3vMinVoltage");
        sEditor.putString("3vMinVoltage", Double.toString(minVoltage));
        sEditor.apply();
    }




    public static double get5VMaxVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("5vMaxVoltage", "5.1" ));
    }

    public static void set5VMaxVoltage(Context context, double maxVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("5vMaxVoltage");
        sEditor.putString("5vMaxVoltage", Double.toString(maxVoltage));
        sEditor.apply();
    }
    public static double get5VMinVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("5vMinVoltage", "4.75" ));
    }

    public static void set5VMinVoltage(Context context, double minVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("5vMinVoltage");
        sEditor.putString("5vMinVoltage", Double.toString(minVoltage));
        sEditor.apply();
    }


    public static double getGNDMaxVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("gndMaxVoltage", "0.09" ));
    }

    public static void setGNDMaxVoltage(Context context, double maxVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("gndMaxVoltage");
        sEditor.putString("gndMaxVoltage", Double.toString(maxVoltage));
        sEditor.apply();
    }

    public static double getGNDMinVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("gndMinVoltage", "0" ));
    }

    public static void setGNDMinVoltage(Context context, double minVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("gndMinVoltage");
        sEditor.putString("gndMinVoltage", Double.toString(minVoltage));
        sEditor.apply();
    }

    public static double getE5VMaxVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("e5vMaxVoltage", "0.25" ));
    }

    public static void setE5VMaxVoltage(Context context, double maxVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("e5vMaxVoltage");
        sEditor.putString("e5vMaxVoltage", Double.toString(maxVoltage));
        sEditor.apply();
    }

    public static double getE5VMinVoltage(Context context){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        return Double.parseDouble(pref.getString("e5vMinVoltage", "0" ));
    }

    public static void setE5VMinVoltage(Context context, double minVoltage){
        SharedPreferences pref = context.getSharedPreferences("APP_SETTING", MODE_PRIVATE);
        SharedPreferences.Editor sEditor = pref.edit();
        sEditor.remove("e5vMinVoltage");
        sEditor.putString("e5vMinVoltage", Double.toString(minVoltage));
        sEditor.apply();
    }

}
