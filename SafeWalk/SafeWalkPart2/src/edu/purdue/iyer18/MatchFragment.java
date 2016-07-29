package edu.purdue.iyer18;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

import android.app.Fragment;
import android.util.Log;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.os.AsyncTask;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

/**
 * This fragment is the "page" where the application display the log from the
 * server and wait for a match.
 *
 * @author Janka Gal, jgal, 803
 * @author Rajalakshmy Sivaramakrishnan Iyer, iyer18, 814
 */
public class MatchFragment extends Fragment implements OnClickListener {
	private Boolean connection = false;
	private Boolean pairFound = false;
	private static final String DEBUG_TAG = "DEBUG";

	/**
	 * Activity which have to receive callbacks.
	 */
	private StartOverCallbackListener activity;

	/**
	 * AsyncTask sending the request to the server.
	 */
	private Client client;

	/**
	 * Coordinate of the server.
	 */
	private String host;
	private int port;

	/**
	 * Command the user should send.
	 */
	private String command;
	private String name;
	private String from;
	private String to;
	private int type;
	// TODO: your own class fields here
	private TextView fromMatch;

	private TextView toMatch;
	private TextView congrats;
	private TextView log1;
	private TextView log2;
	private TextView log3;
	private TextView log4;
	private TextView partnerName;
	public Socket s;
	public BufferedReader in;
	public PrintWriter out;

	// Class methods
	/**
	 * Creates a MatchFragment
	 * 
	 * @param activity
	 *            activity to notify once the user click on the start over
	 *            Button.
	 * @param host
	 *            address or IP address of the server.
	 * @param port
	 *            port number.
	 * 
	 * @param command
	 *            command you have to send to the server.
	 * 
	 * @return the fragment initialized.
	 */
	// TODO: you can add more parameters, follow the way we did it.
	// ** DO NOT CREATE A CONSTRUCTOR FOR MatchFragment **
	public static MatchFragment newInstance(StartOverCallbackListener activity,
			String host, int port, String command, String name, String from,
			String to, int type) {
		MatchFragment f = new MatchFragment();

		f.activity = activity;
		f.host = host;
		f.port = port;
		f.command = command;
		f.name = name;
		f.from = from;
		f.to = to;
		f.type = type;
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

		View view = inflater.inflate(R.layout.match_fragment_layout, container,
				false);

		/**
		 * Register this fragment to be the OnClickListener for the startover
		 * button.
		 */
		view.findViewById(R.id.bu_start_over).setOnClickListener(this);

		// TODO: import your Views from the layout here. See example in
		// ServerFragment.
		this.toMatch = (TextView) view.findViewById(R.id.tv_toResponse);
		this.fromMatch = (TextView) view.findViewById(R.id.tv_fromResponse);
		this.partnerName = (TextView) view
				.findViewById(R.id.tv_partnerResponse);
		this.log1 = (TextView) view.findViewById(R.id.tv_log1);
		this.log2 = (TextView) view.findViewById(R.id.tv_log2);
		this.log3 = (TextView) view.findViewById(R.id.tv_log3);
		this.log4 = (TextView) view.findViewById(R.id.tv_log4);
		this.congrats = (TextView) view.findViewById(R.id.tv_congrats);
		/**
		 * Launch the AsyncTask
		 */
		this.client = new Client();
		this.client.execute("");

		return view;
	}

	/**
	 * Callback function for the OnClickListener interface.
	 */
	@Override
	public void onClick(View v) {
		/**
		 * Close the AsyncTask if still running.
		 */
		Log.d(DEBUG_TAG, "before close called here?");
		if (connection == true) {
			this.client.close();
		}
		/**
		 * Notify the Activity.
		 */
		Log.d(DEBUG_TAG, "onStartOver here here?");
		this.activity.onStartOver();
	}

	class Client extends AsyncTask<String, String, String> implements Closeable {
		BufferedReader in;
		Socket s = null;
		PrintWriter out;
		String response = "";

		/**
		 * NOTE: you can access MatchFragment field from this class:
		 * 
		 * Example: The statement in doInBackground will print the message in
		 * the Eclipse LogCat view.
		 */

