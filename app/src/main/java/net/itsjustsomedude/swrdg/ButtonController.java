package net.itsjustsomedude.swrdg;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;

import java.lang.ref.WeakReference;
import java.util.HashMap;

public class ButtonController {
	private static final String LOG_TAG = "MiniBtnController";

	private static class MovableButtonData {
		public float homeX;
		public float homeY;

		public boolean snapback;

		public boolean dragging;

		public float touchOffsetX;
		public float touchOffsetY;

		public MovableButtonData(
				float homeX,
				float homeY,
				boolean snapback
		) {
			this.homeX = homeX;
			this.homeY = homeY;
			this.snapback = snapback;
		}
	}

	private static class ButtonData {
		public Button button;
		public boolean pressed;
		public float baseTextSize;
		public MovableButtonData movable;

		public ButtonData(Button button) {
			this.button = button;
			this.baseTextSize = button.getTextSize();
		}
	}

	private static WeakReference<MainActivity> mainActivityRef;
	private static WeakReference<ViewGroup> viewRef;

	private static final HashMap<String, ButtonData> buttons =
			new HashMap<>();

	public static void init(MainActivity act, ViewGroup view) {
		mainActivityRef = new WeakReference<>(act);
		viewRef = new WeakReference<>(view);
	}

	@SuppressLint("ClickableViewAccessibility")
	public static void addButton(
			String id,
			String label,
			float nx,
			float ny,
			int w,
			int h
	) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {

			if (buttons.containsKey(id))
				return;

			Button btn = new Button(ctx);

			btn.setText(label);
			btn.setTextColor(0xffffffff);
			btn.setVisibility(Button.VISIBLE);
			btn.setBackgroundResource(R.drawable.game_button);

			ButtonData data = new ButtonData(btn);

			btn.setOnTouchListener((v, e) -> {
				switch (e.getAction()) {
					case MotionEvent.ACTION_DOWN:
						data.pressed = true;
						break;

					case MotionEvent.ACTION_UP:
					case MotionEvent.ACTION_CANCEL:
						data.pressed = false;
						break;
				}
				return false;
			});

			FrameLayout.LayoutParams lp =
					new FrameLayout.LayoutParams(w, h);

			lp.leftMargin =
					(int)(root.getWidth() * nx) - (w / 2);

			lp.topMargin =
					(int)(root.getHeight() * ny) - (h / 2);

			root.addView(btn, lp);
			buttons.put(id, data);
			Log.d(LOG_TAG, "Added button: " + id);
		});
	}

	@SuppressLint("ClickableViewAccessibility")
	public static void makeMovable(
			String id,
			boolean snapback
	) {
		MainActivity ctx = mainActivityRef.get();

		if (ctx == null)
			return;

		ctx.runOnUiThread(() -> {

			ButtonData data = buttons.get(id);

			if (data == null)
				return;

			float[] pos = getPosition(id);

			data.movable = new MovableButtonData(
					pos[0],
					pos[1],
					snapback
			);

			data.button.setOnTouchListener((v, e) -> {

				Button btn = data.button;

				FrameLayout.LayoutParams lp =
						(FrameLayout.LayoutParams)
								btn.getLayoutParams();

				switch (e.getAction()) {

					case MotionEvent.ACTION_DOWN: {

						data.pressed = true;

						data.movable.dragging = true;

						data.movable.touchOffsetX =
								e.getRawX() - lp.leftMargin;

						data.movable.touchOffsetY =
								e.getRawY() - lp.topMargin;

						break;
					}

					case MotionEvent.ACTION_MOVE: {

						if (!data.movable.dragging)
							break;

						lp.leftMargin = (int)(
								e.getRawX()
										- data.movable.touchOffsetX
						);

						lp.topMargin = (int)(
								e.getRawY()
										- data.movable.touchOffsetY
						);

						btn.setLayoutParams(lp);

						break;
					}

					case MotionEvent.ACTION_UP:
					case MotionEvent.ACTION_CANCEL: {

						data.pressed = false;

						data.movable.dragging = false;

						if (data.movable.snapback) {

							setPosition(
									id,
									data.movable.homeX,
									data.movable.homeY
							);
						}

						break;
					}
				}

				return true;
			});
		});
	}

	public static void setPosition(
			String id,
			float nx,
			float ny
	) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {

			ButtonData data = buttons.get(id);

			if (data == null)
				return;

			Button btn = data.button;

			FrameLayout.LayoutParams lp =
					(FrameLayout.LayoutParams)
							btn.getLayoutParams();

			lp.leftMargin =
					(int)((root.getWidth() * nx)
							- (btn.getWidth() / 2.0f));

			lp.topMargin =
					(int)((root.getHeight() * ny)
							- (btn.getHeight() / 2.0f));

			btn.setLayoutParams(lp);
		});
	}

	public static float[] getPosition(String id) {

		ViewGroup root = viewRef.get();

		if (root == null)
			return new float[] { 0.0f, 0.0f };

		ButtonData data = buttons.get(id);

		if (data == null)
			return new float[] { 0.0f, 0.0f };

		Button btn = data.button;

		FrameLayout.LayoutParams lp =
				(FrameLayout.LayoutParams)
						btn.getLayoutParams();

		float nx =
				(lp.leftMargin + (btn.getWidth() / 2.0f))
						/ root.getWidth();

		float ny =
				(lp.topMargin + (btn.getHeight() / 2.0f))
						/ root.getHeight();

		return new float[] { nx, ny };
	}

	public static void removeButton(String id) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();
		if (ctx == null || root == null)
			return;
		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;
			root.removeView(data.button);
			buttons.remove(id);
			Log.d(LOG_TAG, "Removed button: " + id);
		});
	}

	public static void removeAll() {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {

			for (ButtonData data : buttons.values()) {
				root.removeView(data.button);
			}
			buttons.clear();
			Log.d(LOG_TAG, "Destroyed all buttons.");
		});
	}

	public static void hideAll() {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();
		if (ctx == null || root == null) return;

		ctx.runOnUiThread(() -> {
			for (ButtonData data : buttons.values()) {
				Button btn = data.button;
				btn.setVisibility(Button.GONE);
			}
		});
	}

	public static void unhideAll() {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();
		if (ctx == null || root == null) return;

		ctx.runOnUiThread(() -> {
			for (ButtonData data : buttons.values()) {
				Button btn = data.button;
				btn.setVisibility(Button.VISIBLE);
			}
		});
	}

	public static boolean isPressed(String id) {
		ButtonData data = buttons.get(id);
		return data != null && data.pressed;
	}

	public static boolean isDragging(String id) {
		ButtonData data = buttons.get(id);
		if (data == null || data.movable == null) return false;

		return data.movable.dragging;
	}

	public static int getResIdByName(
			Context ctx,
			String resName,
			String type
	) {
		return ctx.getResources().getIdentifier(
				resName,
				type,
				ctx.getPackageName()
		);
	}

	public static void setBackgroundResource(
			String id,
			String resName
	) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		int resId = getResIdByName(
				ctx,
				resName,
				"drawable"
		);

		if (resId == 0) {
			Log.e(LOG_TAG,
					"Invalid drawable resource: " + resName);
			return;
		}

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;

			data.button.setBackgroundResource(resId);
		});
	}

	public static void setScaling(String id, float scaleX, float scaleY) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();
		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;

			Button btn = data.button;

			int baseW = 200;
			int baseH = 125;

			FrameLayout.LayoutParams lp =
					(FrameLayout.LayoutParams) btn.getLayoutParams();

			lp.width = (int) (baseW * scaleX);
			lp.height = (int) (baseH * scaleY);

			btn.setLayoutParams(lp);
		});
	}

	// Text
	public static void setText(String id, String text) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;

			data.button.setText(text);
		});
	}

	public static void setTextScale(String id, float scale) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;

			data.button.setTextSize(
					android.util.TypedValue.COMPLEX_UNIT_PX,
					data.baseTextSize * scale
			);
		});
	}

	public static void setTextColor(String id, int color) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;

			data.button.setTextColor(color);
		});
	}

	public static void setBackground(String id, String resName) {
		MainActivity ctx = mainActivityRef.get();
		ViewGroup root = viewRef.get();

		if (ctx == null || root == null)
			return;

		ctx.runOnUiThread(() -> {
			ButtonData data = buttons.get(id);
			if (data == null)
				return;
			int resId = ctx.getResources().getIdentifier(
					resName,
					"drawable",
					ctx.getPackageName()
			);
			if (resId == 0) {
				Log.e("MiniBtnController", "Invalid drawable: " + resName);
				return;
			}
			data.button.setBackgroundResource(resId);
		});
	}
}