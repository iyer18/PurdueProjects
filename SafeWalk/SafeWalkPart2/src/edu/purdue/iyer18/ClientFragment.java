package edu.purdue.iyer18;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;

/**
 * This fragment is the "page" where the user inputs information about the
 * request, he/she wishes to send.
 *
 * @author Rajalakshmy Sivaramakrishnan Iyer, iyer18, 814
 * @author Janka Gal, jgal, 803
 */
public class ClientFragment extends Fragment implements OnClickListener {

	/**
	 * Activity which have to receive callbacks.
	 */
	private SubmitCallbackListener activity;

	/**
	 * EditText where the user enters the name.
	 */
	private EditText name;

	/**
	 * RadioGroup where the user enters type.
	 */
	private RadioGroup pref;
	private RadioButton req0;
	private RadioButton req1;
	private RadioButton req2;
	/**
	 * Spinner where user selects from
	 */
	private Spinner fromSpinner;
	/**
	 * Spinner where the user selects to
	 */
	private Spinner toSpinner;

	/**
	 * Creates a ProfileFragment
	 * 
	 * @param activity
	 *            activity to notify once the user click on the submit Button.
	 * 
	 *            ** DO NOT CREATE A CONSTRUCTOR FOR MatchFragment **
	 * 
	 * @return the fragment initialized.
	 */
	// ** DO NOT CREATE A CONSTRUCTOR FOR ProfileFragment **
	public static ClientFragment newInstance(SubmitCallbackListener activity) {
		ClientFragment f = new ClientFragment();

		f.activity = activity;
		return f;
	}

	/**
	 * Called when the fragment will be displayed.
	 */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		if (container == null) {
			return null;
		}
		View view = inflater.inflate(R.layout.client_fragment_layout,
				container, false);
		/**
		 * Register this fragment to be the OnClickListener for the submit
		 * Button.
		 */
		view.findViewById(R.id.bu_submit).setOnClickListener(this);

		// TODO: import your Views from the layout here. See example in
		// ServerFragment.
		String[] toValues = { "LWSN", "PMU", "PUSH", "EE", "CL50", "*", };
		this.toSpinner = (Spinner) view.findViewById(R.id.to_spinner);
		ArrayAdapter<String> LTRadapter = new ArrayAdapter<String>(
				this.getActivity(), android.R.layout.simple_spinner_item,
				toValues);
		LTRadapter
				.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
		toSpinner.setAdapter(LTRadapter);

		String[] fromValues = { "PMU", "LWSN", "PUSH", "EE", "CL50", };
		this.fromSpinner = (Spinner) view.findViewById(R.id.from_spinner);
		ArrayAdapter<String> LTRadapter2 = new ArrayAdapter<String>(
				this.getActivity(), android.R.layout.simple_spinner_item,
				fromValues);
		LTRadapter2
				.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
		fromSpinner.setAdapter(LTRadapter2);

		this.name = (EditText) view.findViewById(R.id.et_name);
		this.pref = (RadioGroup) view.findViewById(R.id.rg_pref);
		this.req0 = (RadioButton) view.findViewById(R.id.requester0);
		this.req1 = (RadioButton) view.findViewById(R.id.requester1);
		this.req2 = (RadioButton) view.findViewById(R.id.requester2);

		return view;
	}

	/**
	 * Returns the host enter by the user or the default value if the user
	 * didn't open the fragment.
	 */
	public String getName(String defaultVal) {
		return this.name != null ? name.getText().toString() : defaultVal;
	}

	public int getType() {
		int type = 3;

		if (req0.isChecked()) {
			type = 0;
		} else if (req1.isChecked()) {
			type = 1;
		} else if (req2.isChecked()) {
			type = 2;
		}

		return type;
	}

	public String getFrom() {
		return fromSpinner.getSelectedItem().toString();
	}

	public String getTo() {
		return toSpinner.getSelectedItem().toString();
	}

	public String getCommand() {
		return getName("Your Name") + "," + getFrom() + "," + getTo() + ","
				+ getType();
	}

	/**
	 * Callback function for the OnClickListener interface.
	 */
	@Override
	public void onClick(View v) {
		this.activity.onSubmit();
		name.setText(R.string.default_name);
		fromSpinner.setSelection(0);
		toSpinner.setSelection(0);

	}
}
