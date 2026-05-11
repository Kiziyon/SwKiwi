package net.itsjustsomedude.swrdg;

import com.touchfoo.swordigo.GameTime;
import com.touchfoo.swordigo.Native;

/**
 * @noinspection unused
 */
public class LNIFunctions {
	private static final String TAG = "LNIFunctions";

	public static void register() {
		LuaNativeInterface.registerMethod(LNIFunctions.class, "openUrl");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "copyToClipboard");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "setSpeed");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "getSpeed");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "quit");

		LuaNativeInterface.registerMethod(LNIFunctions.class, "NewButton");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "MakeMovable");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "RemoveButton");

		LuaNativeInterface.registerMethod(LNIFunctions.class, "ButtonPressed");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "ButtonDragged");


		LuaNativeInterface.registerMethod(LNIFunctions.class, "RemoveAllButtons");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetScaling");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetBackgroundResource");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetText");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetTextScale");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetTextColor");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "GetPositionX");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "GetPositionY");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "SetPosition");

		LuaNativeInterface.registerMethod(LNIFunctions.class, "returnTest1");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "argTest1");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "errorTest1");
		LuaNativeInterface.registerMethod(LNIFunctions.class, "errorTest2");
	}

	public static void NewButton(String id, String label, double x, double y, double w, double h) {
		ButtonController.addButton(id, label, (float)x, (float)y, (int)w, (int)h);
	}
	public static void MakeMovable(String id, boolean snapback) {
		ButtonController.makeMovable(id, snapback);
	}

	public static void SetBackgroundResource(String id, String res) {
		ButtonController.setBackgroundResource(id, res);
	}

	public static void SetPosition(String id, double x, double y) {
		ButtonController.setPosition(id, (float)x, (float)y);
	}

	public static double GetPositionX(String id) {
		float[] pos = ButtonController.getPosition(id);
		return (double)pos[0];
	}
	public static double GetPositionY(String id) {
		float[] pos = ButtonController.getPosition(id);
		return (double)pos[1];
	}

	public static void RemoveButton(String id) {
		ButtonController.removeButton(id);
	}
	public static void RemoveAllButtons() {
		ButtonController.removeAll();
	}

	public static void SetScaling(String id, double x, double y) {
		ButtonController.setScaling(id, (float)x, (float)y);
	}

	public static void SetText(String id, String text) {
		ButtonController.setText(id, text);
	}
	public static void SetTextScale(String id, double scale) {
		ButtonController.setTextScale(id, (float) scale);
	}
	public static void SetTextColor(String id, double color) {
		int argb = (int)(long)color;
		ButtonController.setTextColor(id, argb);
	}

	public static boolean ButtonPressed(String id) {
		return ButtonController.isPressed(id);
	}
	public static boolean ButtonDragged(String id) {
		return ButtonController.isDragging(id);
	}

	public static void openUrl(String url) {
		Native.openURL(url);
	}
//    public static void askOpenUrl(String url, boolean askFirst) {}

	public static void copyToClipboard(String content) {
		Native.copyToClipboard("Swordigo", content);
	}

	public static double getSpeed() {
		return GameTime.scaling;
	}

	public static void setSpeed(double speed) {
		GameTime.scaling = Math.clamp(speed, 0.05, 16);
	}

	public static void quit() {
		MainActivity mainActivity = Native.getActivity();
		if (mainActivity == null) return;

		mainActivity.finish();
	}

	public static String returnTest1() {
		return "String from Java";
	}

	public static String argTest1(String arg1, String arg2) {
		return "Args: " + arg1 + ", " + arg2;
	}

	public static String errorTest1() throws Exception {
		throw new Exception("Intentional Java Error.");
	}

	public static String errorTest2(String arg1) throws Exception {
		throw new Exception("Intentional Java Error: " + arg1);
	}
}
