package edu.purdue.iyer18;

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.FragmentTransaction;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

//import android.widget.Toast;
/**
 * @author Janka Gal, jgal, 803
 * @author Rajalakshmy Sivaramakrishnan Iyer, iyer18, 814
 */
public class MainActivity extends Activity implements SubmitCallbackListener,
		StartOverCallbackListener {

	/**
	 * The ClientFragment used by the activity.
	 */
	private ClientFragment clientFragment;

	/**
	 * The ServerFragment used by the activity.
	 */
	private ServerFragment serverFragment;

	/**
	 * UI component of the ActionBar used for navigation.
	 */
	private Button left;
	private Button right;
	private TextView title;

	/**
	 * Called once the activity is created.
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main_layout);

		this.clientFragment = ClientFragment.newInstance(this);
		this.serverFragment = ServerFragment.newInstance();

		FragmentTransaction ft = getFragmentManager().beginTransaction();
		ft.add(R.id.fl_main, this.clientFragment);
		ft.commit();
	}

	/**
	 * Creates the ActionBar: - Inflates the layout - Extracts the components
	 */
	@SuppressLint("InflateParams")
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		final ViewGroup actionBarLayout = (ViewGroup) getLayoutInflater()
				.inflate(R.layout.action_bar, null);

		// Set up the ActionBar
		final ActionBar actionBar = getActionBar();
		actionBar.setDisplayShowHomeEnabled(false);
		actionBar.setDisplayShowTitleEnabled(false);
		actionBar.setDisplayShowCustomEnabled(true);
		actionBar.setCustomView(actionBarLayout);

		// Extract the UI component.
		this.title = (TextView) actionBarLayout.findViewById(R.id.tv_title);
		this.left = (Button) actionBarLayout.findViewById(R.id.bu_left);
		this.right = (Button) actionBarLayout.findViewById(R.id.bu_right);
		this.right.setVisibility(View.INVISIBLE);

		return true;
	}

	/**
	 * Callback function called when the user click on the right button of the
	 * ActionBar.
	 * 
	 * @param v
	 */
	public void onRightClick(View v) {
		FragmentTransaction ft = getFragmentManager().beginTransaction();

		this.title.setText(this.getResources().getString(R.string.client));
		this.left.setVisibility(View.VISIBLE);
		this.right.setVisibility(View.INVISIBLE);
		ft.replace(R.id.fl_main, this.clientFragment);
		ft.commit();
	}

	/**
	 * Callback function called when the user click on the left button of the
	 * ActionBar.
	 * 
	 * @param v
	 */
	public void onLeftClick(View v) {
		FragmentTransaction ft = getFragmentManager().beginTransaction();

		this.title.setText(this.getResources().getString(R.string.server));
		this.left.setVisibility(View.INVISIBLE);
		this.right.setVisibility(View.VISIBLE);
		ft.replace(R.id.fl_main, this.serverFragment);
		ft.commit();

	}

	/**
	 * Callback function called when the user click on the submit button.
	 */
	@Override
	public void onSubmit() {
		// TODO: Get client info via client fragment
		String name = this.clientFragment.getName(getResources().getString(
				R.string.default_name));
		int type = this.clientFragment.getType();
		String from = this.clientFragment.getFrom();
		String to = this.clientFragment.getTo();
		// Toast toast = Toast.makeText(getApplicationContext(),
		// this.clientFragment.getCommand(), Toast.LENGTH_LONG);
		// toast.show();

		// Server info
		String host = this.serverFragment.getHost(getResources().getString(
				R.string.default_host));
		int port = this.serverFragment.getPort(Integer.parseInt(getResources()
				.getString(R.string.default_port)));
		// TODO: sanity check the results of the previous two dialogs
		if (host == null || host.indexOf(" ") != -1 || host.equals("")) {
			alertMessage("host. Host cannot be empty or include spaces.");
			return;
		}
		if (port < 1 || (port > 65535)) {
			alertMessage("port. Port must be in the range 1 to 65535.");
			return;
		}

		if (name == null || name.indexOf(",") != -1 || name.equals("")) {
			alertMessage("name. Name cannot be empty or have a comma.");
			return;
		}
		if (type < 0 || type > 2) {
			alertMessage("preference. Select one.");
			return;
		}
		if (from.equals("*") || !validFromLocation(from)) {
			alertMessage("from.");
			return;
		}
		if (!validToLocation(to) || to.equals(from)) {
			alertMessage("to. Destination cannot be same as current location.");
			return;
		}

		if (to.equals("*")) {
			if (type != 2) {
				alertMessage("form. If to is anywhere, preference must be volunteer.");
				return;
			}
		}
		// TODO: Need to get command from client fragment
		// String command = this.getResources()
		// .getString(R.string.default_command);
		String command = this.clientFragment.getCommand();
		FragmentTransaction ft = getFragmentManager().beginTransaction();

		this.title.setText(getResources().getString(R.string.match));
		this.left.setVisibility(View.INVISIBLE);
		this.right.setVisibility(View.INVISIBLE);

		// TODO: You may want additional parameters here if you tailor
		// the match fragment
		MatchFragment frag = MatchFragment.newInstance(this, host, port,
				command, name, from, to, type);

		ft.replace(R.id.fl_main, frag);
		ft.commit();
	}

	public void alertMessage(String input) {
		final AlertDialog alert = new AlertDialog.Builder(MainActivity.this)
				.create();
		alert.setMessage("Invalid " + input);
		alert.setButton("OK", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				alert.dismiss();
			}
		});
		alert.show();

	}

	public boolean validFromLocation(String location) {
		String[] locations = { "LWSN", "PMU", "PUSH", "EE", "CL50" };
		for (int i = 0; i < locations.length; i++) {
			if (locations[i].equals(location)) {
				return true;
			}
		}
		return false;
	}

	public boolean validToLocation(String location) {
		String[] locations = { "PMU", "LWSN", "PUSH", "EE", "CL50", "*" };
		for (int i = 0; i < locations.length; i++) {
			if (locations[i].equals(location)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Callback function call from MatchFragment when the user want to create a
	 * new request.
	 */
	@Override
	public void onStartOver() {
		onRightClick(null);
	}

}