		/**
		 * The system calls this to perform work in a worker thread and delivers
		 * it the parameters given to AsyncTask.execute()
		 */
		protected String doInBackground(String... params) {

			/**
			 * TODO: Your Client code here.
			 */
			String result = null;
			connection = false;
			pairFound = false;
			Log.d(DEBUG_TAG, String
					.format("The Server at the address %s uses the port %d",
							host, port));
			Log.d(DEBUG_TAG, String.format(
					"The Client will send the command: %s", command));

			try {
				Log.d(DEBUG_TAG, "trying" + connection + pairFound);
				// s = new Socket(host, port);
				s = new Socket();
				s.connect(new InetSocketAddress(host, port), 15000);
				Log.d(DEBUG_TAG, "after socket");

				connection = true;
				publishProgress();
				Log.d(DEBUG_TAG,
						String.format("Connection to the server: Success."));
				out = new PrintWriter(s.getOutputStream(), true);
				Log.d(DEBUG_TAG, "out");
				in = new BufferedReader(new InputStreamReader(
						this.s.getInputStream()));
				Log.d(DEBUG_TAG, " in declared");
				out.println(command);
				out.flush();

				Log.d(DEBUG_TAG, String.format("Into in and out"));

				for (;;) {
					result = in.readLine();
					Log.d(DEBUG_TAG, "Read: " + result);
					if (result.startsWith("RESPONSE: ")) {
						pairFound = true;
						Log.d(DEBUG_TAG, "response");
						out.write(":ACK");
						out.println();
						out.close();
						response = result;
						Log.d(DEBUG_TAG, response);

					}
					if (result.startsWith("ERROR: ")) {
						pairFound = false;
						Log.d(DEBUG_TAG, "ERror");
						out.write(":ACK");
						out.println();
						out.close();
						response = "error";
					}
					break;
				}
			} catch (SocketTimeoutException e) {
				Log.d(DEBUG_TAG, "Socket Timeout");
				Log.d(DEBUG_TAG, e.getMessage());
			} catch (ConnectException e) {
				Log.d(DEBUG_TAG, "Connection failed");
				Log.d(DEBUG_TAG, e.getMessage());
			} catch (UnknownHostException e) {
				e.printStackTrace();
				Log.d(DEBUG_TAG, "Unknown host");
			} catch (IOException e) {
				e.printStackTrace();
				Log.d(DEBUG_TAG, "IO");
			} finally {
				Log.d(DEBUG_TAG, "here finally");
				publishProgress();
			}
			Log.d(DEBUG_TAG, "left try catch");
			publishProgress();
			return "";
		}

		public void close() {
			// TODO: Clean up the client
			Log.d(DEBUG_TAG, "crash here?");
			try {
				s.close();
				Log.d(DEBUG_TAG, " here?");
				in.close();
				Log.d(DEBUG_TAG, "or here?");
				out.close();
				Log.d(DEBUG_TAG, "perhaps here?");
			} catch (IOException e) {
				e.printStackTrace();
			}
			Log.d(DEBUG_TAG, "maybe here?");
		}

		/**
		 * The system calls this to perform work in the UI thread and delivers
		 * the result from doInBackground()
		 */

		// TODO: use the following method to update the UI.
		// ** DO NOT TRY TO CALL UI METHODS FROM doInBackground!!!!!!!!!! **

		/**
		 * Method executed just before the task.
		 */
		@Override
		protected void onPreExecute() {
			log1.setText(String
					.format("The Server at the address %s uses the port %d",
							host, port));
		}

		/**
		 * Method executed once the task is completed.
		 */
		@Override
		protected void onPostExecute(String result) {
			Log.d(DEBUG_TAG, "Enters post execute");
			if (connection == true && pairFound == true) {
				Log.d(DEBUG_TAG,
						String.format("A pair has been found by the server."));
				log4.setText("A pair has been found by the server.");

				congrats.setText(R.string.congrats);
				Log.d(DEBUG_TAG, "congrats");
				String splitResponse = response.substring(
						response.indexOf(':') + 2, response.length());
				Log.d(DEBUG_TAG, splitResponse);
				String[] split = splitResponse.split(",");

				partnerName.setText(split[0]);

				fromMatch.setText(split[1]);

				toMatch.setText(split[2]);

				Log.d(DEBUG_TAG, "Everything printed");
			} else {
				if (response.equals("error")) {
					log4.setText("Server has been reset. Start over.");
					congrats.setText("Server has been reset!");
				} else {
					log4.setText("Press Start Over");
				}
			}
		}

		/**
		 * Method executed when progressUpdate is called in the doInBackground
		 * function.
		 */
		@Override
		protected void onProgressUpdate(String... result) {
			if (connection == true) {
				log2.setText(String
						.format("Connection to the server: Success."));
			} else {
				log2.setText("Connection to the server: Failed.");
				log3.setText("Server is not available.");
				congrats.setText(R.string.failed);

			}
			if (connection == true && pairFound == false) {
				String preference = "";
				switch (type) {
				case 0:
					preference = "with no preference being a requester or a volunteer";
					break;
				case 1:
					preference = "a requester";
					break;
				case 2:
					preference = "a volunteer";
					break;
				}
				String log3Text = String.format(
						"%s, %s, sent a request to move from " + "%s to %s.",
						name, preference, from, to);
				log3.setText(String.format("%s", log3Text));
				log4.setText("Waiting for a pair to be found.");
				congrats.setText(R.string.waiting);
			}
		}
	}

}
