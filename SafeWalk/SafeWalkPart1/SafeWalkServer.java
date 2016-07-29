import java.io.*;
import java.net.*;
import java.util.*;
public class SafeWalkServer implements Runnable {
    ServerSocket server ;//creates an object of the serversocket
    PrintWriter writer = new PrintWriter(System.out);//creates an object of the printwriter class
    private ArrayList<ClientRequest> client = new ArrayList<ClientRequest>();// creates an array list that stores the client requests
    /**
     * Construct the server, set up the socket.
     * 
     * @throws SocketException if the socket or port cannot be obtained properly.
     * @throws IOException if the port cannot be reused.
     */
    public SafeWalkServer(int port) throws SocketException,IOException {
        if ( (port > 1025) && (port < 65535) ) { // the assigned port must be within this range
            this.server = new ServerSocket(port);// initializes the serversocket with a random port in the given range
            server.setReuseAddress(true);  // sets the serversocket to true such that when a server of a 
            //particular port closes it can be reused.          
        }
        if( !((port > 1025) && (port < 65535))){// if the client doesnt enter a valid port, print an errr message
            writer.println("Retry");
            writer.flush();// the flush method is used to send the print message from the server to the client before they are connected to teh server
        }
    }
    public SafeWalkServer() throws SocketException,IOException {
        this.server = new ServerSocket(8888); // creates an instance of the serversocket that sets a efault port
        writer.println("Free port: " + getLocalPort()); // prints a free port that the getLocalPort method gets 
        writer.flush();// flushes this print to the client server
        server.setReuseAddress(true);  //c up
    }
    /**
     * Return the port number on which the server is listening. 
     */
    public int getLocalPort() {
        return server.getLocalPort();// returns the intvalue of a free port using teh getLocalPort() method from the
    }    
    public static String splitFrom(String request) {
        String from = "";
        int naam = request.indexOf(",");
        int kahan = request.indexOf(",", naam + 1);
        for(int i = naam+1; i < kahan; i++) {
            from += request.charAt(i);
        }
        return from;
    }
    public static String splitTo(String request) {
        String to = "";
        int naam = request.indexOf(",");
        int kahan = request.indexOf(",", naam + 1);
        int wahan = request.indexOf(",", kahan + 1);
        for(int i = kahan+1; i < wahan; i++) {
            to += request.charAt(i);
        }
        return to;
    }
    public static String splitPriority(String request) {
        String pri = "";
        int naam = request.indexOf(",");
        int kahan = request.indexOf(",", naam + 1);
        int wahan = request.indexOf(",", kahan + 1);
        for(int i = wahan+1; i < request.length(); i++) {
            pri += request.charAt(i);
        }
        return pri;
    }
    public static boolean valid(String request) {
        boolean fValid = false;
        boolean tValid = false;
        int comma = 0;
        String[] fromLoc = {"CL50", "EE", "LWSN", "PMU", "PUSH"}; // an array of all the possible loactions to go from 
        String[] toLoc = {"CL50", "EE", "LWSN", "PMU", "PUSH", "*"}; // an array of all the possible loactions to go to 
        String priority = splitPriority(request);
        String from = splitFrom(request);
        String to = splitTo(request);
        for (int i = 0; i < request.length(); i++) {
            if(request.charAt(i) == ',') {
                comma++;
            }
        }
        if(comma != 3) { // the client must input 4  arguments seperated by 3 commas
            return false;
        }
        if ( !priority.equals("0")){ // the clients priority must be 0
            return false;
        }
        if( from == to) { // the client cannot ask to be taken from and to the same place
            return false;
        }
        for(int i = 0; i < fromLoc.length; i++) {// the clients 'from' must be one of the locations in the fromLoc array
            if (from.equals(fromLoc[i])){
                fValid =  true;
                break;
            }
        }
        for(int i = 0; i < toLoc.length; i++) { // the clients 'to' must be one of the locations in the toLoc array
            if (from.equals(toLoc[i])){
                tValid = true;
                break;
            }
        }    
        if(tValid == true && fValid == true) { // the clients request isonly valid if both the 'from' and'to'is valid
            return true;
        }
        else return false;// otherwise it is false
    }
    public void run() {
        while (true) {
            try {
                Socket socket = server.accept(); //accepts the cliets request if the port and socket match
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);// creates new object of the output stream
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));//creates new object of input stream
                String request = in.readLine(); //reads the input from the client
                writer.println(request);// writer is used here because the request is printed to the server not the client
                writer.flush();
                if(request.equals(":LIST_PENDING_REQUESTS")){
                    String requests = "[";
                    String comma = ",";
                    for(int i = 0; i < client.size(); i++) {
                        String list = client.get(i).request;
                        requests = requests + String.format("[%s]%s", list, comma);
                    }
                    requests = requests + "]";
                    out.println(requests);
                    out.flush();
                    socket.close();
                    break;
                }
                if(request.equals(":RESET")) {
                    
                    for(int i = 0; i < client.size(); i++) {
                        client.get(i).out.println("ERROR: connection reset");
                        client.get(i).out.flush();
                        client.get(i).socket.close();
                    }
                    out.println("RESPONSE: success");// out is used here as this message is printed to the client rather than the server 
                    out.flush(); //send this message to the client
                    socket.close();//gracefully closes the output connection between client and server
                    //in.close();//gracefully closes the input connection between client and server
                    break;
                }
                if(request.equals(":SHUTDOWN")) {
                    for(int i = 0; i < client.size(); i++) {
                        client.get(i).out.print("ERROR: connection reset");
                        client.get(i).out.flush();
                        client.get(i).socket.close();
                        //client.get(i).out.close();
                    }
                    out.println("RESPONSE: success");// out is used here as this message is printed to the client rather than the server 
                    out.flush(); //send this message to the client
                    socket.close();//gracefully closes the output connection between client and server
                    //in.close();//gracefully closes the input connection between client and server
                    break;
                }
                if(valid(request)) {
                    writer.println(request);
                    writer.flush();
                    client.add(new ClientRequest(request, socket, out, in));
                    int latest = client.size() - 1;
                    String abhiFrom = client.get(latest).from;
                    String abhiTo = client.get(latest).to;
                    ClientRequest abhi = client.get(latest);
                    ClientRequest match;
                    for(int i = 0; i < latest; i++) {
                        if( client.get(i).from.equals(abhiFrom)){
                            match = client.get(i);
                            match.out.println(String.format("RESPONSE: %s",abhi.request));
                            match.out.flush();
                            match.socket.close();
                            abhi.out.println(String.format("RESPONSE: %s",abhi.request));
                            abhi.out.flush();
                            abhi.socket.close();
                            client.remove(i);
                            client.remove(latest - 1);
                            break;
                        }
                        if(client.get(i).to.equals(abhiTo) || client.get(i).to.equals("*")) {
                            if(abhiTo == "*") {
                                continue;
                            }
                            else {
                                match = client.get(i);
                                match.out.println(String.format("RESPONSE: %s",abhi.request));
                                match.out.flush();
                                match.socket.close();
                                abhi.out.println(String.format("RESPONSE: %s",abhi.request));
                                abhi.out.flush();
                                abhi.socket.close();
                                client.remove(i);
                                client.remove(latest - 1);
                                break;
                            }
                        }
                    }
                }  
                
                else {
                    out.println("ERROR: invalid request");
                    out.flush();
                    socket.close();
                    //in.close();
                    break;
                }
            } catch(IOException e) {
                e.printStackTrace();
                break;
            }
        }
    }
}
class ClientRequest {
    public String request;
    public Socket socket;
    public PrintWriter out;
    public BufferedReader in;
    public String to;
    public String from;
    public ClientRequest( String request, Socket socket, PrintWriter out, BufferedReader in) {
        this.request = request;
        this.socket = socket;
        this.out = out;
        this.in = in;
        int naam = request.indexOf(",");
        int kahan = request.indexOf(",", naam + 1);
        int wahan = request.indexOf(",", kahan + 1);
        this.from = request.substring(naam, kahan);
        this.to = request.substring(kahan, wahan);
    } 
}